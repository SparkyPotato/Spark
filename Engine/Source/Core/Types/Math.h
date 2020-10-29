// Copyright 2020 SparkyPotato

#pragma once
#include <immintrin.h>

namespace Spark
{
	namespace Math
	{
		uint Absolute(int value);
		float Absolute(float value);

		float SquareRoot(float value);
	}

	/*
		3 dimensional row vector, which utilizes x86 SIMD.
	*/
	class Vector3
	{
	public:
		Vector3();
		explicit Vector3(float all);
		Vector3(float x, float y, float z);
		explicit Vector3(float values[3]);
		Vector3(const Vector3& other);

		Vector3& operator=(const Vector3& other);

		Vector3 operator+(float scalar) const;
		Vector3 operator+(const Vector3& other) const;
		Vector3& operator+=(float scalar);
		Vector3& operator+=(const Vector3& other);

		Vector3 operator-() const;
		Vector3 operator-(float scalar) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3& operator-=(float scalar);
		Vector3& operator-=(const Vector3& other);

		Vector3 operator*(float scalar) const;
		Vector3& operator*=(float scalar);

		Vector3 operator/(float scalar) const;
		Vector3& operator/=(float scalar);

		// Use when you are comparing the length of vectors as it is much faster than square rooting
		float GetLengthSquare() const;
		// Get the exact length of a vector
		float GetLength() const;

		// Normalize vector in place
		Vector3& Normalize();
		// Get the normalized vector
		Vector3 GetNormalized() const;

		static float Dot(const Vector3& left, const Vector3& right);
		static Vector3 Cross(const Vector3& left, const Vector3& right);

		float& X;
		float& Y;
		float& Z;

	private:
		Vector3(__m128 components);

		alignas(16) __m128 m_Components;
	};
}
