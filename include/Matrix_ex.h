#pragma once

struct EX_MATRIX
{
    FLOAT m11;
    FLOAT m12;
    FLOAT m21;
    FLOAT m22;
    FLOAT dx;
    FLOAT dy;
};

void _matrix_init(D2D1::Matrix3x2F* mx, HEXMATRIX pMatrix);
void _matrix_update(D2D1::Matrix3x2F mx, HEXMATRIX pMatrix);
