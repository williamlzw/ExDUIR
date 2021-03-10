#include "Matrix_ex.h"

bool _matrix_destroy(void* pMatrix)
{
	return Ex_MemFree(pMatrix);
}

bool _matrix_reset(void* pMatrix)
{
	bool ret = false;
	if (!IsBadReadPtr(pMatrix, sizeof(matrix_s)))
	{
		RtlZeroMemory(pMatrix, sizeof(matrix_s));
		((matrix_s*)pMatrix)->m11_ = 1065353216;
		((matrix_s*)pMatrix)->m22_ = 1065353216;
		ret = true;
	}
	return ret;
}

void* _matrix_create()
{
	void* pmx = Ex_MemAlloc(sizeof(matrix_s));
	if (pmx != 0)
	{
		_matrix_reset(pmx);
	}
	return pmx;
}

void _matrix_init(void* mx, void* pMatrix)
{
	//GdipSetMatrixElements(mx, ((matrix_s*)pMatrix)->m11_, ((matrix_s*)pMatrix)->m12_, ((matrix_s*)pMatrix)->m21_, ((matrix_s*)pMatrix)->m22_, ((matrix_s*)pMatrix)->dx_, ((matrix_s*)pMatrix)->dy_);
}

void _matrix_update(void* mx, void* pMatrix)
{
	//GdipGetMatrixElements(mx, pMatrix);
}

bool _matrix_translate(void* pMatrix, float offsetX, float offsetY, int order)
{
	bool ret = false;
	if (!IsBadReadPtr(pMatrix, sizeof(matrix_s)))
	{
		/*_matrix_init(g_Ri.pMatrix, pMatrix);
		GdipTranslateMatrix(g_Ri.pMatrix, offsetX, offsetY, order);
		_matrix_update(g_Ri.pMatrix, pMatrix);
		ret = true;*/
	}
	return ret;
}

bool _matrix_rotate(void* pMatrix, float fAngle, int order)
{
	bool ret = false;
	if (!IsBadReadPtr(pMatrix, sizeof(matrix_s)))
	{
		/*_matrix_init(g_Ri.pMatrix, pMatrix);
		GdipRotateMatrix(g_Ri.pMatrix, fAngle, order);
		_matrix_update(g_Ri.pMatrix, pMatrix);
		ret = true;*/
	}
	return ret;
}

bool _matrix_scale(void* pMatrix, float scaleX, float scaleY, int order)
{
	bool ret = false;
	/*if (!IsBadReadPtr(pMatrix, sizeof(matrix_s)))
	{
		_matrix_init(g_Ri.pMatrix, pMatrix);
		GdipScaleMatrix(g_Ri.pMatrix, scaleX, scaleY, order);
		_matrix_update(g_Ri.pMatrix, pMatrix);
		ret = true;
	}*/
	return ret;
}