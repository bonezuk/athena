//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DLNA_UPNPSPECVERSION_H
#define __ORCUS_DLNA_UPNPSPECVERSION_H
//-------------------------------------------------------------------------------------------

#include "dlna/inc/XMLNodeParser.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace dlna
{
//-------------------------------------------------------------------------------------------
//  <specVersion>
//    <major>1</major>
//    <minor>0</minor>
//  </specVersion>
//-------------------------------------------------------------------------------------------

class DLNA_EXPORT UPnPSpecVersion : public XMLNodeParser
{
	public:
	
		static const tchar *c_keySpecVersion;
		static const tchar *c_keyMajor;
		static const tchar *c_keyMinor;

	public:
		UPnPSpecVersion();
		UPnPSpecVersion(int major,int minor);
		virtual ~UPnPSpecVersion();
		
		virtual tfloat32 version() const;
		
	protected:
	
		int m_major;
		int m_minor;
		
		virtual bool isRootNode(xmlNodePtr pNode) const;
		virtual void processNode(xmlNodePtr pNode);
};

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

