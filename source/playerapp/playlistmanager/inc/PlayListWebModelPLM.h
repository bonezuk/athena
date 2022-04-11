//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_PLAYLISTWEBMODELPLM_H
#define __PLAYERAPP_PLAYLISTMANAGER_PLAYLISTWEBMODELPLM_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/PlayListWebModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PlayListWebModelPLM : public PlayListWebModel, public OmegaWebInterface
{
	Q_OBJECT
	public:
		PlayListWebModelPLM(QObject *parent = 0);
		PlayListWebModelPLM(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		virtual ~PlayListWebModelPLM();
		
		virtual void initialise();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
