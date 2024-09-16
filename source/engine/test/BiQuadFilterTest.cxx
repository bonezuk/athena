#include "gtest/gtest.h"

#include "engine/inc/BiQuadFilter.h"

using namespace omega::engine;

//-------------------------------------------------------------------------------------------

TEST(BiQuadFilter, allPassFirstOrder)
{
	const sample_t x[5] = { -0.25, 0.25, 0.5, 0.75, -0.5 };

	const sample_t fc = 10000.0;
	const sample_t fs = 44100.0;
	sample_t a = (::tan(c_PI_D * fc / fs) - 1.0) / (tan(c_PI_D*fc / fs) + 1.0);

    BiQuadFilter f = BiQuadFilter::allPassFirstOrder(fc, fs);
	EXPECT_EQ(f.type(), BiQuadFilter::e_AllPass_FirstOrder);
	EXPECT_DOUBLE_EQ(f.a0(), a);
	EXPECT_DOUBLE_EQ(f.a1(), 1.0);
	EXPECT_DOUBLE_EQ(f.a2(), 0.0);
	EXPECT_DOUBLE_EQ(f.b1(), a);
	EXPECT_DOUBLE_EQ(f.b2(), 0.0);
	EXPECT_DOUBLE_EQ(f.process(x[0]), 0.018287231797256782);
	EXPECT_DOUBLE_EQ(f.process(x[1]), -0.26694954041003038);
	EXPECT_DOUBLE_EQ(f.process(x[2]), 0.19389846391088889);
	EXPECT_DOUBLE_EQ(f.process(x[3]), 0.45932176922691148);
	EXPECT_DOUBLE_EQ(f.process(x[4]), 0.82017335824802806);
}

//-------------------------------------------------------------------------------------------
