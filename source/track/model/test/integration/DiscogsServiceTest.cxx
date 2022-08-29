#include "track/model/test/integration/DiscogsServiceTest.h"

#include "gtest/gtest.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

DiscogsServiceTest::DiscogsServiceTest(int testNo, int& argc, char **argv) : QCoreApplication(argc, argv)
{
	if(testNo == 0)
	{
		QTimer::singleShot(10, this, SLOT(onRequestToken()));
	}
	else if(testNo == 1)
	{
		QTimer::singleShot(10, this, SLOT(onAccessToken()));
	}
	else if(testNo == 2)
	{
		QTimer::singleShot(10, this, SLOT(onIdentity()));
	}
}

//-------------------------------------------------------------------------------------------

DiscogsServiceTest::~DiscogsServiceTest()
{}

//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "DiscogsServiceTest::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::addUserAgent(network::http::Unit& req)
{
	req.add("User-Agent", "Black Omega/3.0 +https://www.blackomega.co.uk");
}

//-------------------------------------------------------------------------------------------

QString DiscogsServiceTest::generateNOnceToken()
{
	tint i;
	tchar rnd[32];
	tint32 *rndI = reinterpret_cast<tint32 *>(rnd);
	
	for(i = 0; i < 8; i++)
	{
		*rndI++ = QRandomGenerator::global()->generate();
	}
	QByteArray rarr(rnd, 32);
	QByteArray renc = rarr.toBase64();
	QString token = QString::fromUtf8(renc);
	return token;
}

//-------------------------------------------------------------------------------------------

QString DiscogsServiceTest::generateTimeStamp()
{
	time_t ts = ::time(NULL);
	return QString::number(ts);
}

//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::requestHeaderForOAuthToken(network::http::Unit& req)
{
	QString omegaCosumerKey = "VhQXbqpiVcnKxGigTxyf";
	QString omegaCosumerSecret = "EgtpxeHkfSHQzVMzPVfPsaewiNGflwTS";
	QString nonceToken = generateNOnceToken();
	QString timeStamp = generateTimeStamp();
	QString callbackUrl = "https://www.blackomega.co.uk/discogs";
	QString auth = QString("OAuth oauth_consumer_key=\"%1\", oauth_nonce=\"%2\", oauth_signature=\"%3&\", oauth_signature_method=\"PLAINTEXT\", oauth_timestamp=\"%4\", oauth_callback=\"%5\"")
	                      .arg(omegaCosumerKey)
	                      .arg(nonceToken)
	                      .arg(omegaCosumerSecret)
	                      .arg(timeStamp)
	                      .arg(callbackUrl);

	req.request(network::http::Unit::e_Get, "https://api.discogs.com/oauth/request_token");
	req.add("Content-Type", "application/x-www-form-urlencoded");
	req.add("Authorization", auth);
	req.add("Host", "api.discogs.com");
	req.add("Connection", "close");
	addUserAgent(req);
}

//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::accessHeaderForOAuthToken(network::http::Unit& req, const QString& token, const QString& tokenSecret, const QString& verifier)
{
	QString omegaCosumerKey = "VhQXbqpiVcnKxGigTxyf";
	QString omegaCosumerSecret = "EgtpxeHkfSHQzVMzPVfPsaewiNGflwTS";
	QString nonceToken = generateNOnceToken();
	QString timeStamp = generateTimeStamp();
	QString auth = QString("OAuth oauth_consumer_key=\"%1\", oauth_nonce=\"%2\", oauth_token=\"%3\", oauth_signature=\"%4&%5\", oauth_signature_method=\"PLAINTEXT\", oauth_timestamp=\"%6\", oauth_verifier=\"%7\"")
	                      .arg(omegaCosumerKey)
	                      .arg(nonceToken)
	                      .arg(token)
	                      .arg(omegaCosumerSecret)
						  .arg(tokenSecret)
	                      .arg(timeStamp)
	                      .arg(verifier);
	
	fprintf(stdout, "%s\n", auth.toUtf8().constData());

	req.request(network::http::Unit::e_Post, "https://api.discogs.com/oauth/access_token");
	req.add("Content-Type", "application/x-www-form-urlencoded");
	req.add("Authorization", auth);
	req.add("Host", "api.discogs.com");
	req.add("Connection", "close");
	addUserAgent(req);
}

//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::resourceHeaderForOAuth(network::http::Unit& req, const QString& token, const QString& tokenSecret)
{
	QString omegaCosumerKey = "VhQXbqpiVcnKxGigTxyf";
	QString omegaCosumerSecret = "EgtpxeHkfSHQzVMzPVfPsaewiNGflwTS";
	QString nonceToken = generateNOnceToken();
	QString timeStamp = generateTimeStamp();
	QString auth = QString("OAuth oauth_consumer_key=\"%1\", oauth_nonce=\"%2\", oauth_token=\"%3\", oauth_signature=\"%4&%5\", oauth_signature_method=\"PLAINTEXT\", oauth_timestamp=\"%6\", oauth_version=\"1.0\"")
	                      .arg(omegaCosumerKey)
	                      .arg(nonceToken)
	                      .arg(token)
	                      .arg(omegaCosumerSecret)
						  .arg(tokenSecret)
	                      .arg(timeStamp);

	req.request(network::http::Unit::e_Get, "https://api.discogs.com/oauth/identity");
	//req.add("Content-Type", "application/x-www-form-urlencoded");
	req.add("Authorization", auth);
	req.add("Host", "api.discogs.com");
	req.add("Connection", "close");
	addUserAgent(req);
}

