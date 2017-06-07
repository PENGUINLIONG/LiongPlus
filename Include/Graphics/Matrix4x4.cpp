#include "Matrix4x4.hpp"
#include <cmath>
#include <immintrin.h>
#include <stdexcept>

_L_NS_BEG_GRAPHICS

//
// Vector4
//

Float Vector4::Dot(const Vector4& vector) const 
{
	__m256d xy = _mm256_mul_pd(*reinterpret_cast<const __m256d*>(this), *reinterpret_cast<const __m256d*>(&vector));
	__m256d temp = _mm256_hadd_pd(xy, xy);
	__m128d hi128 = _mm256_extractf128_pd(temp, 1);
	__m128d dotproduct = _mm_add_pd(*(__m128d*)&temp, hi128);
	return dotproduct.m128d_f64[0];
}

//
// Matrix4x4
//

Float Matrix4x4::operator()(size_t row, size_t col) const
{
	if (row <= 0 || row > 4 || col <= 0 || row > 4)
		throw std::invalid_argument("Index out of range.");
	return Elements[col - 1][row - 1];
}
Float& Matrix4x4::operator()(size_t row, size_t col)
{
	if (row <= 0 || row > 4 || col <= 0 || row > 4)
		throw std::invalid_argument("Index out of range.");
	return Elements[col - 1][row - 1];
}

Matrix4x4 Matrix4x4::Transpose() const
{
	return
	{ {
		{ Elements[0][0], Elements[1][0], Elements[2][0], Elements[3][0] },
		{ Elements[0][1], Elements[1][1], Elements[2][1], Elements[3][1] },
		{ Elements[0][2], Elements[1][2], Elements[2][2], Elements[3][2] },
		{ Elements[0][3], Elements[1][3], Elements[2][3], Elements[3][3] },
	} };
}

Matrix4x4 Matrix4x4::Transform(const Matrix4x4& matrix) const
{
	auto t = matrix.Transpose();
	auto argVectors = reinterpret_cast<const Vector4*>(&t.Elements);
	auto myVectors = reinterpret_cast<const Vector4*>(&Elements);

	return Matrix4x4
	{ {
		{
			myVectors[0].Dot(argVectors[0]),
			myVectors[1].Dot(argVectors[0]),
			myVectors[2].Dot(argVectors[0]),
			myVectors[3].Dot(argVectors[0]),
		},
		{
			myVectors[0].Dot(argVectors[1]),
			myVectors[1].Dot(argVectors[1]),
			myVectors[2].Dot(argVectors[1]),
			myVectors[3].Dot(argVectors[1]),
		},
		{
			myVectors[0].Dot(argVectors[2]),
			myVectors[1].Dot(argVectors[2]),
			myVectors[2].Dot(argVectors[2]),
			myVectors[3].Dot(argVectors[2]),
		},
		{
			myVectors[0].Dot(argVectors[3]),
			myVectors[1].Dot(argVectors[3]),
			myVectors[2].Dot(argVectors[3]),
			myVectors[3].Dot(argVectors[3]),
		},
	} }.Transpose();
}

Vector4 Matrix4x4::Transform(const Vector4& vector) const
{
	auto myVectors = reinterpret_cast<const Vector4*>(&Elements);

	return
	{
		myVectors[0].Dot(vector),
		myVectors[1].Dot(vector),
		myVectors[2].Dot(vector),
		myVectors[3].Dot(vector),
	};
}


namespace MatrixTransform
{
	namespace details
	{
		// Notice the initialization lists are transposed matricies.
		using namespace std;
		inline Matrix4x4 RotateByX(Float deg)
		{
			return { {
				{ 1, 0, 0, 0 },
				{ 0, cos(deg), -sin(deg), 0 },
				{ 0, sin(deg), cos(deg), 0 },
				{ 0, 0, 0, 1 }
				} };
		}
		inline Matrix4x4 RotateByY(Float deg)
		{
			return { {
				{ cos(deg), 0, sin(deg), 0 },
				{ 0, 1, 0, 0 },
				{ -sin(deg), 0, cos(deg), 0 },
				{ 0, 0, 0, 1 }
				} };
		}
		inline Matrix4x4 RotateByZ(Float deg)
		{
			return { {
				{ cos(deg), -sin(deg), 0, 0 },
				{ sin(deg), cos(deg), 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 }
				} };
		}
	}

	Matrix4x4 Translate(Float x, Float y, Float z)
	{
		return { {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ x, y, z, 1 }
			} };
	}

	template<char TAxis>
	Matrix4x4 Rotate(Float deg)
	{
		static_assert(TAxis == 'x' || TAxis == 'X' ||
			TAxis == 'y' || TAxis == 'Y' ||
			TAxis == 'z' || TAxis == 'Z', "Unknown axis to rotate.");
	}
	template<> Matrix4x4 Rotate<'x'>(Float deg) { return details::RotateByX(deg); }
	template<> Matrix4x4 Rotate<'X'>(Float deg) { return details::RotateByX(deg); }
	template<> Matrix4x4 Rotate<'y'>(Float deg) { return details::RotateByY(deg); }
	template<> Matrix4x4 Rotate<'Y'>(Float deg) { return details::RotateByY(deg); }
	template<> Matrix4x4 Rotate<'z'>(Float deg) { return details::RotateByZ(deg); }
	template<> Matrix4x4 Rotate<'Z'>(Float deg) { return details::RotateByZ(deg); }

	Matrix4x4 Scale(Float x, Float y, Float z)
	{
		return { {
			{ x, 0, 0, 0 },
			{ 0, y, 0, 0 },
			{ 0, 0, z, 0 },
			{ 0, 0, 0, 1 }
		} };
	}
}


_L_NS_END_GRAPHICS
