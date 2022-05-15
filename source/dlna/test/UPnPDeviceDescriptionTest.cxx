#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "dlna/inc/UPnPDeviceDescription.h"

using namespace omega::dlna;
using namespace testing;

//-------------------------------------------------------------------------------------------

class UPnPSSDeviceDescriptionTest : public UPnPServiceDescription
{
	public:
		MOCK_METHOD1(parse,void(xmlNodePtr pNode));
};

//-------------------------------------------------------------------------------------------

class UPnPDeviceDescriptionTest : public UPnPDeviceDescription
{
	public:
		MOCK_CONST_METHOD1(getTextOfNode,QString(xmlNodePtr pNode));
		MOCK_CONST_METHOD1(getTextOfElement,QString(xmlNodePtr pElement));
		MOCK_CONST_METHOD1(getNameOfNode,QString(xmlNodePtr pNode));
		MOCK_CONST_METHOD1(isElementNode,bool(xmlNodePtr pNode));
        MOCK_METHOD1(parse,void(xmlNodePtr pNode));
		
		virtual bool testIsRootNode(xmlNodePtr pNode) const;
		virtual void testProcessNode(xmlNodePtr pNode);
		virtual void testProcessNodeService(xmlNodePtr pNode);
	protected:
        virtual UPnPServiceDescriptionSPtr createServiceDescription() const;
};

//-------------------------------------------------------------------------------------------

bool UPnPDeviceDescriptionTest::testIsRootNode(xmlNodePtr pNode) const
{
	return isRootNode(pNode);
}

//-------------------------------------------------------------------------------------------

void UPnPDeviceDescriptionTest::testProcessNode(xmlNodePtr pNode)
{
	processNode(pNode);
}

//-------------------------------------------------------------------------------------------

void UPnPDeviceDescriptionTest::testProcessNodeService(xmlNodePtr pNode)
{
	m_state++;
	processNode(pNode);
	m_state--;
}

//-------------------------------------------------------------------------------------------

UPnPServiceDescriptionSPtr UPnPDeviceDescriptionTest::createServiceDescription() const
{
	UPnPSSDeviceDescriptionTest *pMockService = new UPnPSSDeviceDescriptionTest;
	UPnPServiceDescriptionSPtr pService(pMockService);
    EXPECT_CALL(*pMockService,parse(testing::_)).Times(1);
	return pService;
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,isRootWhenNotElementNode)
{
	xmlNode node;
	UPnPDeviceDescriptionTest vA;
	EXPECT_CALL(vA,isElementNode(&node)).Times(1).WillOnce(Return(false));
	EXPECT_FALSE(vA.testIsRootNode(&node));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,isRootNodeWhenNotDevice)
{
	xmlNode node;
	UPnPDeviceDescriptionTest vA;
	EXPECT_CALL(vA,isElementNode(&node)).Times(1).WillOnce(Return(true));
	EXPECT_CALL(vA,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("Null")));
	EXPECT_FALSE(vA.testIsRootNode(&node));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,isRootNodeWhenNodeIsDevice)
{
	xmlNode node;
	UPnPDeviceDescriptionTest vA;
	EXPECT_CALL(vA,isElementNode(&node)).Times(1).WillOnce(Return(true));
	EXPECT_CALL(vA,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("Device")));
	EXPECT_TRUE(vA.testIsRootNode(&node));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,constructorDefault)
{
	UPnPDeviceDescription d;
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeDeviceType)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("deviceType")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("urn:schemas-upnp-org:device:tvdevice:1")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType()=="urn:schemas-upnp-org:device:tvdevice:1");
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeFriendlyName)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("friendlyName")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("UPnP Television Emulator")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName()=="UPnP Television Emulator");
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeManufacturer)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("manufacturer")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("TV Manufacturer Name")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer()=="TV Manufacturer Name");
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeManufacturerURL)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("manufacturerURL")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("http://www.manufacturer.com")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL()=="http://www.manufacturer.com");
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeModelDescription)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("modelDescription")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("UPnP Television Device Emulator 1.0")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription()=="UPnP Television Device Emulator 1.0");
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeModelName)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("modelName")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("TVEmulator")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName()=="TVEmulator");
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeModelNumber)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("modelNumber")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("1.0")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber()=="1.0");
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeModelURL)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("modelURL")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("http://www.manufacturer.com/TVEmulator/")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL()=="http://www.manufacturer.com/TVEmulator/");
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeSerialNumber)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("serialNumber")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("123456789001")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber()=="123456789001");
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeUDN)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("UDN")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("uuid:Upnp-TVEmulator-1_0-1234567890001")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn()=="uuid:Upnp-TVEmulator-1_0-1234567890001");
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeUPC)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("UPC")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("123456789")));
    d.testProcessNode(&node);
	EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc()=="123456789");
	EXPECT_TRUE(d.presentationURL().isEmpty());
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodePresentationURL)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("presentationURL")));
	EXPECT_CALL(d,getTextOfElement(&node)).Times(1).WillOnce(Return(QString::fromUtf8("/tvdevicepres.html")));
    d.testProcessNode(&node);
    EXPECT_TRUE(d.deviceType().isEmpty());
	EXPECT_TRUE(d.friendlyName().isEmpty());
	EXPECT_TRUE(d.manufacturer().isEmpty());
	EXPECT_TRUE(d.manufacturerURL().isEmpty());
	EXPECT_TRUE(d.modelDescription().isEmpty());
	EXPECT_TRUE(d.modelName().isEmpty());
	EXPECT_TRUE(d.modelNumber().isEmpty());
	EXPECT_TRUE(d.modelURL().isEmpty());
	EXPECT_TRUE(d.serialNumber().isEmpty());
	EXPECT_TRUE(d.udn().isEmpty());
	EXPECT_TRUE(d.upc().isEmpty());
	EXPECT_TRUE(d.presentationURL()=="/tvdevicepres.html");
	EXPECT_TRUE(d.services().isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeServiceList)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("serviceList")));
	EXPECT_CALL(d,parse(&node)).Times(1);
    d.testProcessNode(&node);
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,processNodeService)
{
	xmlNode node;
	UPnPDeviceDescriptionTest d;
	EXPECT_CALL(d,getNameOfNode(&node)).Times(1).WillOnce(Return(QString::fromUtf8("service")));
    d.testProcessNodeService(&node);
	EXPECT_TRUE(d.services().size()==1);
}

