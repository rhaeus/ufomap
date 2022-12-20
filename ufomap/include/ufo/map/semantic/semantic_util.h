#ifndef UFO_MAP_SEMANTIC_UTIL_H
#define UFO_MAP_SEMANTIC_UTIL_H

#include <algorithm>

namespace ufo::map::semantic {

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
    
    
    // template<std::size_t N>
    // void insert(up, idx,..)

    // template<std::size_t N>
    // void insert(up,..)

	//
	// Clear
	//

    template<std::size_t N>
	void clear(std::unique_ptr<Semantic[]> & semantics) noexcept { semantics.reset(); }

    

    //
	// Empty
	//

    template<std::size_t N>
	[[nodiscard]] bool empty(std::unique_ptr<Semantic[]> const& semantics) noexcept { return nullptr == semantics; }

    //
	// Size
	//

    template<std::size_t N>
	[[nodiscard]] std::array<size_type, N> sizes(std::unique_ptr<Semantic[]> const& semantics) 
	{
		if (empty<N>(semantics)) {
			return std::array<size_type, N>{};
		}

        static constexpr std::size_t N_H = 1 + (N - 1) / 2;

		std::array<size_type, N> s;
		std::copy(semantics.get(), semantics.get() + N_H,
		          reinterpret_cast<Semantic *>(s.data()));
		return s;
	}


    template<std::size_t N>
	[[nodiscard]] std::size_t size(std::unique_ptr<Semantic[]> const& semantics) 
	{
		auto const s = sizes<N>(semantics);
		return std::accumulate(std::begin(s), std::end(s), std::size_t(0));
	}

    template<std::size_t N>
	[[nodiscard]] size_type allocSize(std::unique_ptr<Semantic[]> const& semantics) {
        static constexpr std::size_t N_H = 1 + (N - 1) / 2;

		return empty<N>(semantics) ? 0 : size<N>(semantics) + N_H;
	}

    

    template<std::size_t N>
	[[nodiscard]] size_type size(std::unique_ptr<Semantic[]> const& semantics, index_t const index) 
	{
		// TODO: ignore 2 msb that are used for change detection
        // TODO: switch even and odd case to make compatible with semantic_set?
		return 0 == size<N>(semantics, index)
		           ? 0
		           : (index % 2
		                  ? semantics[index / 2].label // odd index
		                  : reinterpret_cast<label_t const &>(semantics[index / 2].value)); // even index
	}

	//
	// Offset
	//

    template<std::size_t N>
	[[nodiscard]] std::size_t offset(std::unique_ptr<Semantic[]> const& semantics, index_t const index)
	{
		auto const s = sizes<N>(semantics);
		return std::accumulate(std::begin(s), std::begin(s) + index, std::size_t(0));
	}


    template<std::size_t N>
	[[nodiscard]] bool empty(std::unique_ptr<Semantic[]> const& semantics, index_t const index)  { return 0 == size<N>(semantics, index); }

    //
	// Iterators
	//

    template<std::size_t N>
	iterator begin(std::unique_ptr<Semantic[]> const& semantics) noexcept { 
        static constexpr std::size_t N_H = 1 + (N - 1) / 2;
        
        return empty<N>(semantics) ? nullptr : semantics.get() + N_H; 
    }

    template<std::size_t N>
	const_iterator cbegin(std::unique_ptr<Semantic[]> const& semantics) noexcept { return begin<N>(semantics); }



    template<std::size_t N>
	iterator end(std::unique_ptr<Semantic[]> const& semantics) noexcept
	{
		auto const s = allocSize<N>(semantics);
		return 0 == s ? nullptr : semantics.get() + s;
	}

	template<std::size_t N>
	const_iterator cend(std::unique_ptr<Semantic[]> const& semantics) noexcept { return end<N>(semantics); }

    //
	// Index iterators
	//

    template<std::size_t N>
	iterator begin(std::unique_ptr<Semantic[]> const& semantics, index_t const index) noexcept
	{
        static constexpr std::size_t N_H = 1 + (N - 1) / 2;

		return empty<N>(semantics, index) ? nullptr : semantics.get() + N_H + offset<N>(semantics, index);
	}

