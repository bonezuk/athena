#include "network/mime/inc/LicenseClient.h"
#include "common/inc/CRC32.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------

static const tchar c_publicLicenseKey[] = "<PublicKey bits=\"2048\" modulus=\"23400945526777650439415255458984890605445440271028541340182082449345685474323245478717502317677910663468712965242924272298855064129899380714298171874354193628842124788050415147707038766922674384458455504667100142834357508937092584278740722364041648817964448505667902489612952522255140113766270624504334197956471679212277918181536319808531071525023573734472094129159239006380630841985626956619068027274602628252497122360472172975369729211249137432879234823097030356126510679069656944859406732207188068221317983973942390866863498816763354779257409048309720440963368358240821612716791250849832093970678301227861686133317\" exponent=\"3\"></PublicKey>";

QSharedPointer<LicenseClient> LicenseClient::m_instance;

//-------------------------------------------------------------------------------------------
// LicenseClientEvent
//-------------------------------------------------------------------------------------------

LicenseClientEvent::LicenseClientEvent(LicenseClientEventType t) : QEvent(static_cast<QEvent::Type>(t)),
	m_threadId(QThread::currentThreadId()),
	m_name(),
	m_service(0)
{}

//-------------------------------------------------------------------------------------------

Qt::HANDLE LicenseClientEvent::threadId()
{
	return m_threadId;
}

//-------------------------------------------------------------------------------------------

QString& LicenseClientEvent::name()
{
	return m_name;
}

//-------------------------------------------------------------------------------------------

const QString& LicenseClientEvent::name() const
{
	return m_name;
}

//-------------------------------------------------------------------------------------------

LCTService *LicenseClientEvent::service()
{
	return m_service;
}

//-------------------------------------------------------------------------------------------

void LicenseClientEvent::setService(LCTService *s)
{
	m_service = s;
}

//-------------------------------------------------------------------------------------------
// LicenseClientWaitCondition
//-------------------------------------------------------------------------------------------

LicenseClientWaitCondition::LicenseClientWaitCondition() : m_mutex(),
	m_condition(),
	m_flag(false),
	m_time(),
	m_name()
{}

//-------------------------------------------------------------------------------------------

void LicenseClientWaitCondition::get()
{
	m_mutex.lock();
}

//-------------------------------------------------------------------------------------------

void LicenseClientWaitCondition::wait()
{
	m_condition.wait(&m_mutex);
	m_mutex.unlock();
}

//-------------------------------------------------------------------------------------------

void LicenseClientWaitCondition::wake()
{
	m_mutex.lock();
	m_condition.wakeAll();
	m_mutex.unlock();
}

//-------------------------------------------------------------------------------------------

bool& LicenseClientWaitCondition::flag()
{
	return m_flag;
}

//-------------------------------------------------------------------------------------------

const bool& LicenseClientWaitCondition::flag() const
{
	return m_flag;
}

//-------------------------------------------------------------------------------------------

common::TimeStamp& LicenseClientWaitCondition::time()
{
	return m_time;
}

//-------------------------------------------------------------------------------------------

const common::TimeStamp& LicenseClientWaitCondition::time() const
{
	return m_time;
}

//-------------------------------------------------------------------------------------------

QString& LicenseClientWaitCondition::name()
{
	return m_name;
}

//-------------------------------------------------------------------------------------------

const QString& LicenseClientWaitCondition::name() const
{
	return m_name;
}

//-------------------------------------------------------------------------------------------
// LCTService
//-------------------------------------------------------------------------------------------

LCTService::LCTService(QObject *parent) : QObject(parent)
{}

//-------------------------------------------------------------------------------------------

LCTService::~LCTService()
{}

//-------------------------------------------------------------------------------------------
// LicenseClientThread
//-------------------------------------------------------------------------------------------

LicenseClientThread::LicenseClientThread(QObject *parent) : QThread(parent),
	m_licenseClient(),
	m_mutex(),
	m_condition()
{}

//-------------------------------------------------------------------------------------------

LicenseClientThread::~LicenseClientThread()
{}

//-------------------------------------------------------------------------------------------

bool LicenseClientThread::ignite()
{
	m_mutex.lock();
	start();
	m_condition.wait(&m_mutex);
	m_mutex.unlock();
    return (m_licenseClient.data()!=0) ? true : false;
}

