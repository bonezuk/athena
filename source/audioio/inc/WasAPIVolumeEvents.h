//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_AUDIOIO_WASAPIVOLUMEEVENTS_H
#define __OMEGA_AUDIOIO_WASAPIVOLUMEEVENTS_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/WasAPICOMInterfaces.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

// GUID to volume controls identifying the volume source change
DEFINE_GUID(GUID_OMEGA_VOLUME_EVENTS, 0x59aedf6bL, 0x66d0, 0x4539, 0x90,0xf7, 0xa4, 0x44, 0x92, 0x29, 0x32, 0xf3);

//-------------------------------------------------------------------------------------------

typedef void(*VolumeChangeNotifier)(LPVOID, sample_t);

//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT WasAPISharedVolumeEvents : public IAudioSessionEvents
{
	public:
		WasAPISharedVolumeEvents(VolumeChangeNotifier pNotifier, LPVOID pVInstance);
		virtual ~WasAPISharedVolumeEvents();
		
	    // IUnknown methods.
		STDMETHODIMP_(ULONG) AddRef();
		STDMETHODIMP_(ULONG) Release();
		STDMETHODIMP QueryInterface(REFIID iid, VOID **ppvInterface);
		
		// IAudioSessionEvents methods
		STDMETHODIMP OnDisplayNameChanged(LPCWSTR NewDisplayName, LPCGUID EventContext);
		STDMETHODIMP OnIconPathChanged(LPCWSTR NewIconPath, LPCGUID EventContext);
		STDMETHODIMP OnSimpleVolumeChanged(float NewVolume, BOOL NewMute, LPCGUID EventContext);
		STDMETHODIMP OnChannelVolumeChanged(DWORD ChannelCount, float NewChannelVolumeArray[], DWORD ChangedChannel, LPCGUID EventContext);
		STDMETHODIMP OnGroupingParamChanged(LPCGUID NewGroupingParam, LPCGUID EventContext);
		STDMETHODIMP OnStateChanged(AudioSessionState NewState);
		STDMETHODIMP OnSessionDisconnected(AudioSessionDisconnectReason DisconnectReason);

	private:
		LONG m_cRef;
		VolumeChangeNotifier m_pNotifier;
		LPVOID m_pVInstance;
		
		virtual void logEvent(const char *strR, const char *eventStr) const;
		virtual void logEvent(const char *strR, const QString& eventStr) const;
};

//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT WasAPIExclusiveVolumeEvents : public IAudioEndpointVolumeCallback
{
	public:
		WasAPIExclusiveVolumeEvents(VolumeChangeNotifier pNotifier, LPVOID pVInstance);
		virtual ~WasAPIExclusiveVolumeEvents();

	    // IUnknown methods.
		STDMETHODIMP_(ULONG) AddRef();
		STDMETHODIMP_(ULONG) Release();
		STDMETHODIMP QueryInterface(REFIID iid, VOID **ppvInterface);

		// IAudioEndpointVolumeCallback method
		STDMETHODIMP OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);
		
	private:
		LONG m_cRef;
		VolumeChangeNotifier m_pNotifier;
		LPVOID m_pVInstance;
		
		virtual void logEvent(const char *strR, const QString& eventStr) const;
};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