	template<std::size_t N>
	const_iterator cbegin(std::unique_ptr<Semantic[]> const& semantics, index_t const index) noexcept { return begin<N>(semantics, index); }

    template<std::size_t N>
    iterator end(std::unique_ptr<Semantic[]> const& semantics, index_t const index) noexcept
	{
        static constexpr std::size_t N_H = 1 + (N - 1) / 2;

		return empty<N>(semantics, index) ? nullptr : semantics.get() + N_H + offset<N>(semantics, index) + size<N>(semantics, index);
	}

	template<std::size_t N>
	const_iterator cend(std::unique_ptr<Semantic[]> const& semantics, index_t const index) noexcept { return end<N>(semantics, index); }


	//
	// Lower bound
	//

	template<std::size_t N, class InputIt>
	[[nodiscard]] InputIt lower_bound(InputIt first, InputIt last, label_t label) 
	{
		return std::lower_bound(first, last, Semantic(label, std::numeric_limits<value_t>::lowest()));
	}

    template<std::size_t N>
	[[nodiscard]] iterator lower_bound(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label) 
	{
		// return std::lower_bound(begin<N>(semantics, index), end<N>(semantics, index), label);
		return lower_bound<N, iterator>(begin<N>(semantics, index), end<N>(semantics, index), label);
	}

	//
	// Upper bound
	//

	template<std::size_t N, class InputIt>
	[[nodiscard]] InputIt upper_bound(InputIt first, InputIt last, label_t label) 
	{
		return std::upper_bound(first, last, Semantic(label, std::numeric_limits<value_t>::max()));
	}

    template<std::size_t N>
	[[nodiscard]] iterator upper_bound(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label) 
	{
		// return std::upper_bound(begin<N>(semantics, index), end<N>(semantics, index), label);
		return upper_bound<N>(begin<N>(semantics, index), end<N>(semantics, index), label);
	}

	//
	// Equal range
	//

    template<std::size_t N>
	[[nodiscard]] std::pair<iterator, iterator> equal_range(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label)
	{
		return std::make_pair(lower_bound<N>(semantics, index, label), upper_bound<N>(semantics, index, label));
	}


    //
	// Find
	//

    template<std::size_t N>
	[[nodiscard]] iterator find(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label)
	{
		auto it = lower_bound<N>(semantics, index, label);
		return end<N>(semantics, index) != it && it->label == label ? it : end<N>(semantics, index);
	}

    // template<std::size_t N>
	// [[nodiscard]] const_iterator find(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label) 
	// {
	// 	auto it = lower_bound<N>(semantics, index, label);
	// 	return end<N>(semantics, index) != it && it->label == label ? it : end<N>(semantics, index);
	// }


    //
	// Contains
	//

    template<std::size_t N>
	[[nodiscard]] bool contains(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label)  
    { 
        return end<N>(semantics, index) != find<N>(semantics, index, label); 
    }

	//
	// At
	//
    template<std::size_t N>
	[[nodiscard]] std::optional<Semantic> at(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label) 
	{
		iterator it = find<N>(semantics, index, label);
		return end<N>(semantics, index) != it ? std::optional<Semantic>(*it) : std::nullopt;
	}

	//
	// Value
	//
    template<std::size_t N>
    [[nodiscard]] std::optional<value_t> value(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label) 
	{
		auto it = find<N>(semantics, index, label);
		return end<N>(semantics, index) != it ? std::optional<value_t>(it->value) : std::nullopt;
	}

	//
	// Count
	//
    template<std::size_t N>
	[[nodiscard]] size_type count(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label)  
    { 
        return contains<N>(semantics, index, label) ? 1 : 0; 
    }

	//
	// All
	//

    template<std::size_t N>
	[[nodiscard]] bool all(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRangeSet const &ranges) 
	{
		if (ranges.empty()) {
			return true;
		} else if (size<N>(semantics, index) < ranges.numValues()) {
			return false;
		}

		auto first = cbegin<N>(semantics, index);
		auto last = cend<N>(semantics, index);
		for (auto range : ranges) {
			auto lower = lower_bound<N>(first, last, range.lower());
			first = upper_bound<N>(lower, last, range.upper());
			auto range_dist = range.upper() - range.lower() + 1;
			auto sem_dist = std::distance(lower, first);
			if (first == last || range_dist != sem_dist) {
				return false;
			}
		}
		return true;
	}

