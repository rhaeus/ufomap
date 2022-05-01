/*
 * UFOMap: An Efficient Probabilistic 3D Mapping Framework That Embraces the Unknown
 *
 * @author D. Duberg, KTH Royal Institute of Technology, Copyright (c) 2020.
 * @see https://github.com/UnknownFreeOccupied/ufomap
 * License: BSD 3
 */

/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2020, D. Duberg, KTH Royal Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UFO_MATH_VECTOR3_H
#define UFO_MATH_VECTOR3_H

// STL
#include <stddef.h>

#include <algorithm>
#include <array>
#include <cmath>

namespace ufo::math
{
template <typename T>
struct Vector3 {
	T x = 0;
	T y = 0;
	T z = 0;

	constexpr Vector3() = default;

	constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

	constexpr Vector3 cross(Vector3 const& other) const { return cross(*this, other); }
	
	static constexpr Vector3 cross(Vector3 const& first, Vector3 const& second)
	{
		return Vector3((first.y * second.z) - (first.z * second.y),
		               (first.z * second.x) - (first.x * second.z),
		               (first.x * second.y) - (first.y * second.x));
	}

	constexpr T dot(Vector3 const& other) const { return dot(*this, other); }

	static constexpr T dot(Vector3 const& first, Vector3 const& second)
	{
		return (first.x * second.x) + (first.y * second.y) + (first.z * second.z);
	}

	constexpr T& operator()(size_t idx) { return *(&x + idx); }

	constexpr T const& operator()(size_t idx) const { return *(&x + idx); }

	constexpr T& operator[](size_t idx) { return *(&x + idx); }

	constexpr T const& operator[](size_t idx) const { return *(&x + idx); }

	constexpr T& roll() noexcept { return x; }

	constexpr T const& roll() const noexcept { return x; }

	constexpr T& pitch() noexcept { return y; }

	constexpr T const& pitch() const noexcept { return y; }

	constexpr T& yaw() noexcept { return z; }

	constexpr T const& yaw() const noexcept { return z; }

	constexpr Vector3 operator-() const { return Vector3(-x, -y, -z); }

	constexpr Vector3 operator-(Vector3 const& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	constexpr Vector3 operator-(T value) const
	{
		return Vector3(x - value, y - value, z - value);
	}

	constexpr Vector3 operator+(Vector3 const& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	constexpr Vector3 operator+(T value) const
	{
		return Vector3(x + value, y + value, z + value);
	}

	constexpr Vector3 operator*(Vector3 const& other) const
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	constexpr Vector3 operator*(T value) const
	{
		return Vector3(x * value, y * value, z * value);
	}

	constexpr Vector3 operator/(Vector3 const& other) const
	{
		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	constexpr Vector3 operator/(T value) const
	{
		return Vector3(x / value, y / value, z / value);
	}

	constexpr void operator-=(Vector3 const& other) noexcept
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	constexpr void operator+=(Vector3 const& other) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}

	constexpr void operator*=(Vector3 const& other) noexcept
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
	}

	constexpr void operator/=(Vector3 const& other) noexcept
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
	}

	constexpr void operator-=(T value) noexcept
	{
		x -= value;
		y -= value;
		z -= value;
	}

	constexpr void operator+=(T value) noexcept
	{
		x += value;
		y += value;
		z += value;
	}

	constexpr void operator*=(T value) noexcept
	{
		x *= value;
		y *= value;
		z *= value;
	}

	constexpr void operator/=(T value) noexcept
	{
		x /= value;
		y /= value;
		z /= value;
	}

	constexpr bool operator==(Vector3 const& other) const noexcept
	{
		return x == other.x && y == other.y && z == other.z;
	}

	constexpr bool operator!=(Vector3 const& other) const noexcept
	{
		return !(*this == other);
	}

	constexpr T norm() const { return std::sqrt(squaredNorm()); }

	constexpr T squaredNorm() const noexcept { return (x * x) + (y * y) + (z * z); }

	constexpr Vector3& normalize()
	{
		*this /= norm();
		return *this;
	}

	constexpr Vector3 normalized() const { return Vector3(*this).normalize(); }

	constexpr T angleTo(Vector3 const& other) const
	{
		return std::acos(dot(other) / (norm() * other.norm()));
	}

	constexpr T squaredDistance(Vector3 const& other) const
	{
		T x = x - other.x;
		T y = y - other.y;
		T z = z - other.z;
		return (x * x) + (y * y) + (z * z);
	}

	constexpr T distance(Vector3 const& other) const
	{
		return std::sqrt(squaredDistance(other));
	}

	constexpr T squaredDistanceXY(Vector3 const& other) const
	{
		T x = x - other.x;
		T y = y - other.y;
		return (x * x) + (y * y);
	}

	constexpr T distanceXY(Vector3 const& other) const
	{
		return std::sqrt(squaredDistanceXY(other));
	}

	static constexpr size_t size() noexcept { return 3; }

	constexpr T min() const { return x <= y && x <= z ? x : (y <= z ? y : z); }

	constexpr T min() const { return x >= y && x >= z ? x : (y >= z ? y : z); }

	constexpr size_t minElementIndex() const
	{
		return x <= y && x <= z ? 0 : (y <= z ? 1 : 2);
	}

	constexpr size_t maxElementIndex() const
	{
		return x >= y && x >= z ? 0 : (y >= z ? 1 : 2);
	}

	constexpr Vector3& ceil()
	{
		x = std::ceil(x);
		y = std::ceil(y);
		z = std::ceil(z);
		return *this;
	}

	constexpr Vector3 ceil() const
	{
		return Vector3(std::ceil(x), std::ceil(y), std::ceil(z));
	}

	constexpr Vector3& floor()
	{
		x = std::floor(x);
		y = std::floor(y);
		z = std::floor(z);
		return *this;
	}

	constexpr Vector3 floor() const
	{
		return Vector3(std::floor(x), std::floor(y), std::floor(z));
	}

	constexpr Vector3& trunc()
	{
		x = std::trunc(x);
		y = std::trunc(y);
		z = std::trunc(z);
		return *this;
	}

	constexpr Vector3 trunc() const
	{
		return Vector3(std::trunc(x), std::trunc(y), std::trunc(z));
	}

	constexpr Vector3& round()
	{
		x = std::round(x);
		y = std::round(y);
		z = std::round(z);
		return *this;
	}

	constexpr Vector3 round() const
	{
		return Vector3(std::round(x), std::round(y), std::round(z));
	}

	constexpr Vector3& clamp(Vector3 const& min, Vector3 const& max)
	{
		x = std::clamp(x);
		y = std::clamp(y);
		z = std::clamp(z);
		return *this;
	}

	constexpr Vector3 clamp(Vector3 const& min, Vector3 const& max) const
	{
		return clamp(*this, min, max);
	}

	static constexpr Vector3 clamp(Vector3 const& value, Vector3 const& min,
	                               Vector3 const& max)
	{
		return Vector3(std::clamp(value[0], min[0], max[0]),
		               std::clamp(value[1], min[1], max[1]),
		               std::clamp(value[2], min[2], max[2]));
	}
};

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
}  // namespace ufo::math

#endif  // UFO_MATH_VECTOR3_H