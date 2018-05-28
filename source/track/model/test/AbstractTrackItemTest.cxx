#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "track/model/inc/AbstractTrackItem.h"
#include "track/model/inc/AbstractTrackModel.h"
#include "track/model/test/AbstractTrackModelTest.h"

using namespace orcus::track::model;
using namespace testing;

//-------------------------------------------------------------------------------------------

TEST(AbstractTrackItem,constructorDefault)
{
	AbstractTrackItem item;
	EXPECT_FALSE(item.hasParent());
    EXPECT_TRUE(item.parent().data()==0);
}

//-------------------------------------------------------------------------------------------

TEST(AbstractTrackItem,constructorParent)
{
	AbstractTrackModelSPtr parentItem(new AbstractTrackModelTest);
	AbstractTrackItem item(parentItem);
	EXPECT_TRUE(item.hasParent());
    EXPECT_TRUE(item.parent().data()==parentItem.data());
}

//-------------------------------------------------------------------------------------------
