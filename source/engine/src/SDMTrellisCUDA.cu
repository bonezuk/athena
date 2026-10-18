//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_ENGINE_SDMTRELLISCUDA_H
#define __OMEGA_ENGINE_SDMTRELLISCUDA_H
//-------------------------------------------------------------------------------------------

#include <cuda_runtime.h>
#include "engine/inc/SDMTrellis.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
//-------------------------------------------------------------------------------------------

typedef struct s_SMDTrellisDataCUDA
{
    // row-major
    float states[2][2 * c_maxNoSDMTrellisPaths][8];
    float cost[2 * c_maxNoSDMTrellisPaths];
    // row-major
    float filterG[8][8];
    // row-major
    float filterA[8][8];
    int noCandidates;
} SMDTrellisDataCUDA;

//-------------------------------------------------------------------------------------------

ENGINE_EXPORT int isPointerForCUDADevice(void *ptr);
ENGINE_EXPORT cudaMemcpyKind memoryCopyKind(void *pSrc, void *pDest);

//-------------------------------------------------------------------------------------------

ENGINE_EXPORT void sdmTrellisFreeStatesCUDA(SMDTrellisDataCUDA *states);
ENGINE_EXPORT SMDTrellisDataCUDA *sdmTrellisAllocateCUDA();
ENGINE_EXPORT bool sdmTrellisCopyDataCUDA(SMDTrellisDataCUDA *pSrc, SMDTrellisDataCUDA *pDest);
ENGINE_EXPORT bool initSDMTrellisFilterCUDA(int dsdRate, bool isClans, SMDTrellisDataCUDA *pData);

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omege
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

#include <cute/tensor.hpp>
#include <cute/layout.hpp>
#include <cute/atom/mma_traits_sm80.hpp>

#include "common/inc/CommonTypes.h"
#include "engine/inc/FFTRadix2Cuda.h"
#include "engine/inc/SDMTrellisCUDA.h"

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

__device__ __forceinline__ uint32_t floatToTF32(float x)
{
    uint32_t r;
    asm("cvt.rna.tf32.f32 %0, %1;\n" : "=r"(r) : "f"(x));
    return r;
}

__device__  __forceline__ void mmaTF32_16x8x8(
    float& d0, float& d1, float& d2, float& d3,
    uint32_t const& a0, uint32_t const& a1, uint32_t const& a2, uint32_t const& a3,
    uint32_t const& b0, uint32_t const& b1,
    float const& c0, float const& c1, float const& c2, float const& c3)
{
#if defined(__OMEGA_CUDA_SM80)
    asm volatile("mmm.sync.aligned.m16n8k8.row.col.f32.tf32.tf32.f32 "
        "{%0, %1, %2, %3}, "
        "{%4, %5, %6, %7}, "
        "{%8, %9}, "
        "{%10, %11, %12, %13};\n"
        : "=f"(d0), "=f"(d1), "=f"(d2), "=f"(d3)
        :  "r"(a0),  "r"(a1),  "r"(a2),  "r"(a3),
           "r"(b0),  "r"(b1),
           "f"(c0),  "f"(c1),  "f"(c2),  "f"(c3)
    );
#endif
}

using Traits = MMA_Traits<SM80_16x8x8_F32TF32TF32F32_TN>;

__global__ void kernelSDMTrellisTestCalcStateAndCost(SMDTrellisDataCUDA *pTrellis, float xSample)
{
    __shared__ uint32_t matrixAFilter[64];
    __shared__ uint32_t matrixGFilter[64];
    float *pAFilter = pTrellis->filterA;
    float *pGFilter = pTrellis->filterG;
    int idx;
    
    idx = threadIdx.x << 1;
    for(int i = 0; i < 2; i++)
    {
        matrixAFilter[idx + i] = floatToTF32(pAFilter[idx + i]);
        matrixGFilter[idx + i] = floatToTF32(pGFilter[idx + i]);
    }

    __shared__ uint32_t mStateCurr[128];
    __shared__ float dMatrix[128];
    __shared__ float cMatrix[128];
    float *pStateCurr = &(pTrellis->states[0][0][0]);

    idx = threadIdx.x;
    for(int i = 0; i < 4; i++)
    {
        mStateCurr[idx + i] = floatToTF32(pStateCurr[idx + i]);
        dMatrix[idx + i] = 0.0f;
    }
    cMatrix[idx] = (!(threadIdx.x & 0x1)) = xSample : 0.0f;
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
    mmaTF32_16x8x8(dMatrix[tvC(idx, 0)], dMatrix[tvC(idx, 1)], dMatrix[tvC(idx, 2)], dMatrix[tvC(idx, 3)],
                   mStateCurr[tvA(idx, 0)], mStateCurr[tvA(idx, 1)], mStateCurr[tvA(idx, 2)], mStateCurr[tvA(idx, 3)], 
                   matrixGFilter[tvB(idx, 0)], matrixGFilter[tvB(idx, 1)],
                   cMatrix[tvC(idx, 0)], cMatrix[tvC(idx, 1)],cMatrix[tvC(idx, 2)],cMatrix[tvC(idx, 3)]);
    
    __syncwarp();

    

}

