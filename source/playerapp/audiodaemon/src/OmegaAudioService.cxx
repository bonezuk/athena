#include "playerapp/audiodaemon/inc/OmegaAudioService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaAudioService::OmegaAudioService(OmegaAudioDaemon *pDaemon, QObject *parent) : QObject(parent),
	m_pDaemon(pDaemon),
	m_pServiceThread(0)
{}

//-------------------------------------------------------------------------------------------

OmegaAudioService::~OmegaAudioService()
{
	OmegaAudioService::stop();
}

//-------------------------------------------------------------------------------------------

void OmegaAudioService::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaAudioService::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool OmegaAudioService::start()
{
	bool res = false;
	
	stop();
	
	m_pServiceThread = new IPCServiceThread(QString::fromUtf8(OMEGAAUDIOINTERFACE_SERVICE_NAME));
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

void OmegaAudioService::stop()
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

void OmegaAudioService::processRPC(QByteArray rpcArray)
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
				// { "function": "playFile", "fileName": "/Music/file_to_play.wav", "isNext": true }
				if(funcName == "playFile")
				{
					QString fileName = json.value("fileName").toString();
					bool isNext = json.value("isNext").toBool();
					m_pDaemon->playFile(fileName, isNext);
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
