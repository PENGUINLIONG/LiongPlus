// File: Matrix4x4.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <immintrin.h>
#include <cmath>
#include "_"

/*
* Reference:
*   Some optimization methods are from https://github.com/LiraNuna/glsl-sse2/blob/master/source/mat4.h#L324.
*/
_L_NS_BEG

_L_NS_BEG_DETAILS
template<typename TFloat>
TFloat
	clamp(TFloat val, TFloat lower, TFloat upper)
{
	return std::fmin(std::fmax(val, lower), upper);
}

template<int TMask> inline
__m128
shuffle(__m128 xmm)
{
	return _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(xmm), TMask));
}
_L_NS_END_DETAILS

//
// Vector4
//

template<typename TFloat = Float>
struct _Vector4;
template<>
struct _Vector4<float>
{
public:
	union { float X, R{0.f}; };
	union { float Y, G{0.f}; };
	union { float Z, B{0.f}; };
	union { float W, A{0.f}; };
	
	// MATLAB style indexing.
	inline
	float
		operator()(size_t pos) const
	{
		switch (pos)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
		}
	}
	inline
	float&
		operator()(size_t pos)
	{
		switch (pos)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
		}
	}
	
	inline
	float
		operator[](size_t pos) const
	{
		switch (pos)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
		}
	}
	inline
	float&
		operator[](size_t pos)
	{
		switch (pos)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
		}
	}


	inline
	float
		Dot(const _Vector4<float>& vector) const
	{
		__m128 xy = _mm_mul_ps(*reinterpret_cast<const __m128*>(this), *reinterpret_cast<const __m128*>(&vector));
		__m128 temp = _mm_hadd_ps(xy, xy);
		__m128 hi64;
		hi64.m128_i32[0] = _mm_extract_ps(temp, 1);
		__m128 dotproduct = _mm_add_ss(*(__m128*)&temp, hi64);
		return dotproduct.m128_f32[0];
	}
	inline
	_Vector4<float>
		Clamp(float lower, float upper) const
	{
		using namespace details;
		return {
			clamp(X, lower, upper),
			clamp(Y, lower, upper),
			clamp(Z, lower, upper),
			clamp(W, lower, upper),
		};
	}
};
template<>
struct _Vector4<double>
{
public:
	union { double X, R{0.}; };
	union { double Y, G{0.}; };
	union { double Z, B{0.}; };
	union { double W, A{0.}; };

	inline
	double
		operator()(size_t pos) const
	{
		switch (pos)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
		}
	}
	inline
	double&
		operator()(size_t pos)
	{
		switch (pos)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
		}
	}

	inline
	double
		operator[](size_t pos) const
	{
		switch (pos)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
		}
	}
	inline
	double&
		operator[](size_t pos)
	{
		switch (pos)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
		}
	}

	inline
	double
		Dot(const _Vector4<double>& vector) const
	{
		__m256d xy = _mm256_mul_pd(*reinterpret_cast<const __m256d*>(this), *reinterpret_cast<const __m256d*>(&vector));
		__m256d temp = _mm256_hadd_pd(xy, xy);
		__m128d hi128 = _mm256_extractf128_pd(temp, 1);
		__m128d dotproduct = _mm_add_sd(*(__m128d*)&temp, hi128);
		return dotproduct.m128d_f64[0];
	}
	inline
	_Vector4<double>
		Clamp(double lower, double upper) const
	{
		using namespace details;
		return {
			clamp(X, lower, upper),
			clamp(Y, lower, upper),
			clamp(Z, lower, upper),
			clamp(W, lower, upper),
		};
	}
};
using Vector4 = _Vector4<>;

//
// Matrix4x4
//

template<typename TFloat = Float> struct _Matrix4x4;
template<>
struct _Matrix4x4<float>
{
public:
	_Vector4<float> Columns[4]{0.f};
	inline
	float
		operator()(size_t row, size_t col) const
	{
		return Columns[col][row];
	}
	inline
	float&
		operator()(size_t row, size_t col)
	{
		return Columns[col][row];
	}

