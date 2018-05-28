//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_MONKEYOMEGA_MONKEYCODEC_H
#define __ORCUS_ENGINE_MONKEYOMEGA_MONKEYCODEC_H
//-------------------------------------------------------------------------------------------

#include "engine/inc/InterleavedCodec.h"
#include "engine/monkeyomega/inc/MonkeyIF.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace monkeyomega
{
//-------------------------------------------------------------------------------------------

class MONKEYOMEGA_EXPORT MonkeyCodec : public InterleavedCodec
{
	public:
		Q_OBJECT
	
	public:
		static const tint c_BufferLength = 1024;
	
	public:
		MonkeyCodec(QObject *parent = 0);
		virtual ~MonkeyCodec();
		
		virtual bool open(const QString& name);
		virtual void close();
		
		virtual bool isSeek() const;
		virtual bool seek(const common::TimeStamp& v);

		virtual bool isComplete() const;
		
		virtual bool isRemote() const;
		virtual bool isBuffered(tfloat32& percent);
		
		virtual tint bitrate() const;
		virtual tint frequency() const;
		virtual tint noChannels() const;
		virtual common::TimeStamp length() const;

	protected:
		
		QSharedPointer<MonkeyIF> m_pMonkey;

		int m_sampleRate;
		int m_noChannels;
		int m_bitsPerSample;
		int m_bytesPerSample;

		char *m_buffer;
		
		QString m_name;

		virtual void printError(const tchar *strR,const tchar *strE) const;

		virtual bool decodeNextPacket(int& outLen);
		virtual char *getPacketBuffer();
		virtual int bytesPerSample();
		virtual sample_t readSample(char *buffer);
};

//-------------------------------------------------------------------------------------------
} // namespace monkeyomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
