#pragma once
#include "help_ex.h"
#include <shlwapi.h>
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

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Mfplat.lib")  
#pragma comment(lib, "Mf.lib") 
#pragma comment(lib, "mfreadwrite.lib") 
#pragma comment(lib, "mfuuid.lib") 
#pragma comment(lib, "evr.lib")
#pragma comment(lib, "Propsys.lib")
#pragma comment ( lib, "xaudio2.lib" )

template <class T> inline void SAFE_RELEASE(T*& p){
	if(p){
		p->Release();
		p = NULL;
	}
}

using SampleEventCallback = std::function<void(DWORD dwStreamIndex, DWORD dwStreamFlags,LONGLONG llTimeStamp, IMFSample *pSample)>;

//媒体播放对象
#define MFL_OBJ 0

//消息_媒体播放状态_播放
#define MFM_STATE_PLAY 10010
//消息_媒体播放状态_暂停
#define MFM_STATE_PAUSE 10011
//消息_媒体播放状态_继续播放
#define MFM_STATE_CONTINUE 10012
//消息_媒体播放状态_停止
#define MFM_STATE_STOP 10013
// 消息_播放速率 lParam:(float)fRate
#define MFM_RATE 10014
// 消息_置播放位置 lParam:(float)sec
#define MFM_SET_POSITION 10015
// 消息_取视频时长 单位 秒
#define MFM_GET_DURATION 10016

class MFMediaPlayer : public IMFSourceReaderCallback
{
public:
	SampleEventCallback SampleEvent;

	static HRESULT CreateInstance(MFMediaPlayer **ppPlayer);
	STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP OnReadSample(HRESULT hrStatus,DWORD dwStreamIndex,DWORD dwStreamFlags,LONGLONG llTimestamp,IMFSample *pSample);

	STDMETHODIMP OnEvent(DWORD, IMFMediaEvent *)
	{				
		return S_OK;
	}

	STDMETHODIMP OnFlush(DWORD)
	{
		return S_OK;
	}

	HRESULT	Startplay(const WCHAR *pwszFileName);
	HRESULT	Endplay();
	void Pause(BOOL bPause);
	void SetPosition(int position);
	void GetDuration();
	BOOL	Isplaying();
	UINT32	GetVideoWidth() const { return m_uVideoWidth; }
	UINT32 GetVideoHeight() const { return m_uVideoHeight; }
protected:
    enum State
    {
        State_NotReady = 0,
        State_Ready,
        State_playing,
    };
    
	MFMediaPlayer(HANDLE hEvent);
	virtual ~MFMediaPlayer();

	void    NotifyError(HRESULT hr) {  }
	HRESULT ConfigureDecoderV();
	HRESULT ConfigureDecoderA();

	BOOL m_bPause = FALSE;
	BOOL m_bPause1 = FALSE;
	long	m_nRefCount;
	CRITICAL_SECTION	m_critsec;
	HANDLE	m_hEvent; 
	IMFSourceReader	*m_pReader;
	IMFSinkWriter	*m_pWriter;
	IXAudio2* pXAudio2;
	IXAudio2SourceVoice* psourceVoice;
	UINT32		m_uVideoWidth  = 0;
	UINT32 	m_uVideoHeight = 0;
};
void _mediafoundation_register();
LRESULT CALLBACK _mediafoundation_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
