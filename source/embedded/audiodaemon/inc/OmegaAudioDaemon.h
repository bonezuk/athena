//-------------------------------------------------------------------------------------------
#ifndef __EMBEDDED_AUDIODAEMON_OMEGAAUDIODAEMON_H
#define __EMBEDDED_AUDIODAEMON_OMEGAAUDIODAEMON_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"
#include "audioio/inc/AOBase.h"
#include "engine/inc/Codec.h"
#include "track/info/inc/Info.h"
#include "common/inc/DiskOps.h"

#include <QCoreApplication>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusVariant>

#include "embedded/omegapicommon/inc/EmbeddedEnv.h"
#include "embedded/omegapicommon/inc/OmegaPiDBusServiceNames.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaAudioDBusAdaptor;

//-------------------------------------------------------------------------------------------

class OmegaAudioDaemon : public QCoreApplication
{
	Q_OBJECT
	
	public:
		OmegaAudioDaemon(int argc, char **argv, OmegaAudioDBusAdaptor *pInterface);
		virtual ~OmegaAudioDaemon();
		
		virtual void playFileDBUS(const QString& fileName);
		virtual void shutdownDaemon();
	
	private:
	
		QSharedPointer<audioio::AOBase> m_audio;
		OmegaAudioDBusAdaptor *m_pInterface;
		
		virtual void printError(const char *strR, const char *strE) const;
		
	private slots:
		void onInit();
		void onQuitDaemon();
				
		void onAudioStart(const QString& name);
		void onAudioPlay();
		void onAudioPause();
		void onAudioTime(quint64 t);
		void onAudioBuffer(tfloat32 percent);
		void onAudioReadyForNext();
		void onAudioNoNext();
		void onAudioCrossfade();
};

//-------------------------------------------------------------------------------------------

class OmegaAudioDBusAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", OMEGAAUDIODAEMON_DBUS_IFACE_NAME)
	
	public:
		OmegaAudioDBusAdaptor(QObject *obj);
		virtual ~OmegaAudioDBusAdaptor();
		
		virtual void setAudioDaemon(OmegaAudioDaemon *pDaemon);

	public slots:
		Q_NOREPLY void playFile(const QString& fileName);
		
	private:
		OmegaAudioDaemon *m_pDaemon;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
