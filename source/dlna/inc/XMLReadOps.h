//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DLNA_XMLREADOPS_H
#define __ORCUS_DLNA_XMLREADOPS_H
//-------------------------------------------------------------------------------------------

#include "dlna/inc/XMLLibIF.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

class DLNA_EXPORT XMLReadOps
{
	public:
		XMLReadOps();
		virtual ~XMLReadOps();
		
		virtual QString getTextOfNode(xmlNodePtr pNode) const;
		virtual QString getTextOfElement(xmlNodePtr pElement) const;
		virtual QString getNameOfNode(xmlNodePtr pNode) const;
		
		virtual bool isElementNode(xmlNodePtr pNode) const;
		
		virtual QString comparisonKey(const tchar *keyTxt) const;
		
		virtual void removeChildrenFromNode(xmlNodePtr pNode) const;
		
		virtual void replaceTextInNode(xmlNodePtr pNode,const QString& text) const;
		virtual void replaceTextInNode(xmlNodePtr pNode,const tchar *text) const;
		
		virtual void replaceTextInAllNodes(xmlNodePtr pNode,const QString& nodeName,const QString& text) const;

		virtual bool isAttribute(xmlNodePtr pNode,const QString& name) const;
		virtual QString getAttribute(xmlNodePtr pNode,const QString& name) const;
};

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

