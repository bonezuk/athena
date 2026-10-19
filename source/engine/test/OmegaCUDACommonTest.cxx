#include <cuda_runtime.h>
#include "gtest/gtest.h"
#include "engine/inc/OmegaCUDACommon.h"
#include "engine/inc/SDMTrellisCUDA.h"

using namespace omega::engine;

//-------------------------------------------------------------------------------------------

class OmegaCUDACommonTest : public ::testing::Test 
{
protected:
    static void SetUpTestSuite() {
        cuda_init_result_ = initCUDAOmega();

        if (cuda_init_result_ >= 0) {
            std::cout << "[INFO] CUDA device initialized successfully (result = "
                      << cuda_init_result_ << ")." << std::endl;
        } else {
            std::cout << "[WARN] No CUDA device available (initCUDAOmega() returned "
                      << cuda_init_result_ << "). CUDA tests will be skipped." << std::endl;
        }
    }

    void SetUp() override {
        if (cuda_init_result_ < 0) {
            GTEST_SKIP() << "CUDA device not available or initialization failed "
                         << "(initCUDAOmega() returned " << cuda_init_result_ << ")";
        }
    }

    static void TearDownTestSuite() {}

private:
    static int cuda_init_result_;
};

int OmegaCUDACommonTest::cuda_init_result_ = -1;

//-------------------------------------------------------------------------------------------
// CUDA Helper function tests
//-------------------------------------------------------------------------------------------

TEST_F(OmegaCUDACommonTest, isPointerForCUDADeviceWhenAllocatedOnStack)
{
    SMDTrellisDataCUDA data;
    EXPECT_EQ(isPointerForCUDADevice(&data), 0);
}

//-------------------------------------------------------------------------------------------

TEST_F(OmegaCUDACommonTest, isPointerForCUDADeviceWhenAllocatedOnHost)
{
    SMDTrellisDataCUDA *pData = new SMDTrellisDataCUDA;
    ASSERT_TRUE(pData != nullptr);
    EXPECT_EQ(isPointerForCUDADevice(pData), 0);
    delete pData;
}

//-------------------------------------------------------------------------------------------

TEST_F(OmegaCUDACommonTest, isPointerForCUDADeviceWhenAllocatedOnDevice)
{
    SMDTrellisDataCUDA *pData = sdmTrellisAllocateCUDA();
    ASSERT_TRUE(pData != nullptr);
    EXPECT_EQ(isPointerForCUDADevice(pData), 1);
    sdmTrellisFreeStatesCUDA(pData);
}

//-------------------------------------------------------------------------------------------

TEST_F(OmegaCUDACommonTest, memoryCopyKindWhenBothOnHost)
{
    SMDTrellisDataCUDA *pDataA = new SMDTrellisDataCUDA;
    SMDTrellisDataCUDA *pDataB = new SMDTrellisDataCUDA;
    EXPECT_EQ(memoryCopyKind(pDataA, pDataB), cudaMemcpyHostToHost);
    delete pDataA;
    delete pDataB;
}

//-------------------------------------------------------------------------------------------

TEST_F(OmegaCUDACommonTest, memoryCopyKindWhenSourceHostAndDestDevice)
{
    SMDTrellisDataCUDA *pDataA = new SMDTrellisDataCUDA;
    SMDTrellisDataCUDA *pDataB = sdmTrellisAllocateCUDA();
    ASSERT_TRUE(pDataB != nullptr);
    EXPECT_EQ(memoryCopyKind(pDataA, pDataB), cudaMemcpyHostToDevice);
    delete pDataA;
    sdmTrellisFreeStatesCUDA(pDataB);
}

//-------------------------------------------------------------------------------------------

TEST_F(OmegaCUDACommonTest, memoryCopyKindWhenSourceDeviceAndDestHost)
{
    SMDTrellisDataCUDA *pDataB = new SMDTrellisDataCUDA;
    SMDTrellisDataCUDA *pDataA = sdmTrellisAllocateCUDA();
    ASSERT_TRUE(pDataA != nullptr);
    EXPECT_EQ(memoryCopyKind(pDataA, pDataB), cudaMemcpyDeviceToHost);
    delete pDataB;
    sdmTrellisFreeStatesCUDA(pDataA);
}

//-------------------------------------------------------------------------------------------

TEST_F(OmegaCUDACommonTest, memoryCopyKindWhenSourceDeviceAndDestDevice)
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
