#include "dlna/inc/UPnPDeviceDescription.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

const tchar *UPnPDeviceDescription::c_keyDevice = "device";
const tchar *UPnPDeviceDescription::c_keyDeviceType = "deviceType";
const tchar *UPnPDeviceDescription::c_keyFriendlyName = "friendlyName";
const tchar *UPnPDeviceDescription::c_keyManufacturer = "manufacturer";
const tchar *UPnPDeviceDescription::c_keyManufacturerURL = "manufacturerURL";
const tchar *UPnPDeviceDescription::c_keyModelDescription = "modelDescription";
const tchar *UPnPDeviceDescription::c_keyModelName = "modelName";
const tchar *UPnPDeviceDescription::c_keyModelNumber = "modelNumber";
const tchar *UPnPDeviceDescription::c_keyModelURL = "modelURL";
const tchar *UPnPDeviceDescription::c_keySerialNumber = "serialNumber";
const tchar *UPnPDeviceDescription::c_keyUDN = "UDN";
const tchar *UPnPDeviceDescription::c_keyUPC = "UPC";
const tchar *UPnPDeviceDescription::c_keyServiceList = "serviceList";
const tchar *UPnPDeviceDescription::c_keyService = "service";
const tchar *UPnPDeviceDescription::c_keyPresentationURL = "presentationURL";

//-------------------------------------------------------------------------------------------

UPnPDeviceDescription::UPnPDeviceDescription() : XMLNodeParser(),
	m_deviceType(),
	m_friendlyName(),
	m_manufacturer(),
	m_manufacturerURL(),
	m_modelDescription(),
	m_modelName(),
	m_modelNumber(),
	m_modelURL(),
	m_serialNumber(),
	m_udn(),
	m_upc(),
    m_presentationURL(),
	m_services(),
	m_state(0)
{}

//-------------------------------------------------------------------------------------------

UPnPDeviceDescription::~UPnPDeviceDescription()
{}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::deviceType() const
{
	return m_deviceType;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::friendlyName() const
{
	return m_friendlyName;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::manufacturer() const
{
	return m_manufacturer;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::manufacturerURL() const
{
	return m_manufacturerURL;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::modelDescription() const
{
	return m_modelDescription;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::modelName() const
{
	return m_modelName;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::modelNumber() const
{
	return m_modelNumber;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::modelURL() const
{
	return m_modelURL;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::serialNumber() const
{
	return m_serialNumber;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::udn() const
{
	return m_udn;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::upc() const
{
	return m_upc;
}

//-------------------------------------------------------------------------------------------

const QString& UPnPDeviceDescription::presentationURL() const
{
	return m_presentationURL;	
}

//-------------------------------------------------------------------------------------------

const QList<UPnPServiceDescriptionSPtr>& UPnPDeviceDescription::services() const
{
	return m_services;
}

//-------------------------------------------------------------------------------------------

bool UPnPDeviceDescription::isRootNode(xmlNodePtr pNode) const
{
	bool res;

	if(m_state==0)
	{
		res = isSpecifiedNode(pNode,c_keyDevice);
	}
	else
	{
		res = isSpecifiedNode(pNode,c_keyServiceList);
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void UPnPDeviceDescription::processNode(xmlNodePtr pNode)
{
	QString cName = getNameOfNode(pNode).toLower();
	
	m_state++;
	switch(m_state)
	{
		case 1:
			if(cName==comparisonKey(c_keyDeviceType))
			{
				m_deviceType = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyFriendlyName))
			{
                m_friendlyName = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyManufacturer))
			{
				m_manufacturer = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyManufacturerURL))
			{
				m_manufacturerURL = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyModelDescription))
			{
				m_modelDescription = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyModelName))
			{
				m_modelName = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyModelNumber))
			{
				m_modelNumber = getTextOfElement(pNode);
			}
            else if(cName==comparisonKey(c_keyModelURL))
			{
				m_modelURL = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keySerialNumber))
			{
				m_serialNumber = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyUDN))
			{
				m_udn = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyUPC))
			{
				m_upc = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyPresentationURL))
			{
				m_presentationURL = getTextOfElement(pNode);
			}
			else if(cName==comparisonKey(c_keyServiceList))
			{
				parse(pNode);
			}
			break;
		
		case 2:
			if(cName==comparisonKey(c_keyService))
			{
				UPnPServiceDescriptionSPtr pService = createServiceDescription();
				pService->parse(pNode);
				m_services.append(pService);
			}
			break;
		
		default:
			break;
	}
	m_state--;
}

//-------------------------------------------------------------------------------------------

UPnPServiceDescriptionSPtr UPnPDeviceDescription::createServiceDescription() const
{
	UPnPServiceDescriptionSPtr pService(new UPnPServiceDescription);
	return pService;
}

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace orcus
//-------------------------------------------------------------------------------------------
