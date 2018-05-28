//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_PLAYLISTXSPFIO_H
#define __ORCUS_PLAYER_PLAYLISTXSPFIO_H
//-------------------------------------------------------------------------------------------

#include "player/inc/PlaylistAbstractIO.h"

#include <QStack>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class PlaylistXSPFIO : public PlaylistAbstractIO
{
	public:
		PlaylistXSPFIO();
		virtual ~PlaylistXSPFIO();

		virtual bool load(const QString& fileName,QVector<track::info::InfoSPtr>& pList,QPLProgress *progress);
		virtual bool save(const QString& fileName,const QVector<track::info::InfoSPtr>& pList,QPLProgress *progress);
	
	protected:
		
		QString m_outFilename;
		
		void loadXMLNode(xmlDocPtr doc,xmlNodePtr pNode,QStack<QString>& nameStack,QList<QPair<QString,QByteArray> >& fileList);
		void loadXMLTrack(xmlDocPtr doc,xmlNodePtr pNode,QList<QPair<QString,QByteArray> >& fileList);
		bool loadXMLBookmarkExtension(xmlDocPtr doc,xmlNodePtr eNode,QByteArray& bkArray);
		void loadXMLFilename(xmlDocPtr doc,xmlNodePtr fNode,QString& fileName);

		bool saveXMLTrack(xmlTextWriterPtr writer,track::info::InfoSPtr& pInfo);
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
