//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_QPLITEMALBUM_H
#define __ORCUS_PLAYER_QPLITEMALBUM_H
//-------------------------------------------------------------------------------------------

#include "player/inc/QPLItemBase.h"
#include "player/inc/QPlaylistWidget.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class QPLItemAlbum : public QPLItemBase
{
	public:
		Q_OBJECT
	
	public:
        QPLItemAlbum(QPlaylistWidget *playListW,QSharedPointer<track::info::Info>& info);
		virtual ~QPLItemAlbum();
		
		virtual PlaylistItemType type() const;

		virtual tint height() const;
		virtual void paint(tint yTop,QPainter *painter);

		virtual common::TimeStamp lengthOfTrack();

	protected:

		QString getArtist();
		QString getTime();
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

