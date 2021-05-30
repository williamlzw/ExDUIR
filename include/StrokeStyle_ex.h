#pragma once
#include "help_ex.h"

ID2D1StrokeStyle* _strokestyle_create(D2D1_CAP_STYLE startCap, D2D1_CAP_STYLE endCap, D2D1_CAP_STYLE dashCap, D2D1_LINE_JOIN lineJoin, float miterLimit, D2D1_DASH_STYLE dashStyle, float dashOffset);
bool _strokestyle_destroy(ID2D1StrokeStyle* hstrokestyle);