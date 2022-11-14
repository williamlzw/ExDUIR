#include "Class_MediaFoundation.h"

void _mediafoundation_register()
{
	Ex_ObjRegister(L"MediaFoundation", EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER, 4 * sizeof(size_t), NULL, 0, _mediafoundation_proc);
}

LRESULT CALLBACK _mediafoundation_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;

	if (uMsg == WM_CREATE)
	{
		MFMediaPlayer* pPlayer = NULL;
		MFMediaPlayer::CreateInstance(&pPlayer);
		Ex_ObjSetLong(hObj, MFL_OBJ, (LONG_PTR)pPlayer);
		pPlayer->SampleEvent = [=](DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimeStamp, IMFSample* pSample)
		{
			if (pSample)
			{
				IMFMediaBuffer* buffer;
				IMF2DBuffer* buffer2;
				BYTE* buf;
				LONG currentLength;
				HRESULT hr = pSample->ConvertToContiguousBuffer(&buffer);
				if (SUCCEEDED(hr))
				{
					hr = buffer->QueryInterface(IID_IMF2DBuffer, (void**)&buffer2);
					hr = buffer2->Lock2D(&buf, &currentLength);
					if (SUCCEEDED(hr))
					{
						auto vWidth = pPlayer->GetVideoWidth();
						auto vHeight = pPlayer->GetVideoHeight();
						HEXIMAGE img;
						_img_createfrompngbits2(vWidth, vHeight, buf, &img);
						Ex_ObjSetLong(hObj, MFL_IMG, (LONG_PTR)img);

						buffer2->Unlock2D();
						buffer2->Release();
						buffer->Release();
						Ex_ObjInvalidateRect(hObj, 0);
					}
				}
			}
			else {
				Ex_ObjSetLong(hObj, MFL_IMG, 0);
				Ex_ObjInvalidateRect(hObj, 0);
			}
		};
	}
	else if (uMsg == WM_DESTROY)
	{
		MFMediaPlayer* pPlayer = (MFMediaPlayer*)Ex_ObjGetLong(hObj, MFL_OBJ);
		delete pPlayer;
		MFShutdown();
	}
	else if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps))
		{
			HEXIMAGE img = (HEXIMAGE)Ex_ObjGetLong(hObj, MFL_IMG);
			if (img > 0)
			{
				_canvas_drawimagerect(ps.hCanvas, img, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom, 255);
				_img_destroy(img);
			}
			Ex_ObjEndPaint(hObj, &ps);
		}
	}
	else if (uMsg == MFM_STATE_PLAY)//播放   lParam :文件名或url
	{
		MFMediaPlayer* pPlayer = (MFMediaPlayer*)Ex_ObjGetLong(hObj, MFL_OBJ);
		pPlayer->Startplay((PCWSTR)lParam);
	}
	else if (uMsg == MFM_STATE_PAUSE)//暂停
	{
		MFMediaPlayer* pPlayer = (MFMediaPlayer*)Ex_ObjGetLong(hObj, MFL_OBJ);
		pPlayer->Pause(TRUE);
	}
	else if (uMsg == MFM_STATE_CONTINUE)//继续
	{
		MFMediaPlayer* pPlayer = (MFMediaPlayer*)Ex_ObjGetLong(hObj, MFL_OBJ);
		pPlayer->Pause(FALSE);
	}
	else if (uMsg == MFM_STATE_STOP)//停止
	{
		MFMediaPlayer* pPlayer = (MFMediaPlayer*)Ex_ObjGetLong(hObj, MFL_OBJ);
		pPlayer->Pause(TRUE);
		pPlayer->Endplay();
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == MFM_RATE)
	{

	}
	else if (uMsg == MFM_SET_POSITION)
	{
		MFMediaPlayer* pPlayer = (MFMediaPlayer*)Ex_ObjGetLong(hObj, MFL_OBJ);
		pPlayer->SetPosition(lParam);
	}
	else if (uMsg == MFM_GET_DURATION)
	{


	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

HRESULT MFMediaPlayer::CreateInstance(MFMediaPlayer** ppPlayer)
{
	if (ppPlayer == NULL)
	{
		return E_POINTER;
	}
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	MFMediaPlayer* pPlayer = new (std::nothrow) MFMediaPlayer(hEvent);

	if (pPlayer == NULL)
	{
		pPlayer->Release();
		return E_OUTOFMEMORY;
	}
	CoInitializeEx(0, COINIT_MULTITHREADED);
	MFStartup(MF_VERSION);
	*ppPlayer = pPlayer;
	return S_OK;
}

MFMediaPlayer::MFMediaPlayer(HANDLE hEvent = NULL) :
	m_pReader(NULL),
	m_hEvent(hEvent),
	m_nRefCount(1),
	m_pWriter(NULL),
	m_bPause(FALSE)
{
	InitializeCriticalSection(&m_critsec);
}
MFMediaPlayer::~MFMediaPlayer()
{
	m_pReader = NULL;
	m_pWriter = NULL;
	DeleteCriticalSection(&m_critsec);
	CloseHandle(m_hEvent);
}

ULONG MFMediaPlayer::AddRef()
{
	return InterlockedIncrement(&m_nRefCount);
}

ULONG MFMediaPlayer::Release()
{
	ULONG uCount = InterlockedDecrement(&m_nRefCount);
	if (uCount == 0)
	{
		delete this;
	}
	return uCount;
}

HRESULT MFMediaPlayer::QueryInterface(REFIID riid, void** ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(MFMediaPlayer, IMFSourceReaderCallback),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
}

HRESULT MFMediaPlayer::OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimeStamp, IMFSample* pSample)
{
	if (!Isplaying())
	{
		return S_OK;
	}
	EnterCriticalSection(&m_critsec);

	HRESULT hr = S_OK;
	if (FAILED(hrStatus))
	{
		hr = hrStatus;
		goto done;
	}

	if (pSample)
	{
		if (m_bPosition)
		{
			PROPVARIANT var;
			PropVariantInit(&var);
			var.vt = VT_I8;
			var.hVal.QuadPart = m_bPositionc * 10000000;
			if (m_pReader)
			{
				m_pReader->SetCurrentPosition(GUID_NULL, var);
			}
			PropVariantClear(&var);
			m_bPosition = FALSE;
			m_bPositionc = 0;
		}

		if (m_bPause == FALSE)
		{
			if (dwStreamIndex == 0)//音频
			{
				if (m_pStreamSink)
				{
					m_pStreamSink->ProcessSample(pSample);
				}
			}
			if (SampleEvent && dwStreamIndex == 1)
			{
				SampleEvent(dwStreamIndex, dwStreamFlags, llTimeStamp, pSample);
				Sleep(m_fps);
			}
			if (m_pReader)
			{
				m_pReader->ReadSample((DWORD)MF_SOURCE_READER_ANY_STREAM, 0, NULL, NULL, NULL, NULL);
			}
		}
	}
done:
	if (FAILED(hr))
	{
		NotifyError(hr);
	}

	SetEvent(m_hEvent);
	LeaveCriticalSection(&m_critsec);
	return hr;
}

