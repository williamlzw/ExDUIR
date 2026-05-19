#include "stdafx.h"
#ifdef VCL_PLAYER

// ==================== 注册与消息处理 ====================
void _vlcplayer_register() {
	WCHAR wzCls[] = L"VLCPlayer";
	Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE,
		OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
		DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER,
		1 * sizeof(size_t), NULL, 0, _vlcplayer_proc);
}
LRESULT CALLBACK _vlcplayer_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CREATE) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_MemAlloc(sizeof(EX_VLCPLAYER_DATA));
		memset(pData, 0, sizeof(EX_VLCPLAYER_DATA));
		InitializeCriticalSection(&pData->critsec);
		pData->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		pData->hObj = hObj;
		pData->fRate = 1.0f;
		pData->nVolume = 100;
		pData->nCurrentTime = 0;
		pData->nDuration = 0;
		const char* argv[] = { "--no-xlib", "--network-caching=500", "--no-video-title-show", "--disable-screensaver" };
		int argc = sizeof(argv) / sizeof(*argv);
		pData->libVlc = libvlc_new(argc, argv);
		Ex_ObjSetLong(hObj, VLCPLAYER_LONG_DATA, (LONG_PTR)pData);
	}
	else if (uMsg == WM_DESTROY) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData) {
			// 停止定时器

			Ex_ObjKillTimer(hObj);
			_vlcplayer_cleanup(pData);
			if (pData->libVlc) libvlc_release(pData->libVlc);
			DeleteCriticalSection(&pData->critsec);
			CloseHandle(pData->hEvent);
			// cleanup 已释放 hCoverImg 和 hCurrentFrame，此处无需再释放
			Ex_MemFree(pData);
		}
	}
	else if (uMsg == WM_PAINT) {
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps)) {
			_vlcplayer_paint(hObj, &ps);
			Ex_ObjEndPaint(hObj, &ps);
		}
	}
	else if (uMsg == WM_MOUSEMOVE) {
		_vlcplayer_onmousemove(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		_vlcplayer_onlbuttondown(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	else if (uMsg == WM_LBUTTONUP) {
		_vlcplayer_onlbuttonup(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	else if (uMsg == WM_TIMER) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData && pData->mediaPlayer && pData->bIsPlaying && !pData->bDraggingProgress) {
			INT64 newTime = libvlc_media_player_get_time(pData->mediaPlayer);
			if (newTime >= 0) pData->nCurrentTime = newTime;
			INT64 newDur = libvlc_media_player_get_length(pData->mediaPlayer);
			if (newDur > 0) pData->nDuration = newDur;
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	// ===== 外部消息接口 =====
	else if (uMsg == VLCPLAYER_MESSAGE_STATE_PLAY) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData) { _vlcplayer_setfilename(pData, (PCWSTR)lParam, 0); }
	}
	else if (uMsg == VLCPLAYER_MESSAGE_STATE_PLAYFROMURL) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData) { _vlcplayer_setfilename(pData, (PCWSTR)lParam, 1); }
	}
	else if (uMsg == VLCPLAYER_MESSAGE_STATE_PAUSE) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData && pData->mediaPlayer) {
			libvlc_media_player_set_pause(pData->mediaPlayer, 1);
			pData->bIsPaused = TRUE; pData->bIsPlaying = FALSE;
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (uMsg == VLCPLAYER_MESSAGE_STATE_RESUME) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData && pData->mediaPlayer) {
			libvlc_media_player_set_pause(pData->mediaPlayer, 0);
			pData->bIsPaused = FALSE; pData->bIsPlaying = TRUE;
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (uMsg == VLCPLAYER_MESSAGE_STATE_STOP) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData && pData->mediaPlayer) {
			libvlc_media_player_stop(pData->mediaPlayer);
			pData->bIsPlaying = FALSE; pData->bIsPaused = FALSE;
			pData->nCurrentTime = 0;
			Ex_ObjKillTimer(hObj);
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (uMsg == VLCPLAYER_MESSAGE_GET_RATE) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData) return (LRESULT)pData->fRate;
	}
	else if (uMsg == VLCPLAYER_MESSAGE_SET_RATE) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData && pData->mediaPlayer && lParam > 0 && lParam <= 3) {
			pData->fRate = (FLOAT)lParam;
			libvlc_media_player_set_rate(pData->mediaPlayer, pData->fRate);
		}
	}
	else if (uMsg == VLCPLAYER_MESSAGE_GET_VOLUME) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData) return pData->nVolume;
	}
	else if (uMsg == VLCPLAYER_MESSAGE_SET_VOLUME) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData) {
			pData->nVolume = (INT)lParam;
			if (pData->mediaPlayer) libvlc_audio_set_volume(pData->mediaPlayer, pData->nVolume);
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (uMsg == VLCPLAYER_MESSAGE_GET_MEDIATIME) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData) return pData->nCurrentTime;
	}
	else if (uMsg == VLCPLAYER_MESSAGE_SET_MEDIATIME) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData && pData->mediaPlayer) {
			libvlc_media_player_set_time(pData->mediaPlayer, (INT64)lParam);
			pData->nCurrentTime = (INT64)lParam;
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (uMsg == VLCPLAYER_MESSAGE_GET_DURATION) {
		EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
		if (pData) return pData->nDuration;
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
// ==================== 播放器核心逻辑 ====================
void _vlcplayer_cleanup(EX_VLCPLAYER_DATA* pData) {
	if (pData->mediaPlayer) {
		libvlc_media_player_stop(pData->mediaPlayer);
		libvlc_media_player_release(pData->mediaPlayer);
		pData->mediaPlayer = NULL;
	}
	if (pData->pixelBuff) {
		free(pData->pixelBuff);
		pData->pixelBuff = nullptr;
	}
	// ★ 使用临界区保护，防止 VLC 线程同时操作图片
	EnterCriticalSection(&pData->critsec);
	if (pData->hCurrentFrame) {
		_img_destroy(pData->hCurrentFrame);
		pData->hCurrentFrame = NULL;
	}
	if (pData->hCoverImg) {
		_img_destroy(pData->hCoverImg);
		pData->hCoverImg = NULL;
	}
	LeaveCriticalSection(&pData->critsec);
	pData->bHasCover = FALSE;
	pData->bIsPlaying = FALSE;
	pData->bIsPaused = FALSE;
	pData->nCurrentTime = 0;
	pData->nDuration = 0;
}
void _vlcplayer_setfilename(EX_VLCPLAYER_DATA* pData, const WCHAR* pwszFileName, int type) {
	_vlcplayer_cleanup(pData);
	libvlc_media_t* m;
	if (type == 0) {
		m = libvlc_media_new_path(pData->libVlc, Ex_W2U(pwszFileName).c_str());
	}
	else {
		m = libvlc_media_new_location(pData->libVlc, Ex_W2U(pwszFileName).c_str());
	}
	pData->mediaPlayer = libvlc_media_player_new_from_media(m);
	libvlc_media_release(m);
	libvlc_video_set_format_callbacks(pData->mediaPlayer, _vlcplayer_format_cb, NULL);
	libvlc_video_set_callbacks(pData->mediaPlayer, _vlcplayer_lock_cb, _vlcplayer_unlock_cb, _vlcplayer_display_cb, pData);
	libvlc_event_attach(libvlc_media_player_event_manager(pData->mediaPlayer), libvlc_MediaPlayerPlaying, _vlcplayer_event_cb, pData);
	libvlc_event_attach(libvlc_media_player_event_manager(pData->mediaPlayer), libvlc_MediaPlayerPaused, _vlcplayer_event_cb, pData);
	libvlc_event_attach(libvlc_media_player_event_manager(pData->mediaPlayer), libvlc_MediaPlayerStopped, _vlcplayer_event_cb, pData);
	libvlc_event_attach(libvlc_media_player_event_manager(pData->mediaPlayer), libvlc_MediaPlayerEndReached, _vlcplayer_event_cb, pData);
	libvlc_event_attach(libvlc_media_player_event_manager(pData->mediaPlayer), libvlc_MediaPlayerLengthChanged, _vlcplayer_event_cb, pData);
	libvlc_audio_set_volume(pData->mediaPlayer, pData->nVolume);
	libvlc_media_player_set_rate(pData->mediaPlayer, pData->fRate);
	libvlc_media_player_play(pData->mediaPlayer);
}
void _vlcplayer_format_time(INT64 ms, WCHAR* buf, INT bufSize) {
	if (ms < 0) ms = 0;
	INT totalSec = (INT)(ms / 1000);
	INT h = totalSec / 3600;
	INT m = (totalSec % 3600) / 60;
	INT s = totalSec % 60;
	if (h > 0) {
		swprintf_s(buf, bufSize, L"%d:%02d:%02d", h, m, s);
	}
	else {
		swprintf_s(buf, bufSize, L"%02d:%02d", m, s);
	}
}
// ==================== VLC 回调函数 ====================
unsigned int _vlcplayer_format_cb(void** object, char* chroma, unsigned int* width, unsigned int* height, unsigned int* pitches, unsigned int* lines) {
	EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)*object;
	strcpy(chroma, "BGRA");
	*pitches = *width * 4;
	*lines = *height;
	pData->width = *width;
	pData->height = *height;
	if (pData->pixelBuff) free(pData->pixelBuff);
	pData->pixelBuff = calloc(1, *width * *height * 4 + 1);
	return 1;
}
void* _vlcplayer_lock_cb(void* object, void** planes) {
	EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)object;
	EnterCriticalSection(&pData->critsec);
	*planes = pData->pixelBuff;
	return NULL;
}
void _vlcplayer_unlock_cb(void* object, void* picture, void* const* planes) {
	EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)object;
	// ★ 注意：此时 critsec 已由 lock_cb 持有，无需再进入
	HEXIMAGE img = NULL;
	_img_createfrompngbits2(pData->width, pData->height, (BYTE*)*planes, &img);
	// 第一帧作为封面
	if (!pData->bHasCover && img) {
		HEXIMAGE hCover = NULL;
		_img_copy(img, &hCover);
		pData->hCoverImg = hCover;
		pData->bHasCover = TRUE;
	}
	// ★ 不在 VLC 线程中销毁旧帧！将旧帧移到 hPendingFree，由 UI 线程的 paint 负责销毁
	if (pData->hPendingFree) {
		_img_destroy(pData->hPendingFree);
	}
	pData->hPendingFree = pData->hCurrentFrame;
	pData->hCurrentFrame = img;
	LeaveCriticalSection(&pData->critsec);
}
void _vlcplayer_display_cb(void* object, void* picture) {
	EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)object;
	Ex_ObjInvalidateRect(pData->hObj, 0);
}
void _vlcplayer_event_cb(const libvlc_event_t* event, void* object) {
	EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)object;
	HEXOBJ hObj = pData->hObj;
	switch (event->type) {
	case libvlc_MediaPlayerPlaying:
		pData->bIsPlaying = TRUE;
		pData->bIsPaused = FALSE;
		pData->nDuration = libvlc_media_player_get_length(pData->mediaPlayer);
		// ★ 启动定时器刷新进度
		Ex_ObjSetTimer(hObj, 100);
		Ex_ObjDispatchNotify(hObj, VLCPLAYER_EVENT_PLAYING, 0, 0);
		break;
	case libvlc_MediaPlayerPaused:
		pData->bIsPlaying = FALSE;
		pData->bIsPaused = TRUE;
		Ex_ObjKillTimer(hObj);
		Ex_ObjDispatchNotify(hObj, VLCPLAYER_EVENT_PAUSED, 0, 0);
		break;
	case libvlc_MediaPlayerStopped:
		pData->bIsPlaying = FALSE;
		pData->bIsPaused = FALSE;
		Ex_ObjKillTimer(hObj);
		Ex_ObjDispatchNotify(hObj, VLCPLAYER_EVENT_STOPPED, 0, 0);
		break;
	case libvlc_MediaPlayerEndReached:
		pData->bIsPlaying = FALSE;
		pData->bIsPaused = FALSE;
		Ex_ObjKillTimer(hObj);
		Ex_ObjDispatchNotify(hObj, VLCPLAYER_EVENT_END, 0, 0);
		break;
	case libvlc_MediaPlayerLengthChanged:
		pData->nDuration = libvlc_media_player_get_length(pData->mediaPlayer);
		break;
	}
	Ex_ObjInvalidateRect(hObj, 0);
}
// ==================== UI 绘制与交互 ====================
void _vlcplayer_paint(HEXOBJ hObj, EX_PAINTSTRUCT* ps) {
	EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
	if (!pData) return;

	INT W = ps->uWidth;
	INT H = ps->uHeight;
	INT ctrlY = H - VLC_CTRL_HEIGHT;

	// 1. 绘制视频/封面
	EnterCriticalSection(&pData->critsec);
	HEXIMAGE hDrawImg = pData->bIsPlaying || pData->bIsPaused ? pData->hCurrentFrame : pData->hCoverImg;
	if (hDrawImg > 0) {
		_canvas_drawimagerect(ps->hCanvas, hDrawImg, 0, 0, W, ctrlY, 255);
	}
	else {
		HEXBRUSH hBrushBg = _brush_create(ExARGB(0, 0, 0, 255));
		_canvas_fillrect(ps->hCanvas, hBrushBg, 0, 0, W, ctrlY);
		_brush_destroy(hBrushBg);
	}
	if (pData->hPendingFree) {
		_img_destroy(pData->hPendingFree);
		pData->hPendingFree = NULL;
	}
	LeaveCriticalSection(&pData->critsec);

	// 2. 绘制底部控制栏背景
	HEXBRUSH hBrushCtrlBg = _brush_create(ExARGB(20, 20, 25, 220));
	_canvas_fillrect(ps->hCanvas, hBrushCtrlBg, 0, ctrlY, W, H);
	_brush_destroy(hBrushCtrlBg);

	// 3. 绘制进度条
	FLOAT progressY = (FLOAT)ctrlY;
	FLOAT progressPos = 0.0f;
	if (pData->nDuration > 0 && pData->nCurrentTime > 0) {
		progressPos = (FLOAT)pData->nCurrentTime / (FLOAT)pData->nDuration * W;
	}
	HEXBRUSH hBrushTrack = _brush_create(ExARGB(60, 60, 70, 255));
	HEXBRUSH hBrushFill = _brush_create(ExARGB(0, 150, 255, 255));
	_canvas_fillrect(ps->hCanvas, hBrushTrack, 0, progressY, W, progressY + VLC_PROGRESS_H);
	_canvas_fillrect(ps->hCanvas, hBrushFill, 0, progressY, progressPos, progressY + VLC_PROGRESS_H);

	FLOAT sliderX = progressPos;
	EXARGB sliderColor = pData->bHoverProgress || pData->bDraggingProgress ? ExARGB(255, 255, 255, 255) : ExARGB(200, 200, 200, 255);
	HEXBRUSH hBrushSlider = _brush_create(sliderColor);
	_canvas_fillellipse(ps->hCanvas, hBrushSlider, sliderX, progressY + VLC_PROGRESS_H / 2.0f, 4.0f, 4.0f);
	_brush_destroy(hBrushTrack); _brush_destroy(hBrushFill); _brush_destroy(hBrushSlider);

	// 4. 绘制按钮行 (相对布局计算)
	FLOAT btnY = ctrlY + VLC_PROGRESS_H + (VLC_CTRL_HEIGHT - VLC_PROGRESS_H) / 2.0f; // 垂直居中
	FLOAT btnR = 7.0f; // 缩小按钮图标半径
	HEXBRUSH hBrushBtnIcon = _brush_create(ExARGB(220, 220, 220, 255));
	HEXFONT hFontBtn = _font_createfromfamily(L"微软雅黑", 10, 0); // 缩小字体

	// 播放/暂停 (x=20)
	FLOAT playX = 20.0f;
	if (pData->bIsPlaying) {
		_canvas_drawline(ps->hCanvas, hBrushBtnIcon, playX - 3, btnY - btnR, playX - 3, btnY + btnR, 2.0f, 0);
		_canvas_drawline(ps->hCanvas, hBrushBtnIcon, playX + 3, btnY - btnR, playX + 3, btnY + btnR, 2.0f, 0);
	}
	else {
		POINTF pts[3] = { {playX - 5, btnY - btnR}, {playX - 5, btnY + btnR}, {playX + 6, btnY} };
		HEXPATH hPath; _path_create(PATH_FLAG_DISABLESCALE, &hPath); _path_open(hPath);
		_path_beginfigure2(hPath, pts[0].x, pts[0].y);
		_path_addline(hPath, pts[0].x, pts[0].y, pts[1].x, pts[1].y);
		_path_addline(hPath, pts[1].x, pts[1].y, pts[2].x, pts[2].y);
		_path_endfigure(hPath, TRUE); _path_close(hPath);
		_canvas_fillpath(ps->hCanvas, hPath, hBrushBtnIcon); _path_destroy(hPath);
	}

	// 停止 (x=45)
	FLOAT stopX = 45.0f;
	_canvas_fillrect(ps->hCanvas, hBrushBtnIcon, stopX - 5, btnY - 5, stopX + 5, btnY + 5);

	// 快进+10s (x=70)
	FLOAT ffX = 70.0f;
	POINTF ptsFF[3] = { {ffX - 5, btnY - btnR}, {ffX - 5, btnY + btnR}, {ffX + 4, btnY} };
	HEXPATH hPathFF; _path_create(PATH_FLAG_DISABLESCALE, &hPathFF); _path_open(hPathFF);
	_path_beginfigure2(hPathFF, ptsFF[0].x, ptsFF[0].y);
	_path_addline(hPathFF, ptsFF[0].x, ptsFF[0].y, ptsFF[1].x, ptsFF[1].y);
	_path_addline(hPathFF, ptsFF[1].x, ptsFF[1].y, ptsFF[2].x, ptsFF[2].y);
	_path_endfigure(hPathFF, TRUE); _path_close(hPathFF);
	_canvas_fillpath(ps->hCanvas, hPathFF, hBrushBtnIcon); _path_destroy(hPathFF);
	_canvas_drawline(ps->hCanvas, hBrushBtnIcon, ffX + 5, btnY - btnR, ffX + 5, btnY + btnR, 2.0f, 0);

	// 5. 音量滑块 (相对布局：x从 95 到 距离右边110处)
	FLOAT volX = 95.0f;
	FLOAT volEndX = (FLOAT)W - 110.0f;
	FLOAT volW = volEndX - volX;
	if (volW < 20.0f) volW = 20.0f; // 保证最小宽度
	FLOAT volY = btnY;
	FLOAT volPos = volX + (FLOAT)pData->nVolume / 100.0f * volW;

	HEXBRUSH hBrushVolFill = _brush_create(ExARGB(0, 150, 255, 255));
	_canvas_drawline(ps->hCanvas, hBrushBtnIcon, volX, volY, volX + volW, volY, 2.0f, 0);
	_canvas_drawline(ps->hCanvas, hBrushVolFill, volX, volY, volPos, volY, 2.0f, 0);

	EXARGB volSliderColor = pData->bHoverVolume || pData->bDraggingVolume ? ExARGB(255, 255, 255, 255) : ExARGB(200, 200, 200, 255);
	HEXBRUSH hBrushVolSlider = _brush_create(volSliderColor);
	_canvas_fillellipse(ps->hCanvas, hBrushVolSlider, volPos, volY, 4.0f, 4.0f);
	_brush_destroy(hBrushVolSlider); _brush_destroy(hBrushVolFill);

	// 6. 时间文本 (相对布局：右侧 100 宽度)
	WCHAR timeStr[64]; WCHAR curStr[32]; WCHAR durStr[32];
	_vlcplayer_format_time(pData->nCurrentTime, curStr, 32);
	_vlcplayer_format_time(pData->nDuration, durStr, 32);
	swprintf_s(timeStr, L"%s / %s", curStr, durStr);
	_canvas_drawtext(ps->hCanvas, hFontBtn, ExARGB(200, 200, 200, 255), timeStr, -1, DT_RIGHT | DT_VCENTER, W - 105, ctrlY + VLC_PROGRESS_H, W - 5, ctrlY + VLC_CTRL_HEIGHT);

	_brush_destroy(hBrushBtnIcon);
	_font_destroy(hFontBtn);
}
void _vlcplayer_onmousemove(HEXOBJ hObj, INT x, INT y) {
	EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
	if (!pData) return;

	RECT rc;
	Ex_ObjGetClientRect(hObj, &rc);
	INT width = rc.right - rc.left;
	INT height = rc.bottom - rc.top;
	INT ctrlY = height - VLC_CTRL_HEIGHT;
	INT btnY = ctrlY + VLC_PROGRESS_H + (VLC_CTRL_HEIGHT - VLC_PROGRESS_H) / 2;

	BOOL needUpdate = FALSE;

	if (pData->bDraggingProgress) {
		FLOAT pos = (FLOAT)x / (FLOAT)width;
		pos = __max(0.0f, __min(1.0f, pos));
		if (pData->nDuration > 0) pData->nCurrentTime = (INT64)(pos * pData->nDuration);
		needUpdate = TRUE;
	}
	else if (pData->bDraggingVolume) {
		FLOAT volX = 95.0f;
		FLOAT volEndX = (FLOAT)width - 110.0f;
		FLOAT volW = volEndX - volX;
		if (volW < 20.0f) volW = 20.0f;
		FLOAT pos = ((FLOAT)x - volX) / volW;
		pos = __max(0.0f, __min(1.0f, pos));
		pData->nVolume = (INT)(pos * 100);
		if (pData->mediaPlayer) libvlc_audio_set_volume(pData->mediaPlayer, pData->nVolume);
		needUpdate = TRUE;
	}
	else {
		BOOL newHoverProg = (y >= ctrlY && y < ctrlY + VLC_PROGRESS_H);
		BOOL newHoverVol = (y >= btnY - 8 && y <= btnY + 8 && x >= 95 && x <= (width - 110));
		if (pData->bHoverProgress != newHoverProg || pData->bHoverVolume != newHoverVol) {
			pData->bHoverProgress = newHoverProg;
			pData->bHoverVolume = newHoverVol;
			needUpdate = TRUE;
		}
	}
	if (needUpdate) Ex_ObjInvalidateRect(hObj, 0);
}
void _vlcplayer_onlbuttondown(HEXOBJ hObj, INT x, INT y) {
	EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
	if (!pData) return;

	RECT rc;
	Ex_ObjGetClientRect(hObj, &rc);
	INT height = rc.bottom - rc.top;
	INT ctrlY = height - VLC_CTRL_HEIGHT;
	INT btnY = ctrlY + VLC_PROGRESS_H + (VLC_CTRL_HEIGHT - VLC_PROGRESS_H) / 2;

	// 进度条拖拽
	if (y >= ctrlY && y < ctrlY + VLC_PROGRESS_H) {
		pData->bDraggingProgress = TRUE;
		_vlcplayer_onmousemove(hObj, x, y);
		return;
	}

	// 音量条拖拽
	if (y >= btnY - 8 && y <= btnY + 8 && x >= 95 && x <= (rc.right - 110)) {
		pData->bDraggingVolume = TRUE;
		_vlcplayer_onmousemove(hObj, x, y);
		return;
	}

	// 按钮点击检测 (缩小了按钮碰撞体积)
	INT halfBtn = VLC_BTN_SIZE / 2;
	if (y >= btnY - halfBtn && y <= btnY + halfBtn) {
		// 播放/暂停 (x=20)
		if (x >= 20 - halfBtn && x <= 20 + halfBtn) {
			if (pData->bIsPlaying) {
				Ex_ObjSendMessage(hObj, VLCPLAYER_MESSAGE_STATE_PAUSE, 0, 0);
			}
			else if (pData->bIsPaused) {
				Ex_ObjSendMessage(hObj, VLCPLAYER_MESSAGE_STATE_RESUME, 0, 0);
			}
		}
		// 停止 (x=45)
		else if (x >= 45 - halfBtn && x <= 45 + halfBtn) {
			Ex_ObjSendMessage(hObj, VLCPLAYER_MESSAGE_STATE_STOP, 0, 0);
		}
		// 快进 +10s (x=70)
		else if (x >= 70 - halfBtn && x <= 70 + halfBtn) {
			if (pData->mediaPlayer) {
				INT64 newTime = pData->nCurrentTime + 10000;
				if (newTime > pData->nDuration) newTime = pData->nDuration;
				Ex_ObjSendMessage(hObj, VLCPLAYER_MESSAGE_SET_MEDIATIME, 0, (LPARAM)newTime);
			}
		}
	}
}
void _vlcplayer_onlbuttonup(HEXOBJ hObj, INT x, INT y) {
	EX_VLCPLAYER_DATA* pData = (EX_VLCPLAYER_DATA*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_DATA);
	if (!pData) return;
	if (pData->bDraggingProgress) {
		pData->bDraggingProgress = FALSE;
		ReleaseCapture();
		// ★ 不在临界区内调用 VLC API，避免死锁
		if (pData->mediaPlayer && pData->nDuration > 0) {
			libvlc_media_player_set_time(pData->mediaPlayer, pData->nCurrentTime);
		}
	}
	if (pData->bDraggingVolume) {
		pData->bDraggingVolume = FALSE;
		ReleaseCapture();
	}
}
#endif