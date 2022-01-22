//-------------------------------------------------------------------------------------------
#ifndef __EMBEDDED_PLAYLISTMANAGER_AUDIODBUSINTERFACE_H
#define __EMBEDDED_PLAYLISTMANAGER_AUDIODBUSINTERFACE_H
//-------------------------------------------------------------------------------------------

#include <QtDBus/QDBusAbstractAdaptor>

#include "embedded/omegapicommon/inc/OmegaPiDBusServiceNames.h"
#include "embedded/playlistmanager/inc/PlayListModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAudioDBusAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("Playlist Audio callback interface", OMEGAPLMANAGERAUDIO_DBUS_IFACE_NAME)
	
	public:
		PLAudioDBusAdaptor(PlayListModel *pModel, QObject *parent);
		virtual ~PLAudioDBusAdaptor();
		
	public slots:
		Q_NOREPLY void playbackTime(quint64 tS);
		
	private:
		PlayListModel *m_pModel;
};


//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
