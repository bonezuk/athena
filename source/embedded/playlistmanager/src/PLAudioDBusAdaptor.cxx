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
	QQmlContext *root = m_pModel->qmlEngine()->rootContext();
	if(root != 0)
	{
		root->setContextProperty("playbackTime", timeStamp.secondsTotal());
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
