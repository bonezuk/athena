#include "engine/violetomega/inc/IFFCommonChunk.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace violetomega
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(IFFChunkFactory,IFFChunk, \
                            IFFCommonChunkFactory,IFFCommonChunk, \
                            "COMM",false)

//-------------------------------------------------------------------------------------------

IFFCommonChunk::IFFCommonChunk() : IFFChunk(),
	m_noChannels(0),
	m_noSampleFrames(0),
	m_sampleSize(0),
	m_sampleRate(1.0)
{}

//-------------------------------------------------------------------------------------------

IFFCommonChunk::~IFFCommonChunk()
{}

//-------------------------------------------------------------------------------------------

const tint& IFFCommonChunk::noChannels() const
{
	return m_noChannels;
}

//-------------------------------------------------------------------------------------------

const tint& IFFCommonChunk::noSampleFrames() const
{
	return m_noSampleFrames;
}

//-------------------------------------------------------------------------------------------

const tint& IFFCommonChunk::sampleSize() const
{
	return m_sampleSize;
}

//-------------------------------------------------------------------------------------------

const tfloat64& IFFCommonChunk::sampleRate() const
{
	return m_sampleRate;
}

//-------------------------------------------------------------------------------------------

bool IFFCommonChunk::scan()
{
	bool res = false;
	
	if(m_file!=0 && size()==18 && filePositionToStart())
	{
		tbyte mem[18];
		
		if(m_file->read(mem,18)==18)
		{
			m_noChannels = static_cast<tint>(read16BitSigned(mem));
			m_noSampleFrames = static_cast<tint>(read32BitUnsigned(&mem[2]));
			m_sampleSize = static_cast<tint>(read16BitSigned(&mem[6]));
			m_sampleRate = doubleFromExtended(&mem[8]);
			if(m_sampleSize>=1 && m_sampleSize<=32)
			{
				if(m_noChannels>=1 && m_noChannels<=6)
				{
					res = true;
				}
			}
		}
	}
	return res;
}


//-------------------------------------------------------------------------------------------
} // namespace violetomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------