	template<std::size_t N>
	[[nodiscard]] bool all(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRange range) 
	{
		return all<N>(semantics, index, SemanticRangeSet(range));
	}


	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool all(std::unique_ptr<Semantic[]> const& semantics, index_t index, UnaryPredicate p)
	{
		return std::all_of(cbegin<N>(semantics, index), cend<N>(semantics, index), p);
	}

	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool all(std::unique_ptr<Semantic[]> const& semantics, UnaryPredicate p)
	{
		return std::all_of(cbegin<N>(semantics), cend<N>(semantics), p);
	}



	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool all(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRangeSet const &ranges, UnaryPredicate p) 
	{
		if (ranges.empty()) {
			return true;
		} else if (size<N>(semantics, index) < ranges.numValues()) {
			return false;
		}

		auto first = cbegin<N>(semantics, index);
		auto last = cend<N>(semantics, index);
		for (auto range : ranges) {
			auto lower = lower_bound<N>(first, last, range.lower());
			first = upper_bound<N>(lower, last, range.upper());
			auto range_dist = range.upper() - range.lower() + 1;
			auto sem_dist = std::distance(lower, first);
			if (first == last || range_dist != sem_dist) {
				return false;
			}
			for (; lower != first; ++lower) {
				if (!p(*lower)) {
					return false;
				}
			}
		}
		return true;
	}

	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool all(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRange range, UnaryPredicate p)
	{
		return all<N>(semantics, index, SemanticRangeSet(range), p);
	}

	//
	// Any
	//

	template<std::size_t N>
	[[nodiscard]] bool any(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRangeSet const &ranges) 
	{
		if (ranges.empty()) {
			return true;
		}

		if (size<N>(semantics, index) < ranges.size()) {
			for (auto it = cbegin<N>(semantics, index), last = cend<N>(semantics, index); it != last; ++it) {
				if (ranges.contains(it->label)) {
					return true;
				}
			}
		} else {
			auto first = cbegin<N>(semantics, index);
			auto last = cend<N>(semantics, index);
			for (auto range : ranges) {
				first = lower_bound<N>(first, last, range.lower());
				if (first == last) {
					return false;
				} else if (first->label <= range.upper()) {
					return true;
				}
			}
		}
		return false;
	}

	template<std::size_t N>
	[[nodiscard]] bool any(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRange range) 
	{
		return any<N>(semantics, index, SemanticRangeSet(range));
	}

	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool any(std::unique_ptr<Semantic[]> const& semantics, index_t index, UnaryPredicate p) 
	{
		return std::any_of(cbegin<N>(semantics, index), cend<N>(semantics, index), p);
	}

	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool any(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRange range, UnaryPredicate p) 
	{
		return any<N>(semantics, index, SemanticRangeSet(range), p);
	}

	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool any(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRangeSet const &ranges, UnaryPredicate p) 
	{
		if (ranges.empty()) {
			return true;
		}

		if (size<N>(semantics, index) < ranges.size()) {
			for (auto it = cbegin<N>(semantics, index), last = cend<N>(semantics, index); it != last; ++it) {
				if (ranges.contains(it->label) && p(*it)) {
					return true;
				}
			}
		} else {
			auto first = cbegin<N>(semantics, index);
			auto last = cend<N>(semantics, index);
			for (auto range : ranges) {
				first = lower_bound<N>(first, last, range.lower());
				for (; first != last && first->label <= range.upper(); ++first) {
					if (p(*first)) {
						return true;
					}
				}
				if (first == last) {
					return false;
				}
			}
		}
	}

    //
	// None
	//

	template<std::size_t N>
	[[nodiscard]] bool none(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRangeSet const &ranges)  
	{ 
		return !any<N>(semantics, index, ranges); 
	}
	
