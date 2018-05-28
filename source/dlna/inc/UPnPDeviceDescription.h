//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DLNA_UPNPDEVICEDESCRIPTION_H
#define __ORCUS_DLNA_UPNPDEVICEDESCRIPTION_H
//-------------------------------------------------------------------------------------------

#include "dlna/inc/UPnPServiceDescription.h"

//-------------------------------------------------------------------------------------------
// <?xml version="1.0"?>
//  <device>
//    <deviceType>urn:schemas-upnp-org:device:tvdevice:1</deviceType>
//    <friendlyName>UPnP Television Emulator</friendlyName>
//    <manufacturer>TV Manufacturer Name</manufacturer>
//    <manufacturerURL>http://www.manufacturer.com</manufacturerURL>
//    <modelDescription>UPnP Television Device Emulator 1.0</modelDescription>
//    <modelName>TVEmulator</modelName>
//    <modelNumber>1.0</modelNumber>
//    <modelURL>http://www.manufacturer.com/TVEmulator/</modelURL>
//    <serialNumber>123456789001</serialNumber>
//    <UDN>uuid:Upnp-TVEmulator-1_0-1234567890001</UDN>
//    <UPC>123456789</UPC>
//    <serviceList>
//      <service>
//        <serviceType>urn:schemas-upnp-org:service:tvcontrol:1</serviceType>
//        <serviceId>urn:upnp-org:serviceId:tvcontrol1</serviceId>
//        <controlURL>/upnp/control/tvcontrol1</controlURL>
//        <eventSubURL>/upnp/event/tvcontrol1</eventSubURL>
//        <SCPDURL>/tvcontrolSCPD.xml</SCPDURL>
//      </service>
//      <service>
//        <serviceType>urn:schemas-upnp-org:service:tvpicture:1</serviceType>
//        <serviceId>urn:upnp-org:serviceId:tvpicture1</serviceId>
//        <controlURL>/upnp/control/tvpicture1</controlURL>
//        <eventSubURL>/upnp/event/tvpicture1</eventSubURL>
//        <SCPDURL>/tvpictureSCPD.xml</SCPDURL>
//      </service>
//    </serviceList>
//    <presentationURL>/tvdevicepres.html</presentationURL>
//  </device>
//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

class DLNA_EXPORT UPnPDeviceDescription : public XMLNodeParser
{
	public:
	
		static const tchar *c_keyDevice;
		static const tchar *c_keyDeviceType;
		static const tchar *c_keyFriendlyName;
		static const tchar *c_keyManufacturer;
		static const tchar *c_keyManufacturerURL;
		static const tchar *c_keyModelDescription;
		static const tchar *c_keyModelName;
		static const tchar *c_keyModelNumber;
		static const tchar *c_keyModelURL;
		static const tchar *c_keySerialNumber;
		static const tchar *c_keyUDN;
		static const tchar *c_keyUPC;
		static const tchar *c_keyServiceList;
		static const tchar *c_keyService;
		static const tchar *c_keyPresentationURL;
	
	public:
		UPnPDeviceDescription();
		virtual ~UPnPDeviceDescription();
		
		virtual const QString& deviceType() const;
		virtual const QString& friendlyName() const;
		virtual const QString& manufacturer() const;
		virtual const QString& manufacturerURL() const;
		virtual const QString& modelDescription() const;
		virtual const QString& modelName() const;
		virtual const QString& modelNumber() const;
		virtual const QString& modelURL() const;
		virtual const QString& serialNumber() const;
		virtual const QString& udn() const;
		virtual const QString& upc() const;
		virtual const QString& presentationURL() const;
        virtual const QList<UPnPServiceDescriptionSPtr>& services() const;
		
	protected:
	
		QString m_deviceType;
		QString m_friendlyName;
		QString m_manufacturer;
		QString m_manufacturerURL;
		QString m_modelDescription;
		QString m_modelName;
		QString m_modelNumber;
		QString m_modelURL;
		QString m_serialNumber;
		QString m_udn;
		QString m_upc;
		QString m_presentationURL;
		QList<UPnPServiceDescriptionSPtr> m_services;
		
		int m_state;
		
		virtual bool isRootNode(xmlNodePtr pNode) const;
		virtual void processNode(xmlNodePtr pNode);
		virtual UPnPServiceDescriptionSPtr createServiceDescription() const;
};

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