HRESULT MFMediaPlayer::Startplay(const WCHAR* pszURL)
{
	MFMediaPlayer::Endplay();
	m_bPause = FALSE;
	HRESULT hr = S_OK;
	IMFAttributes* pAttributes = NULL;
	HRESULT hrv, hra;
	EnterCriticalSection(&m_critsec);
	hr = MFCreateAttributes(&pAttributes, 0);
	if (FAILED(hr)) {
		goto done;
	}

	pAttributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, (IMFSourceReaderCallback*)this);
	pAttributes->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);
	pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, false);
	pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, TRUE);
	pAttributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, false);

	if (FAILED(hr)) {
		goto done;
	}

	hr = MFCreateSourceReaderFromURL(pszURL, pAttributes, &m_pReader);
	if (FAILED(hr)) {
		goto done;
	}

	hr = ConfigureDecoderV();
	hr = ConfigureDecoderA();
	if (FAILED(hr)) {
		goto done;
	}

	hr = m_pReader->ReadSample((DWORD)MF_SOURCE_READER_ANY_STREAM, 0, NULL, NULL, NULL, NULL);

done:
	SAFE_RELEASE(pAttributes);
	LeaveCriticalSection(&m_critsec);
	return hr;
}

HRESULT MFMediaPlayer::Endplay()
{
	EnterCriticalSection(&m_critsec);
	HRESULT hr = S_OK;
	SAFE_RELEASE(m_pStreamSink);
	SAFE_RELEASE(m_ppClock);
	SAFE_RELEASE(m_pReader);
	SAFE_RELEASE(m_pWriter);
	
	LeaveCriticalSection(&m_critsec);
	return hr;
}

