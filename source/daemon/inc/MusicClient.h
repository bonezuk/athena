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
		virtual void disconnect();
		
		virtual QString lastConnectedHost();
	
	private:
		QString m_hostName;
		network::http::HTTPClientService *m_webClientService;
		QVector<QSharedPointer<network::http::HTTPClient> > m_clients;
		
		virtual void processTrackList(network::http::HTTPCTransaction *trans);
		virtual void setLastConnectedHost(const QString& hostName);
		
		virtual void runTrackRequest();
		virtual void runEventRequest();

		virtual void addClient(QSharedPointer<network::http::HTTPClient>& pClient);
		virtual void removeClient(network::http::HTTPClient *client);

	private slots:
		
		void onTrackTransaction(network::http::HTTPCTransaction *trans);
		void onEventStream(network::http::HTTPCTransaction *trans,const network::http::EventStreamItem& item);
		
		void onClientComplete(network::http::HTTPClient *client);

		void onTransactionError(network::http::HTTPCTransaction *trans,const QString& err);
		void onError(network::http::HTTPClient *client,const QString& err);
	
	signals:
		void onError(const QString&);
		void onLoadTracks(QVector<QSharedPointer<track::info::Info> >&);
		void onAudioTime(int id, tuint64 t);
		void onAudioPlay(int id);
		void onAudioPause(int id);
		void onAudioStop();
};

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

