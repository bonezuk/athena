//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_WHITEOMEGA_ALACATOMCONTAINER_H
#define __ORCUS_ENGINE_WHITEOMEGA_ALACATOMCONTAINER_H
//-------------------------------------------------------------------------------------------

#include "engine/whiteomega/inc/Atom.h"
#include "engine/redomega/inc/ALACContainer.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace whiteomega
{
//-------------------------------------------------------------------------------------------

class WHITEOMEGA_EXPORT ALACAtomContainer : public redomega::ALACContainer
{
	public:
		ALACAtomContainer();
		virtual ~ALACAtomContainer();
		
		virtual bool initWithAtom(Atom *a);
		
	protected:
};

//-------------------------------------------------------------------------------------------
} // namespace whiteomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

