#include "playerapp/playlistmanager/inc/OmegaPLDBusAdaptor.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaPLDBusAdaptor::OmegaPLDBusAdaptor(OmegaPlaylistInterface *pPLInterface, QObject *parent) : QDBusAbstractAdaptor(parent),
	m_pPLInterface(pPLInterface)
{}

//-------------------------------------------------------------------------------------------

OmegaPLDBusAdaptor::~OmegaPLDBusAdaptor()
{}

//-------------------------------------------------------------------------------------------

void OmegaPLDBusAdaptor::playbackTime(quint64 tS)
{
	m_pPLInterface->playbackTime(tS);
}

//-------------------------------------------------------------------------------------------

void OmegaPLDBusAdaptor::onAudioStart(const QString& name)
{
	m_pPLInterface->onAudioStart(name);
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
