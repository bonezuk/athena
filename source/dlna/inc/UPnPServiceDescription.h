//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_DLNA_UPNPSERVICEDESCRIPTION_H
#define __OMEGA_DLNA_UPNPSERVICEDESCRIPTION_H
//-------------------------------------------------------------------------------------------
#include "dlna/inc/XMLNodeParser.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------
//      <service>
//        <serviceType>urn:schemas-upnp-org:service:tvcontrol:1</serviceType>
//        <serviceId>urn:upnp-org:serviceId:tvcontrol1</serviceId>
//        <controlURL>/upnp/control/tvcontrol1</controlURL>
//        <eventSubURL>/upnp/event/tvcontrol1</eventSubURL>
//        <SCPDURL>/tvcontrolSCPD.xml</SCPDURL>
//      </service>
//-------------------------------------------------------------------------------------------

class DLNA_EXPORT UPnPServiceDescription : public XMLNodeParser
{
	public:
	
		static const tchar *c_keyService;
		static const tchar *c_keyServiceType;
		static const tchar *c_keyServiceId;
		static const tchar *c_keyControlURL;
		static const tchar *c_keyEventSubURL;
		static const tchar *c_keySCPDURL;
	
	public:
		UPnPServiceDescription();
		virtual ~UPnPServiceDescription();
		
		virtual const QString& serviceType() const;
		virtual const QString& serviceId() const;
		virtual const QString& controlURL() const;
		virtual const QString& eventSubURL() const;
		virtual const QString& scpdURL() const;
		
	protected:
	
		QString m_serviceType;
		QString m_serviceId;
		QString m_controlURL;
		QString m_eventSubURL;
		QString m_scpdURL;
		
		virtual bool isRootNode(xmlNodePtr pNode) const;
		virtual void processNode(xmlNodePtr pNode);
};

typedef QSharedPointer<UPnPServiceDescription> UPnPServiceDescriptionSPtr;

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
