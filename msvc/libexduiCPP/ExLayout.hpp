#pragma once
#include "ExUIbase.hpp"
#include "ExControl.hpp"

using namespace ExDUIR::FrameWorks::Control;


namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Layout
		{
			class ExBaseLayout
			{
			public:
				HEXLAYOUT m_layout = 0;

				ExBaseLayout(HEXLAYOUT layout)
				{
					m_layout = layout;
				}

				ExBaseLayout(INT nType, ExControl objBind)
				{
					m_layout = _layout_create(nType, objBind.m_handle);
				}

				inline BOOL Destroy()
				{
					return _layout_destroy(m_layout);
				}

				inline BOOL AddChild(ExControl obj)
				{
					return _layout_addchild(m_layout, obj.m_handle);
				}

				inline BOOL AddChildren(BOOL fDesc, EXATOM dwObjClassATOM, INT* nCount)
				{
					return _layout_addchildren(m_layout, fDesc, dwObjClassATOM, nCount);
				}

				inline BOOL DeleteChild(ExControl obj)
				{
					return _layout_deletechild(m_layout, obj.m_handle);
				}

				inline BOOL DeleteChildren(EXATOM dwObjClassATOM)
				{
					return _layout_deletechildren(m_layout, dwObjClassATOM);
				}

				inline BOOL SetChildProp(ExControl obj, INT dwPropID, size_t pvValue)
				{
					return _layout_setchildprop(m_layout, obj.m_handle, dwPropID, pvValue);
				}

				inline BOOL SetProp(INT dwPropID, size_t pvValue)
				{
					return _layout_setprop(m_layout, dwPropID, pvValue);
				}

				inline BOOL Update()
				{
					return _layout_update(m_layout);
				}

				inline BOOL EnableUpdate(BOOL fUpdateable)
				{
					return _layout_enableupdate(m_layout, fUpdateable);
				}

				inline BOOL GetChildProp(ExControl obj, INT dwPropID, size_t* pvValue)
				{
					return _layout_getchildprop(m_layout, obj.m_handle, dwPropID, pvValue);
				}

				inline BOOL GetChildPropList(ExControl obj, LPVOID* lpProps)
				{
					return _layout_getchildproplist(m_layout, obj.m_handle, lpProps);
				}

				inline size_t GetProp(INT dwPropID)
				{
					return _layout_getprop(m_layout, dwPropID);
				}

				inline LPVOID GetPropList()
				{
					return _layout_getproplist(m_layout);
				}

				inline DWORD GetType()
				{
					return _layout_gettype(m_layout);
				}

				inline size_t Notify(INT nEvent, WPARAM wParam, LPARAM lParam)
				{
					return _layout_notify(m_layout, nEvent, wParam, lParam);
				}
			};

			class ExAbsoluteLayout : public ExBaseLayout
			{
				ExAbsoluteLayout(ExControl objBind) :ExBaseLayout(LAYOUT_TYPE_ABSOLUTE, objBind)
				{
				}

				inline BOOL Lock(ExControl obj, INT tLeft, INT tTop, INT tRight, INT tBottom, INT tWidth, INT tHeight)
				{
					return _layout_absolute_lock(m_layout, obj.m_handle, tLeft, tTop, tRight, tBottom, tWidth, tHeight);
				}

				inline BOOL SetEdge(ExControl obj, INT dwEdge, INT dwType, size_t nValue)
				{
					return _layout_absolute_setedge(m_layout, obj.m_handle, dwEdge, dwType, nValue);
				}
			};

			class ExFlowLayout : public ExBaseLayout
			{
				ExFlowLayout(ExControl objBind) :ExBaseLayout(LAYOUT_TYPE_FLOW, objBind)
				{
				}
			};

			class ExLinearLayout : public ExBaseLayout
			{
				ExLinearLayout(ExControl objBind) :ExBaseLayout(LAYOUT_TYPE_LINEAR, objBind)
				{
				}
			};

			class ExPageLayout : public ExBaseLayout
			{
				ExPageLayout(ExControl objBind) :ExBaseLayout(LAYOUT_TYPE_PAGE, objBind)
				{
				}
			};

			class ExRelativateLayout : public ExBaseLayout
			{
				ExRelativateLayout(ExControl objBind) :ExBaseLayout(LAYOUT_TYPE_RELATIVE, objBind)
				{
				}
			};

			class ExTableLayout : public ExBaseLayout
			{
				ExTableLayout(ExControl objBind) :ExBaseLayout(LAYOUT_TYPE_TABLE, objBind)
				{
				}

				inline BOOL SetInfo(INT* aRowHeight, INT cRows, INT* aCellWidth, INT cCells)
				{
					return _layout_table_setinfo(m_layout, aRowHeight, cRows, aCellWidth, cCells);
				}
			};
		}
	}
}