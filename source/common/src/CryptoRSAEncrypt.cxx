#include "common/inc/CryptoRSAEncrypt.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
//-------------------------------------------------------------------------------------------

CryptoRSAEncrypt::CryptoRSAEncrypt() : m_primes(),
	m_bits(0),
	m_exponent(),
	m_coefficient(),
	m_modulus(),
	m_publicExponent()
{}

//-------------------------------------------------------------------------------------------

CryptoRSAEncrypt::~CryptoRSAEncrypt()
{}

//-------------------------------------------------------------------------------------------

bool CryptoRSAEncrypt::generateKeys(tint pBits,const QString& user,const QString& pass)
{
	tint i,j,mLen;
	tubyte *mixA,*mixB;
	QByteArray userArr,passArr;
	bool res = false;

	userArr = user.toUtf8();
	passArr = pass.toUtf8();
	mLen = userArr.length() + passArr.length();
	mixA = new tubyte [mLen];
	mixB = new tubyte [mLen];
	if(mLen>0)
	{
		const tchar *uMem = userArr.data();
		const tchar *pMem = passArr.data();

		for(i = 0, j = 0; j<userArr.length() && i<mLen; i++, j++)
		{
			mixA[i] = static_cast<tubyte>(uMem[j]);
		}
		for(j = 0; j<passArr.length() && i<mLen; i++, j++)
		{
			mixA[i] = static_cast<tubyte>(pMem[j]);
		}
		for(i = 0, j = passArr.length() - 1; j >= 0 && i<mLen; i++, j--)
		{
			mixB[i] = static_cast<tubyte>(pMem[j]);
		}
		for(j = userArr.length() - 1; j >= 0 && i<mLen; i++, j--)
		{
			mixB[i] = static_cast<tubyte>(uMem[j]);
		}

		m_bits = pBits;
		if(m_bits>=MIN_RSA_MODULUS_BITS && m_bits<=MAX_RSA_MODULUS_BITS)
		{
			tint nDigits,pDigits,qBits;
			LargeNumber e,t,u,v,p,q,n,d;
			LargeNumber qInv,pMinus1,qMinus1,phiN,dP,dQ;

			nDigits = (m_bits + NN_DIGIT_BITS-1) / NN_DIGIT_BITS;
			pDigits = (nDigits + 1) / 2;
			pBits = (m_bits + 1 ) / 2;
			qBits = m_bits - pBits;

			e.assignDigit(3,nDigits);

			t.assign2Exp(pBits-1,pDigits);
			u.assign2Exp(pBits-2,pDigits);
			t += u;
			v.assignDigit(1,pDigits);
			v = t - v;
			u += v;
			v.assignDigit(2,pDigits);

			res = true;

			m_primes.random().mix(mixA,mLen);
			do
			{
				p = m_primes.generatePrimes(t,u,v);
				if(p.zero())
				{
					res = false;
				}
			} while(filter(p,e) && res);

			if(res)
			{
				t.assign2Exp(qBits-1,pDigits);
				u.assign2Exp(qBits-2,pDigits);
				t += u;
				v.assignDigit(1,pDigits);
				v = t - v;
				u += v;
				v.assignDigit(2,pDigits);

				m_primes.random().mix(mixB,mLen);
				do
				{
					q = m_primes.generatePrimes(t,u,v);
					if(q.zero())
					{
						res = false;
					}
				} while(filter(q,e) && res);

				if(res)
				{
					if(p < q)
					{
						t = p;
						p = q;
						q = t;
					}

					n = p * q;
					qInv = modInv(q,p);

					t.assignDigit(1,1);
					pMinus1 = p - t;
					qMinus1 = q - t;
					phiN = pMinus1 * qMinus1;

					d = modInv(e,phiN);
					dP = d % pMinus1;
					dQ = d % qMinus1;

					// Public Key
					m_modulus = n;
					m_publicExponent = e;

					// Private Key
					m_exponent = d;
					m_prime[0] = p;
					m_prime[1] = q;
					m_primeExponent[0] = dP;
					m_primeExponent[1] = dQ;
					m_coefficient = qInv;
				}
			}
		}
	}

	delete [] mixA;
	delete [] mixB;

	return res;
}

