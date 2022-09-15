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

#ifndef UFO_MAP_COLOR_MAP_BASE_H
#define UFO_MAP_COLOR_MAP_BASE_H

// UFO
#include <ufo/map/color/color_node.h>
#include <ufo/map/color/color_predicate.h>

// STL
#include <iostream>

namespace ufo::map
{
template <class Derived>
class ColorMapBase
{
 public:
	//
	// Get color
	//

	[[nodiscard]] constexpr Color color(Node node) const
	{
		return derived().leafNode(node).colorIndex(node.index());
	}

	[[nodiscard]] constexpr Color color(Code code) const
	{
		return derived().leafNode(code).colorIndex(code.index());
	}

	[[nodiscard]] constexpr Color color(Key key) const
	{
		return color(Derived::toCode(key));
	}

	[[nodiscard]] constexpr Color color(Point coord, depth_t depth = 0) const
	{
		return color(derived().toCode(coord, depth));
	}

	[[nodiscard]] constexpr Color color(coord_t x, coord_t y, coord_t z,
	                                    depth_t depth = 0) const
	{
		return color(derived().toCode(x, y, z, depth));
	}

	//
	// Set color
	//

	constexpr void setColor(Node node, Color color, bool propagate = true)
	{
		derived().apply(
		    node,
		    [color](auto& node, index_t const index) { node.setColorIndex(index, color); },
		    [color](auto& node) { node.setColor(color); }, propagate);
	}

	constexpr void setColor(Node node, color_t red, color_t green, color_t blue,
	                        bool propagate = true)
	{
		setColor(node, Color(red, green, blue), propagate);
	}

	constexpr void setColor(Code code, Color color, bool propagate = true)
	{
		derived().apply(
		    code,
		    [color](auto& node, index_t const index) { node.setColorIndex(index, color); },
		    [color](auto& node) { node.setColor(color); }, propagate);
	}

	constexpr void setColor(Code code, color_t red, color_t green, color_t blue,
	                        bool propagate = true)
	{
		setColor(code, Color(red, green, blue), propagate);
	}

	constexpr void setColor(Key key, Color color, bool propagate = true)
	{
		setColor(Derived::toCode(key), color, propagate);
	}

	constexpr void setColor(Key key, color_t red, color_t green, color_t blue,
	                        bool propagate = true)
	{
		setColor(key, Color(red, green, blue), propagate);
	}

	constexpr void setColor(Point coord, Color color, bool propagate = true,
	                        depth_t depth = 0)
	{
		setColor(derived().toCode(coord, depth), color, propagate);
	}

	constexpr void setColor(Point coord, color_t red, color_t green, color_t blue,
	                        bool propagate = true, depth_t depth = 0)
	{
		setColor(coord, Color(red, green, blue), propagate, depth);
	}

	constexpr void setColor(coord_t x, coord_t y, coord_t z, Color color,
	                        bool propagate = true, depth_t depth = 0)
	{
		setColor(derived().toCode(x, y, z, depth), color, propagate);
	}

	constexpr void setColor(coord_t x, coord_t y, coord_t z, color_t red, color_t green,
	                        color_t blue, bool propagate = true, depth_t depth = 0)
	{
		setColor(x, y, z, Color(red, green, blue), propagate, depth);
	}

	//
	// Clear color
	//

	constexpr void clearColor(Node node, bool propagate = true)
	{
		derived().apply(
		    node, [](auto& node, index_t const index) { node.clearColorIndex(index); },
		    [](auto& node) { node.clearColor(); }, propagate);
	}

	constexpr void clearColor(Code code, bool propagate = true)
	{
		derived().apply(
		    code, [](auto& node, index_t const index) { node.clearColorIndex(index); },
		    [](auto& node) { node.clearColor(); }, propagate);
	}

	constexpr void clearColor(Key key, bool propagate = true)
	{
		clearColor(Derived::toCode(key), propagate);
	}

	constexpr void clearColor(Point coord, bool propagate = true, depth_t depth = 0)
	{
		clearColor(derived().toCode(coord, depth), propagate);
	}

	constexpr void clearColor(coord_t x, coord_t y, coord_t z, bool propagate = true,
	                          depth_t depth = 0)
	{
		clearColor(derived().toCode(x, y, z, depth), propagate);
	}

 protected:
	//
	// Derived
	//

	[[nodiscard]] constexpr Derived& derived() { return *static_cast<Derived*>(this); }

	[[nodiscard]] constexpr Derived const& derived() const
	{
		return *static_cast<Derived const*>(this);
	}

	//
	// Initilize root
	//

	void initRoot() { derived().root().clearColorIndex(derived().rootIndex()); }

	//
	// Update node
	//

