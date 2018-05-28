//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_PLAYLISTPLSIO_H
#define __ORCUS_PLAYER_PLAYLISTPLSIO_H
//-------------------------------------------------------------------------------------------

#include "player/inc/PlaylistAbstractIO.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class PlaylistPLSIO : public PlaylistAbstractIO
{
	public:
	
		typedef enum
		{
			e_linePlaylist = 0,
			e_lineFile,
			e_lineTitle,
			e_lineLength,
			e_lineNoOfEntries,
			e_lineVersion,
			e_lineUnknown
		} LineType;
		
	public:
		PlaylistPLSIO();
		virtual ~PlaylistPLSIO();
		
		virtual bool load(const QString& fileName,QVector<track::info::InfoSPtr>& pList,QPLProgress *progress);
		virtual bool save(const QString& fileName,const QVector<track::info::InfoSPtr>& pList,QPLProgress *progress);

	protected:
	
		common::BOParse *m_plsLang;
		tint m_plsState[9];
		
		LineType parseLineInfo(const QString& line,QString& txtData,tint& noData);
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
