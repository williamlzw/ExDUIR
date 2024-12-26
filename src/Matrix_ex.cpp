#include "stdafx.h"

BOOL _matrix_destroy(HEXMATRIX pMatrix)
{
    if (!IsBadReadPtr(pMatrix, sizeof(EX_MATRIX)))
    {
        return Ex_MemFree(pMatrix);
    }
    return FALSE;
}

BOOL _matrix_reset(HEXMATRIX pMatrix)
{
    BOOL ret = FALSE;
    if (!IsBadReadPtr(pMatrix, sizeof(EX_MATRIX)))
    {
        RtlZeroMemory(pMatrix, sizeof(EX_MATRIX));
        ((EX_MATRIX *)pMatrix)->m11 = 1.0f;
        ((EX_MATRIX *)pMatrix)->m22 = 1.0f;
        ret = TRUE;
    }
    return ret;
}

HEXMATRIX _matrix_create()
{
    HEXMATRIX pmx = (HEXMATRIX)Ex_MemAlloc(sizeof(EX_MATRIX));
    if (pmx != 0)
    {
        _matrix_reset(pmx);
    }
    return pmx;
}

void _matrix_init(D2D1::Matrix3x2F *mx, HEXMATRIX pMatrix)
{
    mx->m11 = ((EX_MATRIX *)pMatrix)->m11;
    mx->m12 = ((EX_MATRIX *)pMatrix)->m12;
    mx->m21 = ((EX_MATRIX *)pMatrix)->m21;
    mx->m22 = ((EX_MATRIX *)pMatrix)->m22;
    mx->dx = ((EX_MATRIX *)pMatrix)->dx;
    mx->dy = ((EX_MATRIX *)pMatrix)->dy;
}

void _matrix_update(D2D1::Matrix3x2F mx, HEXMATRIX pMatrix)
{
    ((EX_MATRIX *)pMatrix)->m11 = mx.m11;
    ((EX_MATRIX *)pMatrix)->m12 = mx.m12;
    ((EX_MATRIX *)pMatrix)->m21 = mx.m21;
    ((EX_MATRIX *)pMatrix)->m22 = mx.m22;
    ((EX_MATRIX *)pMatrix)->dx = mx.dx;
    ((EX_MATRIX *)pMatrix)->dy = mx.dy;
}

BOOL _matrix_translate(HEXMATRIX pMatrix, FLOAT offsetX, FLOAT offsetY)
{
    BOOL ret = FALSE;
    if (!IsBadReadPtr(pMatrix, sizeof(EX_MATRIX)))
    {
        D2D1::Matrix3x2F mx_index;
        _matrix_init(&mx_index, pMatrix);
        D2D1::Matrix3x2F mx = D2D1::Matrix3x2F::Translation(offsetX, offsetY);
        D2D1::Matrix3x2F mx_mix = mx * mx_index;
        _matrix_update(mx_mix, pMatrix);
        ret = TRUE;
    }
    return ret;
}

BOOL _matrix_rotate(HEXMATRIX pMatrix, FLOAT fAngle)
{
    BOOL ret = FALSE;
    if (!IsBadReadPtr(pMatrix, sizeof(EX_MATRIX)))
    {
        D2D1::Matrix3x2F mx_index;
        _matrix_init(&mx_index, pMatrix);
        D2D1::Matrix3x2F mx = D2D1::Matrix3x2F::Rotation(fAngle);
        D2D1::Matrix3x2F mx_mix = mx * mx_index;
        _matrix_update(mx_mix, pMatrix);
        ret = TRUE;
    }
    return ret;
}

BOOL _matrix_scale(HEXMATRIX pMatrix, FLOAT scaleX, FLOAT scaleY)
{
    BOOL ret = FALSE;
    if (!IsBadReadPtr(pMatrix, sizeof(EX_MATRIX)))
    {
        D2D1::Matrix3x2F mx_index;
        _matrix_init(&mx_index, pMatrix);
        D2D1::Matrix3x2F mx = D2D1::Matrix3x2F::Scale(scaleX, scaleY);
        D2D1::Matrix3x2F mx_mix = mx * mx_index;
        _matrix_update(mx_mix, pMatrix);
        ret = TRUE;
    }
    return ret;
}