#pragma once
#include "ExUIbase.hpp"

using namespace ExDUIR::FrameWorks;

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExImage
			{
			public:
				HEXIMAGE m_image = 0;
				ExImage() = default;
				ExImage(HEXIMAGE image)
				{
					m_image = image;
				}

				ExImage(INT width, INT height)
				{
					_img_create(width, height, &m_image);
				}

				ExImage(LPCWSTR lpwzFilename)
				{
					_img_createfromfile(lpwzFilename, &m_image);
				}

				ExImage(HBITMAP hBitmap, HPALETTE hPalette, BOOL fPreAlpha)
				{
					_img_createfromhbitmap(hBitmap, hPalette, fPreAlpha, &m_image);
				}

				ExImage(HICON hIcon)
				{
					_img_createfromhicon(hIcon, &m_image);
				}

				ExImage(LPVOID lpData, size_t dwLen)
				{
					_img_createfrommemory(lpData, dwLen, &m_image);
				}

				ExImage(LPSTREAM lpStream)
				{
					_img_createfromstream(lpStream, &m_image);
				}

				inline BOOL Destroy()
				{
					return _img_destroy(m_image);
				}

				inline BOOL GetFrameCount(INT* nFrameCount)
				{
					return _img_getframecount(m_image, nFrameCount);
				}

				inline BOOL GetFrameDelay(INT* lpDelayAry, INT nFrames)
				{
					return _img_getframedelay(m_image, lpDelayAry, nFrames);
				}

				inline BOOL GetSize(INT* lpWidth, INT* lpHeight)
				{
					return _img_getsize(m_image, lpWidth, lpHeight);
				}

				inline INT Height(){ return _img_height(m_image); }
				inline INT Width() { return _img_width(m_image); }

				inline BOOL Lock(RECT* lpRectL, DWORD flags, INT PixelFormat, EX_BITMAPDATA* lpLockedBitmapData)
				{
					return _img_lock(m_image, lpRectL, flags, PixelFormat, lpLockedBitmapData);
				}

				inline BOOL UnLock(EX_BITMAPDATA* lpLockedBitmapData)
				{
					return _img_unlock(m_image, lpLockedBitmapData);
				}

				inline std::shared_ptr<ExImage> RotateFlip(INT rfType)
				{
					HEXIMAGE retImage;
					_img_rotateflip(m_image, rfType, &retImage);
					return std::make_shared<ExImage>(retImage);
				}

				inline std::shared_ptr<ExImage> Scale(INT width, INT height)
				{
					HEXIMAGE retImage;
					_img_scale(m_image, width, height, &retImage);
					return std::make_shared<ExImage>(retImage);
				}

				inline std::shared_ptr<ExImage> Copy()
				{
					HEXIMAGE retImage;
					_img_copy(m_image, &retImage);
					return std::make_shared<ExImage>(retImage);
				}

				inline std::shared_ptr<ExImage> CopyRect(INT x, INT y, INT width, INT height)
				{
					HEXIMAGE retImage;
					_img_copyrect(m_image, x, y, width, height, &retImage);
					return std::make_shared<ExImage>(retImage);
				}

				inline std::shared_ptr<ExImage> Clip(INT left, INT top, INT width, INT height)
				{
					HEXIMAGE retImage;
					_img_clip(m_image, left, top, width, height, &retImage);
					return std::make_shared<ExImage>(retImage);
				}

				inline BOOL SaveToFile(LPCWSTR lpwzFileName)
				{
					return _img_savetofile(m_image, lpwzFileName);
				}

				inline size_t SaveToMemory(LPVOID lpBuffer)
				{
					return _img_savetomemory(m_image, lpBuffer);
				}

				inline BOOL SelectActiveFrame(INT nIndex)
				{
					return _img_selectactiveframe(m_image, nIndex);
				}

				inline BOOL SetPixel(INT x, INT y, EXARGB color)
				{
					return _img_setpixel(m_image, x, y, color);
				}

				inline BOOL GetPixel(INT x, INT y, EXARGB* retPixel)
				{
					return _img_getpixel(m_image, x, y, retPixel);
				}

				inline BOOL ChangeColor(EXARGB color)
				{
					return _img_changecolor(m_image, color);
				}
			};
		}
	}
}