	template<std::size_t N>
	[[nodiscard]] bool none(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRange range) 
	{
		return none<N>(semantics, index, SemanticRangeSet(range));
	}

	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool none(std::unique_ptr<Semantic[]> const& semantics, index_t index, UnaryPredicate p) 
	{
		return std::none_of(cbegin<N>(semantics, index), cend<N>(semantics, index), p);
	}

	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool none(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRange range, UnaryPredicate p) 
	{
		return none<UnaryPredicate, N>(semantics, index, SemanticRangeSet(range), p);
	}

	template <std::size_t N, class UnaryPredicate>
	[[nodiscard]] bool none(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRangeSet const &ranges, UnaryPredicate p) 
	{
		return !any<N>(semantics, index, ranges, p);
	}

	//
	// Resize
	//

	/*!
	 * @brief Set the Size
	 *
	 * @note Only call if already allocated.
	 *
	 * @param index
	 * @param size
	 */
	template<std::size_t N>
	void setSize(std::unique_ptr<Semantic[]> const& semantics, index_t const index, label_t const size)
	{
		if (index % 2) {
			semantics[index / 2].label = size;
		} else {
			semantics[index / 2].value = reinterpret_cast<value_t const &>(size);
		}
	}

	template<std::size_t N>
	void resizeLazy(std::unique_ptr<Semantic[]> & semantics, std::array<size_type, N> const &sizes)
	{
        static constexpr std::size_t N_H = 1 + (N - 1) / 2;

		auto const new_size = std::accumulate(std::begin(sizes), std::end(sizes), N_H);
		if (0 == new_size) {
			clear<N>(semantics);
			return;
		}

		auto const cur_sizes = sizes();
		if (cur_sizes == sizes) {
			return;
		}

		if (std::accumulate(std::begin(cur_sizes), std::end(cur_sizes), N_H) != new_size) {
			pointer p_cur = semantics.release(); // Releases ownership of its stored pointer, by returning its value and replacing it with a null pointer.
			pointer p_new = static_cast<pointer>(realloc(p_cur, new_size * sizeof(Semantic)));

			if (!p_new) {
				semantics.reset(p_cur); //Destroys the object currently managed by the unique_ptr (if any) and takes ownership of p.
				throw std::bad_alloc();
			}

			semantics.reset(p_new); //Destroys the object currently managed by the unique_ptr (if any) and takes ownership of p.
		}

		for (index_t i = 0; N != i; ++i) {
			setSize<N>(semantics, i, sizes[i]);
		}
	}

	template<std::size_t N>
	void resizeLazy(std::unique_ptr<Semantic[]> & semantics, size_type const size)
	{
		std::array<size_type, N> sizes;
		sizes.fill(size);
		resizeLazy<N>(semantics, sizes);
	}

	template<std::size_t N>
	void resizeLazy(std::unique_ptr<Semantic[]> & semantics, index_t const index, std::size_t const new_size)
	{
		// FIXME: Optimize
		std::array<size_type, N> s = sizes<N>(semantics);
		s[index] = new_size;
		resizeLazy<N>(semantics, s);
	}

	template<std::size_t N>
	void resize(std::unique_ptr<Semantic[]> & semantics, std::array<size_type, N> const &new_sizes)
	{
        static constexpr std::size_t N_H = 1 + (N - 1) / 2;

		auto const new_size = std::accumulate(std::begin(new_sizes), std::end(new_sizes), N_H);
		if (0 == new_size) {
			clear<N>(semantics);
			return;
		}

		auto const cur_sizes = sizes<N>(semantics);
		if (cur_sizes == new_sizes) {
			return;
		}

		for (int i = N - 1; 0 <= i; --i) {
			// Reduce the indices that should be reduced
			if (cur_sizes[i] <= new_sizes[i]) {
				continue;
			}

			std::move(end<N>(semantics, i), end<N>(semantics), begin<N>(semantics, i) + new_sizes[i]);
			setSize<N>(semantics, i, new_sizes[i]);
		}

		if (std::accumulate(std::begin(cur_sizes), std::end(cur_sizes), N_H) != new_size) {
			pointer p_cur = semantics.release();
			pointer p_new = static_cast<pointer>(realloc(p_cur, new_size * sizeof(Semantic)));

			if (!p_new) {
				semantics.reset(p_cur);
				throw std::bad_alloc();
			}

			semantics.reset(p_new);
		}

		if (0 == std::accumulate(std::begin(cur_sizes), std::end(cur_sizes), N_H)) {
			for (index_t i = 0; N != i; ++i) {
				setSize<N>(semantics, i, new_sizes[i]);
			}
		} else {
			// Increase the indices that should be increased
			auto last = semantics.get() + new_size;
			for (index_t i = N - 1; 0 != i; --i) {
				setSize<N>(semantics,i, new_sizes[i]);

				if (0 == new_sizes[i] || 0 == cur_sizes[i]) {
					continue;
				}

				auto first_i = begin<N>(semantics, i);
				auto last_i = end<N>(semantics, i);
				last = last != last_i ? std::move_backward(first_i, last_i, last) : first_i;
			}
		}
	}