void MFMediaPlayer::Pause(BOOL bPause)
{
	if (!Isplaying())
	{
		return;
	}

	if (bPause)
	{
		m_bPause = bPause;
		if (m_ppClock)
		{
			m_ppClock->Pause();
		}
	}
	else
	{
		m_bPause = bPause;
		if (m_ppClock)
		{
			m_ppClock->Start(PRESENTATION_CURRENT_POSITION);
		}
		if (m_pReader)
		{
			m_pReader->ReadSample((DWORD)MF_SOURCE_READER_ANY_STREAM, 0, NULL, NULL, NULL, NULL);
		}
	}
}

void MFMediaPlayer::GetDuration()
{
	if (!Isplaying())
	{
		return;
	}
}

void MFMediaPlayer::SetPosition(int position)
{
	if (!Isplaying())
	{
		return;
	}
	m_bPosition = true;
	m_bPositionc = position;
}

BOOL MFMediaPlayer::Isplaying()
{
	EnterCriticalSection(&m_critsec);
	BOOL bIsplaying = (m_pReader != NULL);
	LeaveCriticalSection(&m_critsec);
	return bIsplaying;
}

HRESULT MFMediaPlayer::ConfigureDecoderV()
{
	IMFMediaType* inputVideoMediaType = NULL;
	IMFMediaType* outputVideoMediaType = NULL;
	HRESULT hr = m_pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &inputVideoMediaType);
	if (FAILED(hr))  goto done;

	GUID VmajorType;
	hr = inputVideoMediaType->GetGUID(MF_MT_MAJOR_TYPE, &VmajorType);
	if (FAILED(hr)) goto done;

	hr = MFCreateMediaType(&outputVideoMediaType);
	if (FAILED(hr))  goto done;

	hr = outputVideoMediaType->SetGUID(MF_MT_MAJOR_TYPE, VmajorType);
	if (FAILED(hr))  goto done;

	if (VmajorType == MFMediaType_Video)
	{
		UINT frameRateNumerator, frameRateDenominator;
		hr = MFGetAttributeRatio(inputVideoMediaType, MF_MT_FRAME_RATE, &frameRateNumerator, &frameRateDenominator);//取速率
		m_fps = 1000 / (frameRateNumerator / frameRateDenominator) - 13;
		if (FAILED(hr))  goto done;
		hr = MFGetAttributeSize(inputVideoMediaType, MF_MT_FRAME_SIZE, &m_uVideoWidth, &m_uVideoHeight);//取尺寸
		if (FAILED(hr))  goto done;
        //处理非标尺寸的视频
		int VideoWidtharr[12] = {480,640,800,1024,1280,1920,360,480,480,534,720,1080};
		int VideoHeightarr[12] = {360,480,480,534,720,1080,480,640,800,1024,1280,1920};
		BOOL isnormal = FALSE;
		for (int i=0; i<12; i++)
		{
			if(m_uVideoWidth == VideoWidtharr[i] && m_uVideoHeight == VideoHeightarr[i])
			{
				isnormal =TRUE;
				break;
			}
		}
		if(isnormal == FALSE)
		{
			if(m_uVideoWidth >= m_uVideoHeight)
			{
				if(m_uVideoWidth > 1024)
				{
					m_uVideoWidth = 1280;
					m_uVideoHeight = 720;
				}
				else if(m_uVideoWidth > 800)
				{
					m_uVideoWidth = 1024;
					m_uVideoHeight = 534;
				}
				else
				{
					m_uVideoWidth = 800;
					m_uVideoHeight = 480;
				}
			}
			else
			{
				if(m_uVideoHeight > 1024)
				{
					m_uVideoHeight = 1280;
					m_uVideoWidth = 720;
				}
				else if(m_uVideoHeight > 800)
				{
					m_uVideoHeight = 1024;
					m_uVideoWidth = 534;
				}
				else
				{
					m_uVideoHeight = 800;
					m_uVideoWidth = 480;
				}
			}
		}
		outputVideoMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		outputVideoMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32); //置输出格式
		outputVideoMediaType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);//置帧交错模式
		outputVideoMediaType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);

		hr = MFSetAttributeRatio(outputVideoMediaType, MF_MT_FRAME_RATE, frameRateNumerator, frameRateDenominator);//置速率
		if (FAILED(hr))  goto done;

		hr = MFSetAttributeSize(outputVideoMediaType, MF_MT_FRAME_SIZE, m_uVideoWidth, m_uVideoHeight);//置尺寸
		if (FAILED(hr))  goto done;
		hr = m_pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, outputVideoMediaType);//置当前属性生效
		if (FAILED(hr))  goto done;
		hr = m_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE);
		if (FAILED(hr))  goto done;
	}
