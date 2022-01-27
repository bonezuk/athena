//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_AUDIODAEMON_OMEGAPLDBUSINTERFACE_H
#define __ORCUS_PLAYERAPP_AUDIODAEMON_OMEGAPLDBUSINTERFACE_H
//-------------------------------------------------------------------------------------------

#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QDBusInterface>
#include <QSharedPointer>

#include "common/inc/Log.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"
#include "playerapp/playercommon/inc/OmegaPiDBusServiceNames.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaPLBusInterface : public QObject, public OmegaPlaylistInterface
{
	Q_OBJECT
		
	public:
		OmegaPLBusInterface(QObject *parent = 0);
		virtual ~OmegaPLBusInterface();
		
		virtual void playbackTime(quint64 tS);
		virtual void onAudioStart(const QString& name);
		
	private:
		QSharedPointer<QDBusInterface> m_pPLManagerInterface;
		
		virtual void printError(const char *strR, const char *strE) const;
		
		QSharedPointer<QDBusInterface> getPLManagerInterface();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
