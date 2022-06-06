#include "playerapp/playerios/inc/PlayerIOSTrackDBManager.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

PlayerIOSTrackDBManager *PlayerIOSTrackDBManager::m_instance = 0;

//-------------------------------------------------------------------------------------------

PlayerIOSTrackDBManager::PlayerIOSTrackDBManager(QObject *parent) : QObject(parent)
{}

//-------------------------------------------------------------------------------------------

PlayerIOSTrackDBManager::~PlayerIOSTrackDBManager()
{
	m_instance = 0;
}

//-------------------------------------------------------------------------------------------

PlayerIOSTrackDBManager *PlayerIOSTrackDBManager::instance(QObject *parent)
{
	if(m_instance == 0)
	{
		m_instance = new PlayerIOSTrackDBManager(parent);
		if(!m_instance->open())
		{
			delete m_instance;
			m_instance = 0;
		}
	}
	return m_instance;
}

//-------------------------------------------------------------------------------------------

void PlayerIOSTrackDBManager::release()
{
	if(m_instance != 0)
	{
		delete m_instance;
		m_instance = 0;
	}
}

//-------------------------------------------------------------------------------------------

void PlayerIOSTrackDBManager::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "PlayerIOSTrackDBManager::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

QString PlayerIOSTrackDBManager::trackDBPath() const
{
	QString path = common::DiskOps::mergeName(PlayerIOSUtils::appDataDirectory(), "track.db");
	return path;
}

//-------------------------------------------------------------------------------------------

bool PlayerIOSTrackDBManager::open()
{
	track::db::TrackDB *trackDB;
	bool res = false;
	
	trackDB = track::db::TrackDB::instance(trackDBPath());
	if(trackDB != 0)
	{
		res = true;
	}
	else
	{
		printError("open", "Failed to open track database");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void PlayerIOSTrackDBManager::close()
{
	track::db::TrackDB *trackDB = track::db::TrackDB::instance();
	if(trackDB != 0)
	{
		delete trackDB;
	}
}

//-------------------------------------------------------------------------------------------

void PlayerIOSTrackDBManager::addUploadedFile(const QString& fileName)
{
	bool res = false;
	QSharedPointer<track::info::Info> dbInfo;
	
	dbInfo = track::db::DBInfo::readInfo(fileName);
	if(!dbInfo.isNull())
	{
		emit newtrack(fileName);
		res = true;
	}
	else
	{
		QString err = QString("Failed to read information for '%1'").arg(fileName);
		printError("addUploadedFile", err.toUtf8().constData());
	}
	if(!res)
	{
		common::DiskOps::deleteFile(fileName);
	}
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
