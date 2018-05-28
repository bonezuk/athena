//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_AUDIOIO_TEST_AOCHANNELMAPMOCK_H
#define __ORCUS_AUDIOIO_TEST_AOCHANNELMAPMOCK_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOBase.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace orcus::audioio;
using namespace testing;

//-------------------------------------------------------------------------------------------

class AOChannelMapMock : public AOChannelMap
{
	public:
		AOChannelMapMock();
		virtual ~AOChannelMapMock();
		
		MOCK_METHOD1(load,void(const QString& devName));
		MOCK_METHOD1(save,void(const QString& devName));
		MOCK_CONST_METHOD1(channel,int(ChannelType t));
		MOCK_METHOD2(setChannel,void(ChannelType t,int chIdx));
		MOCK_CONST_METHOD0(noChannels,int());
		MOCK_METHOD0(stereoType,AOChannelMap::StereoType());
		MOCK_METHOD1(setStereoType,void(AOChannelMap::StereoType t));
};

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