	// Matrix4x4 Inverse() const;
	inline
	_Vector4<float>
		Transform(const _Vector4<float>& vector) const
	{
		return
		{
			Columns[0].Dot(vector),
			Columns[1].Dot(vector),
			Columns[2].Dot(vector),
			Columns[3].Dot(vector),
		};
	}
	inline
	_Matrix4x4<float>
		Transform(const _Matrix4x4<float>& matrix) const
	{
		auto t = matrix.Transpose();

		return { {
			{
				Columns[0].Dot(t.Columns[0]),
				Columns[0].Dot(t.Columns[1]),
				Columns[0].Dot(t.Columns[2]),
				Columns[0].Dot(t.Columns[3]),
			},
			{
				Columns[1].Dot(t.Columns[0]),
				Columns[1].Dot(t.Columns[1]),
				Columns[1].Dot(t.Columns[2]),
				Columns[1].Dot(t.Columns[3]),
			},
			{
				Columns[2].Dot(t.Columns[0]),
				Columns[2].Dot(t.Columns[1]),
				Columns[2].Dot(t.Columns[2]),
				Columns[2].Dot(t.Columns[3]),
			},
			{
				Columns[3].Dot(t.Columns[0]),
				Columns[3].Dot(t.Columns[1]),
				Columns[3].Dot(t.Columns[2]),
				Columns[3].Dot(t.Columns[3]),
			},
			} };
	}
	inline
	_Matrix4x4<float>
		Transpose() const
	{
		return { {
			{ Columns[0][0], Columns[1][0], Columns[2][0], Columns[3][0] },
			{ Columns[0][1], Columns[1][1], Columns[2][1], Columns[3][1] },
			{ Columns[0][2], Columns[1][2], Columns[2][2], Columns[3][2] },
			{ Columns[0][3], Columns[1][3], Columns[2][3], Columns[3][3] },
		} };
	}

