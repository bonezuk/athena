//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_TRACK_MODEL_DISCOGSSERVICETEST_H
#define __OMEGA_TRACK_MODEL_DISCOGSSERVICETEST_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"
#include "network/http/inc/Query.h"
#include "network/http/inc/Unit.h"
#include "common/inc/Log.h"

#include <QCoreApplication>
#include <QSslSocket>
#include <QTcpSocket>
#include <QTimer>
#include <QRandomGenerator>

#include <time.h>

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class DiscogsServiceTest : public QCoreApplication
{
	public:
		Q_OBJECT
	public:
		DiscogsServiceTest(int testNo, int& argc, char **argv);
		virtual ~DiscogsServiceTest();
		
	public slots:
		void onRequestToken();
		void onAccessToken();
		void onIdentity();
		
	private:
		void printError(const char *strR, const char *strE) const;
		void addUserAgent(network::http::Unit& req);
		QString generateNOnceToken();
		QString generateTimeStamp();
		void requestHeaderForOAuthToken(network::http::Unit& req);
		bool processOAuthResponseToken(QSslSocket *socket, QPair<QString,QString>& oAuthToken);
		void accessHeaderForOAuthToken(network::http::Unit& req, const QString& token, const QString& tokenSecret, const QString& verifier);
		void resourceHeaderForOAuth(network::http::Unit& req, const QString& token, const QString& tokenSecret);
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
