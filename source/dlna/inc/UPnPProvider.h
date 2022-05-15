//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_DLNA_UPNPPROVIDER_H
#define __OMEGA_DLNA_UPNPPROVIDER_H
//-------------------------------------------------------------------------------------------

#include "dlna/inc/UPnPLayerIF.h"

#include "common/inc/HardwareIdentification.h"
#include "common/inc/SHA1Digest.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

class DLNA_EXPORT UPnPProvider
{
	public:
		virtual ~UPnPProvider();
		
		static QSharedPointer<UPnPProvider> instance(const QString& webResourceDir);
		static QSharedPointer<UPnPProvider> instance();
		static void shutdown();
		
	protected:
	
		static QSharedPointer<UPnPProvider> m_instance;
		
		bool m_initFlag;
		QString m_webResourceDir;
	
		UPnPProvider();
		
		virtual bool start(const QString& webResourceDir);
		virtual void stop();
		
        virtual QString getDeviceUDN() const;
        virtual QString getDigestKey(const QByteArray& digest) const;
        
        virtual bool initializeWebDirectory();
        virtual bool initScanWebDirectory(const QString& dirName);
        virtual bool initProcessWebFile(const QString& dirName,const QString& fileName);
		virtual bool initDeviceFromDescription(const QString& dddFileName);
};

typedef QSharedPointer<UPnPProvider> UPnPProviderSPtr;

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
