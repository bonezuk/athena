#include "network/inc/Resource.h"
#include "common/inc/Log.h"
#include "playerapp/playercommon/inc/OmegaPiDBusServiceNames.h"
#include "playerapp/playlistmanager/inc/OmegaAudioBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaAudioBusInterface::OmegaAudioBusInterface(QObject *parent) : OmegaAudioInterface(),
	QObject(parent),
	m_pAudioInterface()
{}

//-------------------------------------------------------------------------------------------

OmegaAudioBusInterface::~OmegaAudioBusInterface()
{}

//-------------------------------------------------------------------------------------------

void OmegaAudioBusInterface::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaAudioBusInterface::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioBusInterface::playFile(const QString& fileName, bool isNext)
{
	QSharedPointer<QDBusInterface> pIface = getAudioInterface();
	if(!pIface.isNull())
	{
		pIface->call(QLatin1String("playFile"), fileName, isNext);
	}
}

//-------------------------------------------------------------------------------------------

void OmegaAudioBusInterface::play()
{
	QSharedPointer<QDBusInterface> pIface = getAudioInterface();
	if(!pIface.isNull())
	{
		pIface->call(QLatin1String("play"));
	}
}

//-------------------------------------------------------------------------------------------

void OmegaAudioBusInterface::pause()
{
	QSharedPointer<QDBusInterface> pIface = getAudioInterface();
	if(!pIface.isNull())
	{
		pIface->call(QLatin1String("pause"));
	}
}

//-------------------------------------------------------------------------------------------

QSharedPointer<QDBusInterface> OmegaAudioBusInterface::getAudioInterface()
{
	if(m_pAudioInterface.isNull() || !m_pAudioInterface->isValid())
	{
#if defined(OMEGA_LINUX)
		QDBusConnection bus = QDBusConnection::systemBus();
#else
		QDBusConnection bus = QDBusConnection::sessionBus();
#endif
		if(bus.isConnected())
		{
			QSharedPointer<QDBusInterface> pInterface(new QDBusInterface(OMEGAAUDIODAEMON_SERVICE_NAME, "/", OMEGAAUDIODAEMON_DBUS_IFACE_NAME, bus, this));
			if(pInterface->isValid())
			{
				m_pAudioInterface = pInterface;
			}
			else
			{
				QString err = QString("Failed to connect to Omega Audio Daemon. %1").arg(bus.lastError().message());
				printError("getAudioInterface", err.toUtf8().constData());
			}
		}
		else
		{
			QString err = QString("Failed to connect to D-Bus. %1").arg(bus.lastError().message());
			printError("getAudioInterface", err.toUtf8().constData());
		}
	}
	return m_pAudioInterface;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
