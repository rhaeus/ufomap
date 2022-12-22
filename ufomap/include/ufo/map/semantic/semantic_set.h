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

#ifndef UFO_MAP_SEMANTICS_H
#define UFO_MAP_SEMANTICS_H

// UFO
#include <ufo/container/range.h>
#include <ufo/container/range_map.h>
#include <ufo/container/range.h>
#include <ufo/util/iterator_wrapper.h>
#include <ufo/util/type_traits.h>
// #include <ufo/map/semantic/semantics_reference.h>
#include <ufo/map/semantic/semantic.h>
#include <ufo/map/semantic/semantic_util.h>

// STL
#include <algorithm>
#include <cstddef>  // For std::ptrdiff_t
#include <functional>
#include <initializer_list>
#include <istream>
#include <iterator>  // For std::random_access_iterator_tag / std::contiguous_iterator_tag
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace ufo::map
{
// Forward declare
template <std::size_t N>
class SemanticNode;

class SemanticSet
{
 public:
	//  Tags
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = Semantic &;  // TODO: Make label const
	using const_reference = Semantic const &;
	using pointer = Semantic *;
	using const_pointer = Semantic const *;
	using iterator = Semantic *;  // TODO: Make label const
	using const_iterator = Semantic const *;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	//
	// Constructors
	//

	constexpr SemanticSet() = default;

	SemanticSet(SemanticSet const &other) { *this = other; }

	SemanticSet(SemanticSet &&other) noexcept = default;

	// SemanticSet(SemanticsReference other)
	// {
	// 	resize(other.size());
	// 	std::copy(std::begin(other), std::end(other), begin());
	// }

	template <class InputIt>
	SemanticSet(InputIt first, InputIt last)
	{
		insert(first, last);
	}

	SemanticSet(std::initializer_list<Semantic> init)
	    : SemanticSet(std::begin(init), std::end(init))
	{
	}

	//
	// Assignment operator
	//

	SemanticSet &operator=(SemanticSet const &rhs)
	{
		if (rhs.empty()) {
			clear();
		} else {
			resize(rhs.size());
			std::copy(std::begin(rhs), std::end(rhs), begin());
		}
		return *this;
	}

	SemanticSet &operator=(SemanticSet &&rhs) noexcept = default;

	// SemanticSet &operator=(SemanticsReference rhs)
	// {
	// 	resize(rhs.size());
	// 	std::copy(std::begin(rhs), std::end(rhs), begin());
	// }

	//
	// Data
	//

	[[nodiscard]] const_pointer data() const { return empty() ? nullptr : data_.get() + 1; }

	//
	// Iterators
	//

	iterator begin() noexcept { return empty() ? nullptr : data_.get() + 1; }

	const_iterator begin() const noexcept { return empty() ? nullptr : data_.get() + 1; }

	const_iterator cbegin() const noexcept { return begin(); }

	iterator end() noexcept { return empty() ? nullptr : data_.get() + allocSize(); }

	const_iterator end() const noexcept
	{
		return empty() ? nullptr : data_.get() + allocSize();
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
	// Empty
	//

	[[nodiscard]] bool empty() const noexcept { return nullptr == data_; }

	//
	// Size
	//

	[[nodiscard]] size_type size() const { return empty() ? 0 : data_[0].label; }

	[[nodiscard]] static constexpr size_type maxSize() noexcept
	{
		return std::numeric_limits<label_t>::max();
	}

	[[nodiscard]] size_type allocSize() const {
		return empty() ? 0 : size() + 1;
	}

	//
	// At
	//

	[[nodiscard]] std::optional<Semantic> at(label_t label) const
	{
		return semantic::at<1>(data_, 0, label);
	}

	//
	// Value
	//

	[[nodiscard]] std::optional<Semantic> value(label_t label) const
	{
		return semantic::value<1>(data_, 0, label);
	}

	//
	// Count
	//

	[[nodiscard]] size_type count(label_t label) const 
	{ 
		return semantic::contains<1>(data_, 0, label);
	}

	//
	// Find
	//

	[[nodiscard]] iterator find(label_t label) 
	{
		return semantic::find<1>(data_, 0, label);
	}

	[[nodiscard]] const_iterator find(label_t label) const
	{
		return semantic::find<1>(data_, 0, label);
	}

	//
	// Contains
	//

	[[nodiscard]] bool contains(label_t label) const 
	{ 
		return semantic::contains<1>(data_, 0, label);
	}

	//
	// Equal range
	//

	[[nodiscard]] std::pair<iterator, iterator> equal_range(label_t label) 
	{
		return semantic::equal_range<1>(data_, 0, label);
	}

	[[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(label_t label) const
	{ 
		return semantic::equal_range<1>(data_, 0, label);
	}


	//
	// Lower bound
	//

	[[nodiscard]] iterator lower_bound(label_t label)
	{
		return semantic::lower_bound<1>(data_, 0, label);
	}

	[[nodiscard]] const_iterator lower_bound(label_t label) const
	{
		return semantic::lower_bound<1>(data_, 0, label);
	}


	//
	// Upper bound
	//

	[[nodiscard]] iterator upper_bound(label_t label)
	{
		return semantic::upper_bound<1>(data_, 0, label);
	}

	[[nodiscard]] const_iterator upper_bound(label_t label) const
	{
		return semantic::upper_bound<1>(data_, 0, label);
	}

	//
	// All
	//

	[[nodiscard]] bool all(SemanticRange range) const
	{
		return all(SemanticRangeSet(range));
	}

	[[nodiscard]] bool all(SemanticRangeSet const &ranges) const
	{
		return semantic::all<1>(data_, 0, ranges);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool all(UnaryPredicate p) const
	{
		return semantic::all<1>(data_, 0, p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool all(SemanticRange range, UnaryPredicate p) const
	{
		return all(SemanticRangeSet(range), p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool all(SemanticRangeSet const &ranges, UnaryPredicate p) const
	{
		return semantic::all<1>(data_, 0, ranges, p);
	}

	//
	// Any
	//

	[[nodiscard]] bool any(SemanticRange range) const
	{
		return any(SemanticRangeSet(range));
	}

	[[nodiscard]] bool any(SemanticRangeSet const &ranges) const
	{
		return semantic::any<1>(data_, 0, ranges);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool any(UnaryPredicate p) const
	{
		return semantic::any<1>(data_, 0, p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool any(SemanticRange range, UnaryPredicate p) const
	{
		return any(SemanticRangeSet(range), p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool any(SemanticRangeSet const &ranges, UnaryPredicate p) const
	{
		return semantic::any<1>(data_, 0, ranges, p);
	}

	//
	// None
	//

	[[nodiscard]] bool none(SemanticRange range) const
	{
		return none(SemanticRangeSet(range));
	}

	[[nodiscard]] bool none(SemanticRangeSet const &ranges) const { return semantic::none<1>(data_, 0, ranges); }

	template <class UnaryPredicate>
	[[nodiscard]] bool none(UnaryPredicate p) const
	{
		return semantic::none<1>(data_, 0, p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool none(SemanticRange range, UnaryPredicate p) const
	{
		return none(SemanticRangeSet(range), p);
	}

	template <class UnaryPredicate>
	[[nodiscard]] bool none(SemanticRangeSet const &ranges, UnaryPredicate p) const
	{
		return semantic::none<1>(data_, 0, ranges, p);
	}

	//
	// Clear
	//

	void clear() noexcept { data_.reset(); }

	//
	// Insert
	//

	std::pair<iterator, bool> insert(Semantic semantic)
	{
		return insert(semantic.label, semantic.value);
	}

	std::pair<iterator, bool> insert(label_t label, value_t value)
	{
		return semantic::insert<1>(data_, 0, label, value);
	}

	iterator insert(const_iterator hint, Semantic semantic)
	{
		return insert(hint, semantic.label, semantic.value);
	}

	iterator insert(const_iterator hint, label_t label, value_t value)
	{
		return semantic::insertOrAssign<1, false>(data_, 0, hint, label, [value](auto) { return value; });
	}

	template <class InputIt>
	void insert(InputIt first, InputIt last)
	{
		semantic::insert<1>(data_, first, last);
	}

	void insert(std::initializer_list<Semantic> ilist)
	{
		insert(std::cbegin(ilist), std::cend(ilist));
	}

	//
	// Insert or assign
	//

	std::pair<iterator, bool> insertOrAssign(Semantic semantic)
	{
		return insertOrAssign(semantic.label, semantic.value);
	}

	std::pair<iterator, bool> insertOrAssign(label_t label, value_t value)
	{
		return semantic::insertOrAssign<1>(data_, 0, label, value);
	}

	iterator insertOrAssign(const_iterator hint, Semantic semantic)
	{
		return insertOrAssign(hint, semantic.label, semantic.value);
	}

	iterator insertOrAssign(const_iterator hint, label_t label, value_t value)
	{
		return semantic::insertOrAssign<1, true>(data_, 0, hint, label, [value](auto) { return value; });

	}

	template <class InputIt>
	void insertOrAssign(InputIt first, InputIt last)
	{
		semantic::insertOrAssign<1>(data_, first, last);
	}

	void insertOrAssign(std::initializer_list<Semantic> ilist)
	{
		insertOrAssign(std::cbegin(ilist), std::cend(ilist));
	}

	//
	// Insert or assign custom function
	//

	template <class UnaryFunction>
	void insertOrAssign(label_t label, UnaryFunction f)
	{
		semantic::insertOrAssign<1, true>(data_, label, f);
	}

	template <class InputIt, class UnaryFunction>
	void insertOrAssign(InputIt first, InputIt last, UnaryFunction f)
	{
		semantic::insertOrAssign<1, true>(data_, first, last, f);
	}

	template <class UnaryFunction>
	void insertOrAssign(std::initializer_list<label_t> ilist, UnaryFunction f)
	{
		insertOrAssign(std::cbegin(ilist), std::cend(ilist), f);
	}

	//
	// Assign
	//

	void assign(SemanticRange range, value_t value)
	{
		assign(SemanticRangeSet{range}, value);
	}

	void assign(SemanticRangeSet const& ranges, value_t value)
	{
		semantic::assign<1>(data_, ranges, value);
	}

	template <class UnaryFunction>
	void assign(SemanticRange range, UnaryFunction f)
	{
		semantic::assign<1>(data_, SemanticRangeSet{range}, f);
	}

	template <class UnaryPredicate>
	void assign(UnaryPredicate p, value_t value)
	{
		assign(p, [value](auto) { return value; });
	}


	template <class UnaryFunction>
	void assign(SemanticRangeSet const &ranges, UnaryFunction f)
	{
		semantic::assign<1>(data_, 0, ranges, f);
	}


	template <class UnaryPredicate, class UnaryFunction>
	void assign(UnaryPredicate p, UnaryFunction f)
	{
		semantic::assign<1>(data_, p, f);
	}

	//
	// Erase
	//

	iterator erase(const_iterator pos) 
	{ 
		return semantic::erase<1>(data_, pos, std::next(pos));
	}

	iterator erase(iterator pos) 
	{ 
		return semantic::erase<1>(data_, pos, std::next(pos));
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		return semantic::erase<1>(data_, first, last);
	}

	size_type erase(label_t label)
	{
		return semantic::erase<1>(data_, label);
	}

	size_type erase(SemanticRangeSet const &ranges) 
	{
		return semantic::erase<1>(data_, ranges);
	}

	size_type erase(SemanticRange range) 
	{ 
		return erase(SemanticRangeSet{range}); 
	}


	//
	// Erase if
	//

	template<class UnaryPredicate>
	size_type eraseIf(UnaryPredicate p)
	{
		return semantic::eraseIf<1>(data_, p);
	}

	template<class UnaryPredicate>
	size_type eraseIf(SemanticRangeSet const &ranges, UnaryPredicate p)
	{
		return semantic::eraseIf<1>(data_, ranges, p);
	}

	template<class UnaryPredicate>
	size_type eraseIf(SemanticRange range, UnaryPredicate p)
	{
		return eraseIf(SemanticRangeSet{range}, p);
	}

	//
	// Swap
	//

	void swap(SemanticSet &other) noexcept { std::swap(data_, other.data_); }

 protected:
	//
	// Data
	//

	[[nodiscard]] pointer data() { return empty() ? nullptr : data_.get() + 1; }


	//
	// Resize
	//

	void resize(size_type size)
	{
		if (0 == size) {
			clear();
			return;
		} else if (this->size() == size) {
			return;
		}

		pointer p_cur = data_.release();
		pointer p_new = static_cast<pointer>(realloc(p_cur, (size + 1) * sizeof(Semantic)));

		if (!p_new) {
			data_.reset(p_cur);
			throw std::bad_alloc();
		}

		data_.reset(p_new);
		data_[0].label = size;
	}

 private:
	std::unique_ptr<Semantic[]> data_;
};
}  // namespace ufo::map

namespace std
{
void swap(ufo::map::SemanticSet &lhs,
          ufo::map::SemanticSet &rhs) noexcept(noexcept(lhs.swap(rhs)))
{
	lhs.swap(rhs);
}
}  // namespace std

#endif  // UFO_MAP_SEMANTICS_H