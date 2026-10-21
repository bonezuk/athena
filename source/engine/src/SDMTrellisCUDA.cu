#include <cuda_runtime.h>
#include <cute/tensor.hpp>
#include <cute/layout.hpp>
#include <cute/atom/mma_traits_sm80.hpp>

#include "common/inc/CommonTypes.h"
#include "engine/inc/FFTRadix2Cuda.h"
#include "engine/inc/SDMTrellisCUDA.h"

using namespace cute;

//-------------------------------------------------------------------------------------------
#if defined(__CUDA_ARCH__) && (__CUDA_ARCH__ >= 800)
#define __OMEGA_CUDA_SM80
#endif
//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
//-------------------------------------------------------------------------------------------

using Traits = MMA_Traits<SM80_16x8x8_F32TF32TF32F32_TN>;

//-------------------------------------------------------------------------------------------
#if defined(__OMEGA_CUDA_SM80)
//-------------------------------------------------------------------------------------------

__device__ __forceinline__ uint32_t kFloatToTF32(float x)
{
    uint32_t r;
    asm("cvt.rna.tf32.f32 %0, %1;\n" : "=r"(r) : "f"(x));
    return r;
}

//-------------------------------------------------------------------------------------------

__device__ __forceinline__ void kSplitTF32(float x, uint32_t& hi, uint32_t& lo)
{
    hi = kFloatToTF32(x);
    lo = kFloatToTF32(x - __uint_as_float(hi));
}

//-------------------------------------------------------------------------------------------

__device__  __forceinline__ void kMatrixMultiplyTF32_16x8x8(
    float& d0, float& d1, float& d2, float& d3,
    uint32_t const& a0, uint32_t const& a1, uint32_t const& a2, uint32_t const& a3,
    uint32_t const& b0, uint32_t const& b1,
    float const& c0, float const& c1, float const& c2, float const& c3)
{
    asm volatile("mma.sync.aligned.m16n8k8.row.col.f32.tf32.tf32.f32 "
        "{%0, %1, %2, %3}, "
        "{%4, %5, %6, %7}, "
        "{%8, %9}, "
        "{%10, %11, %12, %13};\n"
        : "=f"(d0), "=f"(d1), "=f"(d2), "=f"(d3)
        :  "r"(a0),  "r"(a1),  "r"(a2),  "r"(a3),
           "r"(b0),  "r"(b1),
           "f"(c0),  "f"(c1),  "f"(c2),  "f"(c3)
    );
}

//-------------------------------------------------------------------------------------------

__device__ void kSDMTrellisSplitTF32Matrix8x8(const float* __restrict__ inMatrix,
                                              uint32_t* __restrict__ highM,
                                              uint32_t* __restrict__ lowM,
                                              int noRows)
{
    assert(blockDim.x == 32 && "Expected block size of 32");
    for(int idx = 0; idx < (noRows << 3); idx += 32)
    {
        int i = idx + threadIdx.x;
        kSplitTF32(inMatrix[i], highM[i], lowM[i]);
    }
    __syncwarp();
}

//-------------------------------------------------------------------------------------------

__device__ void kSDMTrellisMatrixCalcFP32_16x8x8(const float* __restrict__ A, 
                                                 const uint32_t* __restrict__ hiB, const uint32_t* __restrict__ loB,
                                                 float* __restrict__ D, 
                                                 const int* __restrict__ idxA, const int* __restrict__ idxB, const int* __restrict__ idxC,
                                                 const float xSample)
{
    __shared__ uint32_t loA[16 * 8];
    __shared__ uint32_t hiA[16 * 8];

    float acc[4];
    float cor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    for(int i = 0; i < 4; i++)
    {
        acc[i] = (!(idxC[i] & 0x7)) ? xSample : 0.0f;
    }

    kSDMTrellisSplitTF32Matrix8x8(A, hiA, loA, 16);

    kMatrixMultiplyTF32_16x8x8(cor[0], cor[1], cor[2], cor[3],
                        loA[idxA[0]], loA[idxA[1]], loA[idxA[2]], loA[idxA[3]],
                        hiB[idxB[0]], hiB[idxB[1]],
                        cor[0], cor[1], cor[2], cor[3]);

    kMatrixMultiplyTF32_16x8x8(cor[0], cor[1], cor[2], cor[3],
                        hiA[idxA[0]], hiA[idxA[1]], hiA[idxA[2]], hiA[idxA[3]],
                        loB[idxB[0]], loB[idxB[1]],
                        cor[0], cor[1], cor[2], cor[3]);

    kMatrixMultiplyTF32_16x8x8(acc[0], acc[1], acc[2], acc[3],
                        hiA[idxA[0]], hiA[idxA[1]], hiA[idxA[2]], hiA[idxA[3]],
                        hiB[idxB[0]], hiB[idxB[1]],
                        acc[0], acc[1], acc[2], acc[3]);

    for(int i = 0; i < 4; i++)
    {
        D[idxC[i]] = acc[i] + cor[i];
    }
}

