#pragma once
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfobjects.h >
#include <mferror.h>
#include <evr.h>
#include <mmdeviceapi.h>
#include <propvarutil.h>
#include <functional>
#include <xaudio2.h>
#include <mmdeviceapi.h>
#include "help_ex.h"

#pragma comment(lib, "Mfplat.lib")  
#pragma comment(lib, "Mf.lib") 
#pragma comment(lib, "mfreadwrite.lib") 
#pragma comment(lib, "mfuuid.lib") 
#pragma comment(lib, "evr.lib")
#pragma comment(lib, "Propsys.lib")
#pragma comment(lib, "xaudio2.lib")

template <class T> inline void SAFE_RELEASE(T*& p) {
	if (p) {
		p->Release();
		p = NULL;
	}
}

using SampleEventCallback = std::function<void(DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimeStamp, IMFSample* pSample)>;

// 媒体播放对象
#define MFL_OBJ 0
// 媒体帧图像句柄
#define MFL_IMG 1

class MFMediaPlayer : public IMFSourceReaderCallback
{
public:
	SampleEventCallback SampleEvent;

	static HRESULT CreateInstance(MFMediaPlayer** ppPlayer);
	STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample);

	STDMETHODIMP OnEvent(DWORD, IMFMediaEvent*)
	{
		return S_OK;
	}

	STDMETHODIMP OnFlush(DWORD)
	{
		return S_OK;
	}

	HRESULT	Startplay(const WCHAR* pwszFileName);
	HRESULT	Endplay();
	void Pause(BOOL bPause);
	void SetPosition(int position);
	int GetDuration();
	BOOL	Isplaying();
	UINT32	GetVideoWidth() const { return m_uVideoWidth; }
	UINT32 GetVideoHeight() const { return m_uVideoHeight; }
	~MFMediaPlayer();

protected:
	enum State
	{
		State_NotReady = 0,
		State_Ready,
		State_playing,
	};

	MFMediaPlayer(HANDLE hEvent);

	void    NotifyError(HRESULT hr) {  }
	HRESULT ConfigureDecoderV();
	HRESULT ConfigureDecoderA();

	BOOL m_bPause = FALSE;
	BOOL m_bPosition = FALSE;
	int m_bPositionc = 0;
	long	m_nRefCount;
	double m_fps;
	CRITICAL_SECTION	m_critsec;
	HANDLE	m_hEvent;
	IMFSourceReader* m_pReader = nullptr;
	IMFSinkWriter* m_pWriter = nullptr;
	IMFStreamSink* m_pStreamSink = nullptr;
	IMFPresentationClock* m_ppClock = nullptr;
	UINT32	m_uVideoWidth = 0;
	UINT32 	m_uVideoHeight = 0;
};

void _mediafoundation_register();
LRESULT CALLBACK _mediafoundation_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
