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

#ifndef UFO_MAP_SEMANTIC_MAP_H
#define UFO_MAP_SEMANTIC_MAP_H

// UFO
#include <ufo/container/range_map.h>
#include <ufo/map/semantic/semantic.h>
#include <ufo/map/semantic/semantic_node.h>
#include <ufo/map/semantic/semantic_predicate.h>
#include <ufo/map/semantic/semantic_propagation.h>
// #include <ufo/map/semantic/semantics.h>
#include <ufo/map/semantic/semantics_reference.h>
#include <ufo/map/semantic/semantic_mapping.h>
#include <ufo/map/types.h>

// STL

namespace ufo::map
{
template <class Derived>
class SemanticMapBase : public SemanticMapping
{
 public:
	//
	// Get semantics
	//

	[[nodiscard]] SemanticsReference semantics(Node node) const
	{
		return SemanticsReference(static_cast<SemanticMapping const&>(*this),
		                          derived().leafNode(node), node.index());
	}

	[[nodiscard]] SemanticsReference semantics(Code code) const
	{
		auto [node, depth] = derived().leafNodeAndDepth(code);
		return SemanticsReference(static_cast<SemanticMapping const&>(*this), node,
		                          code.index(depth));
	}

	[[nodiscard]] SemanticsReference semantics(Key key) const
	{
		return semantics(derived().toCode(key));
	}

	[[nodiscard]] SemanticsReference semantics(Point coord, depth_t depth = 0) const
	{
		return semantics(derived().toCode(coord, depth));
	}

	[[nodiscard]] SemanticsReference semantics(coord_t x, coord_t y, coord_t z,
	                                           depth_t depth = 0) const
	{
		return semantics(derived().toCode(x, y, z, depth));
	}

	//
	// Set semantics
	//

