#include "common/inc/CryptoRandom.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
//-------------------------------------------------------------------------------------------

#define RANDOM_BYTES_RQ 256
#define RANDOM_BYTES_RQINT 512

//-------------------------------------------------------------------------------------------

CryptoRandom::CryptoRandom() : m_bytesNeeded(RANDOM_BYTES_RQ),
	m_outputAvailable(0)
{
	::memset(m_state,0,16);
	::memset(m_output,0,16);
}

//-------------------------------------------------------------------------------------------

CryptoRandom::~CryptoRandom()
{
	m_bytesNeeded = 0;
	m_outputAvailable = 0;
	::memset(m_state,0,16);
	::memset(m_output,0,16);
}

//-------------------------------------------------------------------------------------------

void CryptoRandom::update(tubyte *block,tint len)
{
	tint i,j;
	tubyte digest[16];
	MD5Digest md5;
	
	md5.write(block,len);
	md5.final(digest);
	
	for(j=0,i=16;i>0;i--)
	{
		j += m_state[i-1] + digest[i-1];
		m_state[i-1] = static_cast<tubyte>(j);
		j >>= 8;
	}
	
	if(m_bytesNeeded<len)
	{
		m_bytesNeeded = 0;
	}
	else
	{
		m_bytesNeeded -= len;
	}
	
	for(i=0;i<16;i++)
	{
		digest[i] = 0;
	}
}

//-------------------------------------------------------------------------------------------

tint CryptoRandom::required()
{
	return m_bytesNeeded;
}

//-------------------------------------------------------------------------------------------

bool CryptoRandom::generateBytes(tubyte *block,tint len)
{
	tuint avail,i;
	MD5Digest md5;
	
	if(m_bytesNeeded>0)
	{
		return false;
	}
	avail = static_cast<tuint>(m_outputAvailable);
	
	while(avail<static_cast<tuint>(len))
	{
		::memcpy(block,&m_output[16-avail],avail);
		len -= avail;
		block += avail;

		for(i=0;i<16;i++)
		{
			m_state[i] ^= m_output[15-i];
		}

		md5.init();
		md5.write(m_state,16);
		md5.final(m_output);
		avail = 16;		
	}
	memcpy(block,&m_output[16-avail],static_cast<size_t>(len));
	m_outputAvailable = avail - len;
	return true;
}

//-------------------------------------------------------------------------------------------

void CryptoRandom::create()
{
	time_t t;
	clock_t cnow;
	struct tm *gmt = 0;
	
	::memset(m_state,0,16);
	m_outputAvailable = 0;
	m_bytesNeeded = RANDOM_BYTES_RQINT;
	
	while(m_bytesNeeded>0)
	{
		t = ::time(0);
		gmt = ::gmtime(&t);
		cnow = ::clock();
		update(reinterpret_cast<tubyte *>(gmt),sizeof(struct tm));
		update(reinterpret_cast<tubyte *>(&cnow),sizeof(clock_t));
	}
	if(gmt!=0)
	{
		memset(gmt, 0, sizeof(struct tm));
	}
	cnow = 0;
	t = 0;
}

//-------------------------------------------------------------------------------------------

void CryptoRandom::mix()
{
	int i;
	MD5Digest md5;
	
	for(i=0;i<16;i++)
	{
		m_state[i] ^= static_cast<tubyte>(::clock());
		m_state[15-i] ^= static_cast<tubyte>(::time(0));
	}
	md5.write(m_state,16);
	md5.final(m_output);
	m_outputAvailable = 16;
}

//-------------------------------------------------------------------------------------------

void CryptoRandom::mix(tubyte *x,tint len)
{
	MD5Digest md5;

	::memset(m_state,0,16);
	m_outputAvailable = 0;
	m_bytesNeeded = RANDOM_BYTES_RQINT;

	while(m_bytesNeeded)
	{
		update(x,len);
	}
	md5.write(m_state,16);
	md5.final(m_output);
	m_outputAvailable = 16;
}

//-------------------------------------------------------------------------------------------
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