//-------------------------------------------------------------------------------------------

__device__ void kSDMTrellisMMALayout(int* __restrict__ idxA, int* __restrict__ idxB, int* __restrict__ idxC)
{
    // A: M X K
    auto layoutRowA = Layout<Shape<_16, _8>, Stride<_8, _1>>{};
    auto tvA = composition(layoutRowA, Traits::ALayout{});
    // B: N x K
    auto layoutRowB = Layout<Shape<_8, _8>, Stride<_1, _8>>{}; // (n, k) : N x K
    auto tvB = composition(layoutRowB, Traits::BLayout{});
    // C and D : M x N
    auto layoutRowC = Layout<Shape<_16, _8>, Stride<_8, _1>>{};
    auto tvC = composition(layoutRowC, Traits::CLayout{});

    for(int i = 0; i < 4; i++)
    {
        idxA[i] = tvA(threadIdx.x, i);
        idxC[i] = tvC(threadIdx.x, i);
    }
    for(int i = 0; i < 2; i++)
    {
        idxB[i] = tvB(threadIdx.x, i);
    }
}

//-------------------------------------------------------------------------------------------

__device__ void kSDMTrellisNextStateAndCost(float* __restrict__ pState, 
                                            float* __restrict__ pCost,
                                            const float* __restrict__ dMatrix, 
                                            const float* __restrict__ vMatrix, 
                                            float a0)
{
    float* __restrict__ pNextA = pState;
    float* __restrict__ pNextB = &pState[32 * 8];
    int idx = threadIdx.x << 3;

    pNextA[idx] = dMatrix[idx] + 1.0f;
    pNextB[idx] = dMatrix[idx] - 1.0f;
    for(int i = 1; i < 8; i++)
    {
        pNextA[idx + i] = dMatrix[idx + i];
        pNextB[idx + i] = dMatrix[idx + i];
    }

    float vA = vMatrix[idx] + a0;
    vA *= vA;
    pCost[threadIdx.x] += vA;

    float vB = vMatrix[idx] - a0;
    vB *= vB;
    pCost[threadIdx.x + 32] += vB;
}

//-------------------------------------------------------------------------------------------

__global__ void kSDMTrellisTestCalcStateAndCostFP32(SMDTrellisDataCUDA *pTrellis, float xSample)
{
    __shared__ uint32_t matLoAFilter[64], matHiAFilter[64];
    __shared__ uint32_t matLoGFilter[64], matHiGFilter[64];
    float *pAFilter = &(pTrellis->filterA[0][0]);
    float *pGFilter = &(pTrellis->filterG[0][0]);
    kSDMTrellisSplitTF32Matrix8x8(pAFilter, matHiAFilter, matLoAFilter, 8);
    kSDMTrellisSplitTF32Matrix8x8(pGFilter, matHiGFilter, matLoGFilter, 8);

    float *pMatStateCurr = &(pTrellis->states[0][0][0]);

    int idxA[4], idxB[2], idxC[4];
    kSDMTrellisMMALayout(idxA, idxB, idxC);

    __shared__ float dMatrix[32 * 8];
    __shared__ float vMatrix[32 * 8];

    kSDMTrellisMatrixCalcFP32_16x8x8(pMatStateCurr, 
            matHiGFilter, matLoGFilter,
            dMatrix,
            idxA, idxB, idxC,
            xSample);
    kSDMTrellisMatrixCalcFP32_16x8x8(&pMatStateCurr[16 * 8], 
            matHiGFilter, matLoGFilter,
            &dMatrix[16 * 8],
            idxA, idxB, idxC,
            xSample);
    __syncwarp();

    kSDMTrellisMatrixCalcFP32_16x8x8(dMatrix, 
            matHiAFilter, matLoAFilter,
            vMatrix,
            idxA, idxB, idxC,
            xSample);
    kSDMTrellisMatrixCalcFP32_16x8x8(&dMatrix[16 * 8],
            matHiAFilter, matLoAFilter,
            &vMatrix[16 * 8],
            idxA, idxB, idxC,
            xSample);
    __syncwarp();

    kSDMTrellisNextStateAndCost(&(pTrellis->states[1][0][0]), pTrellis->cost, dMatrix, vMatrix, pTrellis->filterA[0][0]);
}

//-------------------------------------------------------------------------------------------

