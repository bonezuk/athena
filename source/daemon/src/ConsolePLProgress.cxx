#include "daemon/inc/ConsolePLProgress.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace daemon
{
//-------------------------------------------------------------------------------------------

ConsolePLProgress::ConsolePLProgress(const QString& plFileName, QObject *parent) : track::db::PLProgress(parent),
	m_plFileName()
{
	QFileInfo plInfo(plFileName);
	m_plFileName = plInfo.fileName();
}

//-------------------------------------------------------------------------------------------

ConsolePLProgress::~ConsolePLProgress()
{}

//-------------------------------------------------------------------------------------------

void ConsolePLProgress::setProgress(tfloat32 v)
{
	track::db::PLProgress::setProgress(v);
	printf("Loading playlist '%s' - %d%%\r", m_plFileName.toUtf8().constData(), static_cast<int>(getProgress()));
}

//-------------------------------------------------------------------------------------------

void ConsolePLProgress::done()
{
	printf("Loading playlist '%s' - done\n", m_plFileName.toUtf8().constData());
}

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
