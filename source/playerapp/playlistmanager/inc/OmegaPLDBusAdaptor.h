//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_OMEGAPLDBUSADAPTOR_H
#define __PLAYERAPP_PLAYLISTMANAGER_OMEGAPLDBUSADAPTOR_H
//-------------------------------------------------------------------------------------------

#include <QtDBus/QDBusAbstractAdaptor>

#include "playerapp/playercommon/inc/OmegaPiDBusServiceNames.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaPLDBusAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", OMEGAPLMANAGERAUDIO_DBUS_IFACE_NAME)
	
	public:
		OmegaPLDBusAdaptor(QSharedPointer<OmegaPlaylistInterface>& pPLInterface, QObject *parent = 0);
		virtual ~OmegaPLDBusAdaptor();
		
	public slots:
		Q_NOREPLY void playbackTime(quint64 tS);
		Q_NOREPLY void onAudioStart(const QString& name);
		Q_NOREPLY void onAudioPlay();
		Q_NOREPLY void onAudioPause();
		Q_NOREPLY void onAudioStop();
		Q_NOREPLY void onAudioBuffer(tfloat32 percent);
		Q_NOREPLY void onAudioReadyForNext();
		Q_NOREPLY void onAudioNoNext();
		Q_NOREPLY void onAudioCrossfade();

	private:
		QSharedPointer<OmegaPlaylistInterface> m_pPLInterface;
};


//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
