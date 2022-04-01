#include "playerapp/playlistmanager/inc/OmegaPLService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaPLService::OmegaPLService(QSharedPointer<OmegaPlaylistInterface>& pPLInterface, QObject *parent) : IPCService(OMEGAPLAYLISTMANAGER_SERVICE_NAME, parent),
	m_pPLInterface(pPLInterface)
{}

//-------------------------------------------------------------------------------------------

OmegaPLService::~OmegaPLService()
{}

//-------------------------------------------------------------------------------------------

void OmegaPLService::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaPLService::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaPLService::handleRPCJson(const QJsonDocument& doc)
{
	if(doc.isObject())
	{
		QJsonObject json = doc.object();
		QString funcName = json.value(OMEGA_IPC_FUNCTION_NAME).toString();
		if(!funcName.isEmpty())
		{
			// { "function": "playbackTime", "timestamp": 12.3456 }
			if(funcName == "playbackTime")
			{
				common::TimeStamp tS = json.value("timestamp").toDouble();
				m_pPLInterface->playbackTime(static_cast<tuint64>(tS));
			}
			// { "function": "onAudioStart", "name": "audio_filename.mp3" }
			else if(funcName == "onAudioStart")
			{
				QString name = json.value("name").toString();
				m_pPLInterface->onAudioStart(name);
			}
			// { "function": "onAudioPlay" }
			else if(funcName == "onAudioPlay")
			{
				m_pPLInterface->onAudioPlay();
			}
			// { "function": "onAudioPause" }
			else if(funcName == "onAudioPause")
			{
				m_pPLInterface->onAudioPause();
			}
			// { "function": "onAudioStop" }
			else if(funcName == "onAudioStop")
			{
				m_pPLInterface->onAudioStop();
			}
			// { "function": "onAudioBuffer", "percent": 12.3456 }
			else if(funcName == "onAudioBuffer")
			{
				tfloat32 percent = static_cast<tfloat32>(json.value("percent").toDouble());
				m_pPLInterface->onAudioBuffer(percent);
			}
			// { "function": "onAudioReadyForNext" }
			else if(funcName == "onAudioReadyForNext")
			{
				m_pPLInterface->onAudioReadyForNext();
			}
			// { "function": "onAudioNoNext" }
			else if(funcName == "onAudioNoNext")
			{
				m_pPLInterface->onAudioNoNext();
			}
			// { "function": "onAudioCrossfade" }
			else if(funcName == "onAudioCrossfade")
			{
				m_pPLInterface->onAudioCrossfade();
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
