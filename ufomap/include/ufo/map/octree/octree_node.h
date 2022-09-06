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

#ifndef UFO_MAP_OCTREE_OCTREE_NODE_H
#define UFO_MAP_OCTREE_OCTREE_NODE_H

// UFO
#include <ufo/map/types.h>

// STL
#include <array>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace ufo::map
{
template <class Data>
struct OctreeLeafNode : Data {
	bool operator==(OctreeLeafNode const& rhs) const
	{
		return static_cast<Data const&>(rhs) == static_cast<Data const&>(*this);
	}

	bool operator!=(OctreeLeafNode const& rhs) const { return !(*this == rhs); }

	//
	// Fill
	//

	void fill(OctreeLeafNode const& other, index_t index)
	{
		if (index_field_t(1) & (other.leaf_ >> index)) {
			setLeaf();
		} else {
			resetLeaf();
		}

		if (index_field_t(1) & (other.modified_ >> index)) {
			setModified();
		} else {
			resetModified();
		}
	}

	//
	// Is leaf
	//

	constexpr index_field_t leaf() const noexcept { return leaf_; }

	constexpr index_field_t leaf(index_field_t indices) const noexcept
	{
		return leaf_ & indices;
	}

	constexpr bool leafIndex(index_t index) const noexcept
	{
		return (leaf_ >> index) & index_t(1);
	}

	constexpr bool allLeaf() const noexcept
	{
		return std::numeric_limits<index_field_t>::max() == leaf_;
	}

	constexpr bool anyLeaf() const noexcept { return 0 != leaf_; }

	constexpr bool noneLeaf() const noexcept { return 0 == leaf_; }

	//
	// Set leaf
	//

	constexpr void setLeaf() noexcept { leaf_ = std::numeric_limits<index_field_t>::max(); }

	constexpr void setLeaf(index_field_t indices) noexcept { leaf_ |= indices; }

	constexpr void setLeafIndex(index_t index) noexcept { leaf_ |= index_t(1) << index; }

	//
	// Reset leaf
	//

	constexpr void resetLeaf() noexcept { leaf_ = 0; }

	constexpr void resetLeaf(index_field_t indices) noexcept { leaf_ &= ~indices; }

	constexpr void resetLeafIndex(index_t index) noexcept
	{
		leaf_ &= ~(index_t(1) << index);
	}

	//
	// Is modified
	//

	constexpr index_field_t modified() const noexcept { return modified_; }

	constexpr index_field_t modified(index_field_t indices) const noexcept
	{
		return modified_ & indices;
	}

	constexpr bool modifiedIndex(index_t index) const noexcept
	{
		return (modified_ >> index) & index_t(1);
	}

	constexpr bool allModified() const noexcept
	{
		return std::numeric_limits<index_field_t>::max() == modified_;
	}

	constexpr bool anyModified() const noexcept { return 0 != modified_; }

	constexpr bool noneModified() const noexcept { return 0 == modified_; }

	//
	// Set modified
	//

	constexpr void setModified() noexcept
	{
		modified_ = std::numeric_limits<index_field_t>::max();
	}

	constexpr void setModified(index_field_t index_field) noexcept
	{
		modified_ |= index_field;
	}

	constexpr void setModifiedIndex(index_t index) noexcept
	{
		modified_ |= index_t(1) << index;
	}

	//
	// Reset modified
	//

	constexpr void resetModified() noexcept { modified_ = 0; }

	constexpr void resetModified(index_field_t index_field) noexcept
	{
		modified_ &= ~index_field;
	}

	constexpr void resetModifiedIndex(index_t index) noexcept
	{
		modified_ &= ~(index_t(1) << index);
	}

 private:
	// Indicates whether this is a leaf node (has no children) or not. If true then the
	// children are not valid and should not be accessed
	index_field_t leaf_;
	// Indicates whether this node has to be updated (get information from children and/or
	// update indicators). Useful when propagating information up the tree
	index_field_t modified_;
};

template <class LeafNode>
struct OctreeInnerNode : LeafNode {
	using InnerNodeBlock = std::array<OctreeInnerNode, 8>;
	using LeafNodeBlock = std::array<LeafNode, 8>;

	// Pointer to children
	union {
		InnerNodeBlock* inner_children = nullptr;
		LeafNodeBlock* leaf_children;
	};
};
}  // namespace ufo::map

#endif  // UFO_MAP_OCTREE_OCTREE_NODE_H