//-------------------------------------------------------------------------------------------

void LicenseClientThread::run()
{
    QSharedPointer<LicenseClient> lClient(new LicenseClient);
	m_licenseClient = lClient;
	m_condition.wakeAll();
    QTimer::singleShot(100,lClient.data(),SLOT(onStart()));
	exec();
	lClient->onStop();
}

//-------------------------------------------------------------------------------------------

QSharedPointer<LicenseClient> LicenseClientThread::licenseClient()
{
	return m_licenseClient;
}

//-------------------------------------------------------------------------------------------
// LicenseClient
//-------------------------------------------------------------------------------------------

LicenseClient::LicenseClient(QObject *parent) : QObject(parent),
	m_state(e_stateInitial),
	m_currentTime(0),
	m_currentRegistrationID(),
	m_thread(0),
	m_mutex(),
	m_waitConditionMap(),
	m_trialTS(),
	m_licClientName(),
	m_licenseFlag(false),
	m_servicesTimer(0),
	m_services()
{}

//-------------------------------------------------------------------------------------------

LicenseClient::~LicenseClient()
{
	freeConditions();
}

//-------------------------------------------------------------------------------------------

QSharedPointer<LicenseClient> LicenseClient::instance()
{
    if(m_instance.data()==0)
	{
		LicenseClientThread *lThread = new LicenseClientThread;
		if(lThread->ignite())
		{
			lThread->licenseClient()->m_thread = lThread;
			m_instance = lThread->licenseClient();
		}
		else
		{
			delete lThread;
		}
	}
	return m_instance;
}

//-------------------------------------------------------------------------------------------

