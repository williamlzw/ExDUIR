#pragma once

void _waveprogressbar_register();
LRESULT CALLBACK _waveprogressbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                       LPARAM lParam);
void             ConvertCurveToBezier(const std::vector<D2D1_POINT_2F>& points, float tension,
                                      std::vector<D2D1_BEZIER_SEGMENT>& beziers);
void             _waveprogressbar_paint(HEXOBJ hObj);
