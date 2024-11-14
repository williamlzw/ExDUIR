#pragma once
#include "ExControl.hpp"
using namespace ExDUIR::FrameWorks;


namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Layout
		{
			class ExBaseLayout
			{
			public:
				ExBaseLayout() = default;
				~ExBaseLayout() = default;
				HEXLAYOUT m_handleLayout = 0;

				ExBaseLayout(HEXLAYOUT layout)
				{
					m_handleLayout = layout;
				}

				ExBaseLayout(INT nType, ExUIbase objBind)
				{
					m_handleLayout = _layout_create(nType, objBind.m_handle);
				}

				inline BOOL Destroy()
				{
					return _layout_destroy(m_handleLayout);
				}

				inline BOOL AddChild(ExControl obj)
				{
					return _layout_addchild(m_handleLayout, obj.m_handle);
				}

				inline BOOL AddChildren(BOOL fDesc, EXATOM dwObjClassATOM, INT* nCount)
				{
					return _layout_addchildren(m_handleLayout, fDesc, dwObjClassATOM, nCount);
				}

				inline BOOL DeleteChild(ExControl obj)
				{
					return _layout_deletechild(m_handleLayout, obj.m_handle);
				}

				inline BOOL DeleteChildren(EXATOM dwObjClassATOM)
				{
					return _layout_deletechildren(m_handleLayout, dwObjClassATOM);
				}

				inline BOOL SetChildProp(ExControl obj, INT dwPropID, size_t pvValue)
				{
					return _layout_setchildprop(m_handleLayout, obj.m_handle, dwPropID, pvValue);
				}

				inline BOOL SetPropPaddingLeft(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_PADDING_LEFT, pvValue);
				}

				inline BOOL SetPropPaddingTop(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_PADDING_TOP, pvValue);
				}

				inline BOOL SetPropPaddingRight(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_PADDING_RIGHT, pvValue);
				}

				inline BOOL SetPropPaddingBottom(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_PADDING_BOTTOM, pvValue);
				}

				inline BOOL SetSubPropMarginLeft(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_MARGIN_LEFT, pvValue);
				}

				inline BOOL SetSubPropMarginTop(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_MARGIN_TOP, pvValue);
				}

				inline BOOL SetSubPropMarginRight(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_MARGIN_RIGHT, pvValue);
				}

				inline BOOL SetSubPropMarginBottom(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_MARGIN_BOTTOM, pvValue);
				}

				inline BOOL SetLayoutProp(INT dwPropID, size_t pvValue)
				{
					return _layout_setprop(m_handleLayout, dwPropID, pvValue);
				}

				inline BOOL Update()
				{
					return _layout_update(m_handleLayout);
				}

				inline BOOL EnableUpdate(BOOL fUpdateable)
				{
					return _layout_enableupdate(m_handleLayout, fUpdateable);
				}

				inline BOOL GetChildProp(ExControl obj, INT dwPropID, size_t* pvValue)
				{
					return _layout_getchildprop(m_handleLayout, obj.m_handle, dwPropID, pvValue);
				}

				inline BOOL GetChildPropList(ExControl obj, LPVOID* lpProps)
				{
					return _layout_getchildproplist(m_handleLayout, obj.m_handle, lpProps);
				}

				inline size_t GetProp(INT dwPropID)
				{
					return _layout_getprop(m_handleLayout, dwPropID);
				}

				inline LPVOID GetPropList()
				{
					return _layout_getproplist(m_handleLayout);
				}

				inline DWORD GetType()
				{
					return _layout_gettype(m_handleLayout);
				}

				inline size_t Notify(INT nEvent, WPARAM wParam, LPARAM lParam)
				{
					return _layout_notify(m_handleLayout, nEvent, wParam, lParam);
				}

				inline BOOL BindObj(ExUIbase obj, BOOL fUpdate = FALSE)
				{
					return Ex_ObjLayoutSet(obj.m_handle, m_handleLayout, fUpdate);
				}
			};

			class ExAbsoluteLayout : public ExBaseLayout
			{
			public:
				ExAbsoluteLayout() = default;
				~ExAbsoluteLayout() = default;
				ExAbsoluteLayout(ExUIbase objBind) : ExBaseLayout(LAYOUT_TYPE_ABSOLUTE, objBind)
				{
				}

				inline BOOL Lock(ExControl obj, INT tLeft, INT tTop, INT tRight, INT tBottom, INT tWidth, INT tHeight)
				{
					return _layout_absolute_lock(m_handleLayout, obj.m_handle, tLeft, tTop, tRight, tBottom, tWidth, tHeight);
				}

				inline BOOL SetEdge(ExControl obj, INT dwEdge, INT dwType, size_t nValue)
				{
					return _layout_absolute_setedge(m_handleLayout, obj.m_handle, dwEdge, dwType, nValue);
				}

				inline BOOL SetSubPropLeft(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_LEFT, pvValue);
				}

				inline BOOL SetSubPropLeftType(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_LEFT_TYPE, pvValue);
				}

				inline BOOL SetSubPropTop(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_TOP, pvValue);
				}

				inline BOOL SetSubPropTopType(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_TOP_TYPE, pvValue);
				}

				inline BOOL SetSubPropRight(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_RIGHT, pvValue);
				}

				inline BOOL SetSubPropRightType(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_RIGHT_TYPE, pvValue);
				}

				inline BOOL SetSubPropBottom(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM, pvValue);
				}

				inline BOOL SetSubPropBottomType(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM_TYPE, pvValue);
				}

				inline BOOL SetSubPropWidth(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_WIDTH, pvValue);
				}

				inline BOOL SetSubPropWidthType(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_WIDTH_TYPE, pvValue);
				}

				inline BOOL SetSubPropHeight(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT, pvValue);
				}

				inline BOOL SetSubPropHeightType(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT_TYPE, pvValue);
				}

				inline BOOL SetSubPropOffsetHorizontal(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_H, pvValue);
				}

				inline BOOL SetSubPropOffsetHorizontalType(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_H_TYPE, pvValue);
				}

				inline BOOL SetSubPropOffsetVertical(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_V, pvValue);
				}

				inline BOOL SetSubPropOffsetVerticalType(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_V_TYPE, pvValue);
				}
			};

			class ExFlowLayout : public ExBaseLayout
			{
			public:
				ExFlowLayout() = default;
				 ~ExFlowLayout() = default;
				ExFlowLayout(ExUIbase objBind) :ExBaseLayout(LAYOUT_TYPE_FLOW, objBind)
				{
				}

				inline BOOL SetPropDirection(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_FLOW_DIRECTION, pvValue);
				}

				inline BOOL SetSubPropSize(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_FLOW_SIZE, pvValue);
				}

				inline BOOL SetSubPropNewLine(ExControl obj, BOOL bNewLine)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_FLOW_NEW_LINE, bNewLine);
				}
			};

			class ExLinearLayout : public ExBaseLayout
			{
			public:
				ExLinearLayout() = default;
				~ExLinearLayout() = default;
				ExLinearLayout(ExUIbase objBind) :ExBaseLayout(LAYOUT_TYPE_LINEAR, objBind)
				{
				}

				inline BOOL SetSubPropSize(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_LINEAR_SIZE, pvValue);
				}

				inline BOOL SetSubPropAlign(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_LINEAR_ALIGN, pvValue);
				}

				inline BOOL SetPropAlign(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_LINEAR_ALIGN, pvValue);
				}

				inline BOOL SetPropDirection(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_LINEAR_DIRECTION, pvValue);
				}

				inline BOOL SetPropAlignLeftTop(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_LINEAR_ALIGN_LEFT_TOP, pvValue);
				}

				inline BOOL SetPropAlignCenter(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_LINEAR_ALIGN_CENTER, pvValue);
				}

				inline BOOL SetPropAlignRightBottom(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_LINEAR_ALIGN_RIGHT_BOTTOM, pvValue);
				}
			};

			class ExPageLayout : public ExBaseLayout
			{
			public:
				ExPageLayout() = default;
				~ExPageLayout() = default;
				ExPageLayout(ExUIbase objBind) :ExBaseLayout(LAYOUT_TYPE_PAGE, objBind)
				{
				}

				inline BOOL SetPropCurrentShow(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_PAGE_CURRENT, pvValue);
				}

				inline BOOL SetSubPropFill(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_PAGE_FILL, pvValue);
				}
			};

			class ExRelativateLayout : public ExBaseLayout
			{
			public:
				ExRelativateLayout() = default;
				~ExRelativateLayout() = default;
				ExRelativateLayout(ExUIbase objBind) :ExBaseLayout(LAYOUT_TYPE_RELATIVE, objBind)
				{
				}

				inline BOOL SetSubPropLeftOf(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_LEFT_OF, pvValue);
				}

				inline BOOL SetSubPropTopOf(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_TOP_OF, pvValue);
				}

				inline BOOL SetSubPropRightOf(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_RIGHT_OF, pvValue);
				}

				inline BOOL SetSubPropBottomOf(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_BOTTOM_OF, pvValue);
				}

				inline BOOL SetSubPropLeftAlignOf(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_LEFT_ALIGN_OF, pvValue);
				}

				inline BOOL SetSubPropTopAlignOf(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_TOP_ALIGN_OF, pvValue);
				}

				inline BOOL SetSubPropRightAlignOf(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF, pvValue);
				}

				inline BOOL SetSubPropBottomAlignOf(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_BOTTOM_ALIGN_OF, pvValue);
				}

				inline BOOL SetSubPropCenterParentHorizontal(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_H, pvValue);
				}

				inline BOOL SetSubPropCenterParentVertical(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_V, pvValue);
				}
			};

			class ExTableLayout : public ExBaseLayout
			{
			public:
				ExTableLayout() = default;
				~ExTableLayout() = default;
				ExTableLayout(ExUIbase objBind) :ExBaseLayout(LAYOUT_TYPE_TABLE, objBind)
				{
				}

				inline BOOL SetInfo(INT* aRowHeight, INT cRows, INT* aCellWidth, INT cCells)
				{
					return _layout_table_setinfo(m_handleLayout, aRowHeight, cRows, aCellWidth, cCells);
				}

				inline BOOL SetPropArrayRow(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_TABLE_ARRAY_ROW, pvValue);
				}

				inline BOOL SetPropArrayCell(size_t pvValue)
				{
					return SetLayoutProp(LAYOUT_PROP_TABLE_ARRAY_CELL, pvValue);
				}

				inline BOOL SetSubPropRow(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_TABLE_ROW, pvValue);
				}

				inline BOOL SetSubPropCell(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_TABLE_CELL, pvValue);
				}

				inline BOOL SetSubPropRowSpan(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_TABLE_ROW_SPAN, pvValue);
				}

				inline BOOL SetSubPropCellSpan(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_TABLE_CELL_SPAN, pvValue);
				}

				inline BOOL SetSubPropFill(ExControl obj, size_t pvValue)
				{
					return SetChildProp(obj, LAYOUT_SUBPROP_TABLE_FILL, pvValue);
				}
			};
		}
	}
}