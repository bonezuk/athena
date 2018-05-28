#if !defined(OMEGA_MAC_STORE)
#import <Foundation/Foundation.h>
#import "Sparkle/Sparkle.h"
#import "Sparkle/SUUpdater.h"
#endif

#include "player/inc/AutoUpdaterMac.h"

//-------------------------------------------------------------------------------------------
#if defined(OMEGA_MACOSX)
//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(AutoUpdaterFactory,AutoUpdater, \
                            AutoUpdaterMacFactory,AutoUpdaterMac, \
                            "mac",false)

//-------------------------------------------------------------------------------------------
// AutoUpdaterMac::Private
//-------------------------------------------------------------------------------------------

class AutoUpdaterMac::Private
{
	public:
		Private();
		
		void *m_null;
#if !defined(OMEGA_MAC_STORE)
		SUUpdater *updater;
#endif
};

//-------------------------------------------------------------------------------------------

AutoUpdaterMac::Private::Private() : m_null(0)
#if !defined(OMEGA_MAC_STORE)
	,updater(nil)
#endif
{}

//-------------------------------------------------------------------------------------------
// AutoUpdaterMac
//-------------------------------------------------------------------------------------------

AutoUpdaterMac::AutoUpdaterMac() : AutoUpdater()
{
#if !defined(OMEGA_MAC_STORE)
	m_url = "https://secure101.prositehosting.co.uk/blackomega/sparkle/appcast_mac.xml";
	d = new Private;
	
	d->updater = [[SUUpdater sharedUpdater] retain];

	NSURL *u = [NSURL URLWithString:[NSString stringWithUTF8String: m_url.toUtf8().data()]];
	[d->updater setFeedURL:u];
	[d->updater checkForUpdatesInBackground];	
#endif
}

//-------------------------------------------------------------------------------------------

AutoUpdaterMac::~AutoUpdaterMac()
{
#if !defined(OMEGA_MAC_STORE)
	if(d->updater!=nil)
	{
		[d->updater release];
	}
	delete d;
#endif
}

//-------------------------------------------------------------------------------------------

void AutoUpdaterMac::checkForUpdates()
{
#if !defined(OMEGA_MAC_STORE)
	[d->updater checkForUpdates:nil];
#endif
}

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