	inline
	float
		Determinant() const
	{
		using details::shuffle;
		auto self = (const __m128*) this;

		__m128 r = shuffle<0x39>(self[2]);
		__m128 v1 = _mm_mul_ps(r, self[3]);
		__m128 v2 = _mm_mul_ps(r, shuffle<0x4E>(self[3]));
		__m128 v3 = _mm_mul_ps(r, shuffle<0x93>(self[3]));
		__m128 r1 = _mm_sub_ps(shuffle<0x39>(v2),
			shuffle<0x4E>(v1));
		__m128 r2 = _mm_sub_ps(shuffle<0x4E>(v3), v3);
		__m128 r3 = _mm_sub_ps(v2, shuffle<0x39>(v1));

		v1 = shuffle<0x93>(self[1]);
		v2 = shuffle<0x39>(self[1]);
		v3 = shuffle<0x4E>(self[1]);
		__m128 d = _mm_mul_ps(_mm_add_ps(_mm_add_ps(
			_mm_mul_ps(v2, r1),
			_mm_mul_ps(v3, r2)),
			_mm_mul_ps(v1, r3)), self[0]);
		d = _mm_add_ps(d, shuffle<0x4E>(d));
		d = _mm_sub_ss(d, shuffle<0x11>(d));
		return _mm_cvtss_f32(d);
	}
	inline
	_Matrix4x4<float>
		Inverse() const
	{
		using details::shuffle;
		auto self = (const __m128*) this;

		__m128 f1 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(self[2], self[1], 0xAA),
			shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0xFF))),
			_mm_mul_ps(shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0xAA)),
				_mm_shuffle_ps(self[2], self[1], 0xFF)));
		__m128 f2 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(self[2], self[1], 0x55),
			shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0xFF))),
			_mm_mul_ps(shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0x55)),
				_mm_shuffle_ps(self[2], self[1], 0xFF)));
		__m128 f3 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(self[2], self[1], 0x55),
			shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0xAA))),
			_mm_mul_ps(shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0x55)),
				_mm_shuffle_ps(self[2], self[1], 0xAA)));
		__m128 f4 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(self[2], self[1], 0x00),
			shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0xFF))),
			_mm_mul_ps(shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0x00)),
				_mm_shuffle_ps(self[2], self[1], 0xFF)));
		__m128 f5 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(self[2], self[1], 0x00),
			shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0xAA))),
			_mm_mul_ps(shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0x00)),
				_mm_shuffle_ps(self[2], self[1], 0xAA)));
		__m128 f6 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(self[2], self[1], 0x00),
			shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0x55))),
			_mm_mul_ps(shuffle<0x80>(_mm_shuffle_ps(self[3], self[2], 0x00)),
				_mm_shuffle_ps(self[2], self[1], 0x55)));
		__m128 v1 = shuffle<0xA8>(_mm_shuffle_ps(self[1], self[0], 0x00));
		__m128 v2 = shuffle<0xA8>(_mm_shuffle_ps(self[1], self[0], 0x55));
		__m128 v3 = shuffle<0xA8>(_mm_shuffle_ps(self[1], self[0], 0xAA));
		__m128 v4 = shuffle<0xA8>(_mm_shuffle_ps(self[1], self[0], 0xFF));
		__m128 s1 = _mm_set_ps(-0.0f, 0.0f, -0.0f, 0.0f);
		__m128 s2 = _mm_set_ps(0.0f, -0.0f, 0.0f, -0.0f);
		__m128 i1 = _mm_xor_ps(s1, _mm_add_ps(
			_mm_sub_ps(_mm_mul_ps(v2, f1),
				_mm_mul_ps(v3, f2)),
			_mm_mul_ps(v4, f3)));
		__m128 i2 = _mm_xor_ps(s2, _mm_add_ps(
			_mm_sub_ps(_mm_mul_ps(v1, f1),
				_mm_mul_ps(v3, f4)),
			_mm_mul_ps(v4, f5)));
		__m128 i3 = _mm_xor_ps(s1, _mm_add_ps(
			_mm_sub_ps(_mm_mul_ps(v1, f2),
				_mm_mul_ps(v2, f4)),
			_mm_mul_ps(v4, f6)));
		__m128 i4 = _mm_xor_ps(s2, _mm_add_ps(
			_mm_sub_ps(_mm_mul_ps(v1, f3),
				_mm_mul_ps(v2, f5)),
			_mm_mul_ps(v3, f6)));
		__m128 d = _mm_mul_ps(self[0], _mm_movelh_ps(_mm_unpacklo_ps(i1, i2),
			_mm_unpacklo_ps(i3, i4)));
		d = _mm_add_ps(d, shuffle<0x4E>(d));
		d = _mm_add_ps(d, shuffle<0x11>(d));
		d = _mm_div_ps(_mm_set1_ps(1.0f), d);

		using out_t = struct { __m128 vecs[4]; };

		auto out = out_t{
			_mm_mul_ps(i1, d),
			_mm_mul_ps(i2, d),
			_mm_mul_ps(i3, d),
			_mm_mul_ps(i4, d)
		};
		return *(_Matrix4x4<float>*)&out;
	}
};
template<>
struct _Matrix4x4<double>
{
public:
	_Vector4<double> Columns[4] ={0.};

	inline
	double
		operator()(size_t row, size_t col) const
	{
		return Columns[col][row];
	}
	inline
	double&
		operator()(size_t row, size_t col)
	{
		return Columns[col][row];
	}

