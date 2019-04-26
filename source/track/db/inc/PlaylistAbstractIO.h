//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_DB_PLAYLISTABSTRACTIO_H
#define __ORCUS_TRACK_DB_PLAYLISTABSTRACTIO_H
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
#include "common/inc/SBService.h"
#include "track/info/inc/Info.h"
#include "track/db/inc/DBInfo.h"
#include "track/db/inc/TrackDB.h"
#include "track/db/inc/PLProgress.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

class TRACK_DB_EXPORT PlaylistAbstractIO
{
	public:
		PlaylistAbstractIO();
		virtual ~PlaylistAbstractIO();
		
		static bool isSupported(const QString& name);
		static QString factoryName(const QString& name);
		
		virtual bool load(const QString& fileName,QVector<track::info::InfoSPtr>& pList,PLProgress *progress);
		virtual bool save(const QString& fileName,const QVector<track::info::InfoSPtr>& pList,PLProgress *progress);
		
	protected:

		static common::BOParse *m_pathParser;
		static tint m_pathParserState[9];
				
		virtual track::info::InfoSPtr getTrack(const QString& fileName);
		
		void appendToList(const QString& lPath,QVector<track::info::InfoSPtr>& pList,PLProgress *progress);

		QString getURLFilename(const QString& uPath);
		
		virtual QByteArray readLine(common::BIOStream& pFile);
		virtual bool writeLine(common::BIOStream& pFile,const QString& line);
		
		virtual QString getFilePath(const QString& inName,const QDir& homeDir,bool commentFlag);
};

//-------------------------------------------------------------------------------------------

ABSTRACT_FACTORY_CLASS(TRACK_DB_EXPORT,PlaylistIOFactory,PlaylistAbstractIO)

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

