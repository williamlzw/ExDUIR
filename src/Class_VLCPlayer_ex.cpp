#include "stdafx.h"

#ifdef VCL_PLAYER

void _vlcplayer_register()
{
    Ex_ObjRegister(L"VLCPlayer", OBJECT_STYLE_VISIBLE,
                   OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
                   DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER, 2 * sizeof(size_t), NULL,
                   0, _vlcplayer_proc);
}

LRESULT CALLBACK _vlcplayer_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT nError = 0;

    if (uMsg == WM_CREATE) {
        VLCPlayer* pPlayer = NULL;
        VLCPlayer::CreateInstance(&pPlayer, hObj);
        Ex_ObjSetLong(hObj, VLCPLAYER_LONG_OBJ, (LONG_PTR)pPlayer);
        Ex_ObjSetLong(hObj, VLCPLAYER_LONG_IMG, 0);
    }
    else if (uMsg == WM_DESTROY) {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        pPlayer->Stop();

        Sleep(100);   // wait for endplay
        delete pPlayer;
    }
    else if (uMsg == WM_PAINT) {
        EX_PAINTSTRUCT ps{0};
        if (Ex_ObjBeginPaint(hObj, &ps)) {
            HEXIMAGE img = (HEXIMAGE)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_IMG);

            if (img > 0) {
                _canvas_drawimagerect(ps.hCanvas, img, ps.rcPaint.left, ps.rcPaint.top,
                                      ps.rcPaint.right, ps.rcPaint.bottom, 255);
                _img_destroy(img);
            }
            else {}
            Ex_ObjEndPaint(hObj, &ps);
        }
    }
    else if (uMsg == VLCPLAYER_MESSAGE_STATE_PLAY)   // 播放   lParam :文件名
    {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        pPlayer->Play((PCWSTR)lParam, 0);
    }
    else if (uMsg == VLCPLAYER_MESSAGE_STATE_PLAYFROMURL)   // 播放   lParam :url
    {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        pPlayer->Play((PCWSTR)lParam, 1);
    }
    else if (uMsg == VLCPLAYER_MESSAGE_STATE_PAUSE)   // 暂停
    {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        pPlayer->Pause();
    }
    else if (uMsg == VLCPLAYER_MESSAGE_STATE_RESUME)   // 继续
    {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        pPlayer->Play(NULL, 0);
    }
    else if (uMsg == VLCPLAYER_MESSAGE_STATE_STOP)   // 停止
    {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        pPlayer->Stop();
        Ex_ObjSetLong(hObj, VLCPLAYER_LONG_IMG, 0);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == VLCPLAYER_MESSAGE_SET_RATE) {
        if (lParam > 0 && lParam <= 3) {
            VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
            pPlayer->SetMediaRate(lParam);
        }
    }
    else if (uMsg == VLCPLAYER_MESSAGE_GET_RATE) {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        return pPlayer->GetMediaRate();
    }
    else if (uMsg == VLCPLAYER_MESSAGE_SET_VOLUME) {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        pPlayer->SetMediaVolume(lParam);
    }
    else if (uMsg == VLCPLAYER_MESSAGE_GET_VOLUME) {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        return pPlayer->GetMediaVolume();
    }
    else if (uMsg == VLCPLAYER_MESSAGE_SET_MEDIATIME) {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        pPlayer->SetMediaTime(lParam);
    }
    else if (uMsg == VLCPLAYER_MESSAGE_GET_MEDIATIME) {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        return pPlayer->GetMediaTime();
    }
    else if (uMsg == VLCPLAYER_MESSAGE_GET_DURATION) {
        VLCPlayer* pPlayer = (VLCPlayer*)Ex_ObjGetLong(hObj, VLCPLAYER_LONG_OBJ);
        return pPlayer->GetMediaLength();
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

HRESULT VLCPlayer::CreateInstance(VLCPlayer** ppPlayer, HEXOBJ hObj)
{
    if (ppPlayer == NULL) {
        return E_POINTER;
    }
    HANDLE     hEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);
    VLCPlayer* pPlayer = new (std::nothrow) VLCPlayer(hEvent);

    if (pPlayer == NULL) {
        pPlayer->Release();
        return E_OUTOFMEMORY;
    }
    pPlayer->m_obj = hObj;
    CoInitializeEx(0, COINIT_MULTITHREADED);
    *ppPlayer = pPlayer;
    return S_OK;
}

VLCPlayer::VLCPlayer(HANDLE hEvent)
    : m_hEvent(hEvent)
    , m_nRefCount(1)
{
    InitializeCriticalSection(&m_critsec);
    const char* argv[] = {
        "--no-xlib", /* tell VLC to not use Xlib */
        "--network-caching=500",
        "--no-video-title-show",
        "--disable-screensaver",
    };
    int argc = sizeof(argv) / sizeof(*argv);
    m_libVlc = libvlc_new(argc, argv);
}

