//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_SILVEROMEGA_VSILVERMODEDATA_H
#define __ORCUS_ENGINE_SILVEROMEGA_VSILVERMODEDATA_H
//-------------------------------------------------------------------------------------------

#include "engine/silveromega/inc/SilverOmegaDLL.h"
#include "engine/silveromega/inc/VSilverFloorBase.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace silveromega
{
//-------------------------------------------------------------------------------------------

class SILVEROMEGA_EXPORT VSilverModeData : public VSilverInfoObject
{
	public:
		VSilverModeData(VSilverCodecData *cData);
		virtual ~VSilverModeData();
		
		virtual bool isValid() const;
		
		virtual bool read(engine::Sequence *seq);
		
		tint m_blockFlag;
		tint m_windowType;
		tint m_transformType;
		tint m_mapping;
		
	protected:
	
		VSilverCodecData *m_cData;
		
		virtual void printError(const tchar *strR,const tchar *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace silveromega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