	template<std::size_t N>
	void resize(std::unique_ptr<Semantic[]> & semantics, size_type const size)
	{
		std::array<size_type, N> sizes;
		sizes.fill(size);
		resize<N>(semantics, sizes);
	}

	template<std::size_t N>
	void resize(std::unique_ptr<Semantic[]> & semantics, index_t const index, std::size_t const new_size)
	{
		// FIXME: Optimize
		std::array<size_type, N> s = sizes<N>(semantics);
		s[index] = new_size;
		resize<N>(semantics, s);
	}

	template<std::size_t N>
	void clear(std::unique_ptr<Semantic[]> & semantics, index_t const index) { resize<N>(semantics, index, 0); }




	template <std::size_t N, class InputIt>
	size_type numAlreadyExists(std::unique_ptr<Semantic[]> const& semantics, index_t const index, InputIt first, InputIt last)
	{
		size_type num = 0;

		auto first_index = cbegin<N>(semantics, index);
		auto last_index = cend<N>(semantics, index);
		for (; first != last && first_index != last_index; ++first) {
			label_t label;
			if constexpr (std::is_same_v<Semantic,
			                             typename std::iterator_traits<InputIt>::value_type>) {
				label = first->label;
			} else {
				label = *first;
			}
			first_index = lower_bound<N>(first_index, last_index, label);
			if (first_index != last_index && first_index->label == label) {
				++num;
			}
		}

		return num;
	}

	// 
	// Insert or assign helper
	//

	// just insert, no allocation
	/*!
	 * @brief
	 *
	 * @note Memory assumed already allocated
	 *
	 * @param index
	 * @param cur_size
	 * @param new_size
	 * @param first
	 * @param last
	 */
	template <std::size_t N, bool Assign, class InputIt>
	void insertOrAssign(std::unique_ptr<Semantic[]> & semantics, index_t index, size_type cur_size, size_type new_size,
	                    InputIt first, InputIt last)
	{
		if (0 == new_size) {
			return;
		} else if (0 == cur_size) {
			std::copy(first, last, begin<N>(semantics, index));
			return;
		}

		if constexpr (!Assign) {
			if (cur_size == new_size) {
				return;
			}
		}

		auto cur = end<N>(semantics, index);
		auto first_index = begin<N>(semantics, index);
		auto last_index = first_index + cur_size;
		while (first != last && first_index != last_index) {
			if constexpr (Assign) {
				if ((last_index - 1)->label == (last - 1)->label) {
					(--cur)->label = (--last_index)->label;
					cur->value = (--last)->value;
					continue;
				}
			}
			if ((last_index - 1)->label < (last - 1)->label) {
				*(--cur) = *(--last);
				if constexpr (!Assign) {
					++cur_size;
					// FIXME: Does this actually improve performance?
					if (cur_size == new_size) {
						return;
					}
				}
			} else {
				// FIXME: Can this be a move? What happens if it is the same?
				*(--cur) = *(--last_index);
			}
		}

		// Copy the remaining to the beginning
		std::copy(first, last, first_index);
	}