VLCPlayer::~VLCPlayer()
{
    ClearUp();
    libvlc_release(m_libVlc);
    DeleteCriticalSection(&m_critsec);
    CloseHandle(m_hEvent);
}

ULONG VLCPlayer::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

ULONG VLCPlayer::Release()
{
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0) {
        delete this;
    }
    return uCount;
}

HRESULT VLCPlayer::QueryInterface(REFIID riid, void** ppv)
{
    return S_OK;
}

void VLCPlayer::ClearUp()
{
    if (m_mediaPlayer != NULL) {
        libvlc_media_player_stop(m_mediaPlayer);
        libvlc_media_player_release(m_mediaPlayer);
    }
    cleanupBuffers();
}

void VLCPlayer::SetState(State state)
{
    switch (state) {
    case Stopped:
        if (m_mediaPlayer) {
            libvlc_media_player_stop(m_mediaPlayer);
            libvlc_media_player_release(m_mediaPlayer);
            m_mediaPlayer = NULL;
            m_fileName    = NULL;
        }
        break;
    case Playing:
        if (m_mediaPlayer) {
            libvlc_media_player_play(m_mediaPlayer);
        }
        break;
    case Paused:
        if (m_mediaPlayer) {
            libvlc_media_player_set_pause(m_mediaPlayer, 1);
        }
        break;
    case Resume:
        if (m_mediaPlayer) {
            libvlc_media_player_set_pause(m_mediaPlayer, 0);
        }
        break;
    }
}

void VLCPlayer::Play(const WCHAR* fileName, int type)
{
    if (fileName != NULL) {
        SetFileName(fileName, type);
    }
    SetState(Playing);
}

void VLCPlayer::DoResume()
{
    SetState(Resume);
}

void VLCPlayer::Pause()
{
    SetState(Paused);
}

void VLCPlayer::Stop()
{
    SetState(Stopped);
}

FLOAT VLCPlayer::GetMediaPosition()
{
    if (m_mediaPlayer) {
        return libvlc_media_player_get_position(m_mediaPlayer);
    }
    return 0;
}

void VLCPlayer::SetMediaPosition(FLOAT pos)
{
    if (m_mediaPlayer) {
        libvlc_media_player_set_position(m_mediaPlayer, pos);
    }
}

INT64 VLCPlayer::GetMediaTime()
{
    if (m_mediaPlayer) {
        return libvlc_media_player_get_time(m_mediaPlayer);
    }
    return 0;
}

void VLCPlayer::SetMediaTime(INT64 timeMs)
{
    if (m_mediaPlayer) {
        libvlc_media_player_set_time(m_mediaPlayer, timeMs);
    }
}

INT64 VLCPlayer::GetMediaLength()
{
    if (m_mediaPlayer) {
        return libvlc_media_player_get_length(m_mediaPlayer);
    }
    return 0;
}

INT VLCPlayer::GetMediaVolume()
{
    if (m_mediaPlayer) {
        return libvlc_audio_get_volume(m_mediaPlayer);
    }
    return 0;
}

BOOL VLCPlayer::SetMediaVolume(INT volume)
{
    if (m_mediaPlayer) {
        return libvlc_audio_set_volume(m_mediaPlayer, volume) == 0;
    }
    return FALSE;
}

FLOAT VLCPlayer::GetMediaRate()
{
    if (m_mediaPlayer) {
        return libvlc_media_player_get_rate(m_mediaPlayer);
    }
    return 0;
}

BOOL VLCPlayer::SetMediaRate(FLOAT rate)
{
    if (m_mediaPlayer) {
        return libvlc_media_player_set_rate(m_mediaPlayer, rate) == 0;
    }
    return FALSE;
}

void VLCPlayer::SetFileName(const WCHAR* pwszFileName, int type)
{
    ClearUp();
    if (m_state != State::Stopped) {
        SetState(State::Stopped);
    }
    m_fileName = pwszFileName;
    libvlc_media_t* m;
    if (type == 0) {
        m = libvlc_media_new_path(m_libVlc, Ex_W2U(pwszFileName).c_str());
    }
    else {
        m = libvlc_media_new_location(m_libVlc, Ex_W2U(pwszFileName).c_str());
    }

    m_mediaPlayer = libvlc_media_player_new_from_media(m);

    libvlc_media_release(m);
    libvlc_video_set_format_callbacks(m_mediaPlayer, vlcVideoFormatCallback, NULL);
    libvlc_video_set_callbacks(m_mediaPlayer, vlcVideoLockCallBack, vlcVideoUnlockCallback,
                               vlcVideoDisplayCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerOpening,
                        vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerPlaying,
                        vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerPaused,
                        vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerStopped,
                        vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer),
                        libvlc_MediaPlayerEndReached, vlcVideoEventCallback, this);
    // libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer),
    // libvlc_MediaPlayerEncounteredError, vlcVideoEventCallback, this);
    // libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer),
    // libvlc_MediaPlayerTimeChanged, vlcVideoEventCallback, this);
    // libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer),
    // libvlc_MediaPlayerPositionChanged, vlcVideoEventCallback, this);
    // libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer),
    // libvlc_MediaPlayerSeekableChanged, vlcVideoEventCallback, this);
    // libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer),
    // libvlc_MediaPlayerPausableChanged, vlcVideoEventCallback, this);
    // libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer),
    // libvlc_MediaPlayerLengthChanged, vlcVideoEventCallback, this);
}