__global__ void kSDMTrellisTestCalcStateAndCostTF32(SMDTrellisDataCUDA *pTrellis, float xSample)
{
    __shared__ uint32_t matrixAFilter[64];
    __shared__ uint32_t matrixGFilter[64];
    float *pAFilter = &(pTrellis->filterA[0][0]);
    float *pGFilter = &(pTrellis->filterG[0][0]);
    int idx;
    
    idx = threadIdx.x << 1;
    for(int i = 0; i < 2; i++)
    {
        matrixAFilter[idx + i] = kFloatToTF32(pAFilter[idx + i]);
        matrixGFilter[idx + i] = kFloatToTF32(pGFilter[idx + i]);
    }

    __shared__ uint32_t mStateCurr[256]; // 32x8
    __shared__ float dMatrix[256]; // 32x8
    __shared__ float cMatrix[128]; // 16x8
    __shared__ float vMatrix[256]; // 32x8
    float *pStateCurr = &(pTrellis->states[0][0][0]);

    idx = threadIdx.x << 3;
    for(int i = 0; i < 8; i++)
    {
        mStateCurr[idx + i] = kFloatToTF32(pStateCurr[idx + i]);
        dMatrix[idx + i] = 0.0f;
        vMatrix[idx + i] = 0.0f;
    }
    idx = threadIdx.x << 2;
    cMatrix[idx] = (!(threadIdx.x & 0x1)) ? xSample : 0.0f;
    for(int i = 1; i < 4; i++)
    {
        cMatrix[idx+i] = 0.0f;
    }

    __syncwarp();
    
    // A: M X K
    auto layoutRowA = Layout<Shape<_16, _8>, Stride<_8, _1>>{};
    auto tvA = composition(layoutRowA, Traits::ALayout{});
    // B: N x K
    auto layoutRowB = Layout<Shape<_8, _8>, Stride<_1, _8>>{}; // (n, k) : N x K
    auto tvB = composition(layoutRowB, Traits::BLayout{});
    // C and D : M x N
    auto layoutRowC = Layout<Shape<_16, _8>, Stride<_8, _1>>{};
    auto tvC = composition(layoutRowC, Traits::CLayout{});

    idx = threadIdx.x;
    kMatrixMultiplyTF32_16x8x8(dMatrix[tvC(idx, 0)], dMatrix[tvC(idx, 1)], dMatrix[tvC(idx, 2)], dMatrix[tvC(idx, 3)],
                   mStateCurr[tvA(idx, 0)], mStateCurr[tvA(idx, 1)], mStateCurr[tvA(idx, 2)], mStateCurr[tvA(idx, 3)], 
                   matrixGFilter[tvB(idx, 0)], matrixGFilter[tvB(idx, 1)],
                   cMatrix[tvC(idx, 0)], cMatrix[tvC(idx, 1)],cMatrix[tvC(idx, 2)],cMatrix[tvC(idx, 3)]);
    
    float *dMatrixP2 = &dMatrix[128];
    uint32_t *mStateCurrP2 = &mStateCurr[128];
    kMatrixMultiplyTF32_16x8x8(dMatrixP2[tvC(idx, 0)], dMatrixP2[tvC(idx, 1)], dMatrixP2[tvC(idx, 2)], dMatrixP2[tvC(idx, 3)],
                   mStateCurrP2[tvA(idx, 0)], mStateCurrP2[tvA(idx, 1)], mStateCurrP2[tvA(idx, 2)], mStateCurrP2[tvA(idx, 3)], 
                   matrixGFilter[tvB(idx, 0)], matrixGFilter[tvB(idx, 1)],
                   cMatrix[tvC(idx, 0)], cMatrix[tvC(idx, 1)],cMatrix[tvC(idx, 2)],cMatrix[tvC(idx, 3)]);

                   
    __syncwarp();

    uint32_t *dMatTF32 = reinterpret_cast<uint32_t *>(dMatrix);
    kMatrixMultiplyTF32_16x8x8(vMatrix[tvC(idx, 0)], vMatrix[tvC(idx, 1)], vMatrix[tvC(idx, 2)], vMatrix[tvC(idx, 3)],
                   dMatTF32[tvA(idx, 0)], dMatTF32[tvA(idx, 1)], dMatTF32[tvA(idx, 2)], dMatTF32[tvA(idx, 3)],
                   matrixAFilter[tvB(idx, 0)], matrixAFilter[tvB(idx, 1)],
                   cMatrix[tvC(idx, 0)], cMatrix[tvC(idx, 1)],cMatrix[tvC(idx, 2)],cMatrix[tvC(idx, 3)]);
    
    float *vMatrixP2 = &vMatrix[128];
    dMatTF32 = reinterpret_cast<uint32_t *>(&dMatrix[128]);
    kMatrixMultiplyTF32_16x8x8(vMatrixP2[tvC(idx, 0)], vMatrixP2[tvC(idx, 1)], vMatrixP2[tvC(idx, 2)], vMatrixP2[tvC(idx, 3)],
                   dMatTF32[tvA(idx, 0)], dMatTF32[tvA(idx, 1)], dMatTF32[tvA(idx, 2)], dMatTF32[tvA(idx, 3)],
                   matrixAFilter[tvB(idx, 0)], matrixAFilter[tvB(idx, 1)],
                   cMatrix[tvC(idx, 0)], cMatrix[tvC(idx, 1)],cMatrix[tvC(idx, 2)],cMatrix[tvC(idx, 3)]);

    __syncwarp();

    kSDMTrellisNextStateAndCost(&(pTrellis->states[1][0][0]), pTrellis->cost, dMatrix, vMatrix, pTrellis->filterA[0][0]);
}

