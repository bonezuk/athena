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

void OmegaPLDBusAdaptor::onAudioPlay()
{
	m_pPLInterface->onAudioPlay();
}

//-------------------------------------------------------------------------------------------

void OmegaPLDBusAdaptor::onAudioPause()
{
	m_pPLInterface->onAudioPause();
}

//-------------------------------------------------------------------------------------------

void OmegaPLDBusAdaptor::onAudioStop()
{
	m_pPLInterface->onAudioStop();
}

//-------------------------------------------------------------------------------------------

void OmegaPLDBusAdaptor::onAudioBuffer(tfloat32 percent)
{
	m_pPLInterface->onAudioBuffer(percent);
}

//-------------------------------------------------------------------------------------------

void OmegaPLDBusAdaptor::onAudioReadyForNext()
{
	m_pPLInterface->onAudioReadyForNext();
}

//-------------------------------------------------------------------------------------------

void OmegaPLDBusAdaptor::onAudioNoNext()
{
	m_pPLInterface->onAudioNoNext();
}

//-------------------------------------------------------------------------------------------

void OmegaPLDBusAdaptor::onAudioCrossfade()
{
	m_pPLInterface->onAudioCrossfade();
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