void* VLCPlayer::vlcVideoLockCallBack(void* object, void** planes)
{
    // Lock the pixel mutex, and hand the pixel buffer to VLC
    VLCPlayer* instance = (VLCPlayer*)object;
    EnterCriticalSection(&instance->m_critsec);
    *planes = (void*)instance->m_pixelBuff;
    return NULL;
}

void VLCPlayer::vlcVideoUnlockCallback(void* object, void* picture, void* const* planes)
{
    VLCPlayer* instance = (VLCPlayer*)object;
    instance->updateTexture(picture, planes);
    LeaveCriticalSection(&instance->m_critsec);
}

void VLCPlayer::vlcVideoDisplayCallback(void* object, void* picture)
{
    VLCPlayer* instance = (VLCPlayer*)object;
    instance->paintFrame(instance->m_obj);
}

unsigned int VLCPlayer::vlcVideoFormatCallback(void** object, char* chroma, unsigned int* width,
                                               unsigned int* height, unsigned int* pitches,
                                               unsigned int* lines)
{
    VLCPlayer*   instance = (VLCPlayer*)*object;
    unsigned int retval   = instance->setupFormat(chroma, width, height, pitches, lines);
    return retval;
}

void VLCPlayer::vlcVideoEventCallback(const libvlc_event_t* event, void* object)
{
    libvlc_event_t* tmp = new libvlc_event_t;
    memcpy(tmp, event, sizeof(libvlc_event_t));
    VLCPlayer* instance = (VLCPlayer*)object;
    instance->playerEvent(tmp);
}

void VLCPlayer::updateTexture(void* picture, void* const* planes)
{
    HEXIMAGE img;
    _img_createfrompngbits2(m_width, m_height, (BYTE*)*planes, &img);
    Ex_ObjSetLong(m_obj, VLCPLAYER_LONG_IMG, (LONG_PTR)img);
}

void VLCPlayer::playerEvent(const libvlc_event_t* event)
{
    switch (event->type) {
    case libvlc_MediaPlayerEndReached:
        m_state = End;
        Ex_ObjDispatchNotify(m_obj, VLCPLAYER_EVENT_STATECHANGED, 0, m_state);
        Ex_ObjDispatchNotify(m_obj, VLCPLAYER_EVENT_END, 0, 0);
        break;
    case libvlc_MediaPlayerStopped:
        m_state = Stopped;
        Ex_ObjDispatchNotify(m_obj, VLCPLAYER_EVENT_STATECHANGED, 0, m_state);
        Ex_ObjDispatchNotify(m_obj, VLCPLAYER_EVENT_STOPPED, 0, 0);
        break;
    case libvlc_MediaPlayerOpening:
        m_state = Opening;
        Ex_ObjDispatchNotify(m_obj, VLCPLAYER_EVENT_STATECHANGED, 0, m_state);
        break;
    case libvlc_MediaPlayerPlaying:
        m_state = Playing;
        Ex_ObjDispatchNotify(m_obj, VLCPLAYER_EVENT_STATECHANGED, 0, m_state);
        Ex_ObjDispatchNotify(m_obj, VLCPLAYER_EVENT_PLAYING, 0, 0);
        break;
    case libvlc_MediaPlayerPaused:
        m_state = Paused;
        Ex_ObjDispatchNotify(m_obj, VLCPLAYER_EVENT_STATECHANGED, 0, m_state);
        Ex_ObjDispatchNotify(m_obj, VLCPLAYER_EVENT_PAUSED, 0, 0);
        break;
    default: break;
    }
    delete event;
}

unsigned int VLCPlayer::setupFormat(char* chroma, unsigned int* width, unsigned int* height,
                                    unsigned int* pitches, unsigned int* lines)
{
    setupPlanes(chroma, width, height, pitches, lines);
    setupBuffers();
    return (m_numPlanes);
}

void VLCPlayer::setupPlanes(char* chroma, unsigned int* width, unsigned int* height,
                            unsigned int* pitches, unsigned int* lines)
{
    strcpy(chroma, "BGRA");
    *pitches    = *width * 4;
    *lines      = *height;
    m_width     = *width;
    m_height    = *height;
    m_numPlanes = 1;
}

void VLCPlayer::setupBuffers()
{
    m_pixelBuff = calloc(1, m_width * m_height * 4 + 1);
}

void VLCPlayer::cleanupBuffers()
{
    if (m_pixelBuff) {
        free(m_pixelBuff);
        m_pixelBuff = nullptr;
    }
}

void VLCPlayer::paintFrame(HEXOBJ hObj)
{
    Ex_ObjInvalidateRect(hObj, 0);
}

#endif