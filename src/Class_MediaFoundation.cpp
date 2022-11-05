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
				BYTE* buf;
				DWORD currentLength;
				HRESULT hr = pSample->ConvertToContiguousBuffer(&buffer);
				if (SUCCEEDED(hr))
				{
					hr = buffer->Lock(&buf, nullptr, &currentLength);
					if (SUCCEEDED(hr))
					{
						auto vWidth = pPlayer->GetVideoWidth();
						auto vHeight = pPlayer->GetVideoHeight();
						HEXIMAGE img;
						_img_createfrompngbits2(vWidth, vHeight, buf, &img);
						Ex_ObjSetLong(hObj, 3, (LONG_PTR)img);
						buffer->Unlock();
						buffer->Release();
						Ex_ObjInvalidateRect(hObj, 0);
					}
				}
			}
			else {
				Ex_ObjSetLong(hObj, 3, 0);
				Ex_ObjInvalidateRect(hObj, 0);
			}
		};
	}
	else if (uMsg == WM_DESTROY)
	{
		MFShutdown();
	}
	else if (uMsg == WM_PAINT)
	{
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps))
		{
			HEXIMAGE img = (HEXIMAGE)Ex_ObjGetLong(hObj, 3);
			if (img > 0)
			{
				_canvas_drawimagerect(ps.hCanvas, img, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom, 255);
				_img_destroy(img);
			}
			Ex_ObjEndPaint(hObj, &ps);
			return 0;
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
	pXAudio2(NULL),
	psourceVoice(NULL)
{
	InitializeCriticalSection(&m_critsec);
}
MFMediaPlayer::~MFMediaPlayer()
{
	m_pReader = NULL;
	m_pWriter = NULL;
	DeleteCriticalSection(&m_critsec);
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
	EnterCriticalSection(&m_critsec);
	if (!Isplaying())
	{
		LeaveCriticalSection(&m_critsec);
		return S_OK;
	}
	HRESULT hr = S_OK;
	if (FAILED(hrStatus))
	{
		hr = hrStatus;
		goto done;
	}
					if(m_bPause1==true)
			{
				//m_pReader->Flush((DWORD)MF_SOURCE_READER_ALL_STREAMS);
				output(L"* 调试信息",44444);
				      PROPVARIANT var = { 0 };
      var.vt = VT_I8;
      
     m_pReader->SetCurrentPosition(GUID_NULL, var),
				m_bPause1=FALSE;
			//pSample->SetSampleDuration(1);
		output(L"* 调试信息",5555,hr );
		PropVariantClear(&var);
		//hr = m_pReader->ReadSample((DWORD)MF_SOURCE_READER_ANY_STREAM, 0, NULL, NULL, NULL, NULL);
			}

	if (dwStreamFlags & MF_SOURCE_READERF_STREAMTICK)
	{
		output(L"* 调试信息",L"ddddddddddddd1");
	}
	if (pSample && m_bPause == FALSE)
	{
		if (dwStreamIndex == 0)//音频
		{
			//m_pWriter->WriteSample(0, pSample);
			IMFMediaBuffer* audiobuffer;
			BYTE* audiobuf;
			DWORD audioCurrentLength;
			pSample->ConvertToContiguousBuffer(&audiobuffer);
			audiobuffer->Lock(&audiobuf, nullptr, &audioCurrentLength);
			XAUDIO2_BUFFER xBuffer = {};
			xBuffer.pAudioData = audiobuf;
			xBuffer.AudioBytes = audioCurrentLength;
			psourceVoice->SubmitSourceBuffer(&xBuffer);
			audiobuffer->Unlock();
			audiobuffer->Release();
			Sleep(14);
			
		}
		if (SampleEvent && dwStreamIndex == 1)
		{
			SampleEvent(dwStreamIndex, dwStreamFlags, llTimeStamp, pSample);
		}


		hr = m_pReader->ReadSample((DWORD)MF_SOURCE_READER_ANY_STREAM, 0, NULL, NULL, NULL, NULL);
	}
	else {
		//sample为NULL
		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)//有MF_SOURCE_READERF_ENDOFSTREAM标识表示结尾
		{
			if (SampleEvent && dwStreamIndex == 1)
			{
				SampleEvent(dwStreamIndex, dwStreamFlags, llTimeStamp, pSample);
			}
		}
		else if (dwStreamFlags & MF_SOURCE_READERF_STREAMTICK)//流中存在间隙
		{
			m_pWriter->SendStreamTick(0, llTimeStamp);
			hr = m_pReader->ReadSample((DWORD)MF_SOURCE_READER_ANY_STREAM, 0, NULL, NULL, NULL, NULL);
		}
		
	}

done:
	if (FAILED(hr))
	{
		NotifyError(hr);
	}

	LeaveCriticalSection(&m_critsec);
	SetEvent(m_hEvent);
	return hr;
}

