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

bool _matrix_destroy(matrix_s* pMatrix);
bool _matrix_reset(matrix_s* pMatrix);
matrix_s* _matrix_create();
void _matrix_init(void* mx, matrix_s* pMatrix);
void _matrix_update(void* mx, matrix_s* pMatrix);
bool _matrix_translate(matrix_s* pMatrix, float offsetX, float offsetY, int order);
bool _matrix_rotate(matrix_s* pMatrix, float fAngle, int order);
bool _matrix_scale(matrix_s* pMatrix, float scaleX, float scaleY, int order);