//-------------------------------------------------------------------------------------------
// Determines if a given pointer is a host or device memory pointer
// res = 1; => pointer is CUDA device memory
// res = 0; => pointer is CPU host memory
// res = -1; => Unable to determine if host or device memory
//-------------------------------------------------------------------------------------------

int isPointerForCUDADevice(void *ptr)
{
    int res = -1;
    cudaPointerAttributes attr;

    if(cudaPointerGetAttributes(&attr, ptr) == cudaSuccess)
    {
        res = (attr.type == cudaMemoryTypeUnregistered) ? 0 : 1;
    }
    return res;
}

//-------------------------------------------------------------------------------------------

cudaMemcpyKind memoryCopyKind(void *pSrc, void *pDest)
{
    cudaMemcpyKind type = cudaMemcpyDefault;
    int sType = isPointerForCUDADevice(pSrc);
    int dType = isPointerForCUDADevice(pDest);
    if(sType >= 0 && dType >= 0)
    {
        if(!sType)
        {
            type = (!dType) ? cudaMemcpyHostToHost : cudaMemcpyHostToDevice;
        }
        else
        {
            type = (!dType) ? cudaMemcpyDeviceToHost : cudaMemcpyDeviceToDevice;
        }
    }
    return type;
}

//-------------------------------------------------------------------------------------------

void sdmTrellisFreeStatesCUDA(SMDTrellisDataCUDA *states)
{
    if(states != nullptr)
    {
        cudaFree(state);
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
            pData->filterG[cIdx][rIdx] = (rIdx == cIdx || (rIdx + 1) == cIdx) ? 1.0f : 0.0f;
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
        pData->filterA[cIdx][0] = static_cast<float>(a);
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
} // namespace engine
} // namespace omege
//-------------------------------------------------------------------------------------------

#include <cuda_runtime.h>
#include "gtest/gtest.h"
#include "engine/inc/SDMTrellisCUDA.h"

using namespace omega::engine;

//-------------------------------------------------------------------------------------------
// CUDA Helper function tests
//-------------------------------------------------------------------------------------------

TEST(CUDAHelperFunctions, isPointerForCUDADeviceWhenAllocatedOnStack)
{
    SMDTrellisDataCUDA data;
    EXPECT_EQ(isPointerForCUDADevice(&data), 0);
}

TEST(CUDAHelperFunctions, isPointerForCUDADeviceWhenAllocatedOnHost)
{
    SMDTrellisDataCUDA *pData = new SMDTrellisDataCUDA;
    ASSERT_TRUE(pData != nullptr);
    EXPECT_EQ(isPointerForCUDADevice(pData), 0);
    delete pData;
}

TEST(CUDAHelperFunctions, isPointerForCUDADeviceWhenAllocatedOnDevice)
{
    SMDTrellisDataCUDA *pData = sdmTrellisAllocateCUDA();
    ASSERT_TRUE(pData != nullptr);
    EXPECT_EQ(isPointerForCUDADevice(pData), 1);
    sdmTrellisFreeStatesCUDA(pData);
}

//-------------------------------------------------------------------------------------------

