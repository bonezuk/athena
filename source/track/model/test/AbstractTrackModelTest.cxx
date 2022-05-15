#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "track/model/test/AbstractTrackModelTest.h"

using namespace omega::track::model;
using namespace testing;


//-------------------------------------------------------------------------------------------

AbstractTrackModelTest::AbstractTrackModelTest() : AbstractTrackModel()
{}

//-------------------------------------------------------------------------------------------

AbstractTrackModelTest::AbstractTrackModelTest(const AbstractTrackModelSPtr& parentItem,const TrackModelKey& filterKey) : AbstractTrackModel(parentItem,filterKey)
{}

//-------------------------------------------------------------------------------------------

TrackModelType AbstractTrackModelTest::type() const
{
	return e_modelUnknown;
}

//-------------------------------------------------------------------------------------------

QVariant AbstractTrackModelTest::data(int sectionIndex,int rowIndex,int columnIndex) const
{
    return QVariant();
}

//-------------------------------------------------------------------------------------------

int AbstractTrackModelTest::size() const
{
	return 0;
}

//-------------------------------------------------------------------------------------------

int AbstractTrackModelTest::numberSections() const
{
	return 0;
}

//-------------------------------------------------------------------------------------------

int AbstractTrackModelTest::numberRowsInSection(int secIdx) const
{
	return 0;
}

//-------------------------------------------------------------------------------------------

bool AbstractTrackModelTest::onAddToDatabase(int albumID,int trackID)
{
	return true;
}

//-------------------------------------------------------------------------------------------

bool AbstractTrackModelTest::onRemoveFromDatabase(int albumID,int trackID)
{
	return true;
}

//-------------------------------------------------------------------------------------------

bool AbstractTrackModelTest::populate()
{
	return true;
}

//-------------------------------------------------------------------------------------------

TEST(AbstractTrackModel,constructorDefault)
{
	AbstractTrackModelTest m;
	EXPECT_FALSE(m.hasParent());
    EXPECT_TRUE(m.parent().data()==0);
	EXPECT_TRUE(m.filterKey().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(AbstractTrackModel,constructorParentAndFilterKey)
{
	TrackModelKey key;
	key.album() = AlbumModelKey(std::pair<bool,int>(false,1));
	key.artist() = "Artist";
	
	AbstractTrackModelSPtr mParent(new AbstractTrackModelTest);
	AbstractTrackModelTest m(mParent,key);
	
	EXPECT_TRUE(m.hasParent());
    EXPECT_TRUE(m.parent().data()==mParent.data());
	
	EXPECT_TRUE(m.filterKey().album()==AlbumModelKey(std::pair<bool,int>(false,1)));
	EXPECT_TRUE(m.filterKey().artist()=="Artist");
	
}

//-------------------------------------------------------------------------------------------
