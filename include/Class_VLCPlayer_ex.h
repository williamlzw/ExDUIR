#pragma once
#include "help_ex.h"
#include "vlc/vlc.h"
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

void _vlcplayer_register();
LRESULT CALLBACK _vlcplayer_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

// 媒体播放对象
#define VLCPLAYER_LONG_OBJ 0
// 媒体帧图像句柄
#define VLCPLAYER_LONG_IMG 1

class VLCPlayer : public IUnknown
{
public:
	static HRESULT CreateInstance(VLCPlayer** ppPlayer, HEXOBJ hObj);
	STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	void Play(const WCHAR* fileName = NULL, int type = 0);
	void Pause();
	void Stop();
	void DoResume();
	FLOAT GetMediaPosition();
	void SetMediaPosition(FLOAT pos);
	void SetMediaTime(INT64 timeMs);
	INT64 GetMediaTime();
	INT64 GetMediaLength();
	INT GetMediaVolume();
	BOOL SetMediaVolume(INT volume);
	BOOL SetMediaRate(FLOAT rate);
	FLOAT GetMediaRate();
	~VLCPlayer();

protected:
	enum State
	{
		Opening,
		Stopped,
		Playing,
		Paused,
		End,
		Resume
	};
	VLCPlayer(HANDLE hEvent);
	void ClearUp();
	unsigned int setupFormat(char* chroma, unsigned int* width, unsigned int* height,
							   unsigned int* pitches, unsigned int* lines);
	void SetFileName(const WCHAR* pwszFileName, int type);
	void SetState(State state);
	
	State m_state = Stopped;
	UINT32	m_width = 0;
	UINT32 	m_height = 0;
	void* m_pixelBuff = nullptr;
	INT m_numPlanes;
	CRITICAL_SECTION	m_critsec;
	HANDLE	m_hEvent;
	LONG	m_nRefCount;
	const WCHAR* m_fileName;
	libvlc_instance_t* m_libVlc = NULL;
	libvlc_media_player_t* m_mediaPlayer = NULL;
	HEXOBJ m_obj = NULL;


private:
	//VLC callback functions
	static unsigned int vlcVideoFormatCallback(void** object, char* chroma, unsigned int* width, unsigned int* height,
							   unsigned int* pitches, unsigned int* lines);
	static void* vlcVideoLockCallBack(void* object, void** planes);
	static void vlcVideoUnlockCallback(void* object, void* picture, void* const* planes);
	static void vlcVideoDisplayCallback(void* object, void* picture);
	static void vlcVideoEventCallback(const libvlc_event_t* event, void* object);

	void setupPlanes(char* chroma, unsigned int* width, unsigned int* height,
				 unsigned int* pitches, unsigned int* lines);
	void setupBuffers();
	void cleanupBuffers();
	void paintFrame(HEXOBJ hObj);
	void playerEvent(const libvlc_event_t* event);
	void updateTexture(void* picture, void* const* planes);
};