HRESULT MFMediaPlayer::Startplay(const WCHAR* pszURL)
{
	MFMediaPlayer::Endplay();
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

	hrv = ConfigureDecoderV();
	hra = ConfigureDecoderA();
	if (FAILED(hrv) && FAILED(hra)) { 
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
	m_bPause = bPause;
	if (m_bPause == FALSE)
	{
		m_pReader->ReadSample((DWORD)MF_SOURCE_READER_ANY_STREAM, 0, NULL, NULL, NULL, NULL);
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
		return ;
	}
	ULONG flags = 0;
	BOOL bCanSeek = FALSE;
	PROPVARIANT var;
	PropVariantInit(&var);

	HRESULT hr = m_pReader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_SOURCE_READER_MEDIASOURCE_CHARACTERISTICS, &var);
	if (SUCCEEDED(hr))
	{
		hr = PropVariantToUInt32(var, &flags);
		if (SUCCEEDED(hr))
		{
			bCanSeek = ((flags & MFMEDIASOURCE_CAN_SEEK) == MFMEDIASOURCE_CAN_SEEK);
			PropVariantClear(&var);
			if (bCanSeek)
			{
				hr = InitPropVariantFromInt64((LONGLONG)(position * 10000000), &var);
				if (SUCCEEDED(hr))
				{
					m_bPause1 = true;
					//hr = m_pReader->Flush((DWORD)MF_SOURCE_READER_ALL_STREAMS);
								IMFSeekInfo *pRateControl = NULL;
								PROPVARIANT var1;
								PROPVARIANT var2;
								ULONG flags1 = 0;
								hr = m_pReader->GetServiceForStream(MF_SOURCE_READER_MEDIASOURCE,MF_SCRUBBING_SERVICE,IID_IMFSeekInfo,(LPVOID  *)&pRateControl);
								pRateControl->GetNearestKeyFrames((const GUID *)&GUID_NULL,&var,&var1,&var2);
								PropVariantToUInt32(var2, &flags1);
								output(L"* 调试信息4444444",hr,(long)flags1/10000000); 
					if (SUCCEEDED(hr))
					{
						//m_pWriter->Flush(MF_SINK_WRITER_ALL_STREAMS);
						//m_pReader->SetCurrentPosition(GUID_NULL, var);
						//m_pReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, NULL, NULL, NULL, NULL);
						//DWORD sflags; LONGLONG readTime; IMFSample* readSample;DWORD streamIndex;
						//hr = m_pReader->ReadSample((DWORD)MF_SOURCE_READER_ANY_STREAM,0, &streamIndex, &sflags, &readTime, &readSample);
						// LONGLONG startTime ;
           					// readSample->GetSampleTime(&startTime);
						//output(L"* 调试信息",333333,startTime/1000000);
						
					}
					PropVariantClear(&var);
				}
			}
		}
	}
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
		hr = MFGetAttributeSize(inputVideoMediaType, MF_MT_FRAME_SIZE, &m_uVideoWidth, &m_uVideoHeight);//取尺寸
		if (FAILED(hr))  goto done;

		outputVideoMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		outputVideoMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32); //置输出格式
		outputVideoMediaType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);

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

	//HRESULT hr = m_pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &inputAudioMediaType);
	HRESULT hr = m_pReader->GetNativeMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &inputAudioMediaType);
	if (FAILED(hr))  goto done;

	GUID AmajorType;

	hr = inputAudioMediaType->GetGUID(MF_MT_MAJOR_TYPE, &AmajorType);
	if (FAILED(hr)) goto done;

	if (AmajorType == MFMediaType_Audio)
	{
		
	UINT32 audioChannels, audioSampleRate, audioBitsPerSample;
	inputAudioMediaType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &audioChannels);
	inputAudioMediaType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &audioSampleRate);
	inputAudioMediaType->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, &audioBitsPerSample);
		output(L"* 调试信息4444444",hr,audioChannels,audioSampleRate,audioBitsPerSample,audioChannels * (audioBitsPerSample / 8)); 
	
	MFCreateMediaType(&outputAudioMediaType);
	outputAudioMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	outputAudioMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	//outputAudioMediaType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
	outputAudioMediaType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, audioChannels);
	outputAudioMediaType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, audioSampleRate );
	outputAudioMediaType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, audioBitsPerSample);
	
	hr = m_pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, outputAudioMediaType);
	if (FAILED(hr))  goto done;
	m_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);

	
	WAVEFORMATEX pwfx;
	pwfx.cbSize = sizeof(WAVEFORMATEX);
	pwfx.wFormatTag = WAVE_FORMAT_PCM;
	pwfx.wBitsPerSample = audioBitsPerSample;
	pwfx.nChannels = audioChannels;
	pwfx.nSamplesPerSec = audioSampleRate;
	pwfx.nBlockAlign = pwfx.nChannels * pwfx.wBitsPerSample / 8;
	pwfx.nAvgBytesPerSec = pwfx.nSamplesPerSec * pwfx.nBlockAlign;

	XAudio2Create(&pXAudio2,0, XAUDIO2_DEFAULT_PROCESSOR);
	IXAudio2MasteringVoice* masteringVoice;
	pXAudio2->CreateMasteringVoice(&masteringVoice);
	output(L"* 调试信息4444444",hr,2222,XAUDIO2_DEFAULT_FREQ_RATIO,XAUDIO2_MAX_FREQ_RATIO,XAUDIO2_MIN_FREQ_RATIO); 
	hr = pXAudio2->CreateSourceVoice(&psourceVoice, &pwfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO);
	psourceVoice->Start();
		output(L"* 调试信息4444444",hr,11111); 
		
