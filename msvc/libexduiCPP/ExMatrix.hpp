#pragma once
#include "ExUIbase.hpp"


namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExMatrix
			{
			public:
				HEXMATRIX m_matrix = nullptr;

				ExMatrix(HEXMATRIX matrix)
				{
					m_matrix = matrix;
				}

				ExMatrix()
				{
					m_matrix = _matrix_create();
				}

				BOOL Destroy()
				{
					return _matrix_destroy(m_matrix);
				}

				BOOL Reset()
				{
					return _matrix_reset(m_matrix);
				}

				BOOL Rotate(FLOAT fAngle)
				{
					return _matrix_rotate(m_matrix, fAngle);
				}

				BOOL Scale(FLOAT scaleX, FLOAT scaleY)
				{
					return _matrix_scale(m_matrix, scaleX, scaleY);
				}

				BOOL Translate(FLOAT offsetX, FLOAT offsetY)
				{
					return _matrix_translate(m_matrix, offsetX, offsetY);
				}
			};
		}
	}
}