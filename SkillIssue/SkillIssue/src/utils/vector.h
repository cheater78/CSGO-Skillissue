#pragma once
#include <numbers>
#include <cmath>

struct Vector3
{
	// constructor
	constexpr Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }
	/*
	constexpr Vector3(Vector3& vec) noexcept : x(vec.x), y(vec.y), z(vec.z) {
	}
	constexpr Vector3(const Vector3& vec) noexcept : x(vec.x), y(vec.y), z(vec.z) {
	}
	*/
	// operator overloads
	constexpr const Vector3& operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}

	constexpr const Vector3& operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	constexpr const Vector3& operator/(const float factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}

	constexpr const Vector3& operator*(const float factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}

	// utils
	constexpr const Vector3& ToAngle() const noexcept
	{
		return Vector3{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / 3.1415926535f),
			std::atan2(y, x) * (180.0f / 3.1415926535f),
			0.0f };
	}
	/*
	const float len() const noexcept {
		return sqrt(x*x + y*y + z*z);
	}

	const float dist(const Vector3& to) const noexcept {
		return (*this - to).len();
	}

	constexpr const Vector3 normAng() const noexcept {
		Vector3 out(*this);

		if (out.y <= -180)
			out.y = 360 + x;
		else if (out.y > 180)
			out.y = -360 + x;

		if (out.x < -89)
			out.x = -89;
		else if (out.x > 89)
			out.x = 89;

		if (out.z != 0)
			out.z = 0;
		return out;
	}

	const float angDist(const Vector3 other) const noexcept {
		return ((*this).normAng() - (other).normAng()).normAng().len();
	}
	*/
	constexpr const bool IsZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	const void debug() const noexcept {
		std::cout << "{ " << x << " ; " << y << " ; " << z << " }" << std::endl;
	}


	static constexpr Vector3 CalculateAngle(
		const Vector3& localPosition,
		const Vector3& enemyPosition,
		const Vector3& viewAngles) noexcept
	{
		return ((enemyPosition - localPosition).ToAngle() - viewAngles);
	}

	

	// struct data
	float x, y, z;
};
