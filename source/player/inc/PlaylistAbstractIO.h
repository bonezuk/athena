//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_PLAYLISTABSTRACTIO_H
#define __ORCUS_PLAYER_PLAYLISTABSTRACTIO_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"

#include <QCoreApplication>
#include <QDir>
#include <QUrl>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>
#include <libxml/uri.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include <QString>
#include <QStringList>
#include <QUrl>

#include "common/inc/Factory.h"
#include "common/inc/BOParse.h"
#include "common/inc/DiskOps.h"
#include "common/inc/BIOBufferedStream.h"
#include "track/info/inc/Info.h"
#include "track/db/inc/DBInfo.h"
#include "track/db/inc/TrackDB.h"
#include "player/inc/QPLItemBase.h"
#include "player/inc/QPLProgress.h"

#if defined(OMEGA_MAC_STORE)
#include "widget/inc/SandBoxMac.h"
#include "track/db/inc/SBBookmarkTrackDB.h"
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

#define PLAYLIST_EXPORT

//-------------------------------------------------------------------------------------------

class PlaylistAbstractIO
{
	public:
		PlaylistAbstractIO();
		virtual ~PlaylistAbstractIO();
		
		static bool isSupported(const QString& name);
		static QString factoryName(const QString& name);
		
		virtual bool load(const QString& fileName,QVector<track::info::InfoSPtr>& pList,QPLProgress *progress);
		virtual bool save(const QString& fileName,const QVector<track::info::InfoSPtr>& pList,QPLProgress *progress);
		
		void setParentWidget(QWidget *w);
		
	protected:

		QWidget *m_parent;
		common::BOParse *m_pathParser;
		tint m_pathParserState[9];
	
		virtual QByteArray readLine(common::BIOStream& pFile);
		virtual bool writeLine(common::BIOStream& pFile,const QString& line);
		
		virtual track::info::InfoSPtr getTrack(const QString& fileName);
		
		QString getFilePath(const QString& inName,const QDir& homeDir,bool commentFlag);
		
		void appendToList(const QString& lPath,QVector<track::info::InfoSPtr>& pList,QPLProgress *progress);

		QString getURLFilename(const QString& uPath);
};

//-------------------------------------------------------------------------------------------

ABSTRACT_FACTORY_CLASS(PLAYLIST_EXPORT,PlaylistIOFactory,PlaylistAbstractIO)

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
