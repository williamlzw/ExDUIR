#pragma once
#ifdef VCL_PLAYER
#include "vlc/vlc.h"

void             _vlcplayer_register();
LRESULT CALLBACK _vlcplayer_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
// 媒体播放对象数据
#    define VLCPLAYER_LONG_DATA 0
// ===== 通知事件 =====

// ===== 播放器 UI 布局常量 =====
#    define VLC_CTRL_HEIGHT      40    // 缩小控制栏高度
#    define VLC_PROGRESS_H       8     // 缩小进度条高度
#    define VLC_BTN_SIZE         16    // 按钮点击区域大小
#    define VLC_SLIDER_H         10
// ===== 播放器内部数据结构 =====
typedef struct {
	// VLC 核心
	libvlc_instance_t* libVlc;
	libvlc_media_player_t* mediaPlayer;
	CRITICAL_SECTION       critsec;
	HANDLE                 hEvent;
	void* pixelBuff;
	UINT32                 width;
	UINT32                 height;
	// UI 状态
	HEXIMAGE hCoverImg;
	HEXIMAGE hCurrentFrame;
	HEXIMAGE hPendingFree;   // ★ 新增：旧帧缓存，由 UI 线程负责释放
	BOOL     bHasCover;
	BOOL  bIsPlaying;
	BOOL  bIsPaused;
	INT64 nDuration;
	INT64 nCurrentTime;
	INT   nVolume;
	FLOAT fRate;
	// UI 交互状态
	BOOL bHoverPlay;
	BOOL bHoverStop;
	BOOL bHoverFastFwd;
	BOOL bHoverRate;
	BOOL bDraggingProgress;
	BOOL bDraggingVolume;
	BOOL bHoverProgress;
	BOOL bHoverVolume;
	HEXOBJ hObj; // 自身句柄，供回调使用
} EX_VLCPLAYER_DATA;
#endif

// ==================== 辅助函数声明 ====================
void _vlcplayer_paint(HEXOBJ hObj, EX_PAINTSTRUCT* ps);
void _vlcplayer_onmousemove(HEXOBJ hObj, INT x, INT y);
void _vlcplayer_onlbuttondown(HEXOBJ hObj, INT x, INT y);
void _vlcplayer_onlbuttonup(HEXOBJ hObj, INT x, INT y);
void _vlcplayer_cleanup(EX_VLCPLAYER_DATA* pData);
void _vlcplayer_setfilename(EX_VLCPLAYER_DATA* pData, const WCHAR* pwszFileName, int type);
void _vlcplayer_format_time(INT64 ms, WCHAR* buf, INT bufSize);
// VLC 回调函数
unsigned int _vlcplayer_format_cb(void** object, char* chroma, unsigned int* width, unsigned int* height, unsigned int* pitches, unsigned int* lines);
void* _vlcplayer_lock_cb(void* object, void** planes);
void         _vlcplayer_unlock_cb(void* object, void* picture, void* const* planes);
void         _vlcplayer_display_cb(void* object, void* picture);
void         _vlcplayer_event_cb(const libvlc_event_t* event, void* object);