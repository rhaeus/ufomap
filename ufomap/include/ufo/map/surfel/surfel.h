/*!
 * UFOMap: An Efficient Probabilistic 3D Mapping Framework That Embraces the Unknown
 *
 * @author Daniel Duberg (dduberg@kth.se)
 * @see https://github.com/UnknownFreeOccupied/ufomap
 * @version 1.0
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
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

#ifndef UFO_MAP_SURFEL_H
#define UFO_MAP_SURFEL_H

// UFO
#include <ufo/math/vector3.h>

// STL
#include <cstdint>
#include <type_traits>

namespace ufo::map
{
template <typename T = float,
          typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
struct Surfel {
	using scalar_t = T;

	constexpr Surfel() = default;

	constexpr Surfel(math::Vector3<scalar_t> point) : num_points_(1), sum_(point) {}

	template <class InputIt>
	constexpr Surfel(InputIt first, InputIt last) : num_points_(std::distance(first, last))
	{
		if (first == last) {
			return;
		}

		for (; first != last; ++first) {
			sum_ += *first;

			sum_squares_[0] = (*first)[0] * (*first)[0];
			sum_squares_[1] = (*first)[0] * (*first)[1];
			sum_squares_[2] = (*first)[0] * (*first)[2];
			sum_squares_[3] = (*first)[1] * (*first)[1];
			sum_squares_[4] = (*first)[1] * (*first)[2];
			sum_squares_[5] = (*first)[2] * (*first)[2];
		}

		scalar_t n = scalar_t(1) / scalar_t(num_points_);

		sum_squares_[0] -= sum_[0] * sum_[0] * n;
		sum_squares_[1] -= sum_[0] * sum_[1] * n;
		sum_squares_[2] -= sum_[0] * sum_[2] * n;
		sum_squares_[3] -= sum_[1] * sum_[1] * n;
		sum_squares_[4] -= sum_[1] * sum_[2] * n;
		sum_squares_[5] -= sum_[2] * sum_[2] * n;
	}

	constexpr Surfel(std::initializer_list<math::Vector3<scalar_t>> points)
	    : Surfel(std::begin(points), std::end(points))
	{
	}

	constexpr Surfel(Surfel const& other) = default;

	constexpr Surfel(Surfel&& other) = default;

	constexpr Surfel& operator=(Surfel const& rhs) = default;

	constexpr Surfel& operator=(Surfel&& rhs) = default;

	constexpr bool operator==(Surfel const& rhs) const
	{
		return num_points_ == rhs.num_points_ && sum_ == rhs.sum_ &&
		       sum_squares_ == rhs.sum_squares_;
	}

	constexpr bool operator!=(Surfel const& rhs) const { return !(*this == rhs); }

	//
	// Empty
	//

	[[nodiscard]] constexpr bool empty() const { return 0 == num_points_; }

	//
	// Add surfel
	//

	Surfel& operator+=(Surfel const& rhs)
	{
		addSurfel(rhs);
		return *this;
	}

	friend Surfel operator+(Surfel lhs, Surfel const& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	constexpr void addSurfel(Surfel const& other)
	{
		if (0 == num_points_) {
			num_points_ = other.num_points_;
			sum_ = other.sum_;
			sum_squares_ = other.sum_squares_;
		} else {
			scalar_t const n = num_points_;
			scalar_t const n_o = other.num_points_;

			auto const alpha = scalar_t(1) / (n * n_o * (n + n_o));
			auto const beta = (sum_ * n_o) - (other.sum_ * n);

			num_points_ += other.num_points_;
			sum_ += other.sum_;

			sum_squares_[0] += other.sum_squares_[0] + alpha * beta[0] * beta[0];
			sum_squares_[1] += other.sum_squares_[1] + alpha * beta[0] * beta[1];
			sum_squares_[2] += other.sum_squares_[2] + alpha * beta[0] * beta[2];
			sum_squares_[3] += other.sum_squares_[3] + alpha * beta[1] * beta[1];
			sum_squares_[4] += other.sum_squares_[4] + alpha * beta[1] * beta[2];
			sum_squares_[5] += other.sum_squares_[5] + alpha * beta[2] * beta[2];
		}
	}

	//
	// Remove surfel
	//

	Surfel& operator-=(Surfel const& rhs)
	{
		removeSurfel(rhs);
		return *this;
	}

	friend Surfel operator-(Surfel lhs, Surfel const& rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	constexpr void removeSurfel(Surfel const& other)
	{
		if (other.num_points_ >= num_points_) {
			clear();
			return;
		}

		num_points_ -= other.num_points_;
		sum_ -= other.sum_;

		scalar_t const n = num_points_;
		scalar_t const n_o = other.num_points_;

		auto const& alpha = scalar_t(1) / (n * n_o * (n + n_o));
		auto const& beta = (sum_ * n_o) - (other.sum_ * n);

		sum_squares_[0] -= other.sum_squares_[0] - alpha * beta[0] * beta[0];
		sum_squares_[1] -= other.sum_squares_[1] - alpha * beta[0] * beta[1];
		sum_squares_[2] -= other.sum_squares_[2] - alpha * beta[0] * beta[2];
		sum_squares_[3] -= other.sum_squares_[3] - alpha * beta[1] * beta[1];
		sum_squares_[4] -= other.sum_squares_[4] - alpha * beta[1] * beta[2];
		sum_squares_[5] -= other.sum_squares_[5] - alpha * beta[2] * beta[2];
	}

	//
	// Add point
	//

	constexpr void addPoint(math::Vector3<scalar_t> point)
	{
		switch (num_points_) {
			case 0:
				num_points_ = 1;
				sum_ = point;
				return;
			case 1:
				++num_points_;
				sum_ += point;
			default:
				scalar_t const n = num_points_;

				auto const alpha = scalar_t(1) / (n * (n + scalar_t(1)));
				auto const beta = (sum_ - (point * n));

				++num_points_;
				sum_ += point;

				sum_squares_[0] += alpha * beta[0] * beta[0];
				sum_squares_[1] += alpha * beta[0] * beta[1];
				sum_squares_[2] += alpha * beta[0] * beta[2];
				sum_squares_[3] += alpha * beta[1] * beta[1];
				sum_squares_[4] += alpha * beta[1] * beta[2];
				sum_squares_[5] += alpha * beta[2] * beta[2];
				break;
		}
	}

	template <class InputIt>
	constexpr void addPoint(InputIt first, InputIt last)
	{
		if (empty()) {
			// FIXME: Make into a function
			if (first == last) {
				return;
			}

			num_points_ = std::distance(first, last);

			for (; first != last; ++first) {
				sum_ += *first;

				sum_squares_[0] = (*first)[0] * (*first)[0];
				sum_squares_[1] = (*first)[0] * (*first)[1];
				sum_squares_[2] = (*first)[0] * (*first)[2];
				sum_squares_[3] = (*first)[1] * (*first)[1];
				sum_squares_[4] = (*first)[1] * (*first)[2];
				sum_squares_[5] = (*first)[2] * (*first)[2];
			}

			scalar_t n = scalar_t(1) / scalar_t(num_points_);

			sum_squares_[0] -= sum_[0] * sum_[0] * n;
			sum_squares_[1] -= sum_[0] * sum_[1] * n;
			sum_squares_[2] -= sum_[0] * sum_[2] * n;
			sum_squares_[3] -= sum_[1] * sum_[1] * n;
			sum_squares_[4] -= sum_[1] * sum_[2] * n;
			sum_squares_[5] -= sum_[2] * sum_[2] * n;
		} else {
			// FIXME: Improve
			std::for_each(first, last, [this](auto&& p) { addPoint(p); });
		}
	}

	constexpr void addPoint(std::initializer_list<math::Vector3<scalar_t>> points)
	{
		addPoint(std::begin(points), std::end(points));
	}

	//
	// Remove point
	//

	constexpr void removePoint(math::Vector3<scalar_t> point)
	{
		switch (num_points_) {
			case 0:
				return;
			case 1:
				clear();
				return;
			default:
				--num_points_;
				sum_ -= point;

				scalar_t const n = num_points_;

				auto const alpha = scalar_t(1) / (n * (n + scalar_t(1)));
				auto const beta = (sum_ - (point * n));

				sum_squares_[0] -= alpha * beta[0] * beta[0];
				sum_squares_[1] -= alpha * beta[0] * beta[1];
				sum_squares_[2] -= alpha * beta[0] * beta[2];
				sum_squares_[3] -= alpha * beta[1] * beta[1];
				sum_squares_[4] -= alpha * beta[1] * beta[2];
				sum_squares_[5] -= alpha * beta[2] * beta[2];
		}
	}

	template <class InputIt>
	constexpr void removePoint(InputIt first, InputIt last)
	{
		// FIXME: Improve
		std::for_each(first, last, [this](auto&& p) { removePoint(p); });
	}

	constexpr void removePoint(std::initializer_list<math::Vector3<scalar_t>> points)
	{
		removePoint(std::begin(points), std::end(points));
	}

	//
	// Clear
	//

	constexpr void clear()
	{
		num_points_ = 0;
		sum_ = math::Vector3<scalar_t>();
		sum_squares_ = {0, 0, 0, 0, 0, 0};
	}

	//
	// Get mean
	//

	constexpr math::Vector3<scalar_t> getMean() const
	{
		return sum_ / scalar_t(num_points_);
	}

	//
	// Get normal
	//

	constexpr math::Vector3<scalar_t> getNormal() const
	{
		return getEigenVectors()[0].normalize();
	}

	constexpr scalar_t getPlanarity() const
	{
		auto const e = getEigenValues();  // FIXME: Normalized?
		return scalar_t(2) * (e[1] - e[0]) / (e[0] + e[1] + e[2]);
	}

	//
	// Get covariance
	//

	constexpr std::array<std::array<scalar_t, 3>, 3> getCovariance() const
	{
		using as = std::array<scalar_t, 3>;
		using cov = std::array<as, 3>;

		scalar_t const f = scalar_t(1) / (scalar_t(num_points_) - scalar_t(1));
		return cov{as{f * sum_squares_[0], f * sum_squares_[1], f * sum_squares_[2]},
		           as{f * sum_squares_[1], f * sum_squares_[3], f * sum_squares_[4]},
		           as{f * sum_squares_[2], f * sum_squares_[4], f * sum_squares_[5]}};
	}

	//
	// Get eigenvalues
	//

	constexpr math::Vector3<scalar_t> getEigenValues() const
	{
		return getEigenValues(getSymmetricCovariance());
	}

	//
	// Get eigen vectors
	//

	constexpr std::array<math::Vector3<scalar_t>, 3> getEigenVectors() const
	{
		return getEigenVector(getSymmetricCovariance());
	}

	//
	// Get num points
	//

	constexpr uint32_t getNumPoints() const { return num_points_; }

	//
	// Get sum
	//

	constexpr math::Vector3<scalar_t> getSum() const { return sum_; }

	//
	// Get sum squares
	//

	constexpr std::array<scalar_t, 6> getSumSquares() const { return sum_squares_; }

 private:
	//
	// Get symmetric convariance
	//

	constexpr std::array<scalar_t, 6> getSymmetricCovariance() const
	{
		scalar_t const f = scalar_t(1) / (scalar_t(num_points_) - scalar_t(1));
		return {f * sum_squares_[0], f * sum_squares_[1], f * sum_squares_[2],
		        f * sum_squares_[3], f * sum_squares_[4], f * sum_squares_[5]};
	}

	//
	// Get eigen values
	//

	constexpr math::Vector3<scalar_t> getEigenValues(
	    std::array<std::array<scalar_t, 3>, 3> const& m) const
	{
		auto const x_1 = m[0][0] * m[0][0] + m[1][1] * m[1][1] + m[2][2] * m[2][2] -
		                 m[0][0] * m[1][1] - m[0][0] * m[2][2] - m[1][1] * m[2][2] +
		                 3 * (m[1][0] * m[1][0] + m[0][2] * m[0][2] + m[2][1] * m[2][1]);

		auto const x_2 = -(2 * m[0][0] - m[1][1] - m[2][2]) *
		                     (2 * m[1][1] - m[0][0] - m[2][2]) *
		                     (2 * m[2][2] - m[0][0] - m[1][1]) +
		                 9 * ((2 * m[2][2] - m[0][0] - m[1][1]) * (m[1][0] * m[1][0]) +
		                      (2 * m[1][1] - m[0][0] - m[2][2]) * (m[0][2] * m[0][2]) +
		                      (2 * m[0][0] - m[1][1] - m[2][2]) * (m[2][1] * m[2][1])) -
		                 54 * (m[0][1] * m[1][2] * m[0][2]);

		auto const phi = 0 != x_2
		                     ? std::atan(std::sqrt(4 * x_1 * x_1 * x_1 - (x_2 * x_2)) / x_2)
		                     : scalar_t(M_PI) / 2;

		math::Vector3<scalar_t> e{
		    (m[0][0] + m[1][1] + m[2][2] - 2 * std::sqrt(x_1) * std::cos(phi / 3)) / 3,
		    (m[0][0] + m[1][1] + m[2][2] +
		     2 * std::sqrt(x_1) * std::cos((phi - scalar_t(M_PI)) / 3)) /
		        3,
		    (m[0][0] + m[1][1] + m[2][2] +
		     2 * std::sqrt(x_1) * std::cos((phi + scalar_t(M_PI)) / 3)) /
		        3};

		if (e[0] > e[1]) {
			std::swap(e[0], e[1]);
		}
		if (e[0] > e[2]) {
			std::swap(e[0], e[2]);
		}
		if (e[1] > e[2]) {
			std::swap(e[1], e[2]);
		}

		return e;
	}

	constexpr math::Vector3<scalar_t> getEigenValues(
	    std::array<scalar_t, 6> const& sym_m) const
	{
		auto const x_1 =
		    sym_m[0] * sym_m[0] + sym_m[3] * sym_m[3] + sym_m[5] * sym_m[5] -
		    sym_m[0] * sym_m[3] - sym_m[0] * sym_m[5] - sym_m[3] * sym_m[5] +
		    3 * (sym_m[1] * sym_m[1] + sym_m[2] * sym_m[2] + sym_m[4] * sym_m[4]);

		auto const x_2 = -(2 * sym_m[0] - sym_m[3] - sym_m[5]) *
		                     (2 * sym_m[3] - sym_m[0] - sym_m[5]) *
		                     (2 * sym_m[5] - sym_m[0] - sym_m[3]) +
		                 9 * ((2 * sym_m[5] - sym_m[0] - sym_m[3]) * (sym_m[1] * sym_m[1]) +
		                      (2 * sym_m[3] - sym_m[0] - sym_m[5]) * (sym_m[2] * sym_m[2]) +
		                      (2 * sym_m[0] - sym_m[3] - sym_m[5]) * (sym_m[4] * sym_m[4])) -
		                 54 * (sym_m[1] * sym_m[4] * sym_m[2]);

		auto const phi = 0 != x_2
		                     ? std::atan(std::sqrt(4 * x_1 * x_1 * x_1 - (x_2 * x_2)) / x_2)
		                     : scalar_t(M_PI) / 2;

		math::Vector3<scalar_t> e(
		    (sym_m[0] + sym_m[3] + sym_m[5] - 2 * std::sqrt(x_1) * std::cos(phi / 3)) / 3,
		    (sym_m[0] + sym_m[3] + sym_m[5] +
		     2 * std::sqrt(x_1) * std::cos((phi - scalar_t(M_PI)) / 3)) /
		        3,
		    (sym_m[0] + sym_m[3] + sym_m[5] +
		     2 * std::sqrt(x_1) * std::cos((phi + scalar_t(M_PI)) / 3)) /
		        3);

		if (e[0] > e[1]) {
			std::swap(e[0], e[1]);
		}
		if (e[0] > e[2]) {
			std::swap(e[0], e[2]);
		}
		if (e[1] > e[2]) {
			std::swap(e[1], e[2]);
		}

		return e;
	}

	//
	// Get eigen vectors
	//

	constexpr std::array<math::Vector3<scalar_t>, 3> getEigenVectors(
	    std::array<std::array<scalar_t, 3>, 3> const& m) const
	{
		return getEigenVectors(m, getEigenValues(m));
	}

	constexpr std::array<math::Vector3<scalar_t>, 3> getEigenVectors(
	    std::array<scalar_t, 6> const& sym_m) const
	{
		return getEigenVectors(sym_m, getEigenValues(sym_m));
	}

	constexpr std::array<math::Vector3<scalar_t>, 3> getEigenVectors(
	    std::array<std::array<scalar_t, 3>, 3> const& m,
	    math::Vector3<scalar_t> const& eigen_values) const
	{
		// FIXME: Make sure denominator is not zero

		auto const m_1 = (m[1][0] * (m[2][2] - eigen_values[0]) - m[1][2] * m[2][0]) /
		                 (m[2][0] * (m[1][1] - eigen_values[0]) - m[1][0] * m[2][1]);

		auto const m_2 = (m[1][0] * (m[2][2] - eigen_values[1]) - m[1][2] * m[2][0]) /
		                 (m[2][0] * (m[1][1] - eigen_values[1]) - m[1][0] * m[2][1]);

		auto const m_3 = (m[1][0] * (m[2][2] - eigen_values[2]) - m[1][2] * m[2][0]) /
		                 (m[2][0] * (m[1][1] - eigen_values[2]) - m[1][0] * m[2][1]);

		return {math::Vector3<scalar_t>((eigen_values[0] - m[2][2] - m[2][1] * m_1) / m[2][0],
		                                m_1, 1),
		        math::Vector3<scalar_t>((eigen_values[1] - m[2][2] - m[2][1] * m_2) / m[2][0],
		                                m_2, 1),
		        math::Vector3<scalar_t>((eigen_values[2] - m[2][2] - m[2][1] * m_3) / m[2][0],
		                                m_3, 1)};
	}

	constexpr std::array<math::Vector3<scalar_t>, 3> getEigenVectors(
	    std::array<scalar_t, 6> const& sym_m,
	    math::Vector3<scalar_t> const& eigen_values) const
	{
		// FIXME: Make sure denominator is not zero

		auto const m_1 = (sym_m[1] * (sym_m[5] - eigen_values[0]) - sym_m[4] * sym_m[2]) /
		                 (sym_m[2] * (sym_m[3] - eigen_values[0]) - sym_m[1] * sym_m[4]);

		auto const m_2 = (sym_m[1] * (sym_m[5] - eigen_values[1]) - sym_m[4] * sym_m[2]) /
		                 (sym_m[2] * (sym_m[3] - eigen_values[1]) - sym_m[1] * sym_m[4]);

		auto const m_3 = (sym_m[1] * (sym_m[5] - eigen_values[2]) - sym_m[4] * sym_m[2]) /
		                 (sym_m[2] * (sym_m[3] - eigen_values[2]) - sym_m[1] * sym_m[4]);

		return {math::Vector3<scalar_t>(
		            (eigen_values[0] - sym_m[5] - sym_m[4] * m_1) / sym_m[2], m_1, 1),
		        math::Vector3<scalar_t>(
		            (eigen_values[1] - sym_m[5] - sym_m[4] * m_2) / sym_m[2], m_2, 1),
		        math::Vector3<scalar_t>(
		            (eigen_values[2] - sym_m[5] - sym_m[4] * m_3) / sym_m[2], m_3, 1)};
	}

 private:
	uint32_t num_points_ = 0;
	math::Vector3<scalar_t> sum_;
	std::array<scalar_t, 6> sum_squares_ = {0, 0, 0, 0, 0, 0};
	// FIXME: Save planarity?
	// FIXME: Save first view position or normal?
};
}  // namespace ufo::map

#endif  // UFO_MAP_SURFEL_H