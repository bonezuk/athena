//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_LICENSE_LICENSER_H
#define __ORCUS_LICENSE_LICENSER_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"
#include "network/inc/Controller.h"
#include "network/http/inc/HTTPService.h"
#include "network/http/inc/HTTPServer.h"
#include "network/http/inc/HTTPConnection.h"
#include "network/mime/inc/LicenseFile.h"
#include "network/mime/inc/LicenseClient.h"
#include "licenser/licenselib/inc/LicenseDB.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace license
{
//-------------------------------------------------------------------------------------------

class LICENSER_EXPORT Licenser : public QCoreApplication
{
	Q_OBJECT
	
	public:
		Licenser(int argc);
		Licenser(int argc,char **argv);
		virtual ~Licenser();
		
	protected:
		
		network::http::HTTPService *m_webService;
		network::http::HTTPServer *m_webServer;
		
		QString generateRegistrationKey(tint noLicenses,const QString& customerName,const QString& transactionID);
		QByteArray generateLicense(network::LicenseFile& inFile,const common::TimeStamp& certTime,const QString& regCode,const QString& clientName);
		bool getHardwareTimeFromLicense(const QByteArray& mem,QString& hID,common::TimeStamp& tS);
		void setHardwareTimeToLicense(const QString& hID,const common::TimeStamp& tS,QByteArray& mem);
		bool deactivateLicense(const QString& regID,const QString& programID);
		tint totalNumberOfLicense(const QString& regID);
		tint usedNumberOfLicense(const QString& regID);
		bool readProgramRecord(const QString& programID,QString& regID,common::TimeStamp& createTS,common::TimeStamp& trialTS,common::TimeStamp& modifyTS);
		bool writeProgramRecord(const QString& regID,const QString& programID,const common::TimeStamp& createTS,const common::TimeStamp& trialTS,const common::TimeStamp& modifyTS);
		bool readHardwareRecord(const QString& regID,const QString& programID,QStringList& hwList);
		bool writeHardwareRecord(const QString& regID,const QString& programID,const QStringList& hwList);
		bool isProgramKeyValid(const network::LicenseFile& licFile);
		QString getDigestKey(common::SHA1Digest& digest);
		QString getCustomerName(const QString& regID);
		QString fromConnection(network::http::HTTPReceive *recieve);
		QString printTime(const common::TimeStamp& t);

	protected slots:
	
		void onStartService();
		void onStopService();
		
		void onWebRoot(network::http::HTTPReceive *recieve);
		void onDeactivate(network::http::HTTPReceive *recieve);
		void onRegister(network::http::HTTPReceive *recieve);
		void onGenerateTrial(network::http::HTTPReceive *recieve);
		void onGenerateKey(network::http::HTTPReceive *recieve);
		
};

//-------------------------------------------------------------------------------------------
} // namespace license
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

