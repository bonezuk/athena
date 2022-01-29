//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_AUDIODAEMON_OMEGAAUDIODBUSADAPTOR_H
#define __ORCUS_PLAYERAPP_AUDIODAEMON_OMEGAAUDIODBUSADAPTOR_H
//-------------------------------------------------------------------------------------------

#include <QCoreApplication>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QDBusInterface>

#include "playerapp/playercommon/inc/EmbeddedEnv.h"
#include "playerapp/playercommon/inc/OmegaPiDBusServiceNames.h"
#include "playerapp/audiodaemon/inc/OmegaAudioDaemon.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaAudioDBusAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", OMEGAAUDIODAEMON_DBUS_IFACE_NAME)
	
	public:
		OmegaAudioDBusAdaptor(OmegaAudioDaemon *pDaemon, QObject *obj);
		virtual ~OmegaAudioDBusAdaptor();

	public slots:
		Q_NOREPLY void playFile(const QString& fileName, bool isNext);
		Q_NOREPLY void play();
		Q_NOREPLY void pause();

	private:
		OmegaAudioDaemon *m_pDaemon;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