	// Matrix4x4 Inverse() const;
	inline
	_Vector4<double>
		Transform(const _Vector4<double>& vector) const
	{
		return
		{
			Columns[0].Dot(vector),
			Columns[1].Dot(vector),
			Columns[2].Dot(vector),
			Columns[3].Dot(vector),
		};
	}
	inline
	_Matrix4x4<double>
		Transform(const _Matrix4x4<double>& matrix) const
	{
		auto t = matrix.Transpose();

		return { {
			{
				Columns[0].Dot(t.Columns[0]),
				Columns[0].Dot(t.Columns[1]),
				Columns[0].Dot(t.Columns[2]),
				Columns[0].Dot(t.Columns[3]),
			},
			{
				Columns[1].Dot(t.Columns[0]),
				Columns[1].Dot(t.Columns[1]),
				Columns[1].Dot(t.Columns[2]),
				Columns[1].Dot(t.Columns[3]),
			},
			{
				Columns[2].Dot(t.Columns[0]),
				Columns[2].Dot(t.Columns[1]),
				Columns[2].Dot(t.Columns[2]),
				Columns[2].Dot(t.Columns[3]),
			},
			{
				Columns[3].Dot(t.Columns[0]),
				Columns[3].Dot(t.Columns[1]),
				Columns[3].Dot(t.Columns[2]),
				Columns[3].Dot(t.Columns[3]),
			},
			} };
	}
	inline
		_Matrix4x4<double>
		Transpose() const
	{
		return { {
			{ Columns[0][0], Columns[1][0], Columns[2][0], Columns[3][0] },
			{ Columns[0][1], Columns[1][1], Columns[2][1], Columns[3][1] },
			{ Columns[0][2], Columns[1][2], Columns[2][2], Columns[3][2] },
			{ Columns[0][3], Columns[1][3], Columns[2][3], Columns[3][3] },
			} };
	}
};
using Matrix4x4 = _Matrix4x4<>;

namespace MatrixTransform
{
	_L_NS_BEG_DETAILS
	// Notice the initialization lists are transposed matricies.
	using namespace std;
	inline
	Matrix4x4
		RotateByX(Float deg)
	{
		return { {
			{ 1, 0, 0, 0 },
			{ 0, cos(deg), -sin(deg), 0 },
			{ 0, sin(deg), cos(deg), 0 },
			{ 0, 0, 0, 1 }
			} };
	}
	inline
	Matrix4x4
		RotateByY(Float deg)
	{
		return { {
			{ cos(deg), 0, sin(deg), 0 },
			{ 0, 1, 0, 0 },
			{ -sin(deg), 0, cos(deg), 0 },
			{ 0, 0, 0, 1 }
			} };
	}
	inline
	Matrix4x4
		RotateByZ(Float deg)
	{
		return { {
			{ cos(deg), -sin(deg), 0, 0 },
			{ sin(deg), cos(deg), 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
			} };
	}
	_L_NS_END_DETAILS

	constexpr
	Matrix4x4
		Identity()
	{
		return { {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
			} };
	}

	constexpr
	Matrix4x4
		Translate(Float x, Float y, Float z = 0)
	{
		return { {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ x, y, z, 1 }
		} };
	}

	template<char TAxis = 'Z'> inline
	Matrix4x4
		Rotate(Float deg)
	{
		static_assert(TAxis == 'x' || TAxis == 'X' ||
			TAxis == 'y' || TAxis == 'Y' ||
			TAxis == 'z' || TAxis == 'Z', "Unknown axis to rotate.");
	}
	template<> inline
	Matrix4x4
		Rotate<'x'>(Float deg) { return details::RotateByX(deg); }
	template<> inline
	Matrix4x4
		Rotate<'X'>(Float deg) { return details::RotateByX(deg); }
	template<> inline
	Matrix4x4
		Rotate<'y'>(Float deg) { return details::RotateByY(deg); }
	template<> inline
	Matrix4x4
		Rotate<'Y'>(Float deg) { return details::RotateByY(deg); }
	template<> inline
	Matrix4x4
		Rotate<'z'>(Float deg) { return details::RotateByZ(deg); }
	template<> inline
	Matrix4x4
		Rotate<'Z'>(Float deg) { return details::RotateByZ(deg); }

	constexpr
	Matrix4x4
		Scale(Float x, Float y, Float z = (Float)1.)
	{
		return { {
			{ x, 0, 0, 0 },
			{ 0, y, 0, 0 },
			{ 0, 0, z, 0 },
			{ 0, 0, 0, 1 }
			} };
	}
	
	constexpr
	Matrix4x4
		Projection(Float width, Float height, Float near, Float far)
	{
		return { {
			{ 1 / width, 0, 0, 0 },
			{ 0, 1 / height, 0, 0 },
			{ 0, 0, -2 / (far - near), -(far + near) / (far - near) },
			{ 0, 0, 0, 1 }
			} };
	}
}

_L_NS_END
