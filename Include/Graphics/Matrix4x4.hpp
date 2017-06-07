#pragma once
#include "_"

_L_NS_BEG_GRAPHICS

struct Vector4
{
public:
	Float X, Y, Z, W = 0;
	
	inline Float Dot(const Vector4& vector) const;
};

struct Matrix4x4
{
public:
	Float Elements[4][4];

	Float operator()(size_t row, size_t col) const;
	Float& operator()(size_t row, size_t col);

	// Matrix4x4 Inverse() const;
	Vector4 Transform(const Vector4& vector) const;
	Matrix4x4 Transform(const Matrix4x4& matrix) const;
	Matrix4x4 Transpose() const;

	static constexpr Matrix4x4 Identity()
	{
		return { {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
		} };
	}
};

namespace MatrixTransform
{
	Matrix4x4 Translate(Float x, Float y, Float z = 0);

	template<char TAxis = 'Z'>
	Matrix4x4 Rotate(Float deg);
	template<> Matrix4x4 Rotate<'x'>(Float deg);
	template<> Matrix4x4 Rotate<'X'>(Float deg);
	template<> Matrix4x4 Rotate<'y'>(Float deg);
	template<> Matrix4x4 Rotate<'Y'>(Float deg);
	template<> Matrix4x4 Rotate<'z'>(Float deg);
	template<> Matrix4x4 Rotate<'Z'>(Float deg);

	Matrix4x4 Scale(Float x, Float y, Float z = 1);
}


_L_NS_END_GRAPHICS
