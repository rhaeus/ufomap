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

#ifndef UFO_MAP_SEMANTIC_NODE_H
#define UFO_MAP_SEMANTIC_NODE_H

// UFO
#include <ufo/map/semantic/semantic_set.h>
#include <ufo/map/semantic/semantic_util.h>

// STL
#include <algorithm>
#include <array>

#include <optional>

namespace ufo::map
{
template <std::size_t N>
struct SemanticNode {
	// Tags
	using size_type = label_t;
	using difference_type = std::ptrdiff_t;
	using reference = Semantic &;  // TODO: Make label const
	using const_reference = Semantic const &;
	using pointer = Semantic *;
	using const_pointer = Semantic const *;
	using iterator = Semantic *;  // TODO: Make label const
	using const_iterator = Semantic const *;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	// -1 half rounded up (i.e., the number of elements needed to store the sizes of the N
	// semantic containers)
	static constexpr std::size_t N_H = 1 + (N - 1) / 2;

	// Data
	// TODO: Most significate size bits indicates whether the labels have change and the
	// second most significate if the values have changed for the particular index.
	std::unique_ptr<Semantic[]> semantics;

	//
	// Size
	//

	[[nodiscard]] static constexpr std::size_t semanticSize() { return N; }

	[[nodiscard]] size_type semanticAllocSize() const {
		return empty() ? 0 : semanticSize() + N_H;
	}


	//
	// Fill
	//

	// fill all our nodes with data from parent node with specified index
	void fill(SemanticNode const &parent, index_t const index)
	{
		semantic::resizeLazy<N>(semantics, parent.size(index));
		auto first = parent.begin(index);
		auto last = parent.end(index);
		for (index_t i = 0; N != i; ++i) {
			std::copy(first, last, begin(i));
		}
	}

	//
	// Is collapsible
	//

	// check if all nodes have exactly the same contents
	[[nodiscard]] bool isCollapsible() const 
	{
		auto first = cbegin(0);
		auto last = cend(0);
		for (index_t i = 1; N != i; ++i) {
			if (!std::equal(first, last, cbegin(i), cend(i))) {
				return false;
			}
		}
		return true;
	}

	// // check if all children have exactly the same elements as parent at specified index
	// [[nodiscard]] bool isCollapsible(SemanticNode const &parent, index_t const index) const
	// {
	// 	auto first = parent.begin(index);
	// 	auto last = parent.end(index);
	// 	for (index_t i = 0; N != i; ++i) {
	// 		if (!std::equal(first, last, cbegin(i), cend(i))) {
	// 			return false;
	// 		}
	// 	}
	// 	return true;
	// }

	//
	// Iterators
	//

	iterator begin() noexcept { return semantic::begin<N>(semantics); }

	const_iterator begin() const noexcept
	{
		return semantic::begin<N>(semantics);
	}

	const_iterator cbegin() const noexcept { return begin(); }

	iterator end() noexcept
	{
		return semantic::end<N>(semantics);
	}

	const_iterator end() const noexcept
	{
		return semantic::end<N>(semantics);
	}

	const_iterator cend() const noexcept { return end(); }

	//
	// Reverse iterators
	//

	reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(end()); }

