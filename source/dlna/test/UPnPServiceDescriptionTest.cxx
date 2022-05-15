#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "dlna/inc/UPnPServiceDescription.h"

using namespace omega::dlna;
using namespace testing;

//-------------------------------------------------------------------------------------------

class UPnPServiceDescriptionTest : public UPnPServiceDescription
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

bool UPnPServiceDescriptionTest::testIsRootNode(xmlNodePtr pNode) const
{
	return isRootNode(pNode);
}

//-------------------------------------------------------------------------------------------

void UPnPServiceDescriptionTest::testProcessNode(xmlNodePtr pNode)
{
	processNode(pNode);
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,constructorDefault)
{
	UPnPServiceDescription service;
	EXPECT_TRUE(service.serviceType().isEmpty());
	EXPECT_TRUE(service.serviceId().isEmpty());
	EXPECT_TRUE(service.controlURL().isEmpty());
	EXPECT_TRUE(service.eventSubURL().isEmpty());
	EXPECT_TRUE(service.scpdURL().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,isRootWhenNotElementNode)
{
	xmlNode node;
	UPnPServiceDescriptionTest vA;
	EXPECT_CALL(vA,isElementNode(&node)).Times(1).WillOnce(Return(false));
	EXPECT_FALSE(vA.testIsRootNode(&node));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,isRootNodeWhenNotService)
{
	xmlNode node;
	UPnPServiceDescriptionTest vA;
	EXPECT_CALL(vA,isElementNode(&node)).Times(1).WillOnce(Return(true));
	EXPECT_CALL(vA,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("Null")));
	EXPECT_FALSE(vA.testIsRootNode(&node));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,isRootNodeWhenNodeIsService)
{
	xmlNode node;
	UPnPServiceDescriptionTest vA;
	EXPECT_CALL(vA,isElementNode(&node)).Times(1).WillOnce(Return(true));
	EXPECT_CALL(vA,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("Service")));
	EXPECT_TRUE(vA.testIsRootNode(&node));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,processNodeGivenSurplusNode)
{
	xmlNode node;
	UPnPServiceDescriptionTest service;
	EXPECT_CALL(service,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("Null")));
	service.testProcessNode(&node);
	EXPECT_TRUE(service.serviceType().isEmpty());
	EXPECT_TRUE(service.serviceId().isEmpty());
	EXPECT_TRUE(service.controlURL().isEmpty());
	EXPECT_TRUE(service.eventSubURL().isEmpty());
	EXPECT_TRUE(service.scpdURL().isEmpty());	
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,processNodeGivenServiceTypeNode)
{
	xmlNode node;
	UPnPServiceDescriptionTest service;
	EXPECT_CALL(service,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("ServiceType")));
	EXPECT_CALL(service,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("urn:schemas-upnp-org:service:tvcontrol:1")));
	service.testProcessNode(&node);
	EXPECT_TRUE(service.serviceType()=="urn:schemas-upnp-org:service:tvcontrol:1");
	EXPECT_TRUE(service.serviceId().isEmpty());
	EXPECT_TRUE(service.controlURL().isEmpty());
	EXPECT_TRUE(service.eventSubURL().isEmpty());
	EXPECT_TRUE(service.scpdURL().isEmpty());	
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,processNodeGivenServiceIdNode)
{
	xmlNode node;
	UPnPServiceDescriptionTest service;
	EXPECT_CALL(service,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("ServiceId")));
	EXPECT_CALL(service,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("urn:upnp-org:serviceId:tvcontrol1")));
	service.testProcessNode(&node);
	EXPECT_TRUE(service.serviceType().isEmpty());
	EXPECT_TRUE(service.serviceId()=="urn:upnp-org:serviceId:tvcontrol1");
	EXPECT_TRUE(service.controlURL().isEmpty());
	EXPECT_TRUE(service.eventSubURL().isEmpty());
	EXPECT_TRUE(service.scpdURL().isEmpty());	
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,processNodeGivenControlURLNode)
{
	xmlNode node;
	UPnPServiceDescriptionTest service;
	EXPECT_CALL(service,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("controlURL")));
	EXPECT_CALL(service,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("/upnp/control/tvcontrol1")));
	service.testProcessNode(&node);
	EXPECT_TRUE(service.serviceType().isEmpty());
	EXPECT_TRUE(service.serviceId().isEmpty());
	EXPECT_TRUE(service.controlURL()=="/upnp/control/tvcontrol1");
	EXPECT_TRUE(service.eventSubURL().isEmpty());
	EXPECT_TRUE(service.scpdURL().isEmpty());	
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,processNodeGivenEventSubURLNode)
{
	xmlNode node;
	UPnPServiceDescriptionTest service;
	EXPECT_CALL(service,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("eventSubURL")));
	EXPECT_CALL(service,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("/upnp/event/tvcontrol1")));
	service.testProcessNode(&node);
	EXPECT_TRUE(service.serviceType().isEmpty());
	EXPECT_TRUE(service.serviceId().isEmpty());
	EXPECT_TRUE(service.controlURL().isEmpty());
	EXPECT_TRUE(service.eventSubURL()=="/upnp/event/tvcontrol1");
	EXPECT_TRUE(service.scpdURL().isEmpty());	
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,processNodeGivenSCPDURLNode)
{
	xmlNode node;
	UPnPServiceDescriptionTest service;
	EXPECT_CALL(service,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("SCPDURL")));
	EXPECT_CALL(service,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("/tvcontrolSCPD.xml")));
	service.testProcessNode(&node);
	EXPECT_TRUE(service.serviceType().isEmpty());
	EXPECT_TRUE(service.serviceId().isEmpty());
	EXPECT_TRUE(service.controlURL().isEmpty());
	EXPECT_TRUE(service.eventSubURL().isEmpty());
	EXPECT_TRUE(service.scpdURL()=="/tvcontrolSCPD.xml");	
}

//-------------------------------------------------------------------------------------------

TEST(UPnPServiceDescription,parseIntegrationXML)
{
	QString s;
	s  = "<?xml version=\"1.0\"?>";
	s += "<service>";
	s += "<serviceType>urn:schemas-upnp-org:service:tvcontrol:1</serviceType>";
	s += "<serviceId>urn:upnp-org:serviceId:tvcontrol1</serviceId>";
	s += "<controlURL>/upnp/control/tvcontrol1</controlURL>";
	s += "<eventSubURL>/upnp/event/tvcontrol1</eventSubURL>";
	s += "<SCPDURL>/tvcontrolSCPD.xml</SCPDURL>";
	s += "</service>";
	
	XMLLibIFSPtr pAPI = XMLLibIF::instance("xml");

	QByteArray inputArray = s.toUtf8().data();
	xmlDocPtr doc = ::xmlParseMemory(inputArray.data(),inputArray.size());
	ASSERT_TRUE(doc!=0);
	xmlNodePtr root = ::xmlDocGetRootElement(doc);
	ASSERT_TRUE(root!=0);
	
	UPnPServiceDescription service;
	service.parse(root);

	EXPECT_TRUE(service.serviceType()=="urn:schemas-upnp-org:service:tvcontrol:1");
	EXPECT_TRUE(service.serviceId()=="urn:upnp-org:serviceId:tvcontrol1");
	EXPECT_TRUE(service.controlURL()=="/upnp/control/tvcontrol1");
	EXPECT_TRUE(service.eventSubURL()=="/upnp/event/tvcontrol1");
	EXPECT_TRUE(service.scpdURL()=="/tvcontrolSCPD.xml");	
	
	::xmlFreeDoc(doc);
	XMLLibIF::release();
}

//-------------------------------------------------------------------------------------------
