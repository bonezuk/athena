#include "dlna/inc/UPnPSpecVersion.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

const tchar *UPnPSpecVersion::c_keySpecVersion = "specVersion";
const tchar *UPnPSpecVersion::c_keyMajor = "major";
const tchar *UPnPSpecVersion::c_keyMinor = "minor";

//-------------------------------------------------------------------------------------------

UPnPSpecVersion::UPnPSpecVersion() : XMLNodeParser(),
	m_major(1),
	m_minor(0)
{}

//-------------------------------------------------------------------------------------------

UPnPSpecVersion::UPnPSpecVersion(int major,int minor) : XMLNodeParser(),
	m_major(major),
	m_minor(minor)
{}

//-------------------------------------------------------------------------------------------

UPnPSpecVersion::~UPnPSpecVersion()
{}

//-------------------------------------------------------------------------------------------

tfloat32 UPnPSpecVersion::version() const
{
	return static_cast<tfloat32>(m_major) + (static_cast<tfloat32>(m_minor) / 10.0f);
}

//-------------------------------------------------------------------------------------------

bool UPnPSpecVersion::isRootNode(xmlNodePtr pNode) const
{
	return isSpecifiedNode(pNode,c_keySpecVersion);
}

//-------------------------------------------------------------------------------------------

void UPnPSpecVersion::processNode(xmlNodePtr pNode)
{
	QString cName = getNameOfNode(pNode).toLower();
	
	if(cName==comparisonKey(c_keyMajor))
	{
		m_major = getTextOfElement(pNode).toInt();
	}
	else if(cName==comparisonKey(c_keyMinor))
	{
		m_minor = getTextOfElement(pNode).toInt();
	}
}

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------
