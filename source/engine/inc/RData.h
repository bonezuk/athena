//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_ENGINE_RDATA_H
#define __OMEGA_ENGINE_RDATA_H
//-------------------------------------------------------------------------------------------

#include <QVector>

#include "common/inc/TimeStamp.h"
#include "engine/inc/AData.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
//-------------------------------------------------------------------------------------------
/* In order to support native integer mode and DSD audio the codec must have the ability  
   output its audio format in its native format. The endianness of the data is that of 
   the executing CPU.
*/

typedef enum
{
	// Defined by a sample_t equating to a 32-bit or 64-bit floating point number with dynamic range (-1.0, 1.0)
	e_SampleFloat = 0x01,
	// 16-bit integer, 2-bytes per sample.
	e_SampleInt16 = 0x02,
	// 24-bit integer, 3-bytes per sample.
	e_SampleInt24 = 0x04,
	// 32-bit integer, 4-bytes per sample.
	e_SampleInt32 = 0x08,
	// DSD 8-bits per packet, 8 samples per packet, bit order LSB
	e_SampleDSD8LSB = 0x10,
	// DSD 8-bits per packet, 8 samples per packet, bit order MSB
	e_SampleDSD8MSB = 0x20
} CodecDataType;

//-------------------------------------------------------------------------------------------

class ENGINE_EXPORT RData : public AData
{
	public:
		class Part;
	
	public:
		RData();
		RData(tint len,tint inChannel,tint outChannel);
		RData(const AData& rhs);
		virtual ~RData();
		
		virtual Part& nextPart();

		virtual tint rOffset() const;
		virtual tint rLength() const;
		
		virtual tint noParts() const;
		
		virtual Part& part(tint i);
		virtual const Part& partConst(tint i) const;

		virtual sample_t *partData(tint i);
		virtual const sample_t *partDataConst(tint i) const;
		
		virtual sample_t *partDataOut(tint i);
		virtual const sample_t *partDataOutConst(tint i) const;		

		virtual sample_t *currentPartData();
		virtual const sample_t *currentPartDataConst() const;
		
		virtual void reset();
		
		virtual void clipToTime(const common::TimeStamp& clipT);
				
	protected:
		
		QVector<Part> m_parts;
		
		virtual void copy(const AData& rhs);
};

//-------------------------------------------------------------------------------------------

class ENGINE_EXPORT RData::Part
{
	public:
		Part();
		Part(const Part& rhs);
		virtual ~Part();
		
		const Part& operator = (const Part& rhs);
	
		virtual tint& offset();
		virtual const tint& offsetConst() const;
		virtual tint& length();
		virtual const tint& lengthConst() const;
		virtual bool& done();
		virtual const bool& doneConst() const;

		virtual common::TimeStamp& start();
		virtual const common::TimeStamp& startConst() const;
		virtual common::TimeStamp& end();
		virtual const common::TimeStamp& endConst() const;
		
		virtual common::TimeStamp& startNext();
		virtual const common::TimeStamp& startNextConst() const;
		virtual common::TimeStamp& endNext();
		virtual const common::TimeStamp& endNextConst() const;
		
		virtual common::TimeStamp& refStartTime();
		virtual const common::TimeStamp& refStartTimeConst() const;

		virtual bool isNext() const;
		virtual void setNext(bool flag);

		virtual CodecDataType getDataType() const;
		virtual void setDataType(CodecDataType dType);

		friend ENGINE_EXPORT bool operator == (const Part& a,const Part& b);
		friend ENGINE_EXPORT bool operator != (const Part& a,const Part& b);
	
	protected:

		tint m_offset;
		tint m_length;
		common::TimeStamp m_start;
		common::TimeStamp m_end;
		bool m_done;
		
		bool m_nextFlag;
		common::TimeStamp m_startNext;
		common::TimeStamp m_endNext;
		
		common::TimeStamp m_refStartTime;
		
		CodecDataType m_dataType;

		void copy(const Part& rhs);
		
		virtual bool isEqual(const Part& rhs) const;
};

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