void LicenseClient::end()
{
    if(m_instance.data()!=0)
	{
		LicenseClientThread *lThread = m_instance->m_thread;
	
		if(lThread!=0)
		{
			lThread->quit();
			lThread->wait();
			delete lThread;
		}
        m_instance = QSharedPointer<LicenseClient>();
	}
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onStart()
{
#if !defined(OMEGA_MAC_STORE)
	QString licFilename = getLicenseFilename();
	
	m_state = e_stateInitial;
	m_currentRegistrationID = getRegistrationKey();
	getCurrentTime();
	
	checkLicenseCRC();

	if(!licFilename.isEmpty() && common::DiskOps::exist(licFilename))
	{
		if(isValidLicense(licFilename))
		{
			m_state = e_stateLicensed;
		}
	}
	onProcess();
#endif

	m_servicesTimer = new QTimer(this);
	QObject::connect(m_servicesTimer,SIGNAL(timeout()),this,SLOT(onServices()));
	m_servicesTimer->start(5000);
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onStop()
{
	if(m_servicesTimer!=0)
	{
		m_servicesTimer->stop();
		delete m_servicesTimer;
		m_servicesTimer = 0;
	}
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onProcess()
{
	switch(m_state)
	{
		case e_stateInitial:
			if(!m_currentRegistrationID.isEmpty())
			{
				getLicense(c_licenseServerHost,c_licenseServerPort,false);
			}
			else
			{
				getLicense(c_licenseServerHost,c_licenseServerPort,true);
			}
			break;
			
		case e_stateCheckLocalLicense:
			break;
			
		case e_stateGetLicenseFromServer:
			break;
			
		case e_stateGetTrialFromServer:
			break;
			
		case e_stateLicensed:
			emit licensed();
			m_state = e_stateDone;
			m_licenseFlag = true;
			break;
			
		case e_stateNotLicensed:
			emit notLicensed();
			m_state = e_stateDone;
			m_licenseFlag = false;
			break;
			
		case e_stateDone:
			break;
	}
	if(m_state!=e_stateDone)
	{
		QTimer::singleShot(250,this,SLOT(onProcess()));
	}
}

//-------------------------------------------------------------------------------------------

QString LicenseClient::getLicenseFilename()
{
	QSettings iSettings(QSettings::IniFormat,QSettings::UserScope,QCoreApplication::organizationName(),QCoreApplication::applicationName());
	QFileInfo fInfo(iSettings.fileName());
	QString userDir = fInfo.absolutePath();
	if(userDir.at(userDir.length()-1)!='/' && userDir.at(userDir.length()-1)!='\\')
	{
		userDir += "/";
	}
	userDir = QDir::toNativeSeparators(userDir);
	orcus::common::DiskOps::path(userDir,true);
	QString licFilename = userDir + "license.dat";
	return licFilename;
}

//-------------------------------------------------------------------------------------------
// Unique ID produced by Program Name and Hardware ID
//-------------------------------------------------------------------------------------------

QString LicenseClient::getProgramKey()
{
	QString programKey;
	QSettings settings;
	
	settings.beginGroup("license");
	if(settings.contains("programKey"))
	{
		programKey = settings.value("programKey").toString();
	}
	if(programKey.isEmpty())
	{
		QVector<QPair<common::HardwareIdentification::HardwareType,QString> > idList;
		if(common::HardwareIdentification::instance().getIdentity(idList))
		{
			int i,j;
			
			for(j=0;j<3 && programKey.isEmpty();j++)
			{
				common::HardwareIdentification::HardwareType hType = static_cast<common::HardwareIdentification::HardwareType>(j);
				for(i=0;i<idList.size() && programKey.isEmpty();i++)
				{
					const QPair<common::HardwareIdentification::HardwareType,QString>& idEntry = idList.at(i);
					if(idEntry.first==hType)
					{
						common::SHA1Digest digest;
						digest.input(idEntry.second.toUtf8().constData(),idEntry.second.toUtf8().length());
						programKey = getDigestKey(digest);
					}
				}
			}
		}
		if(!programKey.isEmpty())
		{
			settings.setValue("programKey",QVariant(programKey));
		}
	}
	settings.endGroup();
	return programKey;
}

//-------------------------------------------------------------------------------------------
// Unique ID produced by blackomega.co.uk website on payment
//-------------------------------------------------------------------------------------------

QString LicenseClient::getRegistrationKey()
{
	QString regKey;
	QSettings settings;
	
	settings.beginGroup("license");
	if(settings.contains("registrationKey"))
	{
		regKey = settings.value("registrationKey").toString();
	}
	return regKey;
}

//-------------------------------------------------------------------------------------------
// Build key from digest
// XXXXX-XXXX-XXXX-XXXX-XXXXX
// 0 1 2  3 4  5 6  7 8  9 0
//-------------------------------------------------------------------------------------------

QString LicenseClient::getDigestKey(common::SHA1Digest& digest)
{
	tint i;
	QString key;
	tbyte mem[common::c_SHA1HashSize];
	
	if(digest.getDigestFinal(mem,common::c_SHA1HashSize))
	{
		for(i=0;i<11;i++)
		{
			tubyte a,b,c;
			tchar t[2] = {'\0','\0'};
			
			a = static_cast<tubyte>(mem[i]);
			b = (a >> 4) & 0x0f;
			c = a & 0x0f;
			
			if(b>=0 && b<10)
			{
				t[0] = static_cast<tchar>(b) + '0';
			}
			else
			{
				t[0] = static_cast<tchar>(b-10) + 'A';
			}
			key += t;
			
			if(i==2 || i==4 || i==6 || i==8)
			{
				key += "-";
			}
			
			if(c>=0 && c<10)
			{
				t[0] = static_cast<tchar>(c) + '0';
			}
			else
			{
				t[0] = static_cast<tchar>(c-10) + 'A';
			}
			key += t;
		}
	}
	return key;
}

//-------------------------------------------------------------------------------------------

bool LicenseClient::isValidLicense(const QString& licFilename)
{
	LicenseFile licFile;
	bool res = false;
	
	if(licFile.read(licFilename))
	{
		res = isValidLicense(licFile);
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool LicenseClient::isValidLicense(const LicenseFile& licFile)
{
	tint i,j;
	common::CryptoRSAEncrypt lCrypto;
	QString publicKeyStr = QString::fromLatin1(c_publicLicenseKey);
	bool res = false;
	
	if(licFile.size()<3)
	{
		return false;
	}
	
	if(lCrypto.readPublicKeyXML(publicKeyStr))
	{
		QVector<QPair<common::HardwareIdentification::HardwareType,QString> > idList;
		
		if(common::HardwareIdentification::instance().getIdentity(idList))			
		{
			for(i=2;i<licFile.size() && !res;i++)
			{
				QByteArray dMem = lCrypto.publicDecrypt(licFile.block(i));
				if(!dMem.isEmpty())
				{
					QString hardwareID;
					common::TimeStamp certTimestamp;
					
					if(getHardwareTimeFromLicense(dMem,hardwareID,certTimestamp))
					{
						for(j=0;j<idList.size() && !res;j++)
						{
							if(idList.at(j).second==hardwareID)
							{
								if(certTimestamp>1.0)
								{
									m_trialTS = certTimestamp;
									if(m_currentTime < certTimestamp)
									{
										res = true;
									}
								}
								else
								{
									m_trialTS = 0.0;
									res = true;
								}
							}
						}
					}
				}
			}
			
			if(res)
			{
				QByteArray rMem = lCrypto.publicDecrypt(licFile.block(0));
				QByteArray cMem = lCrypto.publicDecrypt(licFile.block(1));
				QString lRegCode = QString::fromUtf8(rMem.constData(),rMem.size());
				QString oRegCode = getRegistrationKey();

				m_licClientName = QString::fromUtf8(cMem.constData(),cMem.size());

				if(!oRegCode.isEmpty() && !lRegCode.isEmpty() && lRegCode!=oRegCode)
				{
					res = false;
				}
			}
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool LicenseClient::getHardwareTimeFromLicense(const QByteArray& mem,QString& hID,common::TimeStamp& tS)
{
	bool res = false;
	
	if(mem.length()>8)
	{
		const tchar *x = mem.constData();
		QByteArray hIDArr(&x[8],mem.length()-8);
		tS = NTPClient::toTimeStamp(x);
		hID = QString::fromUtf8(hIDArr.constData(),hIDArr.length());
		if(!hID.isEmpty())
		{
			res = true;
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void LicenseClient::setHardwareTimeToLicense(const QString& hID,const common::TimeStamp& tS,QByteArray& mem)
{
	tchar tMem[8];
	QByteArray hIDArr(hID.toUtf8());
	
	NTPClient::fromTimeStamp(tS,tMem);
	mem.clear();
	mem.append(tMem,8);
	mem.append(hIDArr);
}

//-------------------------------------------------------------------------------------------

void LicenseClient::getCurrentTime()
{
	NTPClient ntp;
	
	m_currentTime = ntp.getTime("pool.ntp.org");
	if(m_currentTime==0.0)
	{
		m_currentTime = common::TimeStamp::now();
	}
}

//-------------------------------------------------------------------------------------------

QByteArray LicenseClient::buildLicense()
{
	QString regID,programID;
	LicenseFile licFile;
	QByteArray licArr;
	
	regID = getRegistrationKey();
	programID = getProgramKey();
	if(!programID.isEmpty())
	{
		QVector<QPair<common::HardwareIdentification::HardwareType,QString> > idList;
		
		if(common::HardwareIdentification::instance().getIdentity(idList) && idList.size()>0)
		{
			tint i;
			QVector<QByteArray> lList;
			
			lList.append(regID.toUtf8());
			lList.append(programID.toUtf8());
			for(i=0;i<idList.size();i++)
			{
				lList.append(idList.at(i).second.toUtf8());
			}
			
			if(!licFile.write(licArr,lList))
			{
				licArr.clear();
			}
		}
	}
	return licArr;
}

//-------------------------------------------------------------------------------------------

void LicenseClient::getLicense(const QString& licServerHost,tint licServerPort,bool trialFlag)
{
	http::HTTPClientService *clientService = dynamic_cast<http::HTTPClientService *>(Controller::instance()->getService("http_client"));
	bool res = false;
	
	if(clientService!=0)
	{
		QByteArray licArr = buildLicense();
		if(licArr.size()>0)
		{
			http::HTTPClient *client = clientService->getClient();
			if(client!=0)
			{
				tint nID;
				
				client->setHost(licServerHost,licServerPort);
				nID = client->newTransaction();
				http::HTTPCTransaction& trans = client->transaction(nID);
			
				if(trialFlag)
				{
					trans.request().request(http::Unit::e_Post,"/generateTrial");
				}
				else
				{
					trans.request().request(http::Unit::e_Post,"/generateKeys");
					if(!m_currentRegistrationID.isEmpty())
					{
						trans.request().query().add("oldRegistration",m_currentRegistrationID);
					}
				}
				trans.request().add("User-Agent","BlackOmega/2.0");
				trans.request().add("Content-Length",QString::number(licArr.size()));
				trans.requestData().AppendRaw(licArr.constData(),licArr.size());
				
				QObject::connect(client,SIGNAL(onTransaction(network::http::HTTPCTransaction*)),this,SLOT(onLicenseTransaction(network::http::HTTPCTransaction*)),Qt::BlockingQueuedConnection);
				QObject::connect(client,SIGNAL(onTransactionError(network::http::HTTPCTransaction*,const QString&)),this,SLOT(onLicenseTransactionError(network::http::HTTPCTransaction*,const QString&)));
				QObject::connect(client,SIGNAL(onError(network::http::HTTPClient*,const QString&)),this,SLOT(onLicenseClientError(network::http::HTTPClient*,const QString&)));
				QObject::connect(client,SIGNAL(onComplete(network::http::HTTPClient*)),this,SLOT(onLicenseComplete(network::http::HTTPClient*)));
				
				client->run();
				
				m_state = (trialFlag) ? e_stateGetTrialFromServer : e_stateGetLicenseFromServer;
				res = true;
			}
		}
	}
	
	if(!res)
	{
		m_state = e_stateLicensed;
	}
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onLicenseTransaction(network::http::HTTPCTransaction *trans)
{
	bool res = false;
	
	if(m_state==e_stateGetLicenseFromServer || m_state==e_stateGetTrialFromServer)
	{
		if(trans->response().response()==200)
		{
			QByteArray licArr(reinterpret_cast<const char *>(trans->responseData().GetData()),trans->responseData().GetSize());
			LicenseFile licFile;
			
			if(licFile.read(licArr))
			{
				if(isValidLicense(licFile))
				{
					QString fileName = getLicenseFilename();
					if(!fileName.isEmpty())
					{
						common::BIOStream wFile(common::e_BIOStream_FileCreate | common::e_BIOStream_FileWrite);
						if(wFile.open(fileName))
						{
							wFile.write(licArr.constData(),licArr.size());
							wFile.close();
						}
						res = true;
					}
				}
			}
		}
		else
		{
			QString err = QString::fromUtf8(reinterpret_cast<const char *>(trans->responseData().GetData()),trans->responseData().GetSize());
			emit licenseError(err);
			m_state = e_stateLicensed;
		}
	}
	if(m_state==e_stateGetLicenseFromServer)
	{
		m_state = (res) ? e_stateLicensed : e_stateGetTrialFromServer;
	}
	else
	{
		m_state = (res) ? e_stateLicensed : e_stateNotLicensed;
	}
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onLicenseTransactionError(network::http::HTTPCTransaction *trans,const QString& err)
{
	if(m_state==e_stateGetLicenseFromServer)
	{
		m_state = e_stateNotLicensed;
	}
	else if(m_state==e_stateGetTrialFromServer)
	{
		m_state = e_stateNotLicensed;
	}
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onLicenseClientError(network::http::HTTPClient *client,const QString& err)
{
	if(m_state==e_stateGetLicenseFromServer)
	{
		QString err("Failed to contact license authentication server.");
		emit licenseError(err);
		m_state = e_stateLicensed;
	}
	else if(m_state==e_stateGetTrialFromServer)
	{
		m_state = e_stateLicensed;
	}
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onLicenseComplete(network::http::HTTPClient *client)
{}

//-------------------------------------------------------------------------------------------

bool LicenseClient::isValid()
{
#if !defined(OMEGA_MAC_STORE)
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_isValidEvent);
	LicenseClientWaitCondition *c = getCondition();
	QCoreApplication::postEvent(this,e);
	c->wait();
	return c->flag();
#else
	return true;
#endif
}

//-------------------------------------------------------------------------------------------

bool LicenseClient::isLicensed()
{
#if !defined(OMEGA_MAC_STORE)
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_isLicensedEvent);
	LicenseClientWaitCondition *c = getCondition();
	QCoreApplication::postEvent(this,e);
	c->wait();
	return c->flag();
#else
	return true;
#endif
}

//-------------------------------------------------------------------------------------------

bool LicenseClient::isTrial()
{
#if !defined(OMEGA_MAC_STORE)
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_isTrialEvent);
	LicenseClientWaitCondition *c = getCondition();
	QCoreApplication::postEvent(this,e);
	c->wait();
	return c->flag();
#else
	return false;
#endif
}

//-------------------------------------------------------------------------------------------

common::TimeStamp LicenseClient::endOfTrialDate()
{
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_isTrialEvent);
	LicenseClientWaitCondition *c = getCondition();
	QCoreApplication::postEvent(this,e);
	c->wait();
	return c->time();
}

//-------------------------------------------------------------------------------------------

QString LicenseClient::getRegistrationCode()
{
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_getRegCodeEvent);
	LicenseClientWaitCondition *c = getCondition();
	QCoreApplication::postEvent(this,e);
	c->wait();
	return c->name();	
}

//-------------------------------------------------------------------------------------------

void LicenseClient::setRegistrationCode(const QString& rCode)
{
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_setRegCodeEvent);
	e->name() = rCode;
	QCoreApplication::postEvent(this,e);
}

//-------------------------------------------------------------------------------------------

QString LicenseClient::getCustomerName()
{
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_getCustomerEvent);
	LicenseClientWaitCondition *c = getCondition();
	QCoreApplication::postEvent(this,e);
	c->wait();
	return c->name();
}

//-------------------------------------------------------------------------------------------

void LicenseClient::deactivate()
{
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_deactivateEvent);
	QCoreApplication::postEvent(this,e);	
}

//-------------------------------------------------------------------------------------------

void LicenseClient::addService(LCTService *s)
{
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_addServiceEvent);
	LicenseClientWaitCondition *c = getCondition();
	e->setService(s);
	QCoreApplication::postEvent(this,e);
	c->wait();
}

//-------------------------------------------------------------------------------------------

void LicenseClient::delService(LCTService *s)
{
	LicenseClientEvent *e = new LicenseClientEvent(LicenseClientEvent::e_delServiceEvent);
	LicenseClientWaitCondition *c = getCondition();
	e->setService(s);
	QCoreApplication::postEvent(this,e);
	c->wait();
}

//-------------------------------------------------------------------------------------------

bool LicenseClient::event(QEvent *e)
{
	LicenseClientEvent *lEvent = dynamic_cast<LicenseClientEvent *>(e);

	if(lEvent!=0 && static_cast<LicenseClientEvent::LicenseClientEventType>(e->type())>=LicenseClientEvent::e_isValidEvent)
	{
		LicenseClientEvent::LicenseClientEventType t = static_cast<LicenseClientEvent::LicenseClientEventType>(e->type());
		LicenseClientWaitCondition *c;
		
		switch(t)
		{
#if !defined(OMEGA_MAC_STORE)
			case LicenseClientEvent::e_isValidEvent:
				{
					c = getCondition(lEvent->threadId());
					c->flag() = (m_state==e_stateDone) ? true : false;
					c->wake();
					e->accept();
				}
				break;
				
			case LicenseClientEvent::e_isLicensedEvent:
				{
					c = getCondition(lEvent->threadId());
					if(m_state==e_stateDone)
					{
						QString licFilename = getLicenseFilename();

						if(m_licenseFlag && !common::DiskOps::exist(licFilename))
						{
							c->flag() = true;
						}
						else
						{
							if(isValidLicense(getLicenseFilename()))
							{
								c->flag() = true;
							}
							else
							{
								c->flag() = false;
							}
						}
					}
					else
					{
						c->flag() = false;
					}
					c->wake();
					e->accept();
				}
				break;
				
			case LicenseClientEvent::e_isTrialEvent:
				{
					c = getCondition(lEvent->threadId());
					if(m_state==e_stateDone)
					{
						QString licFilename = getLicenseFilename();

						if(m_licenseFlag && !common::DiskOps::exist(licFilename))
						{
							c->flag() = true;
						}
						else
						{
							if(isValidLicense(getLicenseFilename()))
							{
								c->flag() = (m_trialTS>1.0) ? true : false;
							}
							else
							{
								c->flag() = false;
							}
						}
					}
					else
					{
						c->flag() = false;
					}
					c->time() = m_trialTS;
					c->wake();
					e->accept();
				}
				break;

			case LicenseClientEvent::e_getRegCodeEvent:
				{
					c = getCondition(lEvent->threadId());
					if(m_state==e_stateDone)
					{
						c->name() = getRegistrationKey();
					}
					else
					{
						c->name() = "";
					}
					c->wake();
					e->accept();
				}
				break;
				
			case LicenseClientEvent::e_setRegCodeEvent:
				{
					QSettings settings;
					
					if(!lEvent->name().isEmpty())
					{
						m_state = e_stateInitial;
						settings.beginGroup("license");
						settings.setValue("registrationKey",QVariant(lEvent->name()));
						settings.endGroup();
						m_currentRegistrationID = lEvent->name();
						onProcess();
					}
				}
				break;
				
			case LicenseClientEvent::e_getCustomerEvent:
				{
					c = getCondition(lEvent->threadId());
					if(m_state==e_stateDone)
					{
						c->name() = m_licClientName;
					}
					else
					{
						c->name() = "";
					}
					c->wake();
					e->accept();
				}
				break;
				
			case LicenseClientEvent::e_deactivateEvent:
				{
					doDeactivate(c_licenseServerHost,c_licenseServerPort);
					e->accept();
				}
				break;
#endif

			case LicenseClientEvent::e_addServiceEvent:
				{
					c = getCondition(lEvent->threadId());
					m_services.insert(lEvent->service());
					c->wake();
					e->accept();
					onServices();
				}
				break;

			case LicenseClientEvent::e_delServiceEvent:
				{
					QSet<LCTService *>::iterator ppI;

					c = getCondition(lEvent->threadId());
					ppI = m_services.find(lEvent->service());
					if(ppI!=m_services.end())
					{
						m_services.erase(ppI);
					}
					c->wake();
					e->accept();
				}
				break;
				
			default:
				return QObject::event(e);
				break;
		}
		return true;
	}
	else
	{
		return QObject::event(e);
	}
}

//-------------------------------------------------------------------------------------------

LicenseClientWaitCondition *LicenseClient::getCondition()
{
	LicenseClientWaitCondition *c = getCondition(QThread::currentThreadId());
	c->get();
	return c;
}

//-------------------------------------------------------------------------------------------

LicenseClientWaitCondition *LicenseClient::getCondition(Qt::HANDLE id)
{
	LicenseClientWaitCondition *c;
	QMap<Qt::HANDLE,LicenseClientWaitCondition *>::iterator ppI;
	
	m_mutex.lock();
	ppI = m_waitConditionMap.find(id);
	if(ppI!=m_waitConditionMap.end())
	{
		c = ppI.value();
	}
	else
	{
		c = new LicenseClientWaitCondition;
		m_waitConditionMap.insert(id,c);
	}
	m_mutex.unlock();
	return c;
}

//-------------------------------------------------------------------------------------------

void LicenseClient::freeConditions()
{
	QMap<Qt::HANDLE,LicenseClientWaitCondition *>::iterator ppI;
	
	m_mutex.lock();
	while(ppI = m_waitConditionMap.begin(), ppI!=m_waitConditionMap.end())
	{
		LicenseClientWaitCondition *c = ppI.value();
		m_waitConditionMap.erase(ppI);
		delete c;
	}
	m_mutex.unlock();
}

//-------------------------------------------------------------------------------------------

void LicenseClient::doDeactivate(const QString& licServerHost,tint licServerPort)
{
	http::HTTPClientService *clientService = dynamic_cast<http::HTTPClientService *>(Controller::instance()->getService("http_client"));
	
	if(clientService!=0)
	{
		QString licFilename = getLicenseFilename();
		if(common::DiskOps::exist(licFilename))
		{
			common::BIOStream lFile(common::e_BIOStream_FileRead);
			
			if(lFile.open(licFilename))
			{
				QByteArray lArr(lFile.size(),'\0');
				if(lFile.read(lArr.data(),lArr.size())==lArr.size())
				{
					http::HTTPClient *client = clientService->getClient();
					if(client!=0)
					{
						tint nID;
				
						client->setHost(licServerHost,licServerPort);
						nID = client->newTransaction();
						http::HTTPCTransaction& trans = client->transaction(nID);
					
						trans.request().request(http::Unit::e_Post,"/deactivate");
						trans.request().query().add("programID",getProgramKey());
						trans.request().add("User-Agent","BlackOmega/2.0");
						trans.request().add("Content-Length",QString::number(lArr.size()));
						trans.requestData().AppendRaw(lArr.constData(),lArr.size());
						
						QObject::connect(client,SIGNAL(onTransaction(network::http::HTTPCTransaction*)),this,SLOT(onDeactivateTransaction(network::http::HTTPCTransaction*)),Qt::BlockingQueuedConnection);
						QObject::connect(client,SIGNAL(onTransactionError(network::http::HTTPCTransaction*,const QString&)),this,SLOT(onDeactivateTransactionError(network::http::HTTPCTransaction*,const QString&)));
						QObject::connect(client,SIGNAL(onError(network::http::HTTPClient*,const QString&)),this,SLOT(onDeactivateClientError(network::http::HTTPClient*,const QString&)));
						QObject::connect(client,SIGNAL(onComplete(network::http::HTTPClient*)),this,SLOT(onDeactivateComplete(network::http::HTTPClient*)));
				
						client->run();
					}
				}
				lFile.close();
			}			
		}
	}
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onDeactivateTransaction(network::http::HTTPCTransaction *trans)
{
	if(trans->response().response()==200)
	{
		QSettings settings;
		QString licFilename = getLicenseFilename();
		
		settings.beginGroup("license");
		settings.remove("registrationKey");
		settings.endGroup();
		common::DiskOps::remove(licFilename);
		m_state = e_stateInitial;
		m_currentRegistrationID = "";
		QTimer::singleShot(500,this,SLOT(onProcess()));
	}
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onDeactivateTransactionError(network::http::HTTPCTransaction *trans,const QString& err)
{
	QString errS = "Failed to contact license server to deactivate license associated with this computer.";
	m_state = e_stateInitial;
	emit licenseError(errS);
	onStart();
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onDeactivateClientError(network::http::HTTPClient *client,const QString& err)
{
	QString errS = "Failed to contact license server to deactivate license associated with this computer.";
	m_state = e_stateInitial;
	emit licenseError(errS);
	onStart();
}

//-------------------------------------------------------------------------------------------

void LicenseClient::onDeactivateComplete(network::http::HTTPClient *client)
{}

//-------------------------------------------------------------------------------------------

void LicenseClient::onServices()
{
	QSet<LCTService *>::iterator ppI;

	for(ppI=m_services.begin();ppI!=m_services.end();ppI++)
	{
		LCTService *s = *ppI;
		s->process();
	}
}

//-------------------------------------------------------------------------------------------

void LicenseClient::checkLicenseCRC()
{
#if !defined(OMEGA_MAC_STORE)
	static const tuint32 lSoundCRC[22] = {
		 8,0xBA4A1220,
		11,0x4E370CAD,
		12,0xD3B1C086,
		16,0x3C127EFC,
		22,0x39DDC864,
		24,0x7A1A1C7C,
		32,0xB13877E9,
		44,0x9AB16FB6,
		48,0x103BC51A,
		64,0x48A605F4,
		96,0x5954D8CB
	};

	int i;
	QString baseName;
	QDir dDir(QCoreApplication::applicationDirPath());
	dDir.cdUp();
	dDir.cd("data");
	dDir.cd("license");

	baseName = dDir.absolutePath() + "/license";
	baseName = QDir::toNativeSeparators(baseName);

	for(i=0;i<11;i++)
	{
		tuint32 crc = 0;

		QString fName = baseName + QString::number(lSoundCRC[i<<1]) + ".ogg";
		common::BIOStream inFile(common::e_BIOStream_FileRead);
		if(inFile.open(fName))
		{
			int len = inFile.size();
			tbyte *mem = new tbyte [len];
			if(inFile.read(mem,len))
			{
				crc = common::CRC32::calc(mem,len);
			}
			inFile.close();
			delete [] mem;
		}
		if(crc!=lSoundCRC[(i<<1)+1])
		{
			QCoreApplication::quit();
		}
	}
#endif
}

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
