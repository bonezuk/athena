//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_OMEGAAUDIOBUSINTERFACE_H
#define __PLAYERAPP_PLAYLISTMANAGER_OMEGAAUDIOBUSINTERFACE_H
//-------------------------------------------------------------------------------------------

#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QDBusInterface>
#include <QSharedPointer>

#include "playerapp/playercommon/inc/OmegaAudioInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaAudioBusInterface : public QObject, public OmegaAudioInterface
{
	Q_OBJECT
	
	public:
		OmegaAudioBusInterface(QObject *parent = 0);
		virtual ~OmegaAudioBusInterface();
		
		virtual void playFile(const QString& fileName);
		virtual void play();
		virtual void pause();
		
	private:
		QSharedPointer<QDBusInterface> m_pAudioInterface;
		
		virtual void printError(const char *strR, const char *strE) const;
		
		virtual QSharedPointer<QDBusInterface> getAudioInterface();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
