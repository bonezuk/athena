//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_AUDIOIO_WASAPIVOLUMEEVENTS_H
#define __OMEGA_AUDIOIO_WASAPIVOLUMEEVENTS_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/WasAPICOMInterfaces.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT WasAPISharedVolumeEvents : public IAudioSessionEvents
{
	public:
		WasAPISharedVolumeEvents();
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
		
		virtual void logEvent(const char *strR, const char *eventStr) const;
		virtual void logEvent(const char *strR, const QString& eventStr) const;
};

//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT WasAPIExclusiveVolumeEvents : public IAudioEndpointVolumeCallback
{
	public:
		WasAPIExclusiveVolumeEvents();
		virtual ~WasAPIExclusiveVolumeEvents();

	    // IUnknown methods.
		STDMETHODIMP_(ULONG) AddRef();
		STDMETHODIMP_(ULONG) Release();
		STDMETHODIMP QueryInterface(REFIID iid, VOID **ppvInterface);

		// IAudioEndpointVolumeCallback method
		STDMETHODIMP OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);
		
	private:
		LONG m_cRef;
		
		virtual void logEvent(const char *strR, const QString& eventStr) const;
};

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
