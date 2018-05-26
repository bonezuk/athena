//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_COMMON_CRYPTORANDOM_H
#define __ORCUS_COMMON_CRYPTORANDOM_H
//-------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <time.h>
#include "common/inc/MD5Digest.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
//-------------------------------------------------------------------------------------------

class COMMON_EXPORT CryptoRandom
{
	public:
		CryptoRandom();
		virtual ~CryptoRandom();
		
		void update(tubyte *block,tint len);
		tint required();
		bool generateBytes(tubyte *block,tint len);
		void create();
		
		void mix();
		void mix(tubyte *x,tint len);
		
	protected:
	
		tint m_bytesNeeded;
		tubyte m_state[16];
		tint m_outputAvailable;
		tubyte m_output[16];
};

//-------------------------------------------------------------------------------------------
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