//-------------------------------------------------------------------------------------------

bool CryptoRSAEncrypt::generateKeys(tint aBits,bool useFermat4)
{
	int nDigits,pBits,pDigits,qBits;
	LargeNumber e,t,u,v,p,q,n,d;
	LargeNumber qInv,pMinus1,qMinus1,phiN,dP,dQ;
	bool res = false;
	
	m_bits = aBits;
	if(m_bits>=MIN_RSA_MODULUS_BITS && m_bits<=MAX_RSA_MODULUS_BITS)
	{
		nDigits = (m_bits + NN_DIGIT_BITS-1) / NN_DIGIT_BITS;
		pDigits = (nDigits+1) / 2;
		pBits = (m_bits + 1) / 2;
		qBits = m_bits - pBits;
		
		e.assignDigit((useFermat4) ? 65537 : 3,nDigits);
		
		t.assign2Exp(pBits-1,pDigits);
		u.assign2Exp(pBits-2,pDigits);
		t += u;
		v.assignDigit(1,pDigits);
		v = t - v;
		u += v;
		v.assignDigit(2,pDigits);
		
		m_primes.random().mix();
		do
		{
			p = m_primes.generatePrimes(t,u,v);
			if(p.zero())
			{
				return false;
			}
		} while(filter(p,e));
		
		t.assign2Exp(qBits-1,pDigits);
		u.assign2Exp(qBits-2,pDigits);
		t += u;
		v.assignDigit(1,pDigits);
		v = t - v;
		u += v;
		v.assignDigit(2,pDigits);
		
		m_primes.random().mix();
		do
		{
			q = m_primes.generatePrimes(t,u,v);
			if(q.zero())
			{
				return false;
			}
		} while(filter(q,e));
		
		if(p < q)
		{
			t = p;
			p = q;
			q = t;
		}
		
		n = p * q;
		qInv = modInv(q,p);
		
		t.assignDigit(1,1);
		pMinus1 = p - t;
		qMinus1 = q - t;
		phiN = pMinus1 * qMinus1;
		
		d = modInv(e,phiN);
		dP = d % pMinus1;
		dQ = d % qMinus1;
		
		// Public Key
		m_modulus = n;
		m_publicExponent = e;
		
		// Private Key
		m_exponent = d;
		m_prime[0] = p;
		m_prime[1] = q;
		m_primeExponent[0] = dP;
		m_primeExponent[1] = dQ;
		m_coefficient = qInv;
		
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool CryptoRSAEncrypt::filter(const LargeNumber& a,const LargeNumber& b)
{
	LargeNumber t,u,aMinus1;
	
	t.assignDigit(1,1);
	aMinus1 = a - t;
	u = gcd(aMinus1,b);
	return (t!=u);
}

//-------------------------------------------------------------------------------------------

QByteArray CryptoRSAEncrypt::publicE(const QByteArray& in)
{
	LargeNumber c,m;
	QByteArray out;
	
	m.decode(in);
	if(m < m_modulus)
	{
		tint outLen = (m_bits + 7) / 8;
		c = modExp(m,m_publicExponent,m_modulus);
		c.encode(out,outLen);
	}
	return out;
}

//-------------------------------------------------------------------------------------------

QString stringFromQByteArrayInHex(const QByteArray& arr)
{
	const tubyte *x = reinterpret_cast<const tubyte *>(arr.constData());
	int i;
	tbyte t[3] = {'\0','\0','\0'};
	QString s;
	
	s = "0x";
	for(i=0;i<arr.size();i++)
	{
		t[0] = static_cast<tbyte>(x[i] & 0x0f);
		t[0] += (t[0] < static_cast<tbyte>(10)) ? '0' : '7';
		t[1] = static_cast<tbyte>((x[i] >> 4) & 0x0f);
		t[1] += (t[1] < static_cast<tbyte>(10)) ? '0' : '7';
		s += t;
	}
	return s;
}

QByteArray CryptoRSAEncrypt::privateD(const QByteArray& in)
{
	LargeNumber c,cP,cQ,mP,mQ,t;
	QByteArray out;
	
	fprintf(stdout,"CryptoRSAEncrypt::privateD - 1\n");
	fprintf(stdout,"in = %s",stringFromQByteArrayInHex(in).toUtf8().constData());
	c.decode(in);
	fprintf(stdout,"c = %s\n",c.toString().toUtf8().constData());
	fprintf(stdout,"CryptoRSAEncrypt::privateD - 2\n");
	if(c < m_modulus)
	{
		tint outLen = (m_bits + 7) / 8;
		
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 3\n");
		cP = c % m_prime[0];
		fprintf(stdout,"cP = %s\n",cP.toString().toUtf8().constData());
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 4\n");
		cQ = c % m_prime[1];
		fprintf(stdout,"cQ = %s\n",cQ.toString().toUtf8().constData());
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 5\n");
		mP = modExp(cP,m_primeExponent[0],m_prime[0]);
		fprintf(stdout,"mP = %s\n",mP.toString().toUtf8().constData());
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 6\n");
		mP.resize(m_modulus.size());
		fprintf(stdout,"mP = %s\n",mP.toString().toUtf8().constData());
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 7\n");
		mQ = modExp(cQ,m_primeExponent[1],m_prime[1]);
		fprintf(stdout,"mQ = %s\n",mQ.toString().toUtf8().constData());
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 8\n");
		mQ.resize(m_modulus.size());
		fprintf(stdout,"mQ = %s\n",mQ.toString().toUtf8().constData());
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 9\n");
		if(mP >= mQ)
		{
			t = mP - mQ;
			fprintf(stdout,"(mP>=mQ) t = %s\n",t.toString().toUtf8().constData());
		}
		else
		{
			t = mQ - mP;
			t = m_prime[0] - t;
			fprintf(stdout,"!(mP>=mQ) t = %s\n",t.toString().toUtf8().constData());
		}
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 10\n");
		t = modMult(t,m_coefficient,m_prime[0]);
		fprintf(stdout,"t = %s\n",t.toString().toUtf8().constData());
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 11\n");
		t = t * m_prime[1];
		fprintf(stdout,"t = %s\n",t.toString().toUtf8().constData());
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 12\n");
		t += mQ;
		fprintf(stdout,"t = %s\n",t.toString().toUtf8().constData());
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 13\n");
		t.encode(out,outLen);
		fprintf(stdout,"CryptoRSAEncrypt::privateD - 14\n");
	}
	fprintf(stdout,"CryptoRSAEncrypt::privateD - 15\n");
	return out;
}

//-------------------------------------------------------------------------------------------

QByteArray CryptoRSAEncrypt::encrypt(const QByteArray& in)
{
	int i,modulusLen;
	QByteArray out,pkcsArr(MAX_RSA_MODULUS_LEN,'\0');
	
	fprintf(stdout,"m_modulus = %s\n",m_modulus.toString().toUtf8().constData());
	fprintf(stdout,"m_publicExponent = %s\n",m_publicExponent.toString().toUtf8().constData());
	fprintf(stdout,"m_bits = %d\n",m_bits);
	
	modulusLen = (m_bits+7) / 8;
	if(in.length() + 11 <= modulusLen)
	{
		tubyte *pkcsBlock = reinterpret_cast<tubyte *>(pkcsArr.data());
		
		pkcsBlock[0] = 0;
		pkcsBlock[1] = 2;
		
		for(i=2;i<modulusLen-in.length()-1;i++)
		{
/*			
			tubyte b;

			do
			{
				m_primes.random().generateBytes(&b,1);
			} while(b==0);

			pkcsBlock[i] = b;
*/
			pkcsBlock[i] = - 9;
		}
		pkcsBlock[i++] = 0;
		memcpy(&pkcsBlock[i],in.data(),static_cast<size_t>(in.length()));
		pkcsArr.resize(modulusLen);
		
		
		
		out = publicE(pkcsArr);
		memset(pkcsArr.data(),0,static_cast<size_t>(pkcsArr.length()));
	}
	return out;
}

//-------------------------------------------------------------------------------------------

QByteArray CryptoRSAEncrypt::decrypt(const QByteArray& in)
{
	int i,modulusLen;
	QByteArray out;
	
	fprintf(stdout,"m_exponent = %s\n",m_exponent.toString().toUtf8().constData());
	fprintf(stdout,"m_prime[0] = %s\n",m_prime[0].toString().toUtf8().constData());
	fprintf(stdout,"m_prime[1] = %s\n",m_prime[1].toString().toUtf8().constData());
	fprintf(stdout,"m_primeExponent[0] = %s\n",m_primeExponent[0].toString().toUtf8().constData());
	fprintf(stdout,"m_primeExponent[1] = %s\n",m_primeExponent[1].toString().toUtf8().constData());
	fprintf(stdout,"m_coefficient = %s\n",m_coefficient.toString().toUtf8().constData());
	fprintf(stdout,"m_modulus = %s\n",m_modulus.toString().toUtf8().constData());
	fprintf(stdout,"m_publicExponent = %s\n",m_publicExponent.toString().toUtf8().constData());
	fprintf(stdout,"m_bits = %d\n",m_bits);
	
	fprintf(stdout,"CryptoRSAEncrypt::decrypt - 1\n");
	modulusLen = (m_bits + 7) / 8;
	if(in.length() <= modulusLen)
	{
		fprintf(stdout,"CryptoRSAEncrypt::decrypt - 2\n");
		QByteArray pkcsArr = privateD(in);
		fprintf(stdout,"CryptoRSAEncrypt::decrypt - 3\n");
		if(!pkcsArr.isEmpty())
		{
		/*
			const tubyte *pkcsBlock = reinterpret_cast<const tubyte *>(pkcsArr.data());
			
			fprintf(stdout,"CryptoRSAEncrypt::decrypt - 4\n");
			if(pkcsBlock[0]==0 && pkcsBlock[1]==2 && pkcsArr.length()==modulusLen)
			{
				fprintf(stdout,"CryptoRSAEncrypt::decrypt - 5\n");
				for(i=2;i<modulusLen-1;i++)
				{
					if(pkcsBlock[i]==0)
					{
						break;
					}
				}
				i++;
				fprintf(stdout,"CryptoRSAEncrypt::decrypt - 6\n");
				if(i < modulusLen)
				{
					int outLen = modulusLen - i;
					if(outLen+11 <= modulusLen)
					{
						QByteArray oArr(reinterpret_cast<const tchar *>(&pkcsBlock[i]),outLen);
						out = oArr;
					}
					fprintf(stdout,"CryptoRSAEncrypt::decrypt - 7\n");
				}
			}
			::memset(pkcsArr.data(),0, static_cast<size_t>(pkcsArr.length()));
*/
		}
		fprintf(stdout,"CryptoRSAEncrypt::decrypt - 8\n");
	}
	fprintf(stdout,"CryptoRSAEncrypt::decrypt - 9\n");
	return out;
}

//-------------------------------------------------------------------------------------------

QByteArray CryptoRSAEncrypt::publicEncrypt(const QByteArray& in)
{
	int i,modulusLen;
	QByteArray out,pkcsArr(MAX_RSA_MODULUS_LEN,'\0');
	
	modulusLen = (m_bits+7) / 8;
	if(in.length()+11 <= modulusLen)
	{
		tubyte *pkcsBlock = reinterpret_cast<tubyte *>(pkcsArr.data());
		
		pkcsBlock[0] = 0;
		pkcsBlock[1] = 2;
		
		for(i=2;i<modulusLen-in.length()-1;i++)
		{
			tubyte b;
			
			do
			{
				m_primes.random().generateBytes(&b,1);
			} while(b==0);
			pkcsBlock[i] = b;
		}
		pkcsBlock[i++] = 0;
		::memcpy(&pkcsBlock[i],in.data(),static_cast<size_t>(in.length()));
		pkcsArr.resize(modulusLen);
		
		out = privateD(pkcsArr);
		::memset(pkcsArr.data(),0,static_cast<size_t>(pkcsArr.length()));
	}
	return out;
}

//-------------------------------------------------------------------------------------------

QByteArray CryptoRSAEncrypt::publicDecrypt(const QByteArray& in)
{
	int i,modulusLen;
	QByteArray out;
	
	modulusLen = (m_bits + 7) / 8;
	if(in.length() <= modulusLen)
	{
		QByteArray pkcsArr = publicE(in);
		if(!pkcsArr.isEmpty())
		{
			const tubyte *pkcsBlock = reinterpret_cast<const tubyte *>(pkcsArr.data());
			
			if(pkcsBlock[0]==0 && pkcsBlock[1]==2 && pkcsArr.length()==modulusLen)
			{
				for(i=2;i<modulusLen-1;i++)
				{
					if(pkcsBlock[i]==0)
					{
						break;
					}
				}
				i++;
				if(i < modulusLen)
				{
					int outLen = modulusLen - i;
					if(outLen+11 <= modulusLen)
					{
						QByteArray oArr(reinterpret_cast<const tchar *>(&pkcsBlock[i]),outLen);
						out = oArr;
					}
				}
			}
			::memset(pkcsArr.data(),0,static_cast<size_t>(pkcsArr.length()));
		}
	}
	return out;
}

//-------------------------------------------------------------------------------------------
// <PublicKey bits="xxx" modulus="yyy" exponent="zzz"></PublicKey>
//-------------------------------------------------------------------------------------------

QString CryptoRSAEncrypt::writePublicKeyXML()
{
	QString pKey;
	pKey = "<PublicKey bits=\"" + QString::number(m_bits) + "\" modulus=\"" + m_modulus.toString() + "\" exponent=\"" + m_publicExponent.toString() + "\"></PublicKey>";
	return pKey;
}

//-------------------------------------------------------------------------------------------

bool CryptoRSAEncrypt::readPublicKeyXML(const QString& iStr)
{
	QByteArray iMem;
	bool res = false;
	
	if(!iStr.isEmpty())
	{
		xmlDocPtr doc;
		
		iMem = iStr.toUtf8();
		doc = xmlParseMemory(iMem.constData(),iMem.length());
		if(doc!=0)
		{
			xmlXPathObjectPtr pKNode = getNodeSet(doc,"/PublicKey");
			if(pKNode!=0)
			{
				int i;
				
				for(i=0;i<pKNode->nodesetval->nodeNr && !res;i++)
				{
					xmlChar *bitsC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("bits"));
					xmlChar *modulusC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("modulus"));
					xmlChar *exponentC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("exponent"));
					
					if(bitsC!=0 && modulusC!=0 && exponentC!=0)
					{
						QString bitsS = QString::fromUtf8(reinterpret_cast<const tchar *>(bitsC));
						QString modulusS = QString::fromUtf8(reinterpret_cast<const tchar *>(modulusC));
						QString exponentS = QString::fromUtf8(reinterpret_cast<const tchar *>(exponentC));
						
						m_bits = bitsS.toInt();
						if(m_bits>=MIN_RSA_MODULUS_BITS && m_bits<=MAX_RSA_MODULUS_BITS)
						{
							m_modulus.fromString(modulusS);
							m_publicExponent.fromString(exponentS);
							res = true;
						}
					}
					
					if(bitsC!=0)
					{
						xmlFree(bitsC);
					}
					if(modulusC!=0)
					{
						xmlFree(modulusC);
					}
					if(exponentC!=0)
					{
						xmlFree(exponentC);
					}
				}
				xmlXPathFreeObject(pKNode);
			}
			xmlFreeDoc(doc);
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------
// <PrivateKey bits="xxx" exponent="yyy" primeA="zzz" primeB="xxx" primeExponentA="yyy" primeExponentB="zzz" coefficient="xxx"></PrivateKey>
//-------------------------------------------------------------------------------------------

QString CryptoRSAEncrypt::writePrivateKeyXML()
{
	QString pKey;
	pKey  = "<PrivateKey ";
	pKey += "bits=\"" + QString::number(m_bits) + "\" ";
	pKey += "exponent=\"" + m_exponent.toString() + "\" ";
	pKey += "primeA=\"" + m_prime[0].toString() + "\" ";
	pKey += "primeB=\"" + m_prime[1].toString() + "\" ";
	pKey += "primeExponentA=\"" + m_primeExponent[0].toString() + "\" ";
	pKey += "primeExponentB=\"" + m_primeExponent[1].toString() + "\" ";
	pKey += "coefficient=\"" + m_coefficient.toString() + "\"></PrivateKey>";
	return pKey;
}

//-------------------------------------------------------------------------------------------

bool CryptoRSAEncrypt::readPrivateKeyXML(const QString& iStr)
{
	QByteArray iMem;
	bool res = false;
	
	if(!iStr.isEmpty())
	{
		xmlDocPtr doc;
		
		iMem = iStr.toUtf8();
		doc = xmlParseMemory(iMem.constData(),iMem.length());
		if(doc!=0)
		{
			xmlXPathObjectPtr pKNode = getNodeSet(doc,"/PrivateKey");
			if(pKNode!=0)
			{
				int i;
				
				for(i=0;i<pKNode->nodesetval->nodeNr && !res;i++)
				{
					xmlChar *bitsC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("bits"));
					xmlChar *exponentCC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("exponent"));
					xmlChar *primeAC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("primeA"));
					xmlChar *primeBC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("primeB"));
					xmlChar *primeExponentAC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("primeExponentA"));
					xmlChar *primeExponentBC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("primeExponentB"));
					xmlChar *coefficientC = xmlGetProp(pKNode->nodesetval->nodeTab[i],reinterpret_cast<const xmlChar *>("coefficient"));
					
					if(bitsC!=0 && exponentCC!=0 && primeAC!=0 && primeBC!=0 && primeExponentAC!=0 && primeExponentBC!=0 && coefficientC!=0)
					{
						QString bitsS = QString::fromUtf8(reinterpret_cast<const tchar *>(bitsC));
						QString exponentC = QString::fromUtf8(reinterpret_cast<const tchar *>(exponentCC));
						QString primeAS = QString::fromUtf8(reinterpret_cast<const tchar *>(primeAC));
						QString primeBS = QString::fromUtf8(reinterpret_cast<const tchar *>(primeBC));
						QString primeExponentAS = QString::fromUtf8(reinterpret_cast<const tchar *>(primeExponentAC));
						QString primeExponentBS = QString::fromUtf8(reinterpret_cast<const tchar *>(primeExponentBC));
						QString coefficientS = QString::fromUtf8(reinterpret_cast<const tchar *>(coefficientC));
						
						m_bits = bitsS.toInt();
						if(m_bits>=MIN_RSA_MODULUS_BITS && m_bits<=MAX_RSA_MODULUS_BITS)
						{
							m_exponent.fromString(exponentC);
							m_prime[0].fromString(primeAS);
							m_prime[1].fromString(primeBS);
							m_primeExponent[0].fromString(primeExponentAS);
							m_primeExponent[1].fromString(primeExponentBS);
							m_coefficient.fromString(coefficientS);
							res = true;
						}
					}
					
					if(bitsC!=0)
					{
						xmlFree(bitsC);
					}
					if(exponentCC!=0)
					{
						xmlFree(exponentCC);
					}
					if(primeAC!=0)
					{
						xmlFree(primeAC);
					}
					if(primeBC!=0)
					{
						xmlFree(primeBC);
					}
					if(primeExponentAC!=0)
					{
						xmlFree(primeExponentAC);
					}
					if(primeExponentBC!=0)
					{
						xmlFree(primeExponentBC);
					}
					if(coefficientC!=0)
					{
						xmlFree(coefficientC);
					}
				}
				xmlXPathFreeObject(pKNode);
			}
			xmlFreeDoc(doc);
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

xmlXPathObjectPtr CryptoRSAEncrypt::getNodeSet(xmlDocPtr doc,const QString& path)
{
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result = 0;
	
	context = xmlXPathNewContext(doc);
	if(context!=0)
	{
		result = xmlXPathEvalExpression(reinterpret_cast<const xmlChar *>(path.toUtf8().constData()),context);
		if(result!=0)
		{
			if(xmlXPathNodeSetIsEmpty(result->nodesetval))
			{
				xmlXPathFreeObject(result);
				result = 0;
			}
		}
		xmlXPathFreeContext(context);
	}
	return result;
}

//-------------------------------------------------------------------------------------------
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
