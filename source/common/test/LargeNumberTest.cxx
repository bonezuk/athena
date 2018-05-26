#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "common/inc/LargeNumber.h"

using namespace orcus::common;
using namespace testing;

//-------------------------------------------------------------------------------------------

class LargeNumberTest : public LargeNumber
{
    public:
        LargeNumberTest();
        void testDMult(tuint32 a,tuint32 b,tuint32& high,tuint32& low) const;
};

LargeNumberTest::LargeNumberTest()
{}

void LargeNumberTest::testDMult(tuint32 a,tuint32 b,tuint32& high,tuint32& low) const
{
	dMult(a,b,high,low);
}

//-------------------------------------------------------------------------------------------

TEST(LargeNumber,dMult)
{

	LargeNumberTest a;
	
    tuint32 tHA,tLA;
    a.testDMult(0,2,tHA,tLA);
    EXPECT_TRUE(tHA==0);
    EXPECT_TRUE(tLA==0);

    tuint32 tHB,tLB;
    a.testDMult(2,0,tHB,tLB);
    EXPECT_TRUE(tHB==0);
    EXPECT_TRUE(tLB==0);

    tuint32 tHC,tLC;
    a.testDMult(10,12,tHC,tLC);
    EXPECT_TRUE(tHC==0);
    EXPECT_TRUE(tLC==120);

    tuint32 tHD,tLD;
    a.testDMult(0x82d7e9d8,0xc609dca3,tHD,tLD);
    EXPECT_TRUE(tHD==1698171193);
    EXPECT_TRUE(tLD==621249672);

    tuint32 tHE,tLE;
    a.testDMult(0xffffffff,0xffffffff,tHE,tLE);
    EXPECT_TRUE(tHE==0xfffffffe);
    EXPECT_TRUE(tLE==1);
}

//-------------------------------------------------------------------------------------------
