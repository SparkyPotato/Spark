// Copyright 2020 SparkyPotato

#include "Core/Types/Math.h"
#include <math.h>

namespace Spark
{
	namespace Math
	{
		uint Absolute(int value)
		{
			// Branchless
			return value * ((value > 0) - (value < 0));
		}

		float Absolute(float value)
		{
			// Branchless as well
			return value * ((value > 0) - (value < 0));
		}

		float SquareRoot(float value)
		{
			return sqrtf(value);
		}
	}

	Vector3::Vector3() 
		: X(reinterpret_cast<float*>(&m_Components)[0]), Y(reinterpret_cast<float*>(&m_Components)[1]), Z(reinterpret_cast<float*>(&m_Components)[2])
	{
		m_Components = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
	}

	Vector3::Vector3(float all) 
		: X(reinterpret_cast<float*>(&m_Components)[0]), Y(reinterpret_cast<float*>(&m_Components)[1]), Z(reinterpret_cast<float*>(&m_Components)[2])
	{
		m_Components = _mm_set_ps(0.f, all, all, all);
	}

	Vector3::Vector3(float x, float y, float z) 
		: X(reinterpret_cast<float*>(&m_Components)[0]), Y(reinterpret_cast<float*>(&m_Components)[1]), Z(reinterpret_cast<float*>(&m_Components)[2])
	{
		m_Components = _mm_set_ps(0.f, z, y, x);
	}

	Vector3::Vector3(float values[3]) 
		: X(reinterpret_cast<float*>(&m_Components)[0]), Y(reinterpret_cast<float*>(&m_Components)[1]), Z(reinterpret_cast<float*>(&m_Components)[2])
	{
		m_Components = _mm_set_ps(0.f, values[2], values[1], values[0]);
	}

	Vector3::Vector3(const Vector3& other) 
		: X(reinterpret_cast<float*>(&m_Components)[0]), Y(reinterpret_cast<float*>(&m_Components)[1]), Z(reinterpret_cast<float*>(&m_Components)[2])
	{
		m_Components = other.m_Components;
	}

	Vector3::Vector3(__m128 components) 
		: X(reinterpret_cast<float*>(&m_Components)[0]), Y(reinterpret_cast<float*>(&m_Components)[1]), Z(reinterpret_cast<float*>(&m_Components)[2])
	{
		m_Components = components;
	}

	Vector3& Vector3::operator=(const Vector3& other) 
	{
		m_Components = other.m_Components;
		return *this;
	}

	Vector3 Vector3::operator+(float scalar) const 
	{
		__m128 temp = _mm_set_ps(0.f, scalar, scalar, scalar);
		return Vector3(_mm_add_ps(m_Components, temp));
	}

	Vector3 Vector3::operator+(const Vector3& other) const 
	{
		return Vector3(_mm_add_ps(m_Components, other.m_Components));
	}

	Vector3& Vector3::operator+=(float scalar) 
	{
		return *this = *this + scalar;
	}

	Vector3& Vector3::operator+=(const Vector3& other) 
	{
		return *this = *this + other;
	}

	Vector3 Vector3::operator-() const 
	{
		return Vector3(_mm_set_ps(0.f, -Z, -Y, -X));
	}

	Vector3 Vector3::operator-(float scalar) const 
	{
		__m128 temp = _mm_set_ps(0.f, scalar, scalar, scalar);
		return Vector3(_mm_sub_ps(m_Components, temp));
	}

	Vector3 Vector3::operator-(const Vector3& other) const 
	{
		return Vector3(_mm_sub_ps(m_Components, other.m_Components));
	}

	Vector3& Vector3::operator-=(float scalar) 
	{
		return *this = *this - scalar;
	}

	Vector3& Vector3::operator-=(const Vector3& other) 
	{
		return *this = *this - other;
	}

	Vector3 Vector3::operator*(float scalar) const 
	{
		__m128 temp = _mm_set_ps(0.f, scalar, scalar, scalar);
		return Vector3(_mm_mul_ps(m_Components, temp));
	}

	Vector3& Vector3::operator*=(float scalar) 
	{
		return *this = *this * scalar;
	}

	Vector3 Vector3::operator/(float scalar) const 
	{
		__m128 temp = _mm_set_ps(0.f, scalar, scalar, scalar);
		return Vector3(_mm_div_ps(m_Components, temp));
	}

	Vector3& Vector3::operator/=(float scalar) 
	{
		return *this = *this / scalar;
	}

	float Vector3::GetLengthSquare() const
	{
		__m128 square = _mm_mul_ps(m_Components, m_Components);
		auto temp = reinterpret_cast<float*>(&square);

		return temp[0] + temp[1] + temp[2];
	}

	float Vector3::GetLength() const
	{
		__m128 square = _mm_mul_ps(m_Components, m_Components);
		auto temp = reinterpret_cast<float*>(&square);

		return Math::SquareRoot(temp[0] + temp[1] + temp[2]);
	}

	Vector3& Vector3::Normalize()
	{
		*this /= GetLength();
		return *this;
	}

	Vector3 Vector3::GetNormalized() const
	{
		return *this / GetLength();
	}

	float Vector3::Dot(const Vector3& left, const Vector3& right)
	{
		__m128 mul = _mm_mul_ps(left.m_Components, right.m_Components);
		auto temp = reinterpret_cast<float*>(&mul);
		
		return temp[0] + temp[1] + temp[2];
	}

	Vector3 Vector3::Cross(const Vector3& left, const Vector3& right)
	{
		__m128 tmp0 = _mm_shuffle_ps(left.m_Components, left.m_Components, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 tmp1 = _mm_shuffle_ps(right.m_Components, right.m_Components, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 tmp2 = _mm_shuffle_ps(left.m_Components, left.m_Components, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 tmp3 = _mm_shuffle_ps(right.m_Components, right.m_Components, _MM_SHUFFLE(3, 0, 2, 1));

		return _mm_sub_ps(
			_mm_mul_ps(tmp0, tmp1),
			_mm_mul_ps(tmp2, tmp3)
		);
	}
}
