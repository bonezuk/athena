//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_DLNA_XMLNODEPARSER_H
#define __OMEGA_DLNA_XMLNODEPARSER_H
//-------------------------------------------------------------------------------------------

#include "dlna/inc/XMLReadOps.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

class DLNA_EXPORT XMLNodeParser : public XMLReadOps
{
	public:
		XMLNodeParser();
		virtual ~XMLNodeParser();
		
		virtual void parse(xmlNodePtr pNode);
		
	protected:
	
		virtual bool isRootNode(xmlNodePtr pNode) const = 0;
		virtual void processNode(xmlNodePtr pNode) = 0;

		virtual bool isSpecifiedNode(xmlNodePtr pNode,const tchar *name) const;
};

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

