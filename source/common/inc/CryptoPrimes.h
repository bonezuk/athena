//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_COMMON_CRYPTOPRIMES_H
#define __ORCUS_COMMON_CRYPTOPRIMES_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CryptoRandom.h"
#include "common/inc/LargeNumber.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
//-------------------------------------------------------------------------------------------

class COMMON_EXPORT CryptoPrimes
{
	public:
		CryptoPrimes();
		virtual ~CryptoPrimes();
		
		bool isProbablePrime(LargeNumber& a);
		
		LargeNumber generatePrimes(LargeNumber& b,LargeNumber& c,LargeNumber& d);
		LargeNumber getPrimes(LargeNumber& b,LargeNumber& c,LargeNumber& d,tubyte *block);

		CryptoRandom& random();

	protected:
	
		CryptoRandom m_random;
};

//-------------------------------------------------------------------------------------------

inline CryptoRandom& CryptoPrimes::random()
{
	return m_random;
}

//-------------------------------------------------------------------------------------------
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
