#pragma once
#include "help_ex.h"

struct matrix_s
{
	float m11_;
	float m12_;
	float m21_;
	float m22_;
	float dx_;
	float dy_;
};

bool _matrix_destroy(void* pMatrix);
bool _matrix_reset(void* pMatrix);
void* _matrix_create();
void _matrix_init(void* mx, void* pMatrix);
void _matrix_update(void* mx, void* pMatrix);
bool _matrix_translate(void* pMatrix, float offsetX, float offsetY, int order);
bool _matrix_rotate(void* pMatrix, float fAngle, int order);
bool _matrix_scale(void* pMatrix, float scaleX, float scaleY, int order);
