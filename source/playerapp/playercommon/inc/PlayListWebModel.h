//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_PLAYLISTWEBMODEL_H
#define __PLAYERAPP_PLAYERCOMMON_PLAYLISTWEBMODEL_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/PlayListModel.h"
#include "playerapp/playercommon/inc/OmegaWebInterface.h"
#include "common/inc/CommonFunctions.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PlayListWebModel : public PlayListModel, public OmegaWebInterface
{
	Q_OBJECT
	public:
		PlayListWebModel(QObject *parent = 0);
		PlayListWebModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		virtual ~PlayListWebModel();

		virtual int playlistSize();
		virtual QJsonDocument playlistAsJson(int fromIndex, int toIndex);
	
	private:
	
		virtual QJsonValue playlistItemToJson(int idx);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
