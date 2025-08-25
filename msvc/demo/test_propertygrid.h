#pragma once
#include "ExDUIR_Func.h"

LRESULT CALLBACK OnPropertyGridButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                           LPARAM lParam);
LRESULT CALLBACK OnPropertyGridEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnPropertyGridEventBUTTONCLICK(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void             test_propertygrid(HWND hParent);
void PropertyGrid_AddEditItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR content, INT parentIndex, INT editStyle);
void PropertyGrid_AddGroup(HEXOBJ hPropGrid, LPCWSTR title, BOOL expanded);
void PropertyGrid_AddColorPickerItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR colorValue, INT parentIndex);
void PropertyGrid_AddComboBoxItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR* options, INT optionCount, INT parentIndex);
void PropertyGrid_AddDateBoxItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR dateValue, INT parentIndex);
void PropertyGrid_AddButtonItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR buttonText, INT parentIndex);
void PropertyGrid_AddEditButtonItem(HEXOBJ hPropGrid, LPCWSTR title, LPCWSTR buttonText, INT parentIndex, INT type);