//		IMFMediaSink* pAudioSink = NULL;
//		IMFStreamSink* pStreamSink = NULL;
//		IMFMediaTypeHandler* pMediaTypeHandler = NULL;
//
//		MFCreateAudioRenderer(NULL, &pAudioSink);
//		pAudioSink->GetStreamSinkByIndex(0, &pStreamSink);
//		pStreamSink->GetMediaTypeHandler(&pMediaTypeHandler);
//		DWORD TypeCount;
//		hr = pMediaTypeHandler->GetMediaTypeCount(&TypeCount);
//		for (UINT i = 0; i < TypeCount; i++)
//		{
//			pMediaTypeHandler->GetMediaTypeByIndex(i, &outputAudioMediaType);
//			if (pMediaTypeHandler->IsMediaTypeSupported(outputAudioMediaType, NULL) == S_OK)
//			{
//				break;
//			}
//		}
//		if (pMediaTypeHandler->IsMediaTypeSupported(outputAudioMediaType, NULL) == S_OK)
//		{
//			pMediaTypeHandler->SetCurrentMediaType(outputAudioMediaType);
//			hr = m_pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, outputAudioMediaType);
//			if (FAILED(hr))  goto done;
//			m_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);
//			if (FAILED(hr))  goto done;
//			MFCreateSinkWriterFromMediaSink(pAudioSink, NULL, &m_pWriter);
//			outputAudioMediaType->SetUINT32(MF_SINK_WRITER_DISABLE_THROTTLING, true);
//
//			m_pWriter->SetInputMediaType(0, outputAudioMediaType, NULL);
//			m_pWriter->BeginWriting();
//
//			SAFE_RELEASE(pMediaTypeHandler);
//			SAFE_RELEASE(pStreamSink);
//			SAFE_RELEASE(pAudioSink);
//		}
//		else
//		{
//			goto done;
//		}
	}
done:
	SAFE_RELEASE(inputAudioMediaType);
	SAFE_RELEASE(outputAudioMediaType);
	return hr;
}