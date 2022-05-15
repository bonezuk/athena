//-------------------------------------------------------------------------------------------
#if defined(OMEGA_MACOSX)
//-------------------------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <AppKit/NSHelpManager.h>

#include "player/inc/ITunesConfig.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace player
{
//-------------------------------------------------------------------------------------------

NSString *IVHomeDirectory()
{
	NSString *homeDir;
	const struct passwd *passwd = getpwnam([NSUserName() UTF8String]);

	if(passwd!=0)
	{
		const char *homeDirC = getpwnam([NSUserName() UTF8String])->pw_dir;
        homeDir = [[NSFileManager defaultManager] stringWithFileSystemRepresentation:homeDirC length:strlen(homeDirC)];
	}
	else
	{
		homeDir = nil;
	}
	return homeDir;
}

//-------------------------------------------------------------------------------------------

void registerHelpBook()
{
	NSBundle *oBundle = [NSBundle mainBundle];
	if(oBundle!=nil)
	{
		[[NSHelpManager sharedHelpManager] registerBooksInBundle:oBundle];
	}
}

//-------------------------------------------------------------------------------------------

void unregisterHelpBook()
{}

//-------------------------------------------------------------------------------------------

void showHelp()
{
	[[NSApplication sharedApplication] showHelp: nil];
}

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
