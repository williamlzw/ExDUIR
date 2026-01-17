#pragma once
#include "ExDUIR_Func.h"

void test_candlestickchart(HWND hParent);
void GenerateTestData(HEXOBJ hChart);
LRESULT CALLBACK OnCandleClicked(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnCandleHover(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void AddRealtimeData(HEXOBJ hChart, DOUBLE open, DOUBLE high, DOUBLE low, DOUBLE close, DOUBLE volume);
void SetPriceRange(HEXOBJ hChart, DOUBLE minPrice, DOUBLE maxPrice);
void ToggleMA(HEXOBJ hChart, BOOL show);
void ClearChartData(HEXOBJ hChart);