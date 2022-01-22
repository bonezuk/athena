#include "embedded/playlistmanager/inc/PLAudioDBusAdaptor.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PLAudioDBusAdaptor::PLAudioDBusAdaptor(PlayListModel *pModel, QObject *parent) : QDBusAbstractAdaptor(parent),
	m_pModel(pModel)
{}

//-------------------------------------------------------------------------------------------

PLAudioDBusAdaptor::~PLAudioDBusAdaptor()
{}

//-------------------------------------------------------------------------------------------

void PLAudioDBusAdaptor::playbackTime(quint64 tS)
{
	common::TimeStamp timeStamp(tS);
	QObject *plTime = m_pModel->qmlEngine()->rootContext()->findChild<QObject *>("playbackTimeContainer");
	if(plTime != 0)
	{
		plTime->setProperty("playbackTime", timeStamp.secondsTotal());
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
