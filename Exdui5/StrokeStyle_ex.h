#pragma once
#include "help_ex.h"
#include "DirectX_ex.h"

void* _strokestyle_create(int startCap, int endCap, int dashCap, int lineJoin, float miterLimit, int dashStyle, float dashOffset);
bool _strokestyle_destroy(void* hstrokestyle);