//-------------------------------------------------------------------------------------------
// https://www.blackomega.co.uk/discogs/?oauth_token=IfJDFgVHeiNYnmezleinJzppfXCjPLDHykmQnaZo&oauth_verifier=MHakSNdVFB
// token = IfJDFgVHeiNYnmezleinJzppfXCjPLDHykmQnaZo
// secret = OPypMTRWECfzVhNlftdCltEZsxKQXmYSjrUQhERW
// verifier = MHakSNdVFB
//-------------------------------------------------------------------------------------------

bool DiscogsServiceTest::processOAuthResponseToken(QSslSocket *socket, QPair<QString,QString>& oAuthToken)
{
	bool end;
	network::http::Unit response;

	end = false;
	do
	{
		if(!end)
		{
			if(socket->waitForReadyRead())
			{
				while(socket->canReadLine())
				{
					QByteArray lineArr = socket->readLine();
					QString line = QString::fromUtf8(lineArr).trimmed();
					if(!end)
					{
						if(!response.insertLine(line, end))
						{
							return false;
						}
					}
				}
			}		
			else
			{
				return false;
			}
		}
		else if(response.response() != 200)
		{
			return false;
		}
		else
		{
			QByteArray bodyArr = socket->readAll();
			QString body = QString::fromUtf8(bodyArr).trimmed();
			network::http::Query q(body);
			if(!q.data("oauth_token").isEmpty() && !q.data("oauth_token_secret").isEmpty())
			{
				oAuthToken.first = q.data("oauth_token");
				oAuthToken.second = q.data("oauth_token_secret");
				return true;
			}
		}
	} while(socket->isOpen());
	
	return false;
}

//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::onRequestToken()
{
	QSslSocket *socket = new QSslSocket(this);
	socket->connectToHostEncrypted("api.discogs.com", 443);
	if(socket->waitForEncrypted())
	{
		QPair<QString, QString> oAuthToken;
		network::http::Unit req;

		requestHeaderForOAuthToken(req);
		
		QString reqTxt;
		req.print(reqTxt);
		QByteArray reqArray = reqTxt.toUtf8();
		socket->write(reqArray.constData(), reqArray.size());
		
		if(processOAuthResponseToken(socket, oAuthToken))
		{
			fprintf(stdout, "token = %s\n", oAuthToken.first.toUtf8().constData());
			fprintf(stdout, "secret = %s\n", oAuthToken.second.toUtf8().constData());
		}
		socket->close();
	}
	else
	{
		QString err = socket->errorString();
		printError("onRequestToken", err.toUtf8().constData());
	}
	delete socket;
	
	quit();
}

//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::onAccessToken()
{
	QSslSocket *socket = new QSslSocket(this);
	socket->connectToHostEncrypted("api.discogs.com", 443);
	if(socket->waitForEncrypted())
	{
		QPair<QString, QString> oAuthToken;
		network::http::Unit req;

		QString token = "eFegqPqhavlCwXKXGElbNLUelttEXuaXTykHGGKe";
		QString tokenSecret = "PlmAnMCkTXigTYGwCMYwhyCgQTgJTLTMlWvQGDnm";
		QString verifier = "VgQuSNHNJS";

		accessHeaderForOAuthToken(req, token, tokenSecret, verifier);
		
		QString reqTxt;
		req.print(reqTxt);
		QByteArray reqArray = reqTxt.toUtf8();
		socket->write(reqArray.constData(), reqArray.size());
		
		while(socket->waitForReadyRead())
		{
			QByteArray arr = socket->readAll();
			QString respTxt = QString::fromUtf8(arr);
			fprintf(stdout, "%s\n", respTxt.toUtf8().constData());
		}
		socket->close();
	}
	else
	{
		QString err = socket->errorString();
		printError("onAccessToken", err.toUtf8().constData());
	}
	delete socket;
	
	quit();
}

//-------------------------------------------------------------------------------------------
//oauth_token=skOKgieJofptyqkBWoboKOYswaBnpCufJnaOGUGu&oauth_token_secret=ABNljhYZTXsbLGXuLcrgACsgfCZQTKgTtZHWEzBr
//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::onIdentity()
{
	QSslSocket *socket = new QSslSocket(this);
	socket->connectToHostEncrypted("api.discogs.com", 443);
	if(socket->waitForEncrypted())
	{
		QPair<QString, QString> oAuthToken;
		network::http::Unit req;
		
		QString accessToken = "skOKgieJofptyqkBWoboKOYswaBnpCufJnaOGUGu";
		QString accessSecret = "ABNljhYZTXsbLGXuLcrgACsgfCZQTKgTtZHWEzBr";
		
		resourceHeaderForOAuth(req, accessToken, accessSecret);
		
		QString reqTxt;
		req.print(reqTxt);
		QByteArray reqArray = reqTxt.toUtf8();
		socket->write(reqArray.constData(), reqArray.size());
		
		while(socket->waitForReadyRead())
		{
			QByteArray arr = socket->readAll();
			QString respTxt = QString::fromUtf8(arr);
			fprintf(stdout, "%s\n", respTxt.toUtf8().constData());
		}
		socket->close();
	}
	else
	{
		QString err = socket->errorString();
		printError("onAccessToken", err.toUtf8().constData());
	}
	delete socket;
	
	quit();
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------

TEST(DiscogsServiceTest, runRequest)
{
	int argc = 0;
	omega::DiscogsServiceTest serviceApp(0, argc, 0);
	serviceApp.exec();
}

//-------------------------------------------------------------------------------------------

TEST(DiscogsServiceTest, runAccess)
{
	int argc = 0;
	omega::DiscogsServiceTest serviceApp(1, argc, 0);
	serviceApp.exec();
}

//-------------------------------------------------------------------------------------------

TEST(DiscogsServiceTest, runIdentity)
{
	int argc = 0;
	omega::DiscogsServiceTest serviceApp(2, argc, 0);
	serviceApp.exec();
}

//-------------------------------------------------------------------------------------------
