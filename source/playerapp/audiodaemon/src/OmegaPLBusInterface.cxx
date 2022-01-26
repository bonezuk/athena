#include "playerapp/audiodaemon/inc/OmegaPLBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaPLBusInterface::OmegaPLBusInterface(QObject *parent) : OmegaPlaylistInterface(),
	QObject(parent)
{}

//-------------------------------------------------------------------------------------------

OmegaPLBusInterface::~OmegaPLBusInterface()
{}

//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaPLBusInterface::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::playbackTime(quint64 tS)
{
	QSharedPointer<QDBusInterface> pIface = getPLManagerInterface();
	if(!pIface.isNull())
	{
		pIface->call("playbackTime", tS);
	}
}

//-------------------------------------------------------------------------------------------

QSharedPointer<QDBusInterface> OmegaPLBusInterface::getPLManagerInterface()
{
	if(m_pPLManagerInterface.isNull() || !m_pPLManagerInterface->isValid())
	{
#if defined(OMEGA_LINUX)
		QDBusConnection bus = QDBusConnection::systemBus();
#else
		QDBusConnection bus = QDBusConnection::sessionBus();
#endif
		if(bus.isConnected())
		{
			QSharedPointer<QDBusInterface> pInterface(new QDBusInterface(OMEGAPLAYLISTMANAGER_SERVICE_NAME, "/", OMEGAPLMANAGERAUDIO_DBUS_IFACE_NAME, bus, this));
			if(pInterface->isValid())
			{
				m_pPLManagerInterface = pInterface;
			}
			else
			{
				QString err = QString("Failed to connect to Playlist Manager. %1").arg(bus.lastError().message());
				printError("getPLManagerInterface", err.toUtf8().constData());
			}
		}
		else
		{
			QString err = QString("Failed to connect to D-Bus. %1").arg(bus.lastError().message());
			printError("getPLManagerInterface", err.toUtf8().constData());
		}
	}
	return m_pPLManagerInterface;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
