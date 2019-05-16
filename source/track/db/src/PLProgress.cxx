#include "track/db/inc/PLProgress.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

PLProgress::PLProgress(QObject *parent) : QObject(parent),
	m_percentProgress(0.0f)
{}

//-------------------------------------------------------------------------------------------

PLProgress::~PLProgress()
{}

//-------------------------------------------------------------------------------------------

void PLProgress::setProgress(tfloat32 v)
{
	if(v < 0.0f)
	{
		v = 0.0f;
	}
	else if (v > 100.0f)
	{
		v = 100.0f;
	}
	m_percentProgress = v;
}

//-------------------------------------------------------------------------------------------

tfloat32 PLProgress::getProgress()
{
	return m_percentProgress;
}

//-------------------------------------------------------------------------------------------

bool PLProgress::isCancelled()
{
	return false;
}

//-------------------------------------------------------------------------------------------

bool PLProgress::isActive()
{
	return false;
}

//-------------------------------------------------------------------------------------------

void PLProgress::activate(bool useReference)
{}

//-------------------------------------------------------------------------------------------

void PLProgress::deactivate(bool useReference)
{}

//-------------------------------------------------------------------------------------------		
#if defined(OMEGA_MAC_STORE)
//-------------------------------------------------------------------------------------------

bool PLProgress::getPermissions(const QString& fileName, QList<QPair<QString,QByteArray> >& fileList)
{
	return false;
}

//-------------------------------------------------------------------------------------------

QByteArray PLProgress::getSandboxBookmark(const QString& outFilename,const QString& trackFilename)
{
	return QByteArray();
}

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
