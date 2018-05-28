//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_ADATA_H
#define __ORCUS_ENGINE_ADATA_H
//-------------------------------------------------------------------------------------------

#include "common/inc/TimeStamp.h"
#include "engine/inc/EngineDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
//-------------------------------------------------------------------------------------------

class ENGINE_EXPORT AData
{
	public:
		AData();
		AData(tint len,tint inChannel,tint outChannel);
		AData(const AData& rhs);
		virtual ~AData();
		
		const AData& operator = (const AData& rhs);
		
		virtual tint noInChannels() const;
		virtual tint noOutChannels() const;
		
		virtual sample_t *data();
		virtual const sample_t *dataConst() const;
		virtual sample_t *dataOut();
		virtual const sample_t *dataOutConst() const;
		
		virtual tint length() const;
		
		virtual void reset();
				
		virtual common::TimeStamp& start();
		virtual const common::TimeStamp& startConst() const;
		
		virtual common::TimeStamp& end();
		virtual const common::TimeStamp& endConst() const;
		
		virtual bool isComplete() const;
		virtual void setComplete(bool flag);

		virtual void mixChannels();
		
	protected:
	
		sample_t *m_data;
		sample_t *m_outData;
		tint m_length;
		tint m_noChannels;
		tint m_noOutChannels;
		common::TimeStamp m_start;
		common::TimeStamp m_end;
		bool m_completeFlag;
		
		virtual void init();
		virtual void copy(const AData& rhs);
		
		virtual void mixAToA();
		virtual void mixAToB();
		virtual void mixAToC();
		virtual void mixAToD();
		virtual void mixAToE();
		virtual void mixAToF();
		virtual void mixAToG();
		virtual void mixAToH();

		virtual void mixBToA();
		virtual void mixBToB();
		virtual void mixBToC();
		virtual void mixBToD();
		virtual void mixBToE();
		virtual void mixBToF();
		virtual void mixBToG();
		virtual void mixBToH();

		virtual void mixCToA();
		virtual void mixCToB();
		virtual void mixCToC();
		virtual void mixCToD();
		virtual void mixCToE();
		virtual void mixCToF();
		virtual void mixCToG();
		virtual void mixCToH();
		
		virtual void mixDToA();
		virtual void mixDToB();
		virtual void mixDToC();
		virtual void mixDToD();
		virtual void mixDToE();
		virtual void mixDToF();
		virtual void mixDToG();
		virtual void mixDToH();

		virtual void mixEToA();
		virtual void mixEToB();
		virtual void mixEToC();
		virtual void mixEToD();
		virtual void mixEToE();
		virtual void mixEToF();
		virtual void mixEToG();
		virtual void mixEToH();
		
		virtual void mixFToA();
		virtual void mixFToB();
		virtual void mixFToC();
		virtual void mixFToD();
		virtual void mixFToE();
		virtual void mixFToF();
		virtual void mixFToG();
		virtual void mixFToH();
		
		virtual void mixGToA();
		virtual void mixGToB();
		virtual void mixGToC();
		virtual void mixGToD();
		virtual void mixGToE();
		virtual void mixGToF();
		virtual void mixGToG();
		virtual void mixGToH();
		
		virtual void mixHToA();
		virtual void mixHToB();
		virtual void mixHToC();
		virtual void mixHToD();
		virtual void mixHToE();
		virtual void mixHToF();
		virtual void mixHToG();
		virtual void mixHToH();
};

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

