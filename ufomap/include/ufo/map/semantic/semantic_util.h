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

	template<class InputIt, std::size_t N>
	[[nodiscard]] InputIt lower_bound(InputIt first, InputIt last, label_t label) 
	{
		return std::lower_bound(first, last, label);
	}

    template<std::size_t N>
	[[nodiscard]] iterator lower_bound(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label) 
	{
		// return std::lower_bound(begin<N>(semantics, index), end<N>(semantics, index), label);
		return lower_bound<iterator, N>(begin<N>(semantics, index), end<N>(semantics, index), label);
	}

	//
	// Upper bound
	//

	template<class InputIt, std::size_t N>
	[[nodiscard]] InputIt upper_bound(InputIt first, InputIt last, label_t label) 
	{
		return std::upper_bound(first, last, label);

	}

    template<std::size_t N>
	[[nodiscard]] iterator upper_bound(std::unique_ptr<Semantic[]> const& semantics, index_t index, label_t label) 
	{
		// return std::upper_bound(begin<N>(semantics, index), end<N>(semantics, index), label);
		return upper_bound<iterator, N>(begin<N>(semantics, index), end<N>(semantics, index), label);
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
			auto lower = lower_bound<const_iterator, N>(first, last, range.lower());
			first = upper_bound<const_iterator, N>(lower, last, range.upper());
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


	template <class UnaryPredicate, std::size_t N>
	[[nodiscard]] bool all(std::unique_ptr<Semantic[]> const& semantics, index_t index, UnaryPredicate p)
	{
		return std::all_of(cbegin<N>(semantics, index), cend<N>(semantics, index), p);
	}

	template <class UnaryPredicate, std::size_t N>
	[[nodiscard]] bool all(std::unique_ptr<Semantic[]> const& semantics, UnaryPredicate p)
	{
		return std::all_of(cbegin<N>(semantics), cend<N>(semantics), p);
	}



	template <class UnaryPredicate, std::size_t N>
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
			auto lower = lower_bound<const_iterator, N>(first, last, range.lower());
			first = upper_bound<const_iterator, N>(lower, last, range.upper());
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

	template <class UnaryPredicate, std::size_t N>
	[[nodiscard]] bool all(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRange range, UnaryPredicate p)
	{
		return all<UnaryPredicate, N>(semantics, index, SemanticRangeSet(range), p);
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
				first = lower_bound<const_iterator, N>(first, last, range.lower());
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

	template <class UnaryPredicate, std::size_t N>
	[[nodiscard]] bool any(std::unique_ptr<Semantic[]> const& semantics, index_t index, UnaryPredicate p) 
	{
		return std::any_of(cbegin<N>(semantics, index), cend<N>(semantics, index), p);
	}

	template <class UnaryPredicate, std::size_t N>
	[[nodiscard]] bool any(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRange range, UnaryPredicate p) 
	{
		return any<N>(semantics, index, SemanticRangeSet(range), p);
	}

	template <class UnaryPredicate, std::size_t N>
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
				first = lower_bound<const_iterator, N>(first, last, range.lower());
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

	template <class UnaryPredicate, std::size_t N>
	[[nodiscard]] bool none(std::unique_ptr<Semantic[]> const& semantics, index_t index, UnaryPredicate p) 
	{
		return std::none_of(cbegin<N>(semantics, index), cend<N>(semantics, index), p);
	}

	template <class UnaryPredicate, std::size_t N>
	[[nodiscard]] bool none(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRange range, UnaryPredicate p) 
	{
		return none<UnaryPredicate, N>(semantics, index, SemanticRangeSet(range), p);
	}

	template <class UnaryPredicate, std::size_t N>
	[[nodiscard]] bool none(std::unique_ptr<Semantic[]> const& semantics, index_t index, SemanticRangeSet const &ranges, UnaryPredicate p) 
	{
		return !any<N>(semantics, index, ranges, p);
	}

	
}

#endif  // UFO_MAP_SEMANTIC_UTIL_H