done:
	SAFE_RELEASE(inputVideoMediaType);
	SAFE_RELEASE(outputVideoMediaType);
	return hr;
}

HRESULT MFMediaPlayer::ConfigureDecoderA()
{
	IMFMediaType* inputAudioMediaType = NULL;
	IMFMediaType* outputAudioMediaType = NULL;
	IMFMediaType* inputVideoMediaType = NULL;
	IMFMediaType* outputVideoMediaType = NULL;
	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice* pDevice = NULL;

	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	HRESULT  hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

	LPWSTR ppstrId;
	hr = pDevice->GetId(&ppstrId);
	IMFAttributes* pAttributes;
	MFCreateAttributes(&pAttributes, 0);
	IMFMediaSink* pIMFMediaSink = NULL;

	hr = pAttributes->SetString(MF_AUDIO_RENDERER_ATTRIBUTE_ENDPOINT_ID, ppstrId);
	hr = pAttributes->SetUINT32(MF_AUDIO_RENDERER_ATTRIBUTE_STREAM_CATEGORY, AUDIO_STREAM_CATEGORY::AudioCategory_Media);
	hr = pAttributes->SetUINT32(MF_AUDIO_RENDERER_ATTRIBUTE_FLAGS, 0);
	hr = pAttributes->SetGUID(MF_AUDIO_RENDERER_ATTRIBUTE_SESSION_ID, GUID_NULL);
	hr = MFCreateAudioRenderer(pAttributes, &pIMFMediaSink);

	DWORD pcStreamSinkCount;
	pIMFMediaSink->GetStreamSinkCount(&pcStreamSinkCount);
	hr = pIMFMediaSink->GetStreamSinkByIndex(0, &m_pStreamSink);
	DWORD pdwCharacteristics;
	pIMFMediaSink->GetCharacteristics(&pdwCharacteristics);
	IMFMediaTypeHandler* pMediaTypeHandler = NULL;
	m_pStreamSink->GetMediaTypeHandler(&pMediaTypeHandler);

	DWORD TypeCount;
	hr = pMediaTypeHandler->GetMediaTypeCount(&TypeCount);

	for (UINT i = 0; i < TypeCount; i++)
	{
		pMediaTypeHandler->GetMediaTypeByIndex(i, &outputAudioMediaType);
		if (pMediaTypeHandler->IsMediaTypeSupported(outputAudioMediaType, NULL) == S_OK)
		{
			break;
		}
	}

	hr = m_pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &inputAudioMediaType);
	IMFPresentationTimeSource* pTimeSource = NULL;
	IMFMediaEvent* pEvent = NULL;
	HRESULT hrStatus = S_OK;

	if (FAILED(hr))  goto done;

	GUID majorType;
	hr = inputAudioMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);

	if (majorType == MFMediaType_Audio)
	{
		outputAudioMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		outputAudioMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		pMediaTypeHandler->SetCurrentMediaType(outputAudioMediaType);
		hr = m_pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, outputAudioMediaType);
		m_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);
	}

	MediaEventType mediaEventType;
	hr = MFCreatePresentationClock(&m_ppClock);
	hr = MFCreateSystemTimeSource(&pTimeSource);
	m_ppClock->SetTimeSource(pTimeSource);
	hr = pIMFMediaSink->SetPresentationClock(m_ppClock);
	hr = m_ppClock->Start(0);
	
done:
	SAFE_RELEASE(inputAudioMediaType);
	SAFE_RELEASE(outputAudioMediaType);
	return hr;
}