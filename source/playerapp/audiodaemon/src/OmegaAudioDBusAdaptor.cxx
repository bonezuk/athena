#include "playerapp/audiodaemon/inc/OmegaAudioDBusAdaptor.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaAudioDBusAdaptor::OmegaAudioDBusAdaptor(OmegaAudioDaemon *pDaemon, QObject *obj) : QDBusAbstractAdaptor(obj),
	m_pDaemon(pDaemon)
{}

//-------------------------------------------------------------------------------------------

OmegaAudioDBusAdaptor::~OmegaAudioDBusAdaptor()
{}

//-------------------------------------------------------------------------------------------

void OmegaAudioDBusAdaptor::playFile(const QString& fileName)
{
	m_pDaemon->playFile(fileName);
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDBusAdaptor::play()
{
	m_pDaemon->play();
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDBusAdaptor::pause()
{
	m_pDaemon->pause();
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
