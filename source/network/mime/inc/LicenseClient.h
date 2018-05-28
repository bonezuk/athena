//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_LICENSECLIENT_H
#define __ORCUS_NETWORK_LICENSECLIENT_H
//-------------------------------------------------------------------------------------------

// Qt
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QEvent>
#include <QWaitCondition>
#include <QSettings>
#include <QCoreApplication>
#include <QTimer>
#include <QStringList>
#include <QByteArray>
#include <QString>
#include <QDir>
#include <QFileInfo>

// Orcus
#include "network/inc/Resource.h"
#include "common/inc/CryptoRSAEncrypt.h"
#include "common/inc/SHA1Digest.h"
#include "common/inc/HardwareIdentification.h"
#include "common/inc/TimeStamp.h"
#include "common/inc/DiskOps.h"
#include "network/inc/NTPClient.h"
#include "network/mime/inc/LicenseFile.h"
#include "network/http/inc/HTTPClient.h"
#include "network/http/inc/HTTPClientService.h"
#include "network/http/inc/HTTPConnection.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------

class LicenseClientThread;

//-------------------------------------------------------------------------------------------

class MIME_EXPORT LCTService : public QObject
{
	public:
		Q_OBJECT

	public:
		LCTService(QObject *parent = 0);
		virtual ~LCTService();

		virtual void process() = 0;
};

//-------------------------------------------------------------------------------------------

class LicenseClientEvent : public QEvent
{
	public:
	
		typedef enum
		{
			e_isValidEvent = QEvent::User + 1000,
			e_isLicensedEvent,
			e_isTrialEvent,
			e_getRegCodeEvent,
			e_setRegCodeEvent,
			e_getCustomerEvent,
			e_deactivateEvent,
			e_addServiceEvent,
			e_delServiceEvent
		} LicenseClientEventType;
		
	public:
		LicenseClientEvent(LicenseClientEventType t);
		
		Qt::HANDLE threadId();
		QString& name();
		const QString& name() const;

		LCTService *service();
		void setService(LCTService *s);
		
	protected:
		
		Qt::HANDLE m_threadId;
		QString m_name;
		LCTService *m_service;
};

//-------------------------------------------------------------------------------------------

class LicenseClientWaitCondition
{
	public:
		LicenseClientWaitCondition();
		
		void get();
		void wait();
		void wake();
		
		bool& flag();
		const bool& flag() const;
		common::TimeStamp& time();
		const common::TimeStamp& time() const;
		QString& name();
		const QString& name() const;
		
	protected:
	
		QMutex m_mutex;
		QWaitCondition m_condition;
		bool m_flag;
		common::TimeStamp m_time;
		QString m_name;
};

//-------------------------------------------------------------------------------------------

class MIME_EXPORT LicenseClient : public QObject
{
	Q_OBJECT
	
	public:
		
		friend class LicenseClientThread;
		
		typedef enum
		{
			e_stateInitial = 0,
			e_stateCheckLocalLicense,
			e_stateGetLicenseFromServer,
			e_stateGetTrialFromServer,
			e_stateLicensed,
			e_stateNotLicensed,
			e_stateDone
		} State;
	
	public:
		LicenseClient(QObject *parent = 0);
		virtual ~LicenseClient();
		
        static QSharedPointer<LicenseClient> instance();
		static void end();
		
		bool isValid();
		bool isLicensed();
		bool isTrial();
		common::TimeStamp endOfTrialDate();
		
		QString getRegistrationCode();
		void setRegistrationCode(const QString& rCode);
		
		QString getCustomerName();
		
		void deactivate();
		
		void addService(LCTService *s);
		void delService(LCTService *s);

	protected:
		
        static QSharedPointer<LicenseClient> m_instance;
		
		State m_state;
		common::TimeStamp m_currentTime;
		QString m_currentRegistrationID;
		LicenseClientThread *m_thread;
		QMutex m_mutex;
		QMap<Qt::HANDLE,LicenseClientWaitCondition *> m_waitConditionMap;
		common::TimeStamp m_trialTS;
		QString m_licClientName;
		bool m_licenseFlag;
		
		QTimer *m_servicesTimer;
		QSet<LCTService *> m_services;

		QString getLicenseFilename();
		QString getProgramKey();
		QString getRegistrationKey();
		QString getDigestKey(common::SHA1Digest& digest);
		bool isValidLicense(const QString& licFilename);
		bool isValidLicense(const LicenseFile& licFile);
		bool getHardwareTimeFromLicense(const QByteArray& mem,QString& hID,common::TimeStamp& tS);
		void setHardwareTimeToLicense(const QString& hID,const common::TimeStamp& tS,QByteArray& mem);
		void getCurrentTime();
		QByteArray buildLicense();
		void getLicense(const QString& licServerHost,tint licServerPort,bool trialFlag);
		
		virtual bool event(QEvent *e);

		LicenseClientWaitCondition *getCondition();
		LicenseClientWaitCondition *getCondition(Qt::HANDLE id);
		void freeConditions();
		
		void doDeactivate(const QString& licServerHost,tint licServerPort);
		
		void checkLicenseCRC();

	public slots:
	
		void onStart();
		void onStop();
	
	protected slots:
		
		void onProcess();
		
		void onLicenseTransaction(network::http::HTTPCTransaction *trans);
		void onLicenseTransactionError(network::http::HTTPCTransaction *trans,const QString& err);
		void onLicenseClientError(network::http::HTTPClient *client,const QString& err);
		void onLicenseComplete(network::http::HTTPClient *client);

		void onDeactivateTransaction(network::http::HTTPCTransaction *trans);
		void onDeactivateTransactionError(network::http::HTTPCTransaction *trans,const QString& err);
		void onDeactivateClientError(network::http::HTTPClient *client,const QString& err);
		void onDeactivateComplete(network::http::HTTPClient *client);

		void onServices();
		
	signals:
	
		void licensed();
		void notLicensed();
		void licenseError(const QString&);
};

//-------------------------------------------------------------------------------------------

class MIME_EXPORT LicenseClientThread : public QThread
{
	Q_OBJECT
	
	public:
		LicenseClientThread(QObject *parent = 0);
		virtual ~LicenseClientThread();
		
		virtual bool ignite();
		
        QSharedPointer<LicenseClient> licenseClient();
		
	protected:
	
        QSharedPointer<LicenseClient> m_licenseClient;
		QMutex m_mutex;
		QWaitCondition m_condition;
		
		virtual void run();
};

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

