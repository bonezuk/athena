#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "dlna/inc/UPnPSpecVersion.h"

using namespace omega::dlna;
using namespace testing;

//-------------------------------------------------------------------------------------------

class UPnPSpecVersionTest : public UPnPSpecVersion
{
	public:
		MOCK_CONST_METHOD1(getTextOfNode,QString(xmlNodePtr pNode));
		MOCK_CONST_METHOD1(getTextOfElement,QString(xmlNodePtr pElement));
		MOCK_CONST_METHOD1(getNameOfNode,QString(xmlNodePtr pNode));
		MOCK_CONST_METHOD1(isElementNode,bool(xmlNodePtr pNode));
		
		virtual bool testIsRootNode(xmlNodePtr pNode) const;
		virtual void testProcessNode(xmlNodePtr pNode);
};

//-------------------------------------------------------------------------------------------

bool UPnPSpecVersionTest::testIsRootNode(xmlNodePtr pNode) const
{
	return isRootNode(pNode);
}

//-------------------------------------------------------------------------------------------

void UPnPSpecVersionTest::testProcessNode(xmlNodePtr pNode)
{
	processNode(pNode);
}

//-------------------------------------------------------------------------------------------

TEST(UPnPSpecVersion,constructorDefault)
{
	UPnPSpecVersion v;
	EXPECT_TRUE(::isEqual(v.version(),1.0f));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPSpecVersion,constructorVersion)
{
	UPnPSpecVersion vA(0,0);
	EXPECT_TRUE(::isEqual(vA.version(),0.0f));
	UPnPSpecVersion vB(0,5);
	EXPECT_TRUE(::isEqual(vB.version(),0.5f));
	UPnPSpecVersion vC(2,1);
	EXPECT_TRUE(::isEqual(vC.version(),2.1f));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPSpecVersion,isRootWhenNotElementNode)
{
	xmlNode node;
	UPnPSpecVersionTest vA;
	EXPECT_CALL(vA,isElementNode(&node)).Times(1).WillOnce(Return(false));
	EXPECT_FALSE(vA.testIsRootNode(&node));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPSpecVersion,isRootNodeWhenNotSpecVersion)
{
	xmlNode node;
	UPnPSpecVersionTest vA;
	EXPECT_CALL(vA,isElementNode(&node)).Times(1).WillOnce(Return(true));
	EXPECT_CALL(vA,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("Null")));
	EXPECT_FALSE(vA.testIsRootNode(&node));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPSpecVersion,isRootNodeWhenNodeIsSpecVersion)
{
	xmlNode node;
	UPnPSpecVersionTest vA;
	EXPECT_CALL(vA,isElementNode(&node)).Times(1).WillOnce(Return(true));
	EXPECT_CALL(vA,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("SpecVersion")));
	EXPECT_TRUE(vA.testIsRootNode(&node));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPSpecVersion,processNodeGivenSurplusNode)
{
	xmlNode node;
	UPnPSpecVersionTest vA;
	EXPECT_CALL(vA,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("Null")));
	vA.testProcessNode(&node);
	EXPECT_TRUE(::isEqual(vA.version(),1.0f));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPSpecVersion,processNodeGivenMajorNode)
{
	xmlNode node;
	UPnPSpecVersionTest vA;
	EXPECT_CALL(vA,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("Major")));
	EXPECT_CALL(vA,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("2")));
	vA.testProcessNode(&node);
	EXPECT_TRUE(::isEqual(vA.version(),2.0f));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPSpecVersion,processNodeGivenMinorNode)
{
	xmlNode node;
	UPnPSpecVersionTest vA;
	EXPECT_CALL(vA,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("Minor")));
	EXPECT_CALL(vA,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("3")));
	vA.testProcessNode(&node);
	EXPECT_TRUE(::isEqual(vA.version(),1.3f));	
}

//-------------------------------------------------------------------------------------------

TEST(UPnPSpecVersion,parseIntegrationXML)
{
	QString s;
	s  = "<?xml version=\"1.0\"?>";
	s += "<specVersion>";
	s += "<major>2</major>";
	s += "<minor>3</minor>";
	s += "</specVersion>";
	
	XMLLibIFSPtr pAPI = XMLLibIF::instance("xml");

	QByteArray inputArray = s.toUtf8().data();
	xmlDocPtr doc = ::xmlParseMemory(inputArray.data(),inputArray.size());
	ASSERT_TRUE(doc!=0);
	xmlNodePtr root = ::xmlDocGetRootElement(doc);
	ASSERT_TRUE(root!=0);
	
	UPnPSpecVersion vA;
	vA.parse(root);
	
	EXPECT_TRUE(::isEqual(vA.version(),2.3f));
	
	::xmlFreeDoc(doc);
	XMLLibIF::release();
}

//-------------------------------------------------------------------------------------------
