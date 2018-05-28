//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_MODEL_TEST_IMAGEREPOSITARYMOCK_H
#define __ORCUS_TRACK_MODEL_TEST_IMAGEREPOSITARYMOCK_H
//-------------------------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "track/model/inc/ImageRepositary.h"

using namespace orcus::track::model;

//-------------------------------------------------------------------------------------------

class ImageRepositaryMock : public orcus::track::model::ImageRepositary
{
	public:
		MOCK_METHOD3(getImage,QImage *(int imageID,int iWidth,int iHeight));
		MOCK_METHOD2(getReference,QImage *(int iWidth,int iHeight));
};

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

