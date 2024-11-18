#pragma once
#include "ExUIbase.hpp"
#include "ExControl.hpp"

using namespace ExDUIR::FrameWorks::Control;

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExFont
			{
			public:
				HEXFONT m_font = 0;

				ExFont(HEXFONT font)
				{
					m_font = font;
				}

				ExFont()
				{
					m_font =_font_create();
				}

				ExFont(std::wstring lpwzFontFace, INT dwFontSize, DWORD dwFontStyle)
				{
					m_font = _font_createfromfamily(lpwzFontFace.c_str(), dwFontSize, dwFontStyle);
				}

				ExFont(std::wstring lpwzFontFace, INT dwFontSize)
				{
					m_font = _font_createfromfile(lpwzFontFace.c_str(), dwFontSize);
				}

				ExFont(LOGFONTW* lpLogfont)
				{
					m_font = _font_createfromlogfont(lpLogfont);
				}

				inline static ExFont GetFontFromObj(ExControl obj)
				{
					return ExFont(Ex_ObjGetFont(obj.m_handle));
				}

				inline BOOL Destroy()
				{
					return _font_destroy(m_font);
				}

				inline LPVOID GetContext()
				{
					return _font_getcontext(m_font);
				}

				inline BOOL GetLogFont(LOGFONTW* lpLogFont)
				{
					return _font_getlogfont(m_font, lpLogFont);
				}
			};
		}
	}
}