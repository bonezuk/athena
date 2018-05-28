//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_INTERLEAVEDCODEC_H
#define __ORCUS_ENGINE_INTERLEAVEDCODEC_H
//-------------------------------------------------------------------------------------------

#include "engine/inc/Codec.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
//-------------------------------------------------------------------------------------------

class ENGINE_EXPORT InterleavedCodec : public Codec
{
	public:
		InterleavedCodec(CodecType type,QObject *parent = 0);
		virtual ~InterleavedCodec();
		
		virtual bool init();
		virtual bool next(AData& data);
		
	protected:
	
		int m_state;
		int m_outLen;
		int m_outOffset;
		char *m_pBuffer;
		common::TimeStamp m_time;

		virtual bool decodeNextPacket(int& outLen) = 0;
		virtual char *getPacketBuffer() = 0;
		virtual int bytesPerSample() = 0;
		virtual sample_t readSample(char *buffer) = 0;
		
		virtual void readDecodedData(sample_t *output,tint sampleOffset,tint amount);
};

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
