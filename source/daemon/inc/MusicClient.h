//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DAEMON_MUSICCLIENT_H
#define __ORCUS_DAEMON_MUSICCLIENT_H
//-------------------------------------------------------------------------------------------

#include "daemon/inc/MusicDaemon.h"
#include "network/http/inc/HTTPClientService.h"
#include "network/http/inc/HTTPClient.h"
#include "track/info/inc/DaemonInfo.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace daemon
{
//-------------------------------------------------------------------------------------------

class DAEMON_EXPORT MusicClient : public QObject
{
	Q_OBJECT
	
	public:
		MusicClient(QObject *parent = 0);
		virtual ~MusicClient();
		
		virtual void connect(const QString& hostName);
	
	private:
		QString m_hostName;
		network::http::HTTPClientService *m_webClientService;
		
		virtual void processTrackList(network::http::HTTPCTransaction *trans);
		virtual void runTrackRequest(const QString& hostName);
		virtual void runEventRequest(const QString& hostName);

	private slots:
		void onTransactionError(network::http::HTTPCTransaction *trans,const QString& err);
		void onError(network::http::HTTPClient *client,const QString& err);
		void onTransaction(network::http::HTTPCTransaction *trans);
		void onEventStream(network::http::HTTPCTransaction *trans,const network::http::EventStreamItem& item);
		void onEventComplete(network::http::HTTPCTransaction *trans);
	
	signals:
		void onError(const QString&);
		void onLoadTracks(QVector<QSharedPointer<track::info::Info> >&);
		void onAudioTime(int id, tuint64 t);
};

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

