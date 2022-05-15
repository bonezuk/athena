#include "dlna/inc/UPnPServiceDescription.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

const tchar *UPnPServiceDescription::c_keyService = "service";
const tchar *UPnPServiceDescription::c_keyServiceType = "serviceType";
const tchar *UPnPServiceDescription::c_keyServiceId = "serviceId";
const tchar *UPnPServiceDescription::c_keyControlURL = "controlURL";
const tchar *UPnPServiceDescription::c_keyEventSubURL = "eventSubURL";
const tchar *UPnPServiceDescription::c_keySCPDURL = "SCPDURL";

//-------------------------------------------------------------------------------------------

UPnPServiceDescription::UPnPServiceDescription() : XMLNodeParser(),
	m_serviceType(),
	m_serviceId(),
	m_controlURL(),
	m_eventSubURL(),
	m_scpdURL()
{}

//-------------------------------------------------------------------------------------------

UPnPServiceDescription::~UPnPServiceDescription()
{}

//-------------------------------------------------------------------------------------------

const QString& UPnPServiceDescription::serviceType() const
{
	return m_serviceType;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPServiceDescription::serviceId() const
{
	return m_serviceId;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPServiceDescription::controlURL() const
{
	return m_controlURL;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPServiceDescription::eventSubURL() const
{
	return m_eventSubURL;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPServiceDescription::scpdURL() const
{
	return m_scpdURL;
}

//-------------------------------------------------------------------------------------------

bool UPnPServiceDescription::isRootNode(xmlNodePtr pNode) const
{
	return isSpecifiedNode(pNode,c_keyService);
}

//-------------------------------------------------------------------------------------------

void UPnPServiceDescription::processNode(xmlNodePtr pNode)
{
	QString cName = getNameOfNode(pNode).toLower();
	
	if(cName==comparisonKey(c_keyServiceType))
	{
		m_serviceType = getTextOfElement(pNode);
	}
	else if(cName==comparisonKey(c_keyServiceId))
	{
		m_serviceId = getTextOfElement(pNode);
	}
	else if(cName==comparisonKey(c_keyControlURL))
	{
		m_controlURL = getTextOfElement(pNode);
	}
	else if(cName==comparisonKey(c_keyEventSubURL))
	{
		m_eventSubURL = getTextOfElement(pNode);
	}
	else if(cName==comparisonKey(c_keySCPDURL))
	{
		m_scpdURL = getTextOfElement(pNode);
	}
}

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------

