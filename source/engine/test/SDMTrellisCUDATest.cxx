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

//-------------------------------------------------------------------------------------------

TEST(CUDAHelperFunctions, isPointerForCUDADeviceWhenAllocatedOnHost)
{
    SMDTrellisDataCUDA *pData = new SMDTrellisDataCUDA;
    ASSERT_TRUE(pData != nullptr);
    EXPECT_EQ(isPointerForCUDADevice(pData), 0);
    delete pData;
}

//-------------------------------------------------------------------------------------------

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
    constexpr float c_inSample = 0.5f;
    constexpr float c_tolerance = 0.0001f;

    SDMTrellisBlockFilter<float> *filterTest = getSDMTrellisBlockFilter<float>(c_dsdRate, false);
    SDMTrellisStates<float> *statesA = allocateSDMTrellisStates<float>();
    SDMTrellisStates<float> *statesB = allocateSDMTrellisStates<float>();

    SMDTrellisDataCUDA trellisCPU;
    memset(&trellisCPU, 0, sizeof(SMDTrellisDataCUDA));
    SMDTrellisDataCUDA *pTrellisGPU = sdmTrellisAllocateCUDA();
    ASSERT_TRUE(pTrellisGPU != nullptr);

    ASSERT_TRUE(initSDMTrellisFilterCUDA(c_dsdRate, false, &trellisCPU));
    testerSineWaveThroughStatesCUDA(statesA, &trellisCPU);
    trellisCPU.noCandidates = 32;
    ASSERT_TRUE(sdmTrellisCopyDataCUDA(&trellisCPU, pTrellisGPU));

    ASSERT_TRUE(sdmCalcTrellisBlockFilter4Lanes(statesA, statesB, filterTest, c_inSample, 0));
    ASSERT_TRUE(sdmCalcTrellisBlockFilter4Lanes(statesA, statesB, filterTest, c_inSample, 4));

    testSDMTrellisCalcStateAndCostCUDA(pTrellisGPU, c_inSample);
    ASSERT_TRUE(sdmTrellisCopyDataCUDA(pTrellisGPU, &trellisCPU));

    for(int i = 0; i < 32; i++)
    {
        int d = i >> 3;
        int r = i & 0x7;
        int nIdxA = (d << 4) + r;
        int nIdxB = nIdxA + 8;
        EXPECT_NEAR(trellisCPU.cost[i], statesB->cost[nIdxA], c_tolerance);
        EXPECT_NEAR(trellisCPU.cost[i+32], statesB->cost[nIdxB], c_tolerance);
        for(int j = 0; j < 8; j++)
        {
            EXPECT_NEAR(trellisCPU.states[1][i][j], statesA->states[j][nIdxA], c_tolerance);
            EXPECT_NEAR(trellisCPU.states[1][i+32][j], statesA->states[j][nIdxB], c_tolerance);
        }
    }

    sdmTrellisFreeStatesCUDA(pTrellisGPU);
    freeSDMFreeTrellisBlockFilter<float>(filterTest);
    freeSDMTrellisStates<float>(statesA);
    freeSDMTrellisStates<float>(statesB);
}

//-------------------------------------------------------------------------------------------
