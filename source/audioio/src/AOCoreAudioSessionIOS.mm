//-------------------------------------------------------------------------------------------
#if defined(OMEGA_IOS)
//-------------------------------------------------------------------------------------------

#import <Foundation/Foundation.h>
#import <AVFAudio/AVAudioSession.h>

#include "audioio/inc/AOCoreAudioSessionIOS.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

QSharedPointer<AOCoreAudioSessionIOS> AOCoreAudioSessionIOS::m_instance;

//-------------------------------------------------------------------------------------------

AOCoreAudioSessionIOS::AOCoreAudioSessionIOS(QObject *parent) : QObject(parent)
{}

//-------------------------------------------------------------------------------------------

AOCoreAudioSessionIOS::~AOCoreAudioSessionIOS()
{}

//-------------------------------------------------------------------------------------------

QSharedPointer<AOCoreAudioSessionIOS> AOCoreAudioSessionIOS::audioInstance()
{
	return m_instance;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioSessionIOS::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "AOCoreAudioSessionIOS::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioSessionIOS::printError(const char *strR, const char *strE, void *pE) const
{
	NSError *error = (NSError *)pE;
	
	if(error != nil)
	{
		QString errS;
		QString desc = QString::fromUtf8(error.localizedDescription.UTF8String);
		QString reason = QString::fromUtf8(error.localizedFailureReason.UTF8String);
		QString recovery = QString::fromUtf8(error.localizedRecoverySuggestion.UTF8String);
		errS  = strE;
		errS +=	common::c_endl;
		errS += "\tDescription - " + desc + common::c_endl;
		errS += "\tReason - " + reason + common::c_endl;
		errS += "\tSuggestion - " + recovery + common::c_endl;
		printError(strR, errS.toUtf8().constData());
	}
	else
	{
		printError(strR, strE);
	}
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioSessionIOS::init()
{
	bool res = false;
	AVAudioSession *aSession = [AVAudioSession sharedInstance];
	
	if(aSession != nil)
	{
		NSError *audioSessionError = nil;
		BOOL r = [aSession setCategory:AVAudioSessionCategoryPlayback error:&audioSessionError];
		if(r == YES)
		{
			r = [aSession setActive:YES error:&audioSessionError];
			if(r == YES)
			{
				res = true;
			}
			else
			{
				printError("init", "Error activating audio session", audioSessionError);
			}
		}
		else
		{
			printError("init", "Failed to set audio session to playback", audioSessionError);
		}
	}
	else
	{
		printError("init", "Failed to get audio session for app");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioSessionIOS::close()
{
	AVAudioSession *aSession = [AVAudioSession sharedInstance];
	
	if(aSession != nil)
	{
		NSError *audioSessionError = nil;
		BOOL r = [aSession setActive:NO error:&audioSessionError];
		if(r != YES)
		{
			printError("init", "Error deactivating audio session", audioSessionError);
		}
	}
	else
	{
		printError("close", "Failed to get audio session for app");
	}	
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioSessionIOS::addToPriorityMap(QMap<int, QList<int> >& rMap, int priority, int rate)
{
	QMap<int, QList<int> >::iterator ppI = rMap.find(priority);
	if(ppI != rMap.end())
	{
		QList<int>& pList = ppI.value();
		pList.append(rate);
	}
	else
	{
		QList<int> pList;
		pList.append(rate);
		rMap.insert(priority, pList);
	}
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioSessionIOS::setFrequency(void *pSess, int frequency)
{
	BOOL res;
	AVAudioSession *aSession = (AVAudioSession *)pSess;
	NSError *audioSessionError = nil;
	double freqD = static_cast<double>(frequency);
	int currentFrequency = static_cast<int>(aSession.sampleRate);
	
	if(currentFrequency == frequency)
	{
		return true;
	}
	
	res = [aSession setActive:NO error:&audioSessionError];
	if(res == YES)
	{
		res = [aSession setPreferredSampleRate:freqD error:&audioSessionError];
		if(res == YES)
		{
			res = [aSession setActive:YES error:&audioSessionError];
			if(res == YES)
			{
				res = YES;
//				currentFrequency = static_cast<int>(aSession.sampleRate);
//				res = (currentFrequency == frequency) ? YES : NO;
			}
		}
	}
	return (res == YES) ? true : false;
}

//-------------------------------------------------------------------------------------------

int AOCoreAudioSessionIOS::startPlaybackWithFrequency(int codecFrequency)
{
	static const int rates[18] = {
		768000, 705600, 384000, 352800,
		192000, 176400, 96000,
		 88200,  64000, 48000,
		 44100,  32000, 24000,
		 22050,  16000, 12000,
		 11025,   8000
	};

	int outputRate = -1;
	AVAudioSession *aSession = [AVAudioSession sharedInstance];
	
	if(aSession != nil)
	{
		NSError *audioSessionError = nil;
		QMap<int, QList<int> > ratePriority;
		QMap<int, QList<int> >::iterator ppI;
		
		for(int idx = 0; idx < 18; idx++)
		{
			if(codecFrequency == rates[idx])
			{
				addToPriorityMap(ratePriority, 1, rates[idx]);
			}
			else if((codecFrequency * 2) == rates[idx] || (codecFrequency * 4) == rates[idx])
			{
				addToPriorityMap(ratePriority, 2, rates[idx]);
			}
			else if(codecFrequency < rates[idx])
			{
				addToPriorityMap(ratePriority, 3, rates[idx]);
			}
			else
			{
				addToPriorityMap(ratePriority, 4, rates[idx]);
			}
		}
		
		for(ppI = ratePriority.begin(); ppI != ratePriority.end() && outputRate < 0; ppI++)
		{
			QList<int>& rList = ppI.value();
			for(QList<int>::iterator ppJ = rList.begin(); ppJ != rList.end() && outputRate < 0; ppJ++)
			{
				int rate = *ppJ;
				if(setFrequency((void *)aSession, rate))
				{
					outputRate = rate;
				}
			}
		}
		outputRate = static_cast<int>(aSession.sampleRate);
	}
	else
	{
		printError("startPlaybackWithFrequency", "Failed to get audio session for app");
	}
	return outputRate;
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
