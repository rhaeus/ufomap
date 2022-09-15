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

#ifndef UFO_MAP_COLOR_NODE_H
#define UFO_MAP_COLOR_NODE_H

// UFO
#include <ufo/algorithm/algorithm.h>
#include <ufo/map/color/color.h>

// STL
#include <array>

namespace ufo::map
{
template <bool Single = false>
struct ColorNode {
	// Data
	std::conditional_t<Single, color_t, std::array<color_t, 8>> red;
	std::conditional_t<Single, color_t, std::array<color_t, 8>> green;
	std::conditional_t<Single, color_t, std::array<color_t, 8>> blue;

	//
	// Fill
	//

	void fill(ColorNode const parent, index_t const index)
	{
		if constexpr (Single) {
			red = other.red;
			green = other.green;
			blue = other.blue;
		} else {
			red.fill(other.red[index]);
			green.fill(other.green[index]);
			blue.fill(other.blue[index]);
		}
	}

	//
	// Is collapsible
	//

	[[nodiscard]] constexpr bool isCollapsible(ColorNode const parent,
	                                           index_t const index) const
	{
		if constexpr (Single) {
			return red == parent.red && green == parent.green && blue == parent.blue;
		} else {
			return all_of(red, [r = parent.red[index]](auto const c) { return c == r; }) &&
			       all_of(green, [g = parent.green[index]](auto const c) { return c == g; }) &&
			       all_of(blue, [b = parent.blue[index]](auto const c) { return c == b; });
		}
	}

	//
	// Get color
	//

	constexpr Color colorIndex(index_t const index) const
	{
		if constexpr (Single) {
			return Color(red, green, blue);
		} else {
			return Color(red[index], green[index], blue[index]);
		}
	}

	//
	// Set color
	//

	void setColor(Color const value)
	{
		if constexpr (Single) {
			red = value.red;
			green = value.green;
			blue = value.blue;
		} else {
			red.fill(value.red);
			green.fill(value.green);
			blue.fill(value.blue);
		}
	}

	constexpr void setColorIndex(index_t const index, Color const value)
	{
		if constexpr (Single) {
			setColor(value);
		} else {
			red[index] = value.red;
			green[index] = value.green;
			blue[index] = value.blue;
		}
	}

	//
	// Clear color
	//

	void clearColor()
	{
		if constexpr (Single) {
			red = 0;
			green = 0;
			blue = 0;
		} else {
			red.fill(0);
			green.fill(0);
			blue.fill(0);
		}
	}

	void clearColorIndex(index_t const index)
	{
		if constexpr (Single) {
			clearColor();
		} else {
			red[index] = 0;
			green[index] = 0;
			blue[index] = 0;
		}
	}
};
}  // namespace ufo::map

#endif  // UFO_MAP_COLOR_NODE_H