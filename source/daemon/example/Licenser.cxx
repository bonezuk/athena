#include "licenser/licenselib/inc/Licenser.h"
#include "common/inc/Log.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace license
{
//-------------------------------------------------------------------------------------------

static const tchar c_privateLicenseKey[] = "<PrivateKey bits=\"2048\" exponent=\"15600630351185100292943503639323260403630293514019027560121388299563790316215496985811668211785273775645808643495282848199236709419932920476198781249569462419228083192033610098471359177948449589638970336444733428556238339291395056185827148242694432545309632337111934993075301681503426742510847083002889465304110043225484195207244489935849802968322064624461706979167379124704595653267765099579837673515819914671344078709319771759566167477878260142613677148138495068082795777585732723577949456643513282389401004853323952232628918023467609140554404090646054141673647168233128875616170651933969682468733016053561977334707\" primeA=\"163414019714140859912233313864639978364069920200738873442075955960224119454182470797865532731114616852628963775667316649841514645918656090606014422485053643975171843863825049591091629861312920542333432076659408419808802787870352260234862726678246423107919824460012285082359495377602863672601510858440180703611\" primeB=\"143200354337484510757351590891727094176406877578794786966094363363513242629796836451445984269758139392852040520825198686178963348513091128352704678404234110027145168827232809901390917380605224094883044617297054098111318993691588808190940185662392805344977781431116014210175777571274706594977266289078539427647\" primeExponentA=\"108942679809427239941488875909759985576046613467159248961383970640149412969454980531910355154076411235085975850444877766561009763945770727070676281656702429316781229242550033060727753240875280361555621384439605613205868525246901506823241817785497615405279882973341523388239663585068575781734340572293453802407\" primeExponentB=\"95466902891656340504901060594484729450937918385863191310729575575675495086531224300963989513172092928568027013883465790785975565675394085568469785602822740018096779218155206600927278253736816063255363078198036065407545995794392538793960123774928536896651854287410676140117185047516471063318177526052359618431\" coefficient=\"30789081282579067419878466015123040134309924198389714931112527031627728804748103922201556285252007756641978058528258352876574878105539845136176470836897575134158673035030607370470368649594370547792842565416487594421497641234996882348570615725234096736847850368893041158008988341571555445983679785455647966248\"></PrivateKey>";
static const tchar c_publicLicenseKey[] = "<PublicKey bits=\"2048\" modulus=\"23400945526777650439415255458984890605445440271028541340182082449345685474323245478717502317677910663468712965242924272298855064129899380714298171874354193628842124788050415147707038766922674384458455504667100142834357508937092584278740722364041648817964448505667902489612952522255140113766270624504334197956471679212277918181536319808531071525023573734472094129159239006380630841985626956619068027274602628252497122360472172975369729211249137432879234823097030356126510679069656944859406732207188068221317983973942390866863498816763354779257409048309720440963368358240821612716791250849832093970678301227861686133317\" exponent=\"3\"></PublicKey>";

//-------------------------------------------------------------------------------------------

Licenser::Licenser(int argc) : QCoreApplication(argc,(char **)0),
	m_webService(0),
	m_webServer(0)
{}

//-------------------------------------------------------------------------------------------

Licenser::Licenser(int argc,char **argv) : QCoreApplication(argc,argv),
	m_webService(0),
	m_webServer(0)
{
	QTimer::singleShot(100,this,SLOT(onStartService()));
	QObject::connect(this,SIGNAL(aboutToQuit()),this,SLOT(onStopService()));
}

//-------------------------------------------------------------------------------------------

Licenser::~Licenser()
{}

//-------------------------------------------------------------------------------------------

void Licenser::onStartService()
{
	bool res = false;

	QString logFilename;
	QDir dbDir(QCoreApplication::applicationDirPath());
	dbDir.cdUp();
	logFilename = dbDir.absoluteFilePath("db/license.log");
	logFilename = QDir::toNativeSeparators(logFilename);
	orcus::common::Log nLog(logFilename.toUtf8().constData());
	orcus::common::Log::g_Log = nLog;

	try
	{
		network::Resource& nResource = network::Resource::instance();
		network::Controller::ControllerSPtr ctrl(network::Controller::instance());
		
		if(ctrl.data()!=0)
		{
			m_webService = dynamic_cast<network::http::HTTPService *>(ctrl->newService("http_server"));
			if(m_webService!=0)
			{
				m_webServer = m_webService->getServer(8191);
				if(m_webServer!=0)
				{
					if(LicenseDB::instance()!=0)
					{
						m_webServer->connect("/",this,SLOT(onWebRoot(network::http::HTTPReceive *)));
						m_webServer->connect("/deactivate",this,SLOT(onDeactivate(network::http::HTTPReceive *)));
						m_webServer->connect("/register",this,SLOT(onRegister(network::http::HTTPReceive *)));
						m_webServer->connect("/generateTrial",this,SLOT(onGenerateTrial(network::http::HTTPReceive *)));
						m_webServer->connect("/generateKeys",this,SLOT(onGenerateKey(network::http::HTTPReceive *)));

						common::Log::g_Log << "Web service started successfully\r\n";

						res = true;
					}
				}
			}
		}
	}
	catch(common::Exception&)
	{
		res = false;
	}
	
	if(!res)
	{
		quit();
	}
}

//-------------------------------------------------------------------------------------------

void Licenser::onStopService()
{
	network::Controller::ControllerSPtr ctrl(network::Controller::instance());
	if(m_webService!=0)
	{
		if(m_webServer!=0)
		{
			m_webService->deleteServer(m_webServer);
			m_webServer = 0;
		}
		ctrl->deleteService(m_webService);
		m_webService = 0;
	}
	network::Controller::end();
	
	if(LicenseDB::instance()!=0)
	{
		LicenseDB *licDB = LicenseDB::instance();
		delete licDB;
	}

	common::Log::g_Log << "Web service shut down\r\n";
}

//-------------------------------------------------------------------------------------------

QString Licenser::fromConnection(network::http::HTTPReceive *recieve)
{
	int rPort;
	QString rHost;

	network::Resource::instance().findAddress(rHost,rPort,recieve->address());
	rHost += ":" + QString::number(rPort);
	return rHost;
}

//-------------------------------------------------------------------------------------------

void Licenser::onWebRoot(network::http::HTTPReceive *recieve)
{
	common::Log::g_Log << "onWebRoot - " << fromConnection(recieve).toUtf8().constData() << "\r\n";

	network::http::Unit hdr;
	
	hdr.response(401);
	hdr.add("Connection","close");
	recieve->connection()->postResponse(hdr);
	recieve->connection()->complete();
	recieve->endProcess();
}

//-------------------------------------------------------------------------------------------
// POST /deactivate?programID=XXXXX-xxxx-XXXX-xxxx-XXXXX HTTP/1.1
// User-Agent: BlackOmega/2.0
// Content-Length: x
// ... License Encrypted File Format
// Block 0 = Registration Key
// Block 1 = Customer Name
// Block 2..n = Hardware ID Blocks
//-------------------------------------------------------------------------------------------

void Licenser::onDeactivate(network::http::HTTPReceive *recieve)
{
	tint res = 400;
	network::http::Unit responseHdr;
	QByteArray postData(reinterpret_cast<const char *>(recieve->body()->GetData()),recieve->body()->GetSize());
	
	common::Log::g_Log << "onDeactivate - " << fromConnection(recieve).toUtf8().constData() << "\r\n";

	if(postData.size()>0)
	{
		network::LicenseFile licFile;
		
		if(licFile.read(postData) && licFile.size()>2)
		{
			common::CryptoRSAEncrypt lCrypt;
			
			if(lCrypt.readPublicKeyXML(c_publicLicenseKey))
			{
				QByteArray dArr = lCrypt.publicDecrypt(licFile.block(0));
				if(dArr.size()>0)
				{
					QString regID = QString::fromUtf8(dArr.constData(),dArr.size());
					QString programID = recieve->header().query().data("programID");
					
					if(!regID.isEmpty() && !programID.isEmpty())
					{
						track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
					
						try
						{
							QString cmdQ,dRegID;
							track::db::SQLiteQuery proQ(db);
						
							db->exec("SAVEPOINT onDeactivate");
							
							common::Log::g_Log << "onDeactivate - deactivating license with register code " << regID.toUtf8().constData() << " with programID " << programID.toUtf8().constData() << "\r\n";

							cmdQ = "SELECT registerID FROM Program WHERE programID='" + track::db::TrackDB::dbString(programID) + "'";
							proQ.prepare(cmdQ);
							proQ.bind(dRegID);
							if(proQ.next())
							{
								if(dRegID==regID)
								{
									deactivateLicense(regID,programID);
									common::Log::g_Log << "onDeactivate - deactivation successful\r\n";
									res = 200;
								}
								else
								{
									common::Log::g_Log << "onDeactivate - cannot deactivate as valid regID" << regID.toUtf8().constData() << "\r\n";
									res = 401;
								}
							}
							else
							{
								common::Log::g_Log << "onDeactivate - deactivation already happened\r\n";
								res = 200;
							}

							db->exec("RELEASE onDeactivate");
						}
						catch(const track::db::SQLiteException&)
						{
							common::Log::g_Log << "onDeactivate - error rolling back database\r\n";
							db->exec("ROLLBACK TO SAVEPOINT onDeactivate");
							res = 500;
						}
					}
					else
					{
						common::Log::g_Log << "onDeactivate - no registration or program identity\r\n";
					}
				}
				else
				{
					common::Log::g_Log << "onDeactivate - license file not properly decrypted\r\n";
				}
			}
			else
			{
				common::Log::g_Log << "onDeactivate - error reading in public key\r\n";
			}
		}
		else
		{
			common::Log::g_Log << "onDeactivate - invalid license file data\r\n";
		}
	}
	else
	{
		common::Log::g_Log << "onDeactivate - no POST data\r\n";
	}
	
	responseHdr.response(res);
	responseHdr.add("Connection","close");
	recieve->connection()->postResponse(responseHdr);
	recieve->connection()->complete();
	recieve->endProcess();
}

//-------------------------------------------------------------------------------------------
// GET /register?noLicenses=1&customer=Stuart HTTP/1.1
// User-Agent: blackomega.co.uk
//
// HTTP/1.1 200 OK
// Content-Length: 20
// Content-Type: text/plain
// Connection: close
// XXXXX-xxxx-XXXX-xxxx-XXXXX
//
// To be restricted to accept connections from blackomega.co.uk
//-------------------------------------------------------------------------------------------

void Licenser::onRegister(network::http::HTTPReceive *recieve)
{
	bool acceptFlag = false;
	
	common::Log::g_Log << "onRegister - " << fromConnection(recieve).toUtf8().constData() << "\r\n";

	if(!network::Resource::instance().isLocal(recieve->address()))
	{
		tuint32 sourceIP;
		
		sourceIP = network::Resource::instance().getIP(recieve->address());
		if((sourceIP & 0xffff0000)==0xc0a80000) // 192.168.x.x source
		{
			acceptFlag = true;
		}
		else
		{
			tuint boIP;
			struct sockaddr_in boAddr;
		
			network::Resource::instance().getAddress("www.blackomega.co.uk",80,&boAddr);
			boIP = network::Resource::instance().getIP(&boAddr);
			if(sourceIP==boIP)
			{
				acceptFlag = true;
			}
			else
			{
				network::Resource::instance().getAddress("secure101.prositehosting.co.uk",80,&boAddr);
				boIP = network::Resource::instance().getIP(&boAddr);
				if(sourceIP==boIP)
				{
					acceptFlag = true;
				}
			}
		}
	}
	else
	{
		acceptFlag = true;
	}
	
	if(acceptFlag)
	{
		tint noLicenses;
		QString customerName,transactionID;
		const network::http::Query& qry = recieve->header().query();
		
		common::Log::g_Log << "onRegister - connection accepted\r\n";

		noLicenses = qry.data("noLicenses").toInt();
		customerName = qry.data("customer");
		transactionID = qry.data("transactionID");

		common::Log::g_Log << "onRegister - generate " << QString::number(noLicenses).toUtf8().constData();
		common::Log::g_Log << " licenses for client \"" << customerName.toUtf8().constData() << "\"\r\n";

		if(noLicenses>=1 && !customerName.isEmpty() && !transactionID.isEmpty())
		{
			QString rKey = generateRegistrationKey(noLicenses,customerName,transactionID);
			if(!rKey.isEmpty())
			{
				network::http::Unit hdr;
				network::NetArraySPtr body(new network::NetArray);
				
				common::Log::g_Log << "onRegister - generated registration key \"" << rKey.toUtf8().constData() << "\"\r\n";

				hdr.response(200);
				hdr.add("Content-Type","text/plain");
				hdr.add("Connection","close");
				recieve->connection()->postResponse(hdr);
				body->AppendRaw(rKey.toUtf8().constData(),rKey.toUtf8().length());
				recieve->connection()->postBody(body);
			}
			else
			{
				common::Log::g_Log << "onRegister - error generating registration key\r\n";
				network::http::Unit hdr;
				hdr.response(500);
				hdr.add("Connection","close");
				recieve->connection()->postResponse(hdr);			
			}
		}
		else
		{
			common::Log::g_Log << "onRegister - invalid parameters\r\n";
			network::http::Unit hdr;
			hdr.response(400);
			hdr.add("Connection","close");
			recieve->connection()->postResponse(hdr);		
		}
	}
	else
	{
		common::Log::g_Log << "onRegister - connection rejected\r\n";
		network::http::Unit hdr;
		hdr.response(401);
		hdr.add("Connection","close");
		recieve->connection()->postResponse(hdr);
	}
	recieve->connection()->complete();
	recieve->endProcess();
}

//-------------------------------------------------------------------------------------------

QString Licenser::generateRegistrationKey(tint noLicenses,const QString& customerName,const QString& transactionID)
{
	tint i,offset = 0;
	tint mLen = customerName.toUtf8().size() + sizeof(tint) + sizeof(struct tm) + sizeof(clock_t);
	time_t t;
	clock_t cnow;
	struct tm *gmt;
	QString key;
	QByteArray sArr,rArr(mLen,'\0');
	common::CryptoRandom rand;
	tchar *x = rArr.data();
	bool loop = true;
	
	t = ::time(0);
	gmt = ::gmtime(&t);
	cnow = ::clock();
	
	sArr = customerName.toUtf8();
	::memcpy(x,sArr.constData(),sArr.size());
	offset += sArr.size();
	::memcpy(&x[offset],&noLicenses,sizeof(tint));
	offset += sizeof(tint);
	::memcpy(&x[offset],gmt,sizeof(struct tm));
	offset += sizeof(struct tm);
	::memcpy(&x[offset],&cnow,sizeof(clock_t));
	rand.mix(reinterpret_cast<tubyte *>(x),mLen);
	
	while(loop)
	{
		track::db::SQLiteDatabase *db = LicenseDB::instance()->db();

		try
		{
			QString cmdI;
			track::db::SQLiteInsert regI(db);
			tubyte s[16];
			tuint64 createTime = static_cast<tuint64>(common::TimeStamp::now());
			
			rand.generateBytes(s,16);
			
			key = "";
			for(i=0;i<11;i++)
			{
				tubyte a,b,c;
				tchar t[2] = {'\0','\0'};
			
				a = static_cast<tubyte>(s[i]);
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
			
			db->exec("SAVEPOINT generateRegistrationKey");
			cmdI = "INSERT INTO License VALUES (?,?,?,?,?)";
			regI.prepare(cmdI);
			regI.bind(key);
			regI.bind(noLicenses);
			regI.bind(createTime);
			QString dbCustomerName(customerName);
			regI.bind(dbCustomerName);
			QString dbTransactionID(transactionID);
			regI.bind(dbTransactionID);
			if(regI.next())
			{
				db->exec("RELEASE generateRegistrationKey");
				loop = false;
			}
			else
			{
				db->exec("ROLLBACK TO SAVEPOINT generateRegistrationKey");
			}
		}
		catch(const track::db::SQLiteException&)
		{
			db->exec("ROLLBACK TO SAVEPOINT generateRegistrationKey");
			loop = true;
		}
	}
	
	return key;
}

//-------------------------------------------------------------------------------------------

QString Licenser::printTime(const common::TimeStamp& t)
{
	static const tchar *weekDays[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	static const tchar *months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

	QString str;
	
	str += weekDays[ t.dayOfWeek() ];
	str += ", ";
	str += common::BString::Int(t.day(),2).GetString();
	str += " ";
	str += months[ t.month() - 1 ];
	str += " ";
	str += common::BString::Int(t.year(),4).GetString();
	str += " ";
	str += common::BString::Int(t.hour(),2).GetString();
	str += ":";
	str += common::BString::Int(t.minute(),2).GetString();
	str += ":";
	str += common::BString::Int(t.second(),2).GetString();
	str += " GMT";
	
	return str;
}

//-------------------------------------------------------------------------------------------
// POST /generateTrial HTTP/1.1
// User-Agent: BlackOmega/2.0
// Content-Length: x
// ... License File Format
// Block 0 = Registration Key
// Block 1 = Program Key
// Block 2..n = Hardware ID Blocks
//-------------------------------------------------------------------------------------------

void Licenser::onGenerateTrial(network::http::HTTPReceive *recieve)
{
	tint res = 400;
	network::http::Unit responseHdr;
	QByteArray postData(reinterpret_cast<const char *>(recieve->body()->GetData()),recieve->body()->GetSize());
	
	common::Log::g_Log << "onGenerateTrial - " << fromConnection(recieve).toUtf8().constData() << "\r\n";

	if(postData.size()>0)
	{
		network::LicenseFile licFile;
		
		if(licFile.read(postData) && licFile.size()>=3)
		{
			if(isProgramKeyValid(licFile))
			{
				QString regID,programID;
				common::TimeStamp createTS,trialTS,modifyTS;
				
				programID = QString::fromUtf8(licFile.block(1).constData(),licFile.block(1).size());
				if(!readProgramRecord(programID,regID,createTS,trialTS,modifyTS))
				{
					regID = "";
					createTS = common::TimeStamp::now();
				}
				modifyTS = common::TimeStamp::now();

				if(trialTS==0)
				{
					tfloat64 sec30Days = 30.0 * 24.0 * 60.0 * 60.0;
					trialTS = modifyTS + sec30Days;
				}

				common::Log::g_Log << "onGeneratTrial - generate new trial regID=\"" << regID.toUtf8().constData();
				common::Log::g_Log << "\" programID=\"" << programID.toUtf8().constData() << "\"\r\n";
				common::Log::g_Log << "onGeneratTrial - trialTS " << printTime(trialTS).toUtf8().constData() << "\r\n";
				common::Log::g_Log << "onGeneratTrial - createTS " << printTime(createTS).toUtf8().constData() << "\r\n";
				common::Log::g_Log << "onGeneratTrial - modifyS " << printTime(modifyTS).toUtf8().constData() << "\r\n";
				
				if(modifyTS < trialTS)
				{
					QByteArray licArr = generateLicense(licFile,trialTS,regID,getCustomerName(regID));
					
					if(licArr.size()>0)
					{
						track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
						bool dbRes = false;
						
						try
						{
							db->exec("SAVEPOINT generateTrial");
							
							if(writeProgramRecord(regID,programID,createTS,trialTS,modifyTS))
							{
								dbRes = true;
							}
							
							if(dbRes)
							{
								db->exec("RELEASE generateTrial");
							}
							else
							{
								db->exec("ROLLBACK TO SAVEPOINT generateTrial");
							}
						}
						catch(const track::db::SQLiteException&)
						{
							db->exec("ROLLBACK TO SAVEPOINT generateTrial");
							dbRes = false;
							res = 500;
						}
						
						if(dbRes)
						{
							network::NetArraySPtr body(new network::NetArray);
							
							common::Log::g_Log << "onGenerateTrial - trial license successfully generated\r\n";

							res = 200;
							responseHdr.response(res);
							responseHdr.add("Content-Type","x-application/black-omega.license");
							responseHdr.add("Content-Length",QString::number(licArr.size()));
							responseHdr.add("Connection","close");
							body->AppendRaw(licArr.constData(),licArr.size());
							recieve->connection()->postResponse(responseHdr);
							recieve->connection()->postBody(body);
						}
						else
						{
							common::Log::g_Log << "onGenerateTrial - failed to generate trail license\r\n";
						}
					}
					else
					{
						common::Log::g_Log << "onGenerateTrial - error generating license\r\n";
					}
				}
				else
				{
					common::Log::g_Log << "onGenerateTrial - trial has expired\r\n";
				}
			}
			else
			{
				common::Log::g_Log << "onGenerateTrial - invalid program key\r\n";
			}
		}
		else
		{
			common::Log::g_Log << "onGenerateTrial - invalid license file format\r\n";
		}
	}
	else
	{
		common::Log::g_Log << "onGenerateTrial - no POST data\r\n";
	}
	if(res>=300)
	{
		responseHdr.response(res);
		responseHdr.add("Connection","close");
		recieve->connection()->postResponse(responseHdr);
	}
	recieve->connection()->complete();
	recieve->endProcess();
}

//-------------------------------------------------------------------------------------------
// POST /generateKeys?oldRegistration=XXXXX-xxxx-XXXX-xxxx-XXXXX HTTP/1.1
// User-Agent: BlackOmega/2.0
// Content-Length: x
// ... License File Format
// Block 0 = Registration Key
// Block 1 = Program Key
// Block 2..n = Hardware ID Blocks
//-------------------------------------------------------------------------------------------

void Licenser::onGenerateKey(network::http::HTTPReceive *recieve)
{
	tint res = 400;
	network::http::Unit responseHdr;
	QByteArray postData(reinterpret_cast<const char *>(recieve->body()->GetData()),recieve->body()->GetSize());
	QString errStr;

	common::Log::g_Log << "onGenerateKey - " << fromConnection(recieve).toUtf8().constData() << "\r\n";

	if(postData.size()>0)
	{
		network::LicenseFile licFile;
		
		if(licFile.read(postData) && licFile.size()>=3)
		{
			tint i,totalLicense,useLicense;
			QString newRegID,oldRegID,programID;
			
			oldRegID = recieve->header().query().data("oldRegistration");
			newRegID = QString::fromUtf8(licFile.block(0).constData(),licFile.block(0).size());
			programID = QString::fromUtf8(licFile.block(1).constData(),licFile.block(1).size());
			
			totalLicense = totalNumberOfLicense(newRegID);

			common::Log::g_Log << "onGenerateKey - old registration ID \"" << oldRegID.toUtf8().constData() << "\"\r\n";
			common::Log::g_Log << "onGenerateKey - new registration ID \"" << newRegID.toUtf8().constData() << "\"\r\n";
			common::Log::g_Log << "onGenerateKey - program ID \"" << programID.toUtf8().constData() << "\"\r\n";
			common::Log::g_Log << "onGenerateKey - total number licenses " << QString::number(totalLicense).toUtf8().constData() << "\r\n";

			if(totalLicense>0)
			{
				QString recRegID;
				common::TimeStamp createTS,trialTS,modifyTS;
				
				if(!readProgramRecord(programID,recRegID,createTS,trialTS,modifyTS))
				{
					createTS = common::TimeStamp::now();
					trialTS = 0.0;
					modifyTS = createTS;
				}

				common::Log::g_Log << "onGeneratKey - createTS " << printTime(createTS).toUtf8().constData() << "\r\n";
				common::Log::g_Log << "onGeneratKey - modifyS " << printTime(modifyTS).toUtf8().constData() << "\r\n";

				if(!oldRegID.isEmpty())
				{
					common::Log::g_Log << "onGenerateKey - deactivating old license\r\n";
					deactivateLicense(oldRegID,programID);
				}
				
				useLicense = usedNumberOfLicense(newRegID);

				common::Log::g_Log << "onGenerateKey - no of used licenses for key = " << QString::number(useLicense).toUtf8().constData() << "\r\n";

				{
					common::TimeStamp tS(0);
					QByteArray licArr = generateLicense(licFile,tS,newRegID,getCustomerName(newRegID));
					if(licArr.size()>0)
					{
						track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
						bool dbRes = false;
						
						try
						{
							db->exec("SAVEPOINT generateLicense");
							if(writeProgramRecord(newRegID,programID,createTS,trialTS,modifyTS))
							{
								QStringList hwList;
								
								for(i=2;i<licFile.size();i++)
								{
									QString hwStr = QString::fromUtf8(licFile.block(i).constData(),licFile.block(i).size());
									hwList.append(hwStr);
								}
								if(writeHardwareRecord(newRegID,programID,hwList))
								{
									dbRes = true;
								}
							}
							if(dbRes)
							{
								db->exec("RELEASE generateLicense");
							}
							else
							{
								db->exec("ROLLBACK TO SAVEPOINT generateLicense");
							}
						}
						catch(const track::db::SQLiteException&)
						{
							db->exec("ROLLBACK TO SAVEPOINT generateLicense");
							dbRes = false;
							res = 500;
						}
						
						if(dbRes)
						{
							network::NetArraySPtr body(new network::NetArray);
						
							common::Log::g_Log << "onGenerateKey - successfully generated license file\r\n";

							res = 200;
							responseHdr.response(res);
							responseHdr.add("Content-Type","x-application/black-omega.license");
							responseHdr.add("Content-Length",QString::number(licArr.size()));
							responseHdr.add("Connection","close");
							body->AppendRaw(licArr.constData(),licArr.size());
							recieve->connection()->postResponse(responseHdr);
							recieve->connection()->postBody(body);
						}
						else
						{
							common::Log::g_Log << "onGenerateKey - failed to generate license file\r\n";
						}
					}
					else
					{
						common::Log::g_Log << "onGenerateKey - error generating license file\r\n";
					}
				}
			}
			else
			{
				common::Log::g_Log << "onGenerateKey - invalid license code \"" << newRegID.toUtf8().constData() << "\"\r\n";
				errStr = "Invalid license registration code.";
				res = 401;
			}
		}
		else
		{
			common::Log::g_Log << "onGenerateKey - invalid license file\r\n";
			errStr = "Invalid license request.";
			res = 401;
		}
	}
	else
	{
		common::Log::g_Log << "onGenerateKey - error no POST data\r\n";
	}

	if(res>=300)
	{
		QByteArray errA = errStr.toUtf8();
		responseHdr.response(res);
		if(!errStr.isEmpty())
		{
			responseHdr.add("Content-Type","text/plain");
			responseHdr.add("Content-Length",QString::number(errA.size()));
		}
		responseHdr.add("Connection","close");
		recieve->connection()->postResponse(responseHdr);
		if(!errStr.isEmpty())
		{
			network::NetArraySPtr body(new network::NetArray);
			body->AppendRaw(errA.constData(),errA.size());
			recieve->connection()->postBody(body);
		}
	}
	recieve->connection()->complete();
	recieve->endProcess();
}

//-------------------------------------------------------------------------------------------

QByteArray Licenser::generateLicense(network::LicenseFile& inFile,const common::TimeStamp& certTime,const QString& regCode,const QString& clientName)
{
	tint i;
	QString regID,programID;
	QByteArray licArr;
	bool res = false;
	
	if(inFile.size()>=3)
	{
		common::CryptoRSAEncrypt lCrypt;
			
		if(lCrypt.readPrivateKeyXML(QString::fromLatin1(c_privateLicenseKey)) && lCrypt.readPublicKeyXML(QString::fromLatin1(c_publicLicenseKey)))
		{
			QVector<QByteArray> idList;
			
			{
				QByteArray dArr,eArr;
				
				dArr = regCode.toUtf8();
				eArr = lCrypt.publicEncrypt(dArr);
				idList.append(eArr);
				
				dArr = clientName.toUtf8();
				eArr = lCrypt.publicEncrypt(dArr);
				idList.append(eArr);
			}
			
			for(i=2;i<inFile.size();i++)
			{
				QString hdStr = QString::fromUtf8(inFile.block(i).constData(),inFile.block(i).size());
				QByteArray dArr,eArr;
				
				setHardwareTimeToLicense(hdStr,certTime,dArr);
				eArr = lCrypt.publicEncrypt(dArr);
				if(eArr.size()>0)
				{
					idList.append(eArr);
				}
			}
			if(idList.size()>0)
			{
				network::LicenseFile outFile;
				
				if(!outFile.write(licArr,idList))
				{
					licArr.clear();
				}
			}
		}
	}
	return licArr;
}

//-------------------------------------------------------------------------------------------

bool Licenser::getHardwareTimeFromLicense(const QByteArray& mem,QString& hID,common::TimeStamp& tS)
{
	bool res = false;
	
	if(mem.length()>8)
	{
		const tchar *x = mem.constData();
		QByteArray hIDArr(&x[8],mem.length()-8);
		tS = network::NTPClient::toTimeStamp(x);
		hID = QString::fromUtf8(hIDArr.constData(),hIDArr.length());
		if(!hID.isEmpty())
		{
			res = true;
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void Licenser::setHardwareTimeToLicense(const QString& hID,const common::TimeStamp& tS,QByteArray& mem)
{
	tchar tMem[8];
	QByteArray hIDArr(hID.toUtf8());
	
	network::NTPClient::fromTimeStamp(tS,tMem);
	mem.clear();
	mem.append(tMem,8);
	mem.append(hIDArr);
}

//-------------------------------------------------------------------------------------------

bool Licenser::deactivateLicense(const QString& regID,const QString& programID)
{
	track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
	bool res = false;

	try
	{
		QString cmdD;
		
		db->exec("SAVEPOINT deactivateLicense");
		cmdD = "DELETE FROM Hardware WHERE programID='" + track::db::TrackDB::dbString(programID) + "'";
		db->exec(cmdD);
		cmdD = "UPDATE Program SET registerID='' WHERE programID='" + track::db::TrackDB::dbString(programID) + "'";
		db->exec(cmdD);
		db->exec("RELEASE deactivateLicense");
		res = true;
	}
	catch(const track::db::SQLiteException&)
	{
		db->exec("ROLLBACK TO SAVEPOINT deactivateLicense");
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

tint Licenser::totalNumberOfLicense(const QString& regID)
{
	track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
	tint num = -1;
	
	try
	{
		QString cmdQ;
		track::db::SQLiteQuery totQ(db);
		
		cmdQ = "SELECT noLicenses FROM License WHERE registerID='" + track::db::TrackDB::dbString(regID) + "'";
		totQ.prepare(cmdQ);
		totQ.bind(num);
		if(!totQ.next())
		{
			num = -1;
		}
	}
	catch(const track::db::SQLiteException&)
	{
		num = -1;
	}
	return num;
}

//-------------------------------------------------------------------------------------------

tint Licenser::usedNumberOfLicense(const QString& regID)
{
	track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
	tint num = totalNumberOfLicense(regID);
	
	try
	{
		QString cmdQ;
		track::db::SQLiteQuery useQ(db);
		
		cmdQ = "SELECT COUNT(registerID) FROM Program WHERE registerID='" + track::db::TrackDB::dbString(regID) + "'";
		useQ.prepare(cmdQ);
		useQ.bind(num);
		if(!useQ.next())
		{
			num = 0;
		}
	}
	catch(const track::db::SQLiteException&)
	{
		num = totalNumberOfLicense(regID);
	}
	return num;
}

//-------------------------------------------------------------------------------------------

bool Licenser::readProgramRecord(const QString& programID,QString& regID,common::TimeStamp& createTS,common::TimeStamp& trialTS,common::TimeStamp& modifyTS)
{
	track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
	bool res = false;
	
	try
	{
		tuint64 createV,trialV,modifyV;
		QString cmdQ;
		track::db::SQLiteQuery proQ(db);
		
		cmdQ  = "SELECT registerID, creationTime, trialTime, modifiedTime FROM Program ";
		cmdQ += "  WHERE programID='" + track::db::TrackDB::dbString(programID) + "'";
		proQ.prepare(cmdQ);
		proQ.bind(regID);
		proQ.bind(createV);
		proQ.bind(trialV);
		proQ.bind(modifyV);
		if(proQ.next())
		{
			createTS = createV;
			trialTS = trialV;
			modifyTS = modifyV;
			res = true;
		}
	}
	catch(const track::db::SQLiteException&)
	{
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool Licenser::writeProgramRecord(const QString& regID,const QString& programID,const common::TimeStamp& createTS,const common::TimeStamp& trialTS,const common::TimeStamp& modifyTS)
{
	track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
	bool res = false;
	
	try
	{
		QString cmdD,cmdI;
		track::db::SQLiteInsert insP(db);
		
		db->exec("SAVEPOINT writeProgramRecord");
		cmdD = "DELETE FROM Program WHERE programID='" + track::db::TrackDB::dbString(programID) + "'";
		db->exec(cmdD);
		
		cmdI = "INSERT INTO Program VALUES (?,?,?,?,?)";
		insP.prepare(cmdI);
		QString dbProgramID(programID);
		insP.bind(dbProgramID);
		QString dbRegID(regID);
		insP.bind(dbRegID);
		tuint64 dbCreateTS = static_cast<tuint64>(createTS);
		insP.bind(dbCreateTS);
		tuint64 dbTrialTS = static_cast<tuint64>(trialTS);
		insP.bind(dbTrialTS);
		tuint64 dbModifyTS = static_cast<tuint64>(modifyTS);
		insP.bind(dbModifyTS);
		if(insP.next())
		{
			res = true;
		}
		
		if(res)
		{
			db->exec("RELEASE writeProgramRecord");
		}
		else
		{
			db->exec("ROLLBACK TO SAVEPOINT writeProgramRecord");
		}
	}
	catch(const track::db::SQLiteException&)
	{
		db->exec("ROLLBACK TO SAVEPOINT writeProgramRecord");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

QString Licenser::getCustomerName(const QString& regID)
{
	QString customerName;
	track::db::SQLiteDatabase *db = LicenseDB::instance()->db();

	if(!regID.isEmpty())
	{
		try
		{
			QString cmdQ;
			track::db::SQLiteQuery cusQ(db);
	
			cmdQ = "SELECT customerName FROM License WHERE registerID='" + track::db::TrackDB::dbString(regID) + "'";
			cusQ.prepare(cmdQ);
			cusQ.bind(customerName);
			cusQ.next();
		}
		catch(const track::db::SQLiteException&)
		{
			customerName = "";
		}
	}
	return customerName;
}

//-------------------------------------------------------------------------------------------

bool Licenser::readHardwareRecord(const QString& regID,const QString& programID,QStringList& hwList)
{
	track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
	bool res = false;
	
	try
	{
		QString cmdQ,hwID;
		track::db::SQLiteQuery hwQ(db);
		
		hwList.clear();
		cmdQ  = "SELECT hardwareID FROM Hardware ";
		cmdQ += "  WHERE registerID='" + track::db::TrackDB::dbString(regID) + "' AND programID='" + track::db::TrackDB::dbString(programID) + "'";
		hwQ.prepare(cmdQ);
		hwQ.bind(hwID);
		while(!hwQ.next())
		{
			hwList.append(hwID);
			res = true;
		}
	}
	catch(const track::db::SQLiteException&)
	{
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool Licenser::writeHardwareRecord(const QString& regID,const QString& programID,const QStringList& hwList)
{
	tint i;
	track::db::SQLiteDatabase *db = LicenseDB::instance()->db();
	bool res = true;
	
	try
	{
		QString cmdI,cmdD;
		QString dbRegID(regID);
		QString dbProgramID(programID);
		track::db::SQLiteInsert hwI(db);
		
		db->exec("SAVEPOINT writeHardwareRecord");
		
		cmdD = "DELETE FROM hardware WHERE registerID='" + track::db::TrackDB::dbString(regID) + "' AND programID='" + track::db::TrackDB::dbString(programID) + "'";
		db->exec(cmdD);
		
		cmdI = "INSERT INTO Hardware VALUES (?,?,?)";
		hwI.prepare(cmdI);
		for(i=0;i<hwList.size() && res;i++)
		{
			QString dbHW(hwList.at(i));
			
			hwI.bind(dbRegID);
			hwI.bind(dbProgramID);
			hwI.bind(dbHW);
			if(!hwI.next())
			{
				res = false;
			}
		}
		
		if(res)
		{
			db->exec("RELEASE writeHardwareRecord");
		}
		else
		{
			db->exec("ROLLBACK TO SAVEPOINT writeHardwareRecord");
		}
	}
	catch(const track::db::SQLiteException&)
	{
		db->exec("ROLLBACK TO SAVEPOINT writeHardwareRecord");
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool Licenser::isProgramKeyValid(const network::LicenseFile& licFile)
{
	tint i;
	bool res = false;
	QString programID;
	
	if(licFile.size()>=3)
	{
		programID = QString::fromUtf8(licFile.block(1).constData(),licFile.block(1).size());
		if(!programID.isEmpty())
		{
			for(i=2;i<licFile.size() && !res;i++)
			{
				QString hwStr,hwKey;
				common::SHA1Digest digest;
				const QByteArray& hwArr = licFile.block(i);
				
				hwStr = QString::fromUtf8(hwArr.constData(),hwArr.length());
				digest.input(hwStr.toUtf8().constData(),hwStr.toUtf8().length());
				hwKey = getDigestKey(digest);
				if(hwKey==programID)
				{
					res = true;
				}
			}
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------
// Build key from digest
// XXXXX-XXXX-XXXX-XXXX-XXXXX
// 0 1 2  3 4  5 6  7 8  9 0
//-------------------------------------------------------------------------------------------

QString Licenser::getDigestKey(common::SHA1Digest& digest)
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
} // namespace license
} // namespace orcus
//-------------------------------------------------------------------------------------------
