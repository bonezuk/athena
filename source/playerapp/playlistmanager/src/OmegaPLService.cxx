#include "playerapp/playlistmanager/inc/OmegaPLService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaPLService::OmegaPLService(QSharedPointer<OmegaPlaylistInterface>& pPLInterface, QObject *parent) : QObject(parent),
	m_pPLInterface(pPLInterface),
	m_pServiceThread(0)
{}

//-------------------------------------------------------------------------------------------

OmegaPLService::~OmegaPLService()
{
	OmegaPLService::stop();
}

//-------------------------------------------------------------------------------------------

void OmegaPLService::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaPLService::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool OmegaPLService::start()
{
	bool res = false;
	
	stop();
	
	m_pServiceThread = new IPCServiceThread(QString::fromUtf8(OMEGAPLAYLISTMANAGER_SERVICE_NAME));
	QObject::connect(m_pServiceThread, SIGNAL(onProcessRPC(QByteArray)), this, SLOT(processRPC(QByteArray)));
	if(m_pServiceThread->startService())
	{
		res = true;
	}
	else
	{
		printError("start", "Failed to start RPC playlist service");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void OmegaPLService::stop()
{
	if(m_pServiceThread != 0)
	{
		QObject::disconnect(m_pServiceThread, SIGNAL(onProcessRPC(QByteArray)), this, SLOT(processRPC(QByteArray)));
		m_pServiceThread->stopService();
		delete m_pServiceThread;
		m_pServiceThread = 0;
	}
}
		
//-------------------------------------------------------------------------------------------

void OmegaPLService::processRPC(QByteArray rpcArray)
{
	QJsonParseError jsonErr;
	QJsonDocument doc = QJsonDocument::fromJson(rpcArray, &jsonErr);
	if(!doc.isNull())
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
	else
	{
		QString err = QString("Error parsing expected JSON object. %1").arg(jsonErr.errorString());
		printError("processRPC", err.toUtf8().constData());
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
