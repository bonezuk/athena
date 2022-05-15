//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_PLAYLISTWEBMODELPLM_H
#define __PLAYERAPP_PLAYLISTMANAGER_PLAYLISTWEBMODELPLM_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/PlayListWebModel.h"
#include "playerapp/playercommon/inc/OmegaWebInterface.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class PlayListWebModelPLM : public PlayListWebModel
{
	public:
		PlayListWebModelPLM(QObject *parent = 0);
		PlayListWebModelPLM(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		virtual ~PlayListWebModelPLM();
		
		virtual void initialise();
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