TEST(CUDAHelperFunctions, memoryCopyKindWhenBothOnHost)
{
    SMDTrellisDataCUDA *pDataA = new SMDTrellisDataCUDA;
    SMDTrellisDataCUDA *pDataB = new SMDTrellisDataCUDA;
    EXPECT_EQ(memoryCopyKind(pDataA, pDataB), cudaMemcpyHostToHost);
    delete pDataA;
    delete pDataB;
}

//-------------------------------------------------------------------------------------------

TEST(CUDAHelperFunctions, memoryCopyKindWhenSourceHostAndDestDevice)
{
    SMDTrellisDataCUDA *pDataA = new SMDTrellisDataCUDA;
    SMDTrellisDataCUDA *pDataB = sdmTrellisAllocateCUDA();
    ASSERT_TRUE(pDataB != nullptr);
    EXPECT_EQ(memoryCopyKind(pDataA, pDataB), cudaMemcpyHostToDevice);
    delete pDataA;
    sdmTrellisFreeStatesCUDA(pDataB);
}

//-------------------------------------------------------------------------------------------

TEST(CUDAHelperFunctions, memoryCopyKindWhenSourceDeviceAndDestHost)
{
    SMDTrellisDataCUDA *pDataB = new SMDTrellisDataCUDA;
    SMDTrellisDataCUDA *pDataA = sdmTrellisAllocateCUDA();
    ASSERT_TRUE(pDataA != nullptr);
    EXPECT_EQ(memoryCopyKind(pDataA, pDataB), cudaMemcpyDeviceToHost);
    delete pDataB;
    sdmTrellisFreeStatesCUDA(pDataA);
}

//-------------------------------------------------------------------------------------------

TEST(CUDAHelperFunctions, memoryCopyKindWhenSourceDeviceAndDestDevice)
{
    SMDTrellisDataCUDA *pDataA = sdmTrellisAllocateCUDA();
    SMDTrellisDataCUDA *pDataB = sdmTrellisAllocateCUDA();
    ASSERT_TRUE(pDataA != nullptr);
    ASSERT_TRUE(pDataB != nullptr);
    EXPECT_EQ(memoryCopyKind(pDataA, pDataB), cudaMemcpyDeviceToDevice);
    sdmTrellisFreeStatesCUDA(pDataA);
    sdmTrellisFreeStatesCUDA(pDataB);
}

//-------------------------------------------------------------------------------------------
// SDMTrellisCUDA tests
//-------------------------------------------------------------------------------------------

void testerSineWaveThroughStatesCUDA(SDMTrellisStates<float> *states, SMDTrellisDataCUDA *pCTrellis)
{
    const double c_period = (2.0 * c_PI_D) / (8.0 * c_maxNoSDMTrellisPaths);

    for(int j = 0; j < 8; j++)
    {
        for(int i = 0; i < c_maxNoSDMTrellisPaths; i++)
        {
            int idx = (j * 8) + i + 1;
            float x = static_cast<float>(sin(c_period * static_cast<double>(idx)));
            states->states[j][i] = x;
            pCTrellis->states[0][i][j] = x;
        }
    }
    for(int i = 0; i < c_maxNoSDMTrellisPaths; i++)
    {
        states->cost[i] = 0.0f;
        pCTrellis->cost[i] = 0.0f;
    }
}

//-------------------------------------------------------------------------------------------

TEST(SDMTrellisCUDA, calculateStateAndCostUsingTensorCore)
{
    constexpr int c_dsdRate = 64;

    SDMTrellisStates<float> *statesA = allocateSDMTrellisStates<float>();

    SMDTrellisDataCUDA trellisCPU;
    memset(&trellisCPU, 0, sizeof(SMDTrellisDataCUDA));
    SMDTrellisDataCUDA *pTrellisGPU = sdmTrellisAllocateStatesCUDA();
    ASSERT_TRUE(pTrellisGPU != nullptr);

    ASSERT_TRUE(initSDMTrellisFilterCUDA(c_dsdRate, false, &trellisCPU));
    testerSineWaveThroughStatesCUDA(statesA, &trellisCPU);
    trellisCPU.noCandidates = 32;
    ASSERT_TRUE(sdmTrellisCopyDataCUDA(&trellisCPU, pTrellisGPU));


    sdmTrellisFreeStatesCUDA(pTrellisGPU);
    freeSDMTrellisStates<float>(stateA);
}

//-------------------------------------------------------------------------------------------