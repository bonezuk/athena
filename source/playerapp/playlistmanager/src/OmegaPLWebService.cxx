#include "playerapp/playlistmanager/inc/OmegaPLWebService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaPLWebService::OmegaPLWebService(QSharedPointer<OmegaWebInterface>& pPLWebInterface, QObject *parent) : IPCService(OMEGAPLWEB_SERVICE_NAME, parent),
	m_pPLWebInterface(pPLWebInterface)
{}

//-------------------------------------------------------------------------------------------

OmegaPLWebService::~OmegaPLWebService()
{}

//-------------------------------------------------------------------------------------------

void OmegaPLWebService::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaPLWebService::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaPLWebService::handleRPCJson(const QJsonDocument& doc)
{
	if(doc.isObject())
	{
		QJsonObject json = doc.object();
		QString funcName = json.value(OMEGA_IPC_FUNCTION_NAME).toString();
		if(!funcName.isEmpty())
		{
			// { "function": "getFullPlaylist" }
			if(funcName == "getFullPlaylist")
			{
				int len = m_pPLWebInterface->playlistSize();
				QJsonDocument doc = m_pPLWebInterface->playlistAsJson(0, len);
				QByteArray repArr = doc.toJson(QJsonDocument::Compact);
				m_pServiceThread->postResponse(repArr);
			}
			// { "function": "getPlaybackState" }
			else if(funcName == "getPlaybackState")
			{
				QJsonDocument doc = m_pPLWebInterface->getPlaybackState();
				QByteArray repArr = doc.toJson(QJsonDocument::Compact);
				m_pServiceThread->postResponse(repArr);
			}
			// { "function": "onPressPlay" }
			else if(funcName == "onPressPlay")
			{
				m_pPLWebInterface->onPressPlay();
			}
			// { "function": "onStartPlaying", "id": 12345 }
			else if(funcName == "onStartPlaying")
			{
				QByteArray arr = doc.toJson(QJsonDocument::Compact);
				QString idStr = json.value("id").toVariant().toString();
				tuint64 id = static_cast<tuint64>(idStr.toULongLong());
				
				if(id > 0)
				{
					m_pPLWebInterface->onStartPlaying(id);
				}
			}
			else
			{
				QString err = QString("Unknown RPC function '%1'").arg(funcName);
				printError("processRPC", err.toUtf8().constData());
			}
		}
		else
		{
			printError("processRPC", "Expected function name not in JSON data");
		}
	}
	else
	{
		printError("processRPC", "RPC JSON data is not an object");
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
