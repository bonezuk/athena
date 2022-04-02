#include "playerapp/audiodaemon/inc/OmegaAudioService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaAudioService::OmegaAudioService(OmegaAudioDaemon *pDaemon, QObject *parent) : IPCService(OMEGAAUDIOINTERFACE_SERVICE_NAME, parent),
	m_pDaemon(pDaemon)
{}

//-------------------------------------------------------------------------------------------

OmegaAudioService::~OmegaAudioService()
{}

//-------------------------------------------------------------------------------------------

void OmegaAudioService::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaAudioService::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioService::handleRPCJson(const QJsonDocument& doc)
{
	if(doc.isObject())
	{
		QJsonObject json = doc.object();
		QString funcName = json.value(OMEGA_IPC_FUNCTION_NAME).toString();
		if(!funcName.isEmpty())
		{
			// { "function": "playFile", "fileName": "/Music/file_to_play.wav", "isNext": true }
			if(funcName == "playFile")
			{
				QString fileName = json.value("fileName").toString();
				bool isNext = json.value("isNext").toBool();
				m_pDaemon->playFile(fileName, isNext);
			}
			// { "function": "playFileWithTime", "fileName": "/Music/file_to_play.wav", "isNext": true, start: 30.01, length: 40.5}
			else if(funcName == "playFileWithTime")
			{
				QString fileName = json.value("fileName").toString();
				common::TimeStamp start = json.value("start").toDouble();
				common::TimeStamp length = json.value("length").toDouble();
				bool isNext = json.value("isNext").toBool();
				m_pDaemon->playFileWithTime(fileName, start, length, isNext);
			}
			// { "function": "play" }
			else if(funcName == "play")
			{
				m_pDaemon->play();
			}
			// { "function": "pause" }
			else if(funcName == "pause")
			{
				m_pDaemon->pause();
			}
			else
			{
				QString err = QString("Unknown RPC function '%1'").arg(funcName);
				printError("handleRPCJson", err.toUtf8().constData());
			}
		}
		else
		{
			printError("handleRPCJson", "Expected function name not in JSON data");
		}
	}
	else
	{
		printError("handleRPCJson", "RPC JSON data is not an object");
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