	// Allocates space for the element and inserts the element at the correct position
	// Assign: if true value of semantic is overwritten if label is already present, 
	// if false nothing happens
	// UnaryFunction: function that is applied to Semantic with label
	//
	// return iterator to element and a bool if insertion happened
	template <std::size_t N, bool Assign, class UnaryFunction>
	std::pair<iterator, bool> insertOrAssign(std::unique_ptr<Semantic[]> & semantics, index_t const index, label_t label,
	                                         UnaryFunction f)
	{
		if (empty<N>(semantics, index)) {
			resize<N>(semantics, index, 1);
			auto it = begin<N>(semantics, index);
			it->label = label;
			it->value = f(Semantic(label));
			return {it, true};
		}

		auto it = lower_bound<N>(semantics, index, label);
		if (end<N>(semantics, index) != it && it->label == label) {
			// Label already exists
			if constexpr (Assign) {
				it->value = f(*it);
			}
			return {it, false};
		}

		// insert at right position
		auto i = std::distance(begin<N>(semantics, index), it);
		resize<N>(semantics, index, size<N>(semantics, index) + 1);
		it = begin<N>(semantics, index) + i;
		auto last_index = end<N>(semantics, index);
		std::move_backward(it, last_index - 1, last_index);
		it->label = label;
		it->value = f(Semantic(label));
		return {it, true};
	}

	// allocate the space and insert/assign to all nodes
	template <std::size_t N, bool Assign, class UnaryFunction>
	void insertOrAssign(std::unique_ptr<Semantic[]> & semantics, label_t label, UnaryFunction f)
	{
		if (empty<N>(semantics)) {
			std::array<size_type, N> s;
			s.fill(1);
			resize(s);
			std::fill(begin<N>(semantics), end<N>(semantics), Semantic(label, f(Semantic(label))));
			return;
		}

		std::array<size_type, N> new_sizes = sizes<N>(semantics);
		std::array<difference_type, N> dist;
		for (index_t index = 0; N != index; ++index) {
			auto it = lower_bound<N>(semantics, index, label);
			if (end<N>(semantics, index) != it && it->label == label) {
				// Label already exists
				if constexpr (Assign) {
					it->value = f(*it);
				}
				dist[index] = 0;
			} else {
				++new_sizes[index];
				dist[index] = std::distance(index, it);
			}
		}

		if (0 == std::accumulate(std::begin(dist), std::end(dist))) {
			return;
		}

		resize(new_sizes);

		for (index_t index = 0; N != index; ++index) {
			if (0 == dist[index]) {
				continue;
			}
			auto it = begin<N>(semantics, index) + dist[index];
			auto last_index = end<N>(semantics, index);
			std::move_backward(it, last_index - 1, last_index);
			it->label = label;
			it->value = f(Semantic(label));
		}
	}


	// allocate the space and insert/assign to index, iterators
	template <std::size_t N, bool Assign, class InputIt>
	void insertOrAssign(std::unique_ptr<Semantic[]> & semantics, index_t const index, InputIt first, InputIt last)
	{
		std::vector vec(first, last);

		std::sort(std::begin(vec), std::end(vec));

		// Erase duplicate labels, saving highest value for each label
		auto r_last = std::unique(std::rbegin(vec), std::rend(vec),
		                          [](auto a, auto b) { return a.label == b.label; });

		auto f = r_last.base();
		auto l = std::end(vec);
		auto s = std::distance(f, l);

		if (empty<N>(semantics, index)) {
			// Optimized insert
			resize<N>(semantics, index, s);
			std::copy(f, l, begin<N>(semantics, index));
		} else {
			auto new_size = s - numAlreadyExists<N>(semantics, index, f, l);
			auto cur_size = size<N>(semantics, index);

			resize<N>(semantics, index, new_size);

			// Do insert where memory already has been allocated
			insertOrAssign<N, Assign>(semantics, index, cur_size, new_size, f, l);
		}
	}

