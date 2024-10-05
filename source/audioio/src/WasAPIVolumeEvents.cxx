#include "audioio/inc/WasAPIVolumeEvents.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
//-------------------------------------------------------------------------------------------
// WasAPISharedVolumeEvents
//-------------------------------------------------------------------------------------------

WasAPISharedVolumeEvents::WasAPISharedVolumeEvents() : m_cRef(1)
{}

//-------------------------------------------------------------------------------------------

WasAPISharedVolumeEvents::~WasAPISharedVolumeEvents()
{}

//-------------------------------------------------------------------------------------------

STDMETHODIMP_(ULONG) WasAPISharedVolumeEvents::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

//-------------------------------------------------------------------------------------------

STDMETHODIMP_(ULONG) WasAPISharedVolumeEvents::Release()
{
    LONG c = InterlockedDecrement( &m_cRef );
    if(c == 0)
    {
        delete this;
    }
    return c;
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPISharedVolumeEvents::QueryInterface(REFIID riid, VOID **ppvInterface)
{
	HRESULT hRes = S_OK;
	
	if(riid == IID_IUnknown)
	{
		AddRef();
		*ppvInterface = reinterpret_cast<IUnknown *>(this);
	}
	else if(riid == __uuidof(IAudioSessionEvents))
	{
		AddRef();
		*ppvInterface = reinterpret_cast<IAudioSessionEvents *>(this);
	}
	else
	{
		*ppvInterface = NULL;
		hRes = E_NOINTERFACE;
	}
	return hRes;
}

//-------------------------------------------------------------------------------------------

void WasAPISharedVolumeEvents::logEvent(const char *strR, const char *eventStr) const
{
	common::Log::g_Log << "WasAPISharedVolumeEvents::" << strR << " - " << eventStr << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void WasAPISharedVolumeEvents::logEvent(const char *strR, const QString& eventStr) const
{
	logEvent(strR, eventStr.toUtf8().constData());
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPISharedVolumeEvents::OnDisplayNameChanged(LPCWSTR NewDisplayName, LPCGUID EventContext)
{
	QString displayName = QString::fromWCharArray(reinterpret_cast<const wchar_t *>(NewDisplayName));
	logEvent("OnDisplayNameChanged", displayName);
	return S_OK;
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPISharedVolumeEvents::OnIconPathChanged(LPCWSTR NewIconPath, LPCGUID EventContext)
{
	return S_OK;
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPISharedVolumeEvents::OnSimpleVolumeChanged(float NewVolume, BOOL NewMute, LPCGUID EventContext)
{
	// TODO: Implement callback for volume change
	return S_OK;
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPISharedVolumeEvents::OnChannelVolumeChanged(DWORD ChannelCount, float NewChannelVolumeArray[], DWORD ChangedChannel, LPCGUID EventContext)
{
	return S_OK;
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPISharedVolumeEvents::OnGroupingParamChanged(LPCGUID NewGroupingParam, LPCGUID EventContext)
{
	return S_OK;
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPISharedVolumeEvents::OnStateChanged(AudioSessionState newState)
{
	QString state;
	
	switch(newState)
	{
		case AudioSessionStateActive:
			state = "active";
			break;
		case AudioSessionStateInactive:
			state = "inactive";
			break;
		default:
			state = "unknown";
			break;
	}
	QString eventStr("New session state = %1").arg(state);
	logEvent("OnStateChanged", eventStr);
	return S_OK;
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPISharedVolumeEvents::OnSessionDisconnected(AudioSessionDisconnectReason DisconnectReason)
{
	QString reason;

	switch (DisconnectReason)
	{
		case DisconnectReasonDeviceRemoval:
			reason = "device removed";
			break;
		case DisconnectReasonServerShutdown:
			reason = "server shut down";
			break;
		case DisconnectReasonFormatChanged:
			reason = "format changed";
			break;
		case DisconnectReasonSessionLogoff:
			reason = "user logged off";
			break;
		case DisconnectReasonSessionDisconnected:
			reason = "session disconnected";
			break;
		case DisconnectReasonExclusiveModeOverride:
			reason = "exclusive-mode override";
			break;
		default:
			reason = "unknown";
			break;
	}
	QString eventStr = QString("Audio session disconnected (reason %1)").arg(reason);
	logEvent("OnSessionDisconnected", eventStr);
	return S_OK;
}

//-------------------------------------------------------------------------------------------
// WasAPIExclusiveVolumeEvents
//-------------------------------------------------------------------------------------------

WasAPIExclusiveVolumeEvents::WasAPIExclusiveVolumeEvents() : m_cRef(1)
{}

//-------------------------------------------------------------------------------------------

WasAPIExclusiveVolumeEvents::~WasAPIExclusiveVolumeEvents()
{}

//-------------------------------------------------------------------------------------------

STDMETHODIMP_(ULONG) WasAPIExclusiveVolumeEvents::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

//-------------------------------------------------------------------------------------------

STDMETHODIMP_(ULONG) WasAPIExclusiveVolumeEvents::Release()
{
    LONG c = InterlockedDecrement( &m_cRef );
    if(c == 0)
    {
        delete this;
    }
    return c;
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPIExclusiveVolumeEvents::QueryInterface(REFIID riid, VOID **ppvInterface)
{
	HRESULT hRes = S_OK;
	
	if(riid == IID_IUnknown)
	{
		AddRef();
		*ppvInterface = reinterpret_cast<IUnknown *>(this);
	}
	else if(riid == __uuidof(IAudioEndpointVolumeCallback))
	{
		AddRef();
		*ppvInterface = reinterpret_cast<IAudioEndpointVolumeCallback *>(this);
	}
	else
	{
		*ppvInterface = NULL;
		hRes = E_NOINTERFACE;
	}
	return hRes;
}

//-------------------------------------------------------------------------------------------

void WasAPIExclusiveVolumeEvents::logEvent(const char *strR, const QString& eventStr) const
{
	common::Log::g_Log << "WasAPISharedVolumeEvents::" << strR << " - " << eventStr << common::c_endl;
}

//-------------------------------------------------------------------------------------------

HRESULT WasAPIExclusiveVolumeEvents::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
	//TODO: Implement endpoint volume notification back to app.
	QString eventStr = QString("Volume = %1, isMute=%2").arg(pNotify->fMasterVolume).arg(pNotify->bMuted);
	logEvent("OnNotify", eventStr);
	return S_OK;
}

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------