//-------------------------------------------------------------------------------------------
#else
//-------------------------------------------------------------------------------------------

__global__ void kSDMTrellisTestCalcStateAndCostFP32(SMDTrellisDataCUDA *pTrellis, float xSample)
{}

__global__ void kSDMTrellisTestCalcStateAndCostTF32(SMDTrellisDataCUDA *pTrellis, float xSample)
{}

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

void sdmTrellisFreeStatesCUDA(SMDTrellisDataCUDA *states)
{
    if(states != nullptr)
    {
        cudaFree(states);
    }
}

//-------------------------------------------------------------------------------------------

SMDTrellisDataCUDA *sdmTrellisAllocateCUDA()
{
    SMDTrellisDataCUDA *states = nullptr;

    if(cudaMalloc(&states, sizeof(SMDTrellisDataCUDA)) == cudaSuccess)
    {
        cudaMemset(states, 0, sizeof(SMDTrellisDataCUDA));
    }
    return states;
}

//-------------------------------------------------------------------------------------------

bool sdmTrellisCopyDataCUDA(SMDTrellisDataCUDA *pSrc, SMDTrellisDataCUDA *pDest)
{
    cudaMemcpyKind type = memoryCopyKind(pSrc, pDest);
    return (cudaMemcpy(pDest, pSrc, sizeof(SMDTrellisDataCUDA), type) == cudaSuccess) ? true : false;
}

//-------------------------------------------------------------------------------------------

void initSDMTrellisFilterG_CUDA(const double *g, SMDTrellisDataCUDA *pData)
{
    for(int cIdx = 0; cIdx < 8; cIdx++)
    {
        for(int rIdx = 0; rIdx < 8; rIdx++)
        {
            pData->filterG[cIdx][rIdx] = (rIdx == cIdx || (rIdx - 1) == cIdx) ? 1.0f : 0.0f;
        }
    }
    pData->filterG[1][0] = static_cast<float>(0.0 - g[0]);
    pData->filterG[3][2] = static_cast<float>(0.0 - g[2]);
    pData->filterG[5][4] = static_cast<float>(0.0 - g[4]);
    pData->filterG[7][6] = static_cast<float>(0.0 - g[6]);
}

//-------------------------------------------------------------------------------------------

void initSDMTrellisFilterA_CUDA(const double *a, SMDTrellisDataCUDA *pData)
{
    for(int cIdx = 0; cIdx < 8; cIdx++)
    {
        pData->filterA[cIdx][0] = static_cast<float>(a[cIdx]);
        for(int rIdx = 1; rIdx < 8; rIdx++)
        {
            pData->filterA[cIdx][rIdx] = 0.0f;
        }
    }
}

//-------------------------------------------------------------------------------------------

bool initSDMTrellisFilterCUDA(int dsdRate, bool isClans, SMDTrellisDataCUDA *pData)
{
    if(pData == nullptr)
        return false;
    // Ensure the structure is CPU host memory
    if(isPointerForCUDADevice(pData))
        return false;

    const SDMTrellisFilterBase *filter = getTrellisBaseFilter(dsdRate, isClans);
    if(filter == nullptr)
        return false;
    
    initSDMTrellisFilterA_CUDA(filter->a, pData);
    initSDMTrellisFilterG_CUDA(filter->g, pData);

    return true;
}

//-------------------------------------------------------------------------------------------

void testSDMTrellisCalcStateAndCostCUDATF32(SMDTrellisDataCUDA *pTrellis, float xSample)
{
    kSDMTrellisTestCalcStateAndCostTF32<<<1, 32>>>(pTrellis, xSample);
}

//-------------------------------------------------------------------------------------------

void testSDMTrellisCalcStateAndCostCUDAFP32(SMDTrellisDataCUDA *pTrellis, float xSample)
{
    kSDMTrellisTestCalcStateAndCostFP32<<<1, 32>>>(pTrellis, xSample);
}

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omege
//-------------------------------------------------------------------------------------------