	const_reverse_iterator rbegin() const noexcept
	{
		return std::make_reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const noexcept { return rbegin(); }

	reverse_iterator rend() noexcept { return std::make_reverse_iterator(begin()); }

	const_reverse_iterator rend() const noexcept
	{
		return std::make_reverse_iterator(begin());
	}

	const_reverse_iterator crend() const noexcept { return rend(); }

	//
	// Index iterators
	//

	iterator begin(index_t const index) noexcept
	{
		return semantic::begin<N>(semantics, index);
	}

	const_iterator begin(index_t const index) const noexcept
	{
		return semantic::begin<N>(semantics, index);
	}

	const_iterator cbegin(index_t const index) const noexcept { return begin(index); }

	iterator end(index_t const index) noexcept
	{
		return semantic::end<N>(semantics, index);
	}

	const_iterator end(index_t const index) const noexcept
	{
		return semantic::end<N>(semantics, index);
	}

	const_iterator cend(index_t const index) const noexcept { return end(index); }

	//
	// Reverse index iterators
	//

	reverse_iterator rbegin(index_t const index) noexcept
	{
		return std::make_reverse_iterator(end(index));
	}

	const_reverse_iterator rbegin(index_t const index) const noexcept
	{
		return std::make_reverse_iterator(end(index));
	}

	const_reverse_iterator crbegin(index_t const index) const noexcept
	{
		return rbegin(index);
	}

	reverse_iterator rend(index_t const index) noexcept
	{
		return std::make_reverse_iterator(begin(index));
	}

	const_reverse_iterator rend(index_t const index) const noexcept
	{
		return std::make_reverse_iterator(begin(index));
	}

	const_reverse_iterator crend(index_t const index) const noexcept { return rend(index); }

	//
	// Empty
	//

	[[nodiscard]] bool empty() const noexcept { return semantic::empty<N>(semantics); }

	[[nodiscard]] bool empty(index_t const index) const { semantic::empty<N>(semantics, index);}

	//
	// Size
	//

	[[nodiscard]] std::size_t size() const
	{
		return semantic::size<N>(semantics);
	}

	[[nodiscard]] size_type size(index_t const index) const
	{
		return semantic::size<N>(semantics, index);
	}

	[[nodiscard]] std::array<size_type, N> sizes() const
	{
		return semantic::sizes<N>(semantics);
	}

	//
	// Offset
	//

	[[nodiscard]] std::size_t offset(index_t const index) const
	{
		return semantic::offset<N>(semantics, index);
	}

	//
	// Clear
	//

	void clear() noexcept { semantic::clear<N>(semantics);}

	void clear(index_t const index) { semantic::clear<N>(semantics, index);}

	//
	// Set
	//

	void set(SemanticSet const &semantics)
	{
		semantic::resizeLazy<N>(this->semantics, semantics.size());
		auto first = std::begin(semantics);
		auto last = std::end(semantics);
		for (index_t i = 0; N != i; ++i) {
			std::copy(first, last, begin(i));
		}
	}

	void set(index_t index, SemanticSet const &semantics)
	{
		semantic::resize<N>(this->semantics, index, semantics.size());
		std::copy(std::begin(semantics), std::end(semantics), begin(index));
	}

	//
	// TODO: Change label
	//

	// void changeLabel(label_t old_label, label_t new_label)
	// {
	// 	// TODO: Implement
	// }

	// void changeLabel(index_t index, label_t old_label, label_t new_label)
	// {
	// 	// TODO: Implement
	// }

	//
	// Insert
	//

	void insert(label_t label, value_t value)
	{
		semantic::insert<N>(semantics, label, value);
	}

	void insert(index_t index, label_t label, value_t value)
	{
		semantic::insert<N>(semantics, index, label, value);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insert(InputIt first, InputIt last)
	{
		semantic::insert<N>(semantics, first, last);
	}

	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insert(index_t index, InputIt first, InputIt last)
	{
		semantic::insert<N>(semantics, index, first, last);
	}

	//
	// Insert or assign
	//

	void insertOrAssign(label_t label, value_t value)
	{
		semantic::insertOrAssign<N>(semantics, label, value);
	}

	void insertOrAssign(index_t index, label_t label, value_t value)
	{
		semantic::insertOrAssign<N>(semantics, index, label, value);
	}

	template <class UnaryFunction, class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssign(label_t label, UnaryFunction f)
	{
		semantic::insertOrAssign<N>(semantics, label, f);
	}

	template <class UnaryFunction, class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>>
	void insertOrAssign(index_t index, label_t label, UnaryFunction f)
	{
		semantic::insertOrAssign<N>(semantics, index, label, f);
	}

	// iterator to semantics
	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssign(InputIt first, InputIt last)
	{
		semantic::insertOrAssign<N>(semantics, first, last);
	}

	// iterator to semantics
	template <class InputIt,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssign(index_t index, InputIt first, InputIt last)
	{
		semantic::insertOrAssign<N>(semantics, index, first, last);
	}

	// TODO: enable if InputIt is iterator, otherwise this function is called instead of insertOrAssign(index, label, f)
	// iterator to label
	template <class InputIt, class UnaryFunction, 
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssign(InputIt first, InputIt last, UnaryFunction f)
	{
		semantic::insertOrAssign<N>(semantics, first, last, f);
	}

	// iterator to label
	template <class InputIt, class UnaryFunction, 
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>,
	typename = std::enable_if_t<std::is_base_of_v<
	              std::input_iterator_tag,
	              typename std::iterator_traits<InputIt>::iterator_category>>
				  >
	void insertOrAssign(index_t index, InputIt first, InputIt last, UnaryFunction f)
	{
		semantic::insertOrAssign<N>(semantics, index, first, last, f);
	}

	//
	// Assign
	//

	// all
	void assign(SemanticRange range, value_t value)
	{
		assign(semantics, SemanticRangeSet{range}, value);
	}

	void assign(SemanticRangeSet const& ranges, value_t value)
	{
		semantic::assign<N>(semantics, ranges, value);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>
	>
	void assign(SemanticRange range, UnaryFunction f)
	{
		assign(SemanticRangeSet{range}, f);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>
	>
	void assign(SemanticRangeSet const& ranges, UnaryFunction f)
	{
		semantic::assign<N>(semantics, ranges, f);
	}

	template <class UnaryPredicate, class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>
	>
	void assign(UnaryPredicate p, UnaryFunction f)
	{
		semantic::assign<N>(semantics, p, f);
	}

	// index
	void assign(index_t const index, SemanticRange range, value_t value)
	{
		assign(index, SemanticRangeSet{range}, value);
	}

	void assign(index_t const index, SemanticRangeSet const& ranges, value_t value)
	{
		semantic::assign<N>(semantics, index, ranges, value);
	}

	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>
	>
	void assign(index_t const index, SemanticRangeSet const& ranges, UnaryFunction f)
	{
		semantic::assign<N>(semantics, index, ranges, f);
	}
	
	template <class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>
	>
	void assign(index_t const index, SemanticRange range, UnaryFunction f)
	{
		assign(index, SemanticRangeSet{range}, f);
	}

	template <class UnaryPredicate, class UnaryFunction,
	class = std::enable_if_t<std::is_invocable<UnaryFunction, Semantic>::value>,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>
	>
	void assign(index_t const index, UnaryPredicate p, UnaryFunction f)
	{
		semantic::assign<N>(semantics, index, p, f);
	}

	//
	// Erase
	//

	iterator erase(const_iterator pos) 
	{ 
		return semantic::erase<N>(semantics, pos, std::next(pos));
	}

	iterator erase(iterator pos) 
	{ 
		return semantic::erase<N>(semantics, pos, std::next(pos));
	}

	// Removes the elements in the range [first, last)
	iterator erase(const_iterator first, const_iterator last)
	{
		return semantic::erase<N>(semantics, first, last);
	}

	size_type erase(label_t label)
	{
		return semantic::erase<N>(semantics, label);
	}

	size_type erase(SemanticRangeSet const &ranges) 
	{
		return semantic::erase<N>(semantics, ranges);
	}

	size_type erase(SemanticRange range) 
	{ 
		return erase(SemanticRangeSet{range}); 
	}

	size_type erase(index_t const index, label_t label)
	{
		return semantic::erase<N>(semantics, index, label);
	}

	size_type erase(index_t const index, SemanticRangeSet const &ranges)
	{
		return semantic::erase<N>(semantics, index, ranges);
	}

	size_type erase(index_t const index, SemanticRange range)
	{
		return erase(index, SemanticRangeSet{range});
	}

	//
	// Erase if
	//

	template<class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	size_type eraseIf(UnaryPredicate p)
	{
		return semantic::eraseIf<N>(semantics, p);
	}

	template<class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	size_type eraseIf(SemanticRangeSet const &ranges, UnaryPredicate p)
	{
		return semantic::eraseIf<N>(semantics, ranges, p);
	}

	template<class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	size_type eraseIf(SemanticRange range, UnaryPredicate p)
	{
		return eraseIf(SemanticRangeSet{range}, p);
	}

	template<class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	size_type eraseIf(index_t const index, UnaryPredicate p) 
	{
		return semantic::eraseIf<N>(semantics, index, p);
	}

	template<class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	size_type eraseIf(index_t const index, SemanticRangeSet const &ranges, UnaryPredicate p)
	{
		return semantic::eraseIf<N>(semantics, index, ranges, p);
	}

	template<class UnaryPredicate,
	class = std::enable_if_t<std::is_invocable<UnaryPredicate, Semantic>::value>>
	size_type eraseIf(index_t const index, SemanticRange range, UnaryPredicate p)
	{
		return eraseIf(index, SemanticRangeSet{range}, p);
	}

	//
	// At
	//

	std::optional<Semantic> at(index_t index, label_t label) const {
		return semantic::at<N>(semantics, index, label);
	}

	//
	// Value
	//

	std::optional<value_t> value(index_t index, label_t label) const
	{
		return semantic::value<N>(semantics, index, label);
	}

	//
	// Count
	//

	size_type count(index_t index, label_t label) const
	{
		return semantic::count<N>(semantics, index, label);
	}

	//
	// Find
	//

	const_iterator find(index_t index, label_t label) const
	{
		return semantic::find<N>(semantics, index, label);
	}

	//
	// Contains
	//

	bool contains(index_t index, label_t label) const
	{
		return semantic::contains<N>(semantics, index, label);
	}

	//
	// Equal range
	//

	std::pair<const_iterator, const_iterator> equal_range(index_t index, label_t label) const
	{
		return semantic::equal_range<N>(semantics, index, label);
	}

	//
	// Lower bound
	//

	[[nodiscard]] const_iterator lower_bound(index_t index, label_t label) const
	{
		return semantic::lower_bound<N>(semantics, index, label);
	}

	//
	// Upper bound
	//

	[[nodiscard]] const_iterator upper_bound(index_t index, label_t label) const
	{
		return semantic::upper_bound<N>(semantics, index, label);
	}

	//
	// All
	//

	[[nodiscard]] bool all(index_t index, SemanticRange range) const
	{
		return semantic::all<N>(semantics, index, range);
	}

	[[nodiscard]] bool all(index_t index, SemanticRangeSet const& ranges) const
	{
		return semantic::all<N>(semantics, index, ranges);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool all(index_t index, UnaryPredicate p) const
	{
		return semantic::all<N>(semantics, index, p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool all(index_t index, SemanticRange range, UnaryPredicate p) const
	{
		return semantic::all<N>(semantics, index, range, p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool all(index_t index, SemanticRangeSet const& ranges, UnaryPredicate p) const
	{
		return semantic::all<N>(semantics, index, ranges, p);
	}

	//
	// Any
	//

	[[nodiscard]] bool any(index_t index, SemanticRange range) const
	{
		return semantic::any<N>(semantics, index, range);
	}

	[[nodiscard]] bool any(index_t index, SemanticRangeSet const& ranges) const
	{
		return semantic::any<N>(semantics, index, ranges);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool any(index_t index, UnaryPredicate p) const
	{
		return semantic::any<N>(semantics, index, p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool any(index_t index, SemanticRange range, UnaryPredicate p) const
	{
		return semantic::any<N>(semantics, index, range, p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool any(index_t index, SemanticRangeSet const& ranges, UnaryPredicate p) const
	{
		return semantic::any<N>(semantics, index, ranges, p);
	}

	//
	// None
	//

	[[nodiscard]] bool none(index_t index, SemanticRange range) const
	{
		return semantic::none<N>(semantics, index, range);
	}

	[[nodiscard]] bool none(index_t index, SemanticRangeSet const &ranges) const 
	{ 
		return semantic::none<N>(semantics, index, ranges); 
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool none(index_t index, UnaryPredicate p) const
	{
		return semantic::none<N>(semantics, index, p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool none(index_t index, SemanticRange range, UnaryPredicate p) const
	{
		return semantic::none<N>(semantics, index, range, p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool none(index_t index, SemanticRangeSet const &ranges, UnaryPredicate p) const
	{
		return semantic::none<N>(semantics, index, ranges, p);
	}

	std::string toString() const {
		return semantic::toString<N>(semantics);
	}

};
}  // namespace ufo::map

#endif  // UFO_MAP_SEMANTIC_NODE_H