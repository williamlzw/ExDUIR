#pragma once
#include "ExUIbase.hpp"

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

				ExFont(LPCWSTR lpwzFontFace, INT dwFontSize, DWORD dwFontStyle)
				{
					m_font = _font_createfromfamily(lpwzFontFace, dwFontSize, dwFontStyle);
				}

				ExFont(LOGFONTW* lpLogfont)
				{
					m_font = _font_createfromlogfont(lpLogfont);
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