//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_REDOMEGA_ALACCAFCONTAINER_H
#define __ORCUS_ENGINE_REDOMEGA_ALACCAFCONTAINER_H
//-------------------------------------------------------------------------------------------

#include "engine/redomega/inc/CAFFile.h"
#include "engine/redomega/inc/ALACContainer.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace redomega
{
//-------------------------------------------------------------------------------------------

class REDOMEGA_EXPORT ALACCafContainer : public ALACContainer
{
	public:
		ALACCafContainer();
		virtual ~ALACCafContainer();
		
		virtual bool init(engine::File *f);
		
	protected:
		
		CAFFile m_cafFile;
		
		virtual void printError(const tchar *strR,const tchar *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace redomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