//-------------------------------------------------------------------------------------------

TEST(UPnPDeviceDescription,parseIntegrationXML)
{
	QString s;
    s  = "<?xml version=\"1.0\"?>";
	s += "<device>";
	s += "<deviceType>urn:schemas-upnp-org:device:tvdevice:1</deviceType>";
	s += "<friendlyName>UPnP Television Emulator</friendlyName>";
	s += "<manufacturer>TV Manufacturer Name</manufacturer>";
	s += "<manufacturerURL>http://www.manufacturer.com</manufacturerURL>";
	s += "<modelDescription>UPnP Television Device Emulator 1.0</modelDescription>";
	s += "<modelName>TVEmulator</modelName>";
	s += "<modelNumber>1.0</modelNumber>";
	s += "<modelURL>http://www.manufacturer.com/TVEmulator/</modelURL>";
	s += "<serialNumber>123456789001</serialNumber>";
	s += "<UDN>uuid:Upnp-TVEmulator-1_0-1234567890001</UDN>";
	s += "<UPC>123456789</UPC>";
	s += "<serviceList>";
	s += "<service>";
	s += "<serviceType>urn:schemas-upnp-org:service:tvcontrol:1</serviceType>";
	s += "<serviceId>urn:upnp-org:serviceId:tvcontrol1</serviceId>";
	s += "<controlURL>/upnp/control/tvcontrol1</controlURL>";
	s += "<eventSubURL>/upnp/event/tvcontrol1</eventSubURL>";
	s += "<SCPDURL>/tvcontrolSCPD.xml</SCPDURL>";
	s += "</service>";
	s += "<service>";
	s += "<serviceType>urn:schemas-upnp-org:service:tvpicture:1</serviceType>";
	s += "<serviceId>urn:upnp-org:serviceId:tvpicture1</serviceId>";
	s += "<controlURL>/upnp/control/tvpicture1</controlURL>";
	s += "<eventSubURL>/upnp/event/tvpicture1</eventSubURL>";
	s += "<SCPDURL>/tvpictureSCPD.xml</SCPDURL>";
	s += "</service>";
	s += "</serviceList>";
	s += "<presentationURL>/tvdevicepres.html</presentationURL>";
	s += "</device>";

	XMLLibIFSPtr pAPI = XMLLibIF::instance("xml");

	QByteArray inputArray = s.toUtf8().data();
	xmlDocPtr doc = ::xmlParseMemory(inputArray.data(),inputArray.size());
	ASSERT_TRUE(doc!=0);
	xmlNodePtr root = ::xmlDocGetRootElement(doc);
	ASSERT_TRUE(root!=0);
	
    UPnPDeviceDescription d;
    d.parse(root);

	EXPECT_TRUE(d.deviceType()=="urn:schemas-upnp-org:device:tvdevice:1");
	EXPECT_TRUE(d.friendlyName()=="UPnP Television Emulator");
	EXPECT_TRUE(d.manufacturer()=="TV Manufacturer Name");
	EXPECT_TRUE(d.manufacturerURL()=="http://www.manufacturer.com");
	EXPECT_TRUE(d.modelDescription()=="UPnP Television Device Emulator 1.0");
	EXPECT_TRUE(d.modelName()=="TVEmulator");
	EXPECT_TRUE(d.modelNumber()=="1.0");
	EXPECT_TRUE(d.modelURL()=="http://www.manufacturer.com/TVEmulator/");
	EXPECT_TRUE(d.serialNumber()=="123456789001");
	EXPECT_TRUE(d.udn()=="uuid:Upnp-TVEmulator-1_0-1234567890001");
	EXPECT_TRUE(d.upc()=="123456789");
	EXPECT_TRUE(d.presentationURL()=="/tvdevicepres.html");

	EXPECT_TRUE(d.services().size()==2);

	UPnPServiceDescriptionSPtr service = d.services().at(0);
	EXPECT_TRUE(service->serviceType()=="urn:schemas-upnp-org:service:tvcontrol:1");
	EXPECT_TRUE(service->serviceId()=="urn:upnp-org:serviceId:tvcontrol1");
	EXPECT_TRUE(service->controlURL()=="/upnp/control/tvcontrol1");
	EXPECT_TRUE(service->eventSubURL()=="/upnp/event/tvcontrol1");
	EXPECT_TRUE(service->scpdURL()=="/tvcontrolSCPD.xml");	

    UPnPServiceDescriptionSPtr serviceB = d.services().at(1);
    EXPECT_TRUE(serviceB->serviceType()=="urn:schemas-upnp-org:service:tvpicture:1");
    EXPECT_TRUE(serviceB->serviceId()=="urn:upnp-org:serviceId:tvpicture1");
    EXPECT_TRUE(serviceB->controlURL()=="/upnp/control/tvpicture1");
    EXPECT_TRUE(serviceB->eventSubURL()=="/upnp/event/tvpicture1");
    EXPECT_TRUE(serviceB->scpdURL()=="/tvpictureSCPD.xml");
	
	::xmlFreeDoc(doc);
	XMLLibIF::release();
}

//-------------------------------------------------------------------------------------------