	void setSemantics(Node node, SemanticSet const& semantics, bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, &semantics](auto& node, index_t index) {
			    semanticNode(node).set(index, semantics);
		    },
		    [this, &semantics](auto& node) { semanticNode(node).set(semantics); }, propagate);
	}

	void setSemantics(Code code, SemanticSet const& semantics, bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, &semantics](auto& node, index_t index) {
			    semanticNode(node).set(index, semantics);
		    },
		    [this, &semantics](auto& node) { semanticNode(node).set(semantics); }, propagate);
	}

	void setSemantics(Key key, SemanticSet const& semantics, bool propagate = true)
	{
		setSemantics(derived().toCode(key), semantics, propagate);
	}

	void setSemantics(Point coord, SemanticSet const& semantics, bool propagate = true,
	                  depth_t depth = 0)
	{
		setSemantics(derived().toCode(coord, depth), semantics, propagate);
	}

	void setSemantics(coord_t x, coord_t y, coord_t z, SemanticSet const& semantics,
	                  bool propagate = true, depth_t depth = 0)
	{
		setSemantics(derived().toCode(x, y, z, depth), semantics, propagate);
	}

	//
	// Insert semantics
	//

	void insertSemantics(Node node, label_t label, value_t value, bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, label, value](auto& node, index_t index) {
			    semanticNode(node).insert(index, label, value);
		    },
		    [this, label, value](auto& node) { semanticNode(node).insert(label, value); },
		    propagate);
	}

	void insertSemantics(Code code, label_t label, value_t value, bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, label, value](auto& node, index_t index) {
			    semanticNode(node).insert(index, label, value);
		    },
		    [this, label, value](auto& node) { semanticNode(node).insert(label, value); },
		    propagate);
	}

	void insertSemantics(Key key, label_t label, value_t value, bool propagate = true)
	{
		insertSemantics(derived().toCode(key), label, value, propagate);
	}

	void insertSemantics(Point coord, label_t label, value_t value, bool propagate = true,
	                     depth_t depth = 0)
	{
		insertSemantics(derived().toCode(coord, depth), label, value, propagate);
	}

	void insertSemantics(coord_t x, coord_t y, coord_t z, label_t label, value_t value,
	                     bool propagate = true, depth_t depth = 0)
	{
		insertSemantics(derived().toCode(x, y, z, depth), label, value, propagate);
	}

	void insertSemantics(Node node, Semantic semantic, bool propagate = true)
	{
		insertSemantics(node, semantic.label, semantic.value, propagate);
	}

	void insertSemantics(Code code, Semantic semantic, bool propagate = true)
	{
		insertSemantics(code, semantic.label, semantic.value, propagate);
	}

	void insertSemantics(Key key, Semantic semantic, bool propagate = true)
	{
		insertSemantics(derived().toCode(key), semantic, propagate);
	}

	void insertSemantics(Point coord, Semantic semantic, bool propagate = true,
	                     depth_t depth = 0)
	{
		insertSemantics(derived().toCode(coord, depth), semantic, propagate);
	}

	void insertSemantics(coord_t x, coord_t y, coord_t z, Semantic semantic,
	                     bool propagate = true, depth_t depth = 0)
	{
		insertSemantics(derived().toCode(x, y, z, depth), semantic, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertSemantics(Node node, InputIt first, InputIt last, bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, first, last](auto& node, index_t index) {
			    semanticNode(node).insert(index, first, last);
		    },
		    [this, first, last](auto& node) { semanticNode(node).insert(first, last); }, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertSemantics(Code code, InputIt first, InputIt last, bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, first, last](auto& node, index_t index) {
			    semanticNode(node).insert(index, first, last);
		    },
		    [this, first, last](auto& node) { semanticNode(node).insert(first, last); }, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertSemantics(Key key, InputIt first, InputIt last, bool propagate = true)
	{
		insertSemantics(derived().toCode(key), first, last, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertSemantics(coord_t x, coord_t y, coord_t z, InputIt first, InputIt last,
	                     bool propagate = true, depth_t depth = 0)
	{
		insertSemantics(derived().toCode(x, y, z, depth), first, last, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertSemantics(Point coord, InputIt first, InputIt last, bool propagate = true,
	                     depth_t depth = 0)
	{
		insertSemantics(derived().toCode(coord, depth), first, last, propagate);
	}

	void insertSemantics(Node node, std::initializer_list<Semantic> ilist,
	                     bool propagate = true)
	{
		insertSemantics(node, std::cbegin(ilist), std::cend(ilist), propagate);
	}

	void insertSemantics(Code code, std::initializer_list<Semantic> ilist,
	                     bool propagate = true)
	{
		insertSemantics(code, std::cbegin(ilist), std::cend(ilist), propagate);
	}

	void insertSemantics(Key key, std::initializer_list<Semantic> ilist,
	                     bool propagate = true)
	{
		insertSemantics(derived().toCode(key), ilist, propagate);
	}

	void insertSemantics(Point coord, std::initializer_list<Semantic> ilist,
	                     bool propagate = true, depth_t depth = 0)
	{
		insertSemantics(derived().toCode(coord, depth), ilist, propagate);
	}

	void insertSemantics(coord_t x, coord_t y, coord_t z,
	                     std::initializer_list<Semantic> ilist, bool propagate = true,
	                     depth_t depth = 0)
	{
		insertSemantics(derived().toCode(x, y, z, depth), ilist, propagate);
	}

	//
	// Insert or assign semantics
	//

	void insertOrAssignSemantics(Node node, label_t label, value_t value,
	                             bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, label, value](auto& node, index_t index) {
			    semanticNode(node).insertOrAssign(index, label, value);
		    },
		    [this, label, value](auto& node) { semanticNode(node).insertOrAssign(label, value); },
		    propagate);
	}

	void insertOrAssignSemantics(Code code, label_t label, value_t value,
	                             bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, label, value](auto& node, index_t index) {
			    semanticNode(node).insertOrAssign(index, label, value);
		    },
		    [this, label, value](auto& node) { semanticNode(node).insertOrAssign(label, value); },
		    propagate);
	}

	void insertOrAssignSemantics(Key key, label_t label, value_t value,
	                             bool propagate = true)
	{
		insertOrAssignSemantics(derived().toCode(key), label, value, propagate);
	}

	void insertOrAssignSemantics(Point coord, label_t label, value_t value,
	                             bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(coord, depth), label, value, propagate);
	}

	void insertOrAssignSemantics(coord_t x, coord_t y, coord_t z, label_t label,
	                             value_t value, bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(x, y, z, depth), label, value, propagate);
	}

	void insertOrAssignSemantics(Node node, Semantic semantic, bool propagate = true)
	{
		insertOrAssignSemantics(node, semantic.label, semantic.value, propagate);
	}

	void insertOrAssignSemantics(Code code, Semantic semantic, bool propagate = true)
	{
		insertOrAssignSemantics(code, semantic.label, semantic.value, propagate);
	}

	void insertOrAssignSemantics(Key key, Semantic semantic, bool propagate = true)
	{
		insertOrAssignSemantics(derived().toCode(key), semantic, propagate);
	}

	void insertOrAssignSemantics(Point coord, Semantic semantic, bool propagate = true,
	                             depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(coord, depth), semantic, propagate);
	}

	void insertOrAssignSemantics(coord_t x, coord_t y, coord_t z, Semantic semantic,
	                             bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(x, y, z, depth), semantic, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(Node node, InputIt first, InputIt last,
	                             bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, first, last](auto& node, index_t index) {
			    semanticNode(node).insertOrAssign(index, first, last);
		    },
		    [this, first, last](auto& node) { semanticNode(node).insertOrAssign(first, last); },
		    propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(Code code, InputIt first, InputIt last,
	                             bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, first, last](auto& node, index_t index) {
			    semanticNode(node).insertOrAssign(index, first, last);
		    },
		    [this, first, last](auto& node) { semanticNode(node).insertOrAssign(first, last); },
		    propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(Key key, InputIt first, InputIt last,
	                             bool propagate = true)
	{
		insertOrAssignSemantics(derived().toCode(key), first, last, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(Point coord, InputIt first, InputIt last,
	                             bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(coord, depth), first, last, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(coord_t x, coord_t y, coord_t z, InputIt first,
	                             InputIt last, bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(x, y, z, depth), first, last, propagate);
	}

	void insertOrAssignSemantics(Node node, std::initializer_list<Semantic> ilist,
	                             bool propagate = true)
	{
		insertOrAssignSemantics(node, std::begin(ilist), std::end(ilist), propagate);
	}

	void insertOrAssignSemantics(Code code, std::initializer_list<Semantic> ilist,
	                             bool propagate = true)
	{
		insertOrAssignSemantics(code, std::begin(ilist), std::end(ilist), propagate);
	}

	void insertOrAssignSemantics(Key key, std::initializer_list<Semantic> ilist,
	                             bool propagate = true)
	{
		insertOrAssignSemantics(derived().toCode(key), std::begin(ilist), std::end(ilist), propagate);
	}

	void insertOrAssignSemantics(Point coord, std::initializer_list<Semantic> ilist,
	                             bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(coord, depth), std::begin(ilist), std::end(ilist), propagate);
	}

	void insertOrAssignSemantics(coord_t x, coord_t y, coord_t z,
	                             std::initializer_list<Semantic> ilist,
	                             bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(x, y, z, depth), std::begin(ilist), std::end(ilist), propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(Node node, label_t label, UnaryFunction f,
	                             bool propagate = true)
	{
		derived().apply(
		    node,
		   [this, label, f](auto& node, index_t index) {
			    semanticNode(node).insertOrAssign(index, label, f);
		    },
		    [this, label, f](auto& node) { semanticNode(node).insertOrAssign(label, f); },
		    propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(Code code, label_t label, UnaryFunction f,
	                             bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, label, f](auto& node, index_t index) {
			    semanticNode(node).insertOrAssign(index, label, f);
		    },
		    [this, label, f](auto& node) { semanticNode(node).insertOrAssign(label, f); },
		    propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(Key key, label_t label, UnaryFunction f,
	                             bool propagate = true)
	{
		insertOrAssignSemantics(derived().toCode(key), label, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(Point coord, label_t label, UnaryFunction f,
	                             bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(coord, depth), label, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(coord_t x, coord_t y, coord_t z, label_t label,
	                             UnaryFunction f, bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(x, y, z, depth), label, f, propagate);
	}

	template <class InputIt, class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(Node node, InputIt first, InputIt last, UnaryFunction f,
	                             bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, first, last, f](auto& node, index_t index) {
			    semanticNode(node).insertOrAssign(index, first, last, f);
		    },
		    [this, first, last, f](auto& node) { semanticNode(node).insertOrAssign(first, last, f); },
		    propagate);
	}

	template <class InputIt, class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(Code code, InputIt first, InputIt last, UnaryFunction f,
	                             bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, first, last, f](auto& node, index_t index) {
			    semanticNode(node).insertOrAssign(index, first, last, f);
		    },
		    [this, first, last, f](auto& node) { semanticNode(node).insertOrAssign(first, last, f); },
		    propagate);
	}

	template <class InputIt, class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(Key key, InputIt first, InputIt last, UnaryFunction f,
	                             bool propagate = true)
	{
		insertOrAssignSemantics(derived().toCode(key), first, last, f, propagate);
	}

	template <class InputIt, class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(Point coord, InputIt first, InputIt last, UnaryFunction f,
	                             bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(coord, depth), first, last, f, propagate);
	}

	template <class InputIt, class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssignSemantics(coord_t x, coord_t y, coord_t z, InputIt first,
	                             InputIt last, UnaryFunction f, bool propagate = true,
	                             depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(x, y, z, depth), first, last, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(Node node, std::initializer_list<label_t> ilist,
	                             UnaryFunction f, bool propagate = true)
	{
		insertOrAssignSemantics(node, std::begin(ilist), std::end(ilist), f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(Code code, std::initializer_list<label_t> ilist,
	                             UnaryFunction f, bool propagate = true)
	{
		insertOrAssignSemantics(code, std::begin(ilist), std::end(ilist), f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(Key key, std::initializer_list<label_t> ilist,
	                             UnaryFunction f, bool propagate = true)
	{
		insertOrAssignSemantics(derived().toCode(key), std::begin(ilist), std::end(ilist), f,
		               propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(Point coord, std::initializer_list<label_t> ilist,
	                             UnaryFunction f, bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(coord, depth), std::begin(ilist), std::end(ilist), f,
		               propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssignSemantics(coord_t x, coord_t y, coord_t z,
	                             std::initializer_list<label_t> ilist, UnaryFunction f,
	                             bool propagate = true, depth_t depth = 0)
	{
		insertOrAssignSemantics(derived().toCode(x, y, z, depth), std::begin(ilist), std::end(ilist),
		               f, propagate);
	}

	//
	// Assign
	//

	void assignSemantics(Node node, std::string const& tag, value_t value,
	                     bool propagate = true)
	{
		assignSemantics(node, labels(tag), value);
		// auto ls = labels(tag);
		// derived().apply(
		//     node,
		//     [this, ls, value](auto& node, index_t index) {
		// 	    semanticNode(node).assign(index, ls, value);
		//     },
		//     [this, ls, value](auto& node) { semanticNode(node).assign(ls, value); },
		//     propagate);

	}

	void assignSemantics(Code code, std::string const& tag, value_t value,
	                     bool propagate = true)
	{
		assignSemantics(code, labels(tag), value);
		// auto ls = labels(tag);
		// derived().apply(
		//     code,
		//     [this, ls, value](auto& node, index_t index) {
		// 	    semanticNode(node).assign(index, ls, value);
		//     },
		//     [this, ls, value](auto& node) { semanticNode(node).assign(ls, value); },
		//     propagate);
	}

	void assignSemantics(Key key, std::string const& tag, value_t value,
	                     bool propagate = true)
	{
		assignSemantics(derived().toCode(key), tag, value, propagate);
	}

	void assignSemantics(Point coord, std::string const& tag, value_t value,
	                     bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(coord, depth), tag, value, propagate);
	}

	void assignSemantics(coord_t x, coord_t y, coord_t z, std::string const& tag,
	                     value_t value, bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(x, y, z, depth), tag, value, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Node node, std::string const& tag, UnaryFunction f,
	                     bool propagate = true)
	{
		assignSemantics(node, labels(tag), f);
		// auto ls = labels(tag);
		// derived().apply(
		//     node,
		//     [this, ls, f](auto& node, index_t index) {
		// 	    semanticNode(node).assign(index, ls, f);
		//     },
		//     [this, ls, f](auto& node) { semanticNode(node).assign(ls, f); },
		//     propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Code code, std::string const& tag, UnaryFunction f,
	                     bool propagate = true)
	{
		assignSemantics(code, labels(tag), f);
		// auto ls = labels(tag);
		// derived().apply(
		//     code,
		//     [this, ls, f](auto& node, index_t index) {
		// 	    semanticNode(node).assign(index, ls, f);
		//     },
		//     [this, ls, f](auto& node) { semanticNode(node).assign(ls, f); },
		//     propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Key key, std::string const& tag, UnaryFunction f,
	                     bool propagate = true)
	{
		assignSemantics(derived().toCode(key), tag, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Point coord, std::string const& tag, UnaryFunction f,
	                     bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(coord, depth), tag, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(coord_t x, coord_t y, coord_t z, std::string const& tag,
	                     UnaryFunction f, bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(x, y, z, depth), tag, f, propagate);
	}

	void assignSemantics(Node node, SemanticRange range, value_t value,
	                     bool propagate = true)
	{
		assignSemantics(node, SemanticRangeSet(range), value, propagate);
	}

	void assignSemantics(Code code, SemanticRange range, value_t value,
	                     bool propagate = true)
	{
		assignSemantics(code, SemanticRangeSet(range), value, propagate);
	}

	void assignSemantics(Key key, SemanticRange range, value_t value, bool propagate = true)
	{
		assignSemantics(derived().toCode(key), range, value, propagate);
	}

	void assignSemantics(Point coord, SemanticRange range, value_t value,
	                     bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(coord, depth), range, value, propagate);
	}

	void assignSemantics(coord_t x, coord_t y, coord_t z, SemanticRange range,
	                     value_t value, bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(x, y, z, depth), range, value, propagate);
	}

	void assignSemantics(Node node, SemanticRangeSet const& ranges, value_t value,
	                     bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, ranges, value](auto& node, index_t index) {
			    semanticNode(node).assign(index, ranges, value);
		    },
		    [this, ranges, value](auto& node) { semanticNode(node).assign(ranges, value); },
		    propagate);
	}

	void assignSemantics(Code code, SemanticRangeSet const& ranges, value_t value,
	                     bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, ranges, value](auto& node, index_t index) {
			    semanticNode(node).assign(index, ranges, value);
		    },
		    [this, ranges, value](auto& node) { semanticNode(node).assign(ranges, value); },
		    propagate);
	}

	void assignSemantics(Key key, SemanticRangeSet const& ranges, value_t value,
	                     bool propagate = true)
	{
		assignSemantics(derived().toCode(key), ranges, value, propagate);
	}

	void assignSemantics(Point coord, SemanticRangeSet const& ranges, value_t value,
	                     bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(coord, depth), ranges, value, propagate);
	}

	void assignSemantics(coord_t x, coord_t y, coord_t z, SemanticRangeSet const& ranges,
	                     value_t value, bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(x, y, z, depth), ranges, value, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Node node, SemanticRange range, UnaryFunction f,
	                     bool propagate = true)
	{
		assignSemantics(node, SemanticRangeSet(range), f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Code code, SemanticRange range, UnaryFunction f,
	                     bool propagate = true)
	{
		assignSemantics(code, SemanticRangeSet(range), f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Key key, SemanticRange range, UnaryFunction f,
	                     bool propagate = true)
	{
		assignSemantics(derived().toCode(key), range, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Point coord, SemanticRange range, UnaryFunction f,
	                     bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(coord, depth), range, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(coord_t x, coord_t y, coord_t z, SemanticRange range,
	                     UnaryFunction f, bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(x, y, z, depth), range, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Node node, SemanticRangeSet const& ranges, UnaryFunction f,
	                     bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, ranges, f](auto& node, index_t index) {
			    semanticNode(node).assign(index, ranges, f);
		    },
		    [this, ranges, f](auto& node) { semanticNode(node).assign(ranges, f); },
		    propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Code code, SemanticRangeSet const& ranges, UnaryFunction f,
	                     bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, ranges, f](auto& node, index_t index) {
			    semanticNode(node).assign(index, ranges, f);
		    },
		    [this, ranges, f](auto& node) { semanticNode(node).assign(ranges, f); },
		    propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Key key, SemanticRangeSet const& ranges, UnaryFunction f,
	                     bool propagate = true)
	{
		assignSemantics(derived().toCode(key), ranges, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(Point coord, SemanticRangeSet const& ranges, UnaryFunction f,
	                     bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(coord, depth), ranges, f, propagate);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void assignSemantics(coord_t x, coord_t y, coord_t z, SemanticRangeSet const& ranges,
	                     UnaryFunction f, bool propagate = true, depth_t depth = 0)
	{
		assignSemantics(derived().toCode(x, y, z, depth), ranges, f, propagate);
	}

	//
	// Erase
	//

	void eraseSemantics(label_t label, bool propagate = true)
	{
		eraseSemantics(derived().rootCode(), label, propagate);
	}

	void eraseSemantics(Node node, label_t label, bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, label](auto& node, index_t index) {
			    semanticNode(node).erase(index, label);
		    },
		    [this, label](auto& node) { semanticNode(node).erase(label); },
		    propagate);
	}

	void eraseSemantics(Code code, label_t label, bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, label](auto& node, index_t index) {
			    semanticNode(node).erase(index, label);
		    },
		    [this, label](auto& node) { semanticNode(node).erase(label); },
		    propagate);
	}

	void eraseSemantics(Key key, label_t label, bool propagate = true)
	{
		eraseSemantics(derived().toCode(key), label, propagate);
	}

	void eraseSemantics(Point coord, label_t label, bool propagate = true,
	                    depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(coord, depth), label, propagate);
	}

	void eraseSemantics(coord_t x, coord_t y, coord_t z, label_t label,
	                    bool propagate = true, depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(x, y, z, depth), label, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void eraseSemantics(InputIt first, InputIt last, bool propagate = true)
	{
		eraseSemantics(derived().rootCode(), first, last, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void eraseSemantics(Node node, InputIt first, InputIt last, bool propagate = true)
	{
		eraseSemantics(node, SemanticRangeSet(first, last));
		// SemanticRangeSet rs(first, last);
		// derived().apply(
		//     node,
		//     [this, rs](auto& node, index_t index) {
		// 	    semanticNode(node).erase(index, rs);
		//     },
		//     [this, rs](auto& node) { semanticNode(node).erase(rs); },
		//     propagate);
	}

	template <class InputIt,typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void eraseSemantics(Code code, InputIt first, InputIt last, bool propagate = true)
	{
		eraseSemantics(code, SemanticRangeSet(first, last));
		// SemanticRangeSet rs(first, last);
		// derived().apply(
		//     code,
		//     [this, rs](auto& node, index_t index) {
		// 	    semanticNode(node).erase(index, rs);
		//     },
		//     [this, rs](auto& node) { semanticNode(node).erase(rs); },
		//     propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void eraseSemantics(Key key, InputIt first, InputIt last, bool propagate = true)
	{
		eraseSemantics(derived().toCode(key), first, last, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void eraseSemantics(Point coord, InputIt first, InputIt last, bool propagate = true,
	                    depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(coord, depth), first, last, propagate);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void eraseSemantics(coord_t x, coord_t y, coord_t z, InputIt first, InputIt last,
	                    bool propagate = true, depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(x, y, z, depth), first, last, propagate);
	}

	void eraseSemantics(std::initializer_list<label_t> ilist, bool propagate = true)
	{
		eraseSemantics(derived().rootCode(), ilist, propagate);
	}

	void eraseSemantics(Node node, std::initializer_list<label_t> ilist,
	                    bool propagate = true)
	{
		eraseSemantics(node, std::begin(ilist), std::end(ilist), propagate);
	}

	void eraseSemantics(Code code, std::initializer_list<label_t> ilist,
	                    bool propagate = true)
	{
		eraseSemantics(code, std::begin(ilist), std::end(ilist), propagate);
	}

	void eraseSemantics(Key key, std::initializer_list<label_t> ilist,
	                    bool propagate = true)
	{
		eraseSemantics(derived().toCode(key), ilist, propagate);
	}

	void eraseSemantics(Point coord, std::initializer_list<label_t> ilist,
	                    bool propagate = true, depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(coord, depth), ilist, propagate);
	}

	void eraseSemantics(coord_t x, coord_t y, coord_t z,
	                    std::initializer_list<label_t> ilist, bool propagate = true,
	                    depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(x, y, z, depth), ilist, propagate);
	}

	void eraseSemantics(SemanticRange range, bool propagate = true)
	{
		eraseSemantics(derived().rootCode(), range, propagate);
	}

	void eraseSemantics(Node node, SemanticRange range, bool propagate = true)
	{
		eraseSemantics(node, SemanticRangeSet(range), propagate);
	}

	void eraseSemantics(Code code, SemanticRange range, bool propagate = true)
	{
		eraseSemantics(code, SemanticRangeSet(range), propagate);
	}

	void eraseSemantics(Key key, SemanticRange range, bool propagate = true)
	{
		eraseSemantics(derived().toCode(key), range, propagate);
	}

	void eraseSemantics(Point coord, SemanticRange range, bool propagate = true,
	                    depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(coord, depth), range, propagate);
	}

	void eraseSemantics(coord_t x, coord_t y, coord_t z, SemanticRange range,
	                    bool propagate = true, depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(x, y, z, depth), range, propagate);
	}

	void eraseSemantics(SemanticRangeSet const& ranges, bool propagate = true)
	{
		eraseSemantics(derived().rootCode(), ranges, propagate);
	}

	void eraseSemantics(Node node, SemanticRangeSet const& ranges, bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, ranges](auto& node, index_t index) {
			    semanticNode(node).erase(index, ranges);
		    },
		    [this, ranges](auto& node) { semanticNode(node).erase(ranges); },
		    propagate);
	}

	void eraseSemantics(Code code, SemanticRangeSet const& ranges, bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, ranges](auto& node, index_t index) {
			    semanticNode(node).erase(index, ranges);
		    },
		    [this, ranges](auto& node) { semanticNode(node).erase(ranges); },
		    propagate);
	}

	void eraseSemantics(Key key, SemanticRangeSet const& ranges, bool propagate = true)
	{
		eraseSemantics(derived().toCode(key), ranges, propagate);
	}

	void eraseSemantics(Point coord, SemanticRangeSet const& ranges, bool propagate = true,
	                    depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(coord, depth), ranges, propagate);
	}

	void eraseSemantics(coord_t x, coord_t y, coord_t z, SemanticRangeSet const& ranges,
	                    bool propagate = true, depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(x, y, z, depth), ranges, propagate);
	}

	void eraseSemantics(std::string const& tag, bool propagate = true)
	{
		eraseSemantics(derived().rootCode(), tag, propagate);
	}

	void eraseSemantics(Node node, std::string const& tag, bool propagate = true)
	{
		eraseSemantics(node, labels(tag));
	}

	void eraseSemantics(Code code, std::string const& tag, bool propagate = true)
	{
		eraseSemantics(code, labels(tag));
	}

	void eraseSemantics(Key key, std::string const& tag, bool propagate = true)
	{
		eraseSemantics(derived().toCode(key), tag, propagate);
	}

	void eraseSemantics(Point coord, std::string const& tag, bool propagate = true,
	                    depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(coord, depth), tag, propagate);
	}

	void eraseSemantics(coord_t x, coord_t y, coord_t z, std::string const& tag,
	                    bool propagate = true, depth_t depth = 0)
	{
		eraseSemantics(derived().toCode(x, y, z, depth), tag, propagate);
	}

	//
	// Erase if
	//

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(UnaryPredicate p, bool propagate = true)
	{
		eraseIfSemantics(derived().rootCode(), p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Node node, UnaryPredicate p, bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, p](auto& node, index_t index) {
			    semanticNode(node).eraseIf(index, p);
		    },
		    [this, p](auto& node) { semanticNode(node).eraseIf(p); },
		    propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Code code, UnaryPredicate p, bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, p](auto& node, index_t index) {
			    semanticNode(node).eraseIf(index, p);
		    },
		    [this, p](auto& node) { semanticNode(node).eraseIf(p); },
		    propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Key key, UnaryPredicate p, bool propagate = true)
	{
		eraseIfSemantics(derived().toCode(key), p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Point coord, UnaryPredicate p, bool propagate = true,
	                      depth_t depth = 0)
	{
		eraseIfSemantics(derived().toCode(coord, depth), p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(coord_t x, coord_t y, coord_t z, UnaryPredicate p,
	                      bool propagate = true, depth_t depth = 0)
	{
		eraseIfSemantics(derived().toCode(x, y, z, depth), p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(std::string const& tag, UnaryPredicate p, bool propagate = true)
	{
		eraseIfSemantics(derived().rootCode(), tag, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Node node, std::string const& tag, UnaryPredicate p,
	                      bool propagate = true)
	{
		eraseIfSemantics(node, labels(tag), p);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Code code, std::string const& tag, UnaryPredicate p,
	                      bool propagate = true)
	{
		eraseIfSemantics(code, labels(tag), p);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Key key, std::string const& tag, UnaryPredicate p,
	                      bool propagate = true)
	{
		eraseIfSemantics(derived().toCode(key), tag, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Point coord, std::string const& tag, UnaryPredicate p,
	                      bool propagate = true, depth_t depth = 0)
	{
		eraseIfSemantics(derived().toCode(coord, depth), tag, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(coord_t x, coord_t y, coord_t z, std::string const& tag,
	                      UnaryPredicate p, bool propagate = true, depth_t depth = 0)
	{
		eraseIfSemantics(derived().toCode(x, y, z, depth), tag, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(SemanticRange range, UnaryPredicate p, bool propagate = true)
	{
		eraseIfSemantics(derived().rootCode(), range, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Node node, SemanticRange range, UnaryPredicate p,
	                      bool propagate = true)
	{
		eraseIfSemantics(node, SemanticRangeSet(range), p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Code code, SemanticRange range, UnaryPredicate p,
	                      bool propagate = true)
	{
		eraseIfSemantics(code, SemanticRangeSet(range), p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Key key, SemanticRange range, UnaryPredicate p,
	                      bool propagate = true)
	{
		eraseIfSemantics(derived().toCode(key), range, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Point coord, SemanticRange range, UnaryPredicate p,
	                      bool propagate = true, depth_t depth = 0)
	{
		eraseIfSemantics(derived().toCode(coord, depth), range, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(coord_t x, coord_t y, coord_t z, SemanticRange range,
	                      UnaryPredicate p, bool propagate = true, depth_t depth = 0)
	{
		eraseIfSemantics(derived().toCode(x, y, z, depth), range, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(SemanticRangeSet const& ranges, UnaryPredicate p,
	                      bool propagate = true)
	{
		eraseIfSemantics(derived().rootCode(), ranges, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Node node, SemanticRangeSet const& ranges, UnaryPredicate p,
	                      bool propagate = true)
	{
		derived().apply(
		    node,
		    [this, ranges, p](auto& node, index_t index) {
			    semanticNode(node).eraseIf(index, ranges, p);
		    },
		    [this, ranges, p](auto& node) { semanticNode(node).eraseIf(ranges, p); },
		    propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Code code, SemanticRangeSet const& ranges, UnaryPredicate p,
	                      bool propagate = true)
	{
		derived().apply(
		    code,
		    [this, ranges, p](auto& node, index_t index) {
			    semanticNode(node).eraseIf(index, ranges, p);
		    },
		    [this, ranges, p](auto& node) { semanticNode(node).eraseIf(ranges, p); },
		    propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Key key, SemanticRangeSet const& ranges, UnaryPredicate p,
	                      bool propagate = true)
	{
		eraseIfSemantics(derived().toCode(key), ranges, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(Point coord, SemanticRangeSet const& ranges, UnaryPredicate p,
	                      bool propagate = true, depth_t depth = 0)
	{
		eraseIfSemantics(derived().toCode(coord, depth), ranges, p, propagate);
	}

	template <class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	void eraseIfSemantics(coord_t x, coord_t y, coord_t z, SemanticRangeSet const& ranges,
	                      UnaryPredicate p, bool propagate = true, depth_t depth = 0)
	{
		eraseIfSemantics(derived().toCode(x, y, z, depth), ranges, p, propagate);
	}

	//
	// Clear
	//

	template <std::size_t N>
	void clear(SemanticNode<N>&)
	{
	}

	void clearSemantics(bool propagate = true)
	{
		clearSemantics(derived().rootCode(), propagate);
	}

	void clearSemantics(Node node, bool propagate = true)
	{
		derived().apply(
		    node, [](auto& node, index_t index) { semanticNode(node).clear(index); },
		    [](auto& node) { semanticNode(node).clear(); }, propagate);
	}

	void clearSemantics(Code code, bool propagate = true)
	{
		derived().apply(
		    code, [](auto& node, index_t index) { semanticNode(node).clear(index); },
		    [](auto& node) { semanticNode(node).clear(); }, propagate);
	}

	void clearSemantics(Key key, bool propagate = true)
	{
		clearSemantics(derived().toCode(key), propagate);
	}

	void clearSemantics(Point coord, bool propagate = true, depth_t depth = 0)
	{
		clearSemantics(derived().toCode(coord, depth), propagate);
	}

	void clearSemantics(coord_t x, coord_t y, coord_t z, bool propagate = true,
	                    depth_t depth = 0)
	{
		clearSemantics(derived().toCode(x, y, z, depth), propagate);
	}

	//
	// Change semantic label
	//

	void changeSemantics(label_t old_label, label_t new_label, bool propagate = true)
	{
		changeLabel(derived().rootCode(), old_label, new_label, propagate);
	}

	void changeSemantics(Node node, label_t old_label, label_t new_label,
	                     bool propagate = true)
	{
		derived().apply(
		    node,
		    [old_label, new_label](auto& node, index_t index) {
			    semanticNode(node).changeLabel(index, old_label, new_label);
		    },
		    [old_label, new_label](auto& node) {
			    semanticNode(node).changeLabel(old_label, new_label);
		    },
		    propagate);
	}

	void changeSemantics(Code code, label_t old_label, label_t new_label,
	                     bool propagate = true)
	{
		derived().apply(
		    code,
		    [old_label, new_label](auto& node, index_t index) {
			    semanticNode(node).changeLabel(index, old_label, new_label);
		    },
		    [old_label, new_label](auto& node) {
			    semanticNode(node).changeLabel(old_label, new_label);
		    },
		    propagate);
	}

	void changeSemantics(Key key, label_t old_label, label_t new_label,
	                     bool propagate = true)
	{
		changeSemantics(derived().toCode(key), old_label, new_label, propagate);
	}

	void changeSemantics(Point coord, label_t old_label, label_t new_label,
	                     bool propagate = true, depth_t depth = 0)
	{
		changeSemantics(derived().toCode(coord, depth), old_label, new_label, propagate);
	}

	void changeSemantics(coord_t x, coord_t y, coord_t z, label_t old_label,
	                     label_t new_label, bool propagate = true, depth_t depth = 0)
	{
		changeSemantics(derived().toCode(x, y, z, depth), old_label, new_label, propagate);
	}

	//
	// Fill
	//

	template <std::size_t N>
	void fill(SemanticNode<N>& node, SemanticNode<N> const& parent, index_t index)
	{
		node.fill(parent, index);
	}

	template <std::size_t N>
	[[nodiscard]] bool isCollapsible(SemanticNode<N> const& node) const
	{
		return node.isCollapsible();
	}

 protected:
	//
	// Constructors
	//

	SemanticMapBase() = default;

	SemanticMapBase(SemanticMapBase const& other) = default;

	SemanticMapBase(SemanticMapBase&& other) = default;

	// template <class Derived2>
	// SemanticMapBase(SemanticMapBase<Derived2> const& other) : Derived2  // TODO: Fill in
	// {
	// }

	//
	// Assignment operator
	//

	SemanticMapBase& operator=(SemanticMapBase const& rhs) = default;

	SemanticMapBase& operator=(SemanticMapBase&& rhs) = default;

	template <class Derived2>
	SemanticMapBase& operator=(SemanticMapBase<Derived2> const& rhs)
	{
		// TODO: Fill in
		return *this;
	}

	//
	// Swap
	//

	void swap(SemanticMapBase& other) noexcept
	{
		std::swap(mapping_, other.mapping_);
		std::swap(propagation_, other.propagation_);
	}

	//
	// Derived
	//

	[[nodiscard]] constexpr Derived& derived() { return *static_cast<Derived*>(this); }

	[[nodiscard]] constexpr Derived const& derived() const
	{
		return *static_cast<Derived const*>(this);
	}

	//
	// Semantic node
	//

	template <class T>
	[[nodiscard]] constexpr auto& semanticNode(T& node)
	{
		return static_cast<SemanticNode<T::semanticSize()>&>(node);
	}

	template <class T>
	[[nodiscard]] constexpr auto const& semanticNode(T const& node) const
	{
		return static_cast<SemanticNode<T::semanticSize()> const&>(node);
	}

	//
	// Initilize root
	//

	void initRoot() { semanticNode(derived().root()).clear(); }

	//
	// Update node
	//

	template <std::size_t N, class T>
	void updateNode(SemanticNode<N>& node, index_t index, T const& children)
	{
		if constexpr (1 == N) {
			node.set(0, SemanticSet(children.cbegin(), children.cend()));
		} else {
			node.set(index, SemanticSet(children.cbegin(0), children.cend(0)));
			// TODO use propagation criteria, for now using max of values if labels are the same
			// TODO make more efficient
			for (int i = 1; i < N; ++i) {
				for(auto iter = children.begin(i); iter != children.end(i); ++iter) {
					auto semantic = node.find(index, iter->label); 
					// auto end = node.end(index);
					if (semantic != node.end(index)) {
						node.insertOrAssign(index, iter->label, std::max(iter->value, semantic->value));
					} else {
						node.insert(index, iter->label, iter->value);
					}
				}
			}
		} 
	}

	//
	// Input/output (read/write)
	//

	[[nodiscard]] static constexpr MapType mapType() noexcept { return MapType::SEMANTIC; }

	[[nodiscard]] static constexpr bool canReadData(MapType mt) noexcept
	{
		return mapType() == mt;
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	[[nodiscard]] static constexpr uint8_t numData() noexcept
	{
		using value_type = typename std::iterator_traits<InputIt>::value_type;
		using node_type = typename value_type::node_type;
		return node_type::semanticSize();
	}

	// template <class OutputIt>
	// void readNodes(std::istream& in, OutputIt first, OutputIt last)
	// {
	// 	mapping_.read(in);

	// 	std::array<size_type, N> sizes ; // TODO: fix this
	// 	for (; first != last; ++first) {
	// 		auto& sem = semanticNode(first->node);

	// 		out.write(reinterpret_cast<char*>(sizes.data()), N * sizeof(size_type));

	// 		if (first->indices.all()) {
	// 			sem.resizeLazy(sizes);
	// 			auto size = std::accumulate(std::cbegin(sizes), std::cend(sizes), std::size_t(0));
	// 			if (0 != size) {
	// 				in.read(reinterpret_cast<char*>(sem.begin()), size * sizeof(Semantic));
	// 			}
	// 		} else {
	// 			auto cur_sizes = sem.sizes();
	// 			for (index_t i = 0; N != i; ++i) {
	// 				if (!first->indices[i]) {
	// 					sizes[i] = cur_sizes[i];
	// 				}
	// 			}

	// 			sem.resize(sizes);

	// 			for (index_t i = 0; N != i; ++i) {
	// 				if (first->indices[i]) {
	// 					in.read(reinterpret_cast<char*>(sem.begin(i)), sizes[i] * sizeof(Semantic));
	// 				} else {
	// 					// Skip forward
	// 					in.seekg(sizes[i] * sizeof(Semantic), std::istream::cur);
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	// template <class InputIt>
	// void writeNodes(std::ostream& out, InputIt first, InputIt last) const
	// {
	// 	mapping_.write(out);

	// 	for (; first != last; ++first) {
	// 		auto const& sem = semanticNode(first->node);

	// 		std::array<size_type, N> sizes = sem.sizes();
	// 		out.write(reinterpret_cast<char const*>(sizes.data()), N * sizeof(size_type));
	// 		auto size = std::accumulate(std::cbegin(sizes), std::cend(sizes), std::size_t(0));
	// 		if (0 != size) {
	// 			out.write(reinterpret_cast<char const*>(sem.begin()), size * sizeof(Semantic));
	// 		}
	// 	}
	// }

 protected:
	// Propagation
	SemanticPropagation propagation_;
};
}  // namespace ufo::map

#endif  // UFO_MAP_SEMANTIC_MAP_H