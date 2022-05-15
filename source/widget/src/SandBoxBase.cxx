#include "widget/inc/SandBoxBase.h"
#include "common/inc/DiskOps.h"

#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QApplication>
#include <QFileDialog>
#include <QSettings>
#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#endif

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace widget
{
//-------------------------------------------------------------------------------------------
// SBServiceBase
//-------------------------------------------------------------------------------------------

SBServiceBase::SBServiceBase(QObject *parent) : common::SBService(parent)
{}

//-------------------------------------------------------------------------------------------

SBServiceBase::~SBServiceBase()
{}

//-------------------------------------------------------------------------------------------

QString SBServiceBase::getHomeDirectory()
{
	QString hDir;
    QStringList hDirList;

#if QT_VERSION >= 0x050000
    hDirList = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
#else
    hDirList.append(QDir::homePath());
#endif

    if(hDirList.size()>0)
    {
        hDir = hDirList.at(0);
    }
	return hDir;
}

//-------------------------------------------------------------------------------------------

QString SBServiceBase::getTempDirectory()
{
	return QDir::tempPath();
}

//-------------------------------------------------------------------------------------------

QString SBServiceBase::getApplicationDataDirectory()
{
	QSettings iSettings(QSettings::IniFormat,QSettings::UserScope,QCoreApplication::organizationName(),QCoreApplication::applicationName());
	QString userDir = QFileInfo(iSettings.fileName()).absolutePath();
	if(userDir.at(userDir.length()-1)!='/' && userDir.at(userDir.length()-1)!='\\')
	{
		userDir += "/";
	}
	userDir = QDir::toNativeSeparators(userDir);
	omega::common::DiskOps::path(userDir,true);
	return userDir;
}

//-------------------------------------------------------------------------------------------

void SBServiceBase::loadDirDialog(QObject *parent,const QString& title,const QString& dirName)
{
	QWidget *parentWidget = dynamic_cast<QWidget *>(parent);
	QString dName = QFileDialog::getExistingDirectory(parentWidget,title,dirName);
	if(!dName.isEmpty())
	{
		emit onLoadDirectory(dName);
	}
}

//-------------------------------------------------------------------------------------------

void SBServiceBase::loadFilesDialog(QObject *parent,const QString& title,const QString& dirName,const QString& filter)
{
	QWidget *parentWidget = dynamic_cast<QWidget *>(parent);
	QStringList fileList = QFileDialog::getOpenFileNames(parentWidget,title,dirName,filter);
	if(!fileList.isEmpty())
	{
		emit onLoadFiles(fileList);
	}
}

//-------------------------------------------------------------------------------------------

void SBServiceBase::saveFileDialog(QObject *parent,const QString& title,const QString& dirName,const QString& filter)	
{
	QString selFilter;
	QWidget *parentWidget = dynamic_cast<QWidget *>(parent);
	QString fileName = QFileDialog::getSaveFileName(parentWidget,title,dirName,filter,&selFilter);
	if(!fileName.isEmpty())
	{
		emit onSaveFile(fileName,selFilter);
	}
}

//-------------------------------------------------------------------------------------------

void *SBServiceBase::allocatePool()
{
	return 0;
}

//-------------------------------------------------------------------------------------------

void SBServiceBase::releasePool(void *pPtr)
{}

//-------------------------------------------------------------------------------------------
} // namespace widget
} // namespace omega
//-------------------------------------------------------------------------------------------