	// allocate the space and insert/assign to all nodes, iterators
	// allocate all space for all nodes at the same time, then insert
	template <std::size_t N, bool Assign, class InputIt>
	void insertOrAssign(std::unique_ptr<Semantic[]> & semantics, InputIt first, InputIt last)
	{
		std::vector vec(first, last);

		std::sort(std::begin(vec), std::end(vec));

		// Erase duplicate labels, saving highest value for each label
		auto r_last = std::unique(std::rbegin(vec), std::rend(vec),
		                          [](auto a, auto b) { return a.label == b.label; });

		auto f = r_last.base();
		auto l = std::end(vec);
		auto s = std::distance(f, l);

		std::array<size_type, N> new_sizes;
		new_sizes.fill(s);

		if (empty<N>(semantics)) {
			resize<N>(semantics, new_sizes);
			for (index_t index = 0; N != index; ++index) {
				std::copy(f, l, begin<N>(semantics, index));
			}
		} else {
			for (index_t index = 0; N != index; ++index) {
				new_sizes[index] -= numAlreadyExists<N>(semantics, index, f, l);
			}

			std::array<size_type, N> cur_sizes = sizes<N>(semantics);
			resize<N>(semantics, new_sizes);

			// Do insert where memory already has been allocated
			for (index_t index = 0; N != index; ++index) {
				insertOrAssign<N, Assign>(semantics, index, cur_sizes[index], new_sizes[index], f, l);
			}
		}
	}
	

	//
	// Insert
	//

	// all
	template<std::size_t N>
	void insert(std::unique_ptr<Semantic[]> & semantics, label_t label, value_t value)
	{
		insertOrAssign<N, false>(semantics, label, [value](auto) { return value; });
	}

	// template<std::size_t N>
	// void insert(std::unique_ptr<Semantic[]> & semantics, Semantic semantic) { return insert<N>(semantic.label, semantic.value); }


	template <std::size_t N, class InputIt>
	void insert(std::unique_ptr<Semantic[]> & semantics, InputIt first, InputIt last)
	{
		insertOrAssign<N, false>(semantics, first, last);
	}


	// index
	template<std::size_t N>
	std::pair<iterator, bool> insert(std::unique_ptr<Semantic[]> & semantics, index_t const index, label_t label, value_t value)
	{
		return insertOrAssign<N, false>(semantics, index, label, [value](auto) { return value; });
	}

	template <std::size_t N, class InputIt>
	std::pair<iterator, bool> insert(std::unique_ptr<Semantic[]> & semantics, index_t const index, InputIt first, InputIt last)
	{
		return insertOrAssign<N, false>(semantics, index, first, last);
	}

	// template<std::size_t N>
	// std::pair<iterator, bool> insert(std::unique_ptr<Semantic[]> & semantics, index_t const index, Semantic semantic)
	// {
	// 	return insert<N>(semantics, index, semantic.label, semantic.value);
	// }

	// iterator insert(index_t const index, const_iterator hint, Semantic semantic)
	// {
	// 	return insert(index, hint, semantic.label, semantic.value);
	// }

	// iterator insert(index_t const index, const_iterator hint, label_t label, value_t value)
	// {
	// 	return insertOrAssign<false>(index, hint, label, [value](auto) { return value; });
	// }



	//
	// InsertOrAssign
	//

	// all
	template<std::size_t N>
	void insertOrAssign(std::unique_ptr<Semantic[]> & semantics, label_t label, value_t value) 
	{
		insertOrAssign<N, true>(semantics, label, [value](auto) { return value; });
	}
	
	template <std::size_t N, class InputIt>
	void insertOrAssign(std::unique_ptr<Semantic[]> & semantics, InputIt first, InputIt last)
	{
		insertOrAssign<N, true>(semantics, first, last);
	}

	// index
	template<std::size_t N>
	std::pair<iterator, bool> insertOrAssign(std::unique_ptr<Semantic[]> & semantics, index_t index, label_t label, value_t value) 
	{
		return insertOrAssign<N, true>(semantics, index, label, [value](auto) { return value; });
	}
	
	template <std::size_t N, class InputIt>
	std::pair<iterator, bool>  insertOrAssign(std::unique_ptr<Semantic[]> & semantics, index_t index, InputIt first, InputIt last)
	{
		return insertOrAssign<N, true>(semantics, index, first, last);
	}

	
}

#endif  // UFO_MAP_SEMANTIC_UTIL_H