	template <bool Single, class T>
	void updateNode(ColorNode<Single>& node, index_field_t const indices, T const& children)
	{
		if constexpr (Single) {
			std::array<color_t, children.size()> red;
			std::array<color_t, children.size()> green;
			std::array<color_t, children.size()> blue;
			for (std::size_t i = 0; children.size() != i; ++i) {
				red[i] = children[i].red;
				green[i] = children[i].green;
				blue[i] = children[i].blue;
			}
			node.red = average(red);
			node.green = average(green);
			node.blue = average(blue);
		} else {
			for (index_t index = 0; children.size() != index; ++index) {
				if ((indices >> index) & index_field_t(1)) {
					node.red[index] = average(children[index].red);
					node.green[index] = average(children[index].green);
					node.blue[index] = average(children[index].blue);
				}
			}
		}
	}

	template <class Container>
	[[nodiscard]] static constexpr color_t average(Container const& c)
	{
		return average(std::cbegin(c), std::cend(c));
	}

	template <class InputIt>
	[[nodiscard]] static constexpr color_t average(InputIt first, InputIt last)
	{
		return std::reduce(first, last, 0.0) / double(std::distance(first, last));
	}

	//
	// Input/output (read/write)
	//

	[[nodiscard]] static constexpr DataIdentifier dataIdentifier() noexcept
	{
		return DataIdentifier::COLOR;
	}

	[[nodiscard]] static constexpr bool canReadData(DataIdentifier identifier) noexcept
	{
		return dataIdentifier() == identifier;
	}

	template <class InputIt>
	[[nodiscard]] static constexpr uint8_t isSingle() noexcept
	{
		using typename std::iterator_traits<InputIt>::value_type;
		using typename value_type::node_type;
		if constexpr (std::is_base_of_v(ColorNode<true>, node_type)) {
			return 1;
		} else {
			return 0;
		}
	}

	template <class OutputIt>
	void readNodes(std::istream& in, OutputIt first, OutputIt last)
	{
		uint8_t single;
		in.read(reinterpret_cast<char*>(&single), sizeof(single));

		std::size_t num_nodes = 3 * std::distance(first, last);
		if (!single) {
			num_nodes *= 8;
		}

		auto data = std::make_unique<color_t[]>(num_nodes);
		in.read(reinterpret_cast<char*>(data.get()),
		        num_nodes * sizeof(typename decltype(data)::element_type));

		auto const d = data.get();
		if constexpr (isSingle<OutputIt>()) {
			if (single) {
				for (std::size_t i = 0; first != last; ++first, i += 3) {
					first->node.red = *(d + i);
					first->node.green = *(d + i + 1);
					first->node.blue = *(d + i + 2);
				}
			} else {
				for (std::size_t i = 0; first != last; ++first, i += 24) {
					first->node.red = average(d + i, d + i + 8);
					first->node.green = average(d + i + 8, d + i + 16);
					first->node.blue = average(d + i + 16, d + i + 24);
				}
			}
		} else {
			if (single) {
				for (std::size_t i = 0; first != last; ++first, i += 3) {
					first->node.red.fill(*(d + i));
					first->node.green.fill(*(d + i + 1));
					first->node.blue.fill(*(d + i + 2));
				}
			} else {
				for (std::size_t i = 0; first != last; ++first, i += 24) {
					if (std::numeric_limits<index_field_t>::max() == first->index_field) {
						std::copy(d + i, d + i + 8, first->node.red.data());
						std::copy(d + i + 8, d + i + 16, first->node.green.data());
						std::copy(d + i + 16, d + i + 24, first->node.blue.data());
					} else {
						for (std::size_t index = 0; first->node.red.size() != index; ++index) {
							if ((first.index_field >> index) & index_field_t(1)) {
								first->node.red[index] = *(d + i + index);
								first->node.green[index] = *(d + i + index + 8);
								first->node.blue[index] = *(d + i + index + 16);
							}
						}
					}
				}
			}
		}
	}

	template <class InputIt>
	void writeNodes(std::ostream& out, InputIt first, InputIt last)
	{
		constexpr uint8_t const single = isSingle<InputIt>();
		out.write(reinterpret_cast<char const*>(&single), sizeof(single));

		std::size_t num_nodes = 3 * std::distance(first, last);
		if constexpr (!single) {
			num_nodes *= 8;
		}

		auto data = std::make_unique<color_t[]>(num_nodes);
		auto d = data.get();
		if constexpr (single) {
			for (std::size_t i = 0; first != last; ++first, i += 3) {
				*(d + i) = first->node.red;
				*(d + i + 1) = first->node.green;
				*(d + i + 2) = first->node.blue;
			}
		} else {
			for (std::size_t i = 0; first != last; ++first, i += 24) {
				std::copy(std::cbegin(first->node.red), std::cend(first->node.red), d + i);
				std::copy(std::cbegin(first->node.green), std::cend(first->node.green),
				          d + i + 8);
				std::copy(std::cbegin(first->node.blue), std::cend(first->node.blue), d + i + 16);
			}
		}

		out.write(reinterpret_cast<char const*>(data.get()),
		          num_nodes * sizeof(typename decltype(data)::element_type));
	}
};
}  // namespace ufo::map

#endif  // UFO_MAP_COLOR_MAP_BASE_H