#pragma once
#include "ExUIbase.hpp"
#include "ExImage.hpp"
#include "ExCanvas.hpp"

using namespace ExDUIR::FrameWorks;

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExImageList
			{
			public:
				HEXIMAGELIST m_imagelist = 0;
				ExImageList() = default;
				ExImageList(HEXIMAGELIST imageList)
				{
					m_imagelist = imageList;
				}

				ExImageList(INT width, INT height)
				{
					m_imagelist = _imglist_create(width, height);
				}

				inline BOOL Destroy()
				{
					return _imglist_destroy(m_imagelist);
				}

				inline BOOL GetImageSize(INT& pWidth, INT& pHeight)
				{
					return _imglist_size(m_imagelist, &pWidth, &pHeight);
				}

				inline BOOL DelImage(size_t nIndex)
				{
					return _imglist_del(m_imagelist, nIndex);
				}

				inline ExImage GetImage(size_t nIndex)
				{
					return ExImage(_imglist_get(m_imagelist, nIndex));
				}

				inline BOOL SetImage(size_t nIndex, ExImage image)
				{
					return _imglist_setimage(m_imagelist, nIndex, image.m_image);
				}

				inline size_t AddImage(size_t nIndex, ExImage image)
				{
					return _imglist_addimage(m_imagelist, nIndex, image.m_image);
				}

				inline BOOL SetImageFromMemory(size_t nIndex, LPVOID lpData, size_t dwLen)
				{
					return _imglist_set(m_imagelist, nIndex, lpData, dwLen);
				}

				inline BOOL SetImageFromData(size_t nIndex, std::vector<CHAR> imgdata)
				{
					return SetImageFromMemory(nIndex, imgdata.data(), imgdata.size());
				}

				inline BOOL SetImageFromFile(size_t nIndex, std::wstring filePath)
				{
					std::vector<CHAR> imgdata;
					Ex_ReadFile(filePath.c_str(), &imgdata);
					return SetImageFromMemory(nIndex, imgdata.data(), imgdata.size());
				}

				inline size_t AddImageFromMemory(size_t nIndex, LPVOID lpData, size_t dwLen)
				{
					return _imglist_add(m_imagelist, nIndex, lpData, dwLen);
				}

				inline size_t AddImageFromData(size_t nIndex, std::vector<CHAR> imgdata)
				{
					return AddImageFromMemory(nIndex, imgdata.data(), imgdata.size());
				}

				inline size_t AddImageFromFile(size_t nIndex, std::wstring filePath)
				{
					std::vector<CHAR> imgdata;
					Ex_ReadFile(filePath.c_str(), &imgdata);
					return AddImageFromMemory(nIndex, imgdata.data(), imgdata.size());
				}

				inline BOOL DrawImage(size_t nIndex, ExCanvas canvas, INT nLeft, INT nTop, INT nRight, INT nBottom, DWORD nAlpha)
				{
					return _imglist_draw(m_imagelist, nIndex, canvas.m_canvas, nLeft, nTop, nRight, nBottom, nAlpha);
				}

				inline INT GetImageCount()
				{
					return _imglist_count(m_imagelist);
				}
			};
		}
	}
}