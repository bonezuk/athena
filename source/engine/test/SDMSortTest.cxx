#include "gtest/gtest.h"

#include <string.h>

#include "common/inc/Random.h"
#include "engine/inc/SDMSort.h"
#include "engine/inc/SDMTrellis.h"

using namespace omega;

//-------------------------------------------------------------------------------------------

template <typename T> void testSDMSortOfRandomIndices(int noIters)
{
	constexpr int c_maxNoEntries = 2 * engine::c_maxNoSDMTrellisPaths;
	int *indices;
	T data[c_maxNoEntries];
	common::Random *rand = common::Random::instance();
	rand->seed(1);
	
	for(int iter = 0; iter < noIters; iter++)
	{
		int idx;
		for(idx = 0; idx < c_maxNoEntries; idx++)
		{
			data[idx] = static_cast<T>(rand->randomReal1());
		}
		
		engine::SDMSort<T> sorter;
		
		for(int N = 2; N < c_maxNoEntries; N++)
		{
			indices = const_cast<int *>(sorter.sort(data, N));
			ASSERT_TRUE(indices != nullptr);
			
			for(idx = 0; idx < N - 1; idx++)
			{
				int iA = indices[idx];
				int iB = indices[idx + 1];
				if(data[iA] > data[iB])
				{
					int a = 0;
				}
				ASSERT_LE(data[iA], data[iB]);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

TEST(SDMSort, sortIndicesFloat)
{
	testSDMSortOfRandomIndices<float>(100000);
}

//-------------------------------------------------------------------------------------------

TEST(SDMSort, sortIndicesDouble)
{
	testSDMSortOfRandomIndices<double>(1000);
}

//-------------------------------------------------------------------------------------------

TEST(SDMSort, printStepLayout)
{
	constexpr int N = 32;

	for(int step = 2; step < (N << 1); step <<= 1)
	{
		for(int pos = 0; pos < N; pos += step)
		{
			int left = pos;
			int right = pos + step;
			int diff = right - left;
			int mid = left + (diff >> 1);
			int n1 = mid - left;
			int n2 = right - mid;
			fprintf(stdout, "step=%d, left=%d, right=%d, diff=%d, mid=%d, n1=%d, n2=%d\n", 
				step, left, right, diff, mid, n1, n2);
		}
	}
}

//-------------------------------------------------------------------------------------------
