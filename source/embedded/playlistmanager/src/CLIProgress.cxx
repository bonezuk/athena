#include "embedded/playlistmanager/inc/CLIProgress.h"

#if !defined(ORCUS_WIN32)
#include <termios.h>
#include <sys/ioctl.h>
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

CLIProgress::CLIProgress() : common::AbstractProgressInterface(0),
	m_progress(0.0f)
{}

//-------------------------------------------------------------------------------------------

CLIProgress::~CLIProgress()
{}

//-------------------------------------------------------------------------------------------

void CLIProgress::printProgressToConsole()
{
	tint i, cWidth, sbLen, sbPos;
	QString line, progressStr;
	
	progressStr = " " + QString::number(static_cast<tint>(floor(m_progress + 0.495f))) + "%";
#if defined(ORCUS_WIN32)
	cWidth = 80;
#else
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	cWidth = static_cast<tint>(ws.ws_col);
#endif
	if(progressStr.length() + 9 < cWidth)
	{
		sbLen = cWidth - (progressStr.length() + 6);
		sbPos = static_cast<tint>((m_progress / 100.0f) * static_cast<tfloat32>(sbLen));
		if(sbPos < 0)
		{
			sbPos = 0;
		}
		else if(sbPos > sbLen)
		{
			sbPos = sbLen;
		}
		
		line = "[";
		for(i=0;i<sbPos;i++)
		{
			line += "#";
		}
		line += "*";
		for(;i<sbLen;i++)
		{
			line += "-";
		}
		line += "] " + progressStr;
	}
	else
	{
		line = progressStr;
	}
#if defined(ORCUS_WIN32)
	printf("%s\r",line.toLatin1().constData());
#else
//	printf("\33[2K%s\r",line.toLatin1().constData());
#endif
	fflush(stdout);
}

//-------------------------------------------------------------------------------------------

void CLIProgress::setProgress(tfloat32 v)
{
	if(v < m_progress)
	{
		v = 0.0f;
	}
	else if(v > 100.0f)
	{
		v = 100.0f;
	}
	m_progress = v;
	printProgressToConsole();
}

//-------------------------------------------------------------------------------------------

tfloat32 CLIProgress::getProgress()
{
	return m_progress;
}

//-------------------------------------------------------------------------------------------

bool CLIProgress::isCancelled()
{
	return false;
}

//-------------------------------------------------------------------------------------------
		
bool CLIProgress::isActive()
{
	return true;
}

//-------------------------------------------------------------------------------------------

void CLIProgress::activate(bool useReference)
{}

//-------------------------------------------------------------------------------------------

void CLIProgress::deactivate(bool useReference)
{}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
