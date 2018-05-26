//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_COMMON_CRYPTORSAENCRYPT_H
#define __ORCUS_COMMON_CRYPTORSAENCRYPT_H
//-------------------------------------------------------------------------------------------

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>

#include "common/inc/LargeNumber.h"
#include "common/inc/CryptoPrimes.h"
#include "common/inc/CryptoRandom.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
//-------------------------------------------------------------------------------------------

#define MIN_RSA_MODULUS_BITS 508
#define MAX_RSA_MODULUS_BITS 2048 * 2
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 7) / 8)

//-------------------------------------------------------------------------------------------

class COMMON_EXPORT CryptoRSAEncrypt
{
	public:
		CryptoRSAEncrypt();
		virtual ~CryptoRSAEncrypt();
		
		virtual bool generateKeys(tint pBits,const QString& user,const QString& pass);
		virtual bool generateKeys(tint aBits,bool useFermat4);
		
		virtual QByteArray encrypt(const QByteArray& in);
		virtual QByteArray decrypt(const QByteArray& in);
		
		virtual QByteArray publicEncrypt(const QByteArray& in);
		virtual QByteArray publicDecrypt(const QByteArray& in);
		
		virtual QString writePublicKeyXML();
		virtual bool readPublicKeyXML(const QString& iStr);
		virtual QString writePrivateKeyXML();
		virtual bool readPrivateKeyXML(const QString& iStr);
		
	protected:
	
		// Prime number generator
		CryptoPrimes m_primes;
		// Encryption key size
		tint m_bits;
		// Private key
		LargeNumber m_exponent;
		LargeNumber m_prime[2];
		LargeNumber m_primeExponent[2];
		LargeNumber m_coefficient;
		// Public key
		LargeNumber m_modulus;
		LargeNumber m_publicExponent;
		
		virtual bool filter(const LargeNumber& a,const LargeNumber& b);
		
		virtual QByteArray publicE(const QByteArray& in);
		virtual QByteArray privateD(const QByteArray& in);
		
		xmlXPathObjectPtr getNodeSet(xmlDocPtr doc,const QString& path);
};

//-------------------------------------------------------------------------------------------
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
