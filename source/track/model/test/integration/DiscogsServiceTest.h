//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_TRACK_MODEL_DISCOGSSERVICETEST_H
#define __OMEGA_TRACK_MODEL_DISCOGSSERVICETEST_H
//-------------------------------------------------------------------------------------------

#include <QCoreApplication>
#include <QSslSocket>
#include <QTcpSocket>
#include <QTimer>

//-------------------------------------------------------------------------------------------

class DiscogsServiceTest : public QCoreApplication
{
	public:
		Q_OBJECT
	public:
		DiscogsServiceTest(int& argc, char **argv);
		virtual ~DiscogsServiceTest();
		
	public slots:
		void onRun();
		
	private:
		void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
