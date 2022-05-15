//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_ENGINE_VIOLETOMEGA_IFFCOMMONCHUNK_H
#define __OMEGA_ENGINE_VIOLETOMEGA_IFFCOMMONCHUNK_H
//-------------------------------------------------------------------------------------------

#include "engine/violetomega/inc/IFFChunk.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
namespace violetomega
{
//-------------------------------------------------------------------------------------------

#define IFF_COMM_ID IFF_ID('C','O','M','M')

//-------------------------------------------------------------------------------------------

class VIOLETOMEGA_EXPORT IFFCommonChunk : public IFFChunk
{
	public:
		IFFCommonChunk();
		virtual ~IFFCommonChunk();
		
		virtual const tint& noChannels() const;
		virtual const tint& noSampleFrames() const;
		virtual const tint& sampleSize() const;
		virtual const tfloat64& sampleRate() const;
		
		virtual bool scan();
		
	protected:
	
		tint m_noChannels;
		tint m_noSampleFrames;
		tint m_sampleSize;
		tfloat64 m_sampleRate;
};

typedef QSharedPointer<IFFCommonChunk> IFFCommonChunkSPtr;

//-------------------------------------------------------------------------------------------
} // namespace violetomega
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
