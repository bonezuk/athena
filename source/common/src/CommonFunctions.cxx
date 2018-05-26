#include "common/inc/CommonFunctions.h"
#include <QtGlobal>
#include <QThread>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
//-------------------------------------------------------------------------------------------

QString getOSErrorString(tint errCode)
{
	QString err;

#if defined(OMEGA_WIN32)
	tushort *wMem = new tushort [1024];
	
	if(::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,0,static_cast<DWORD>(errCode),0,reinterpret_cast<LPWSTR>(wMem),1024,0)!=0)
	{
		err = QString::fromUtf16(wMem);
	}
	delete [] wMem;
	
#elif defined(OMEGA_POSIX)
	const tchar *e = ::strerror(errCode);
	if(e!=0)
	{
		err = QString::fromUtf8(e);
	}
#endif
	return err;
}

//-------------------------------------------------------------------------------------------

void msleepThread(tint msecs)
{
#if QT_VERSION >= 0x050000
	QThread::msleep(msecs);
#else
	usleepThread(msecs * 1000);
#endif
}

//-------------------------------------------------------------------------------------------

void usleepThread(tint usecs)
{
#if QT_VERSION >= 0x050000
	QThread::usleep(usecs);
#else
#if defined(OMEGA_WIN32)
	LARGE_INTEGER lFrequency;
	LARGE_INTEGER lEndTime;
	LARGE_INTEGER lCurTime;

	QueryPerformanceFrequency(&lFrequency);
	if (lFrequency.QuadPart)
	{
		QueryPerformanceCounter(&lEndTime);
		lEndTime.QuadPart += (LONGLONG) usec * lFrequency.QuadPart / 1000000;
		do 
		{
			QueryPerformanceCounter(&lCurTime);
			Sleep(0);
		} while (lCurTime.QuadPart < lEndTime.QuadPart);
	}
#elif defined(OMEGA_POSIX)
	::usleep(usecs);
#endif
#endif
}

//-------------------------------------------------------------------------------------------
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
