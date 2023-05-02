#ifndef PUZZLE_HPP_INCLUDED
#define PUZZLE_HPP_INCLUDED

#include "symbol.hpp"

#include <etl/algorithm.h>
#include <etl/array.h>
#include <etl/vector.h>

/*
 * A puzzle is just a sequence of up to 8 symbols. A custom class is used here instead of
 * etl::vector because it needs to be constexpr to be put in progmem.
 */
class Puzzle
{
public:
	using value_type = Symbol;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type&;
	using const_reference = value_type const&;
	using pointer = value_type*;
	using const_pointer = value_type const*;
	using iterator = etl::array<Symbol, 8>::iterator;
	using const_iterator = etl::array<Symbol, 8>::const_iterator;
	using reverse_iterator = etl::array<Symbol, 8>::reverse_iterator;
	using const_reverse_iterator = etl::array<Symbol, 8>::const_reverse_iterator;

private:
	etl::array<Symbol, 8> data_ = {};
	size_type size_ = 0;

public:
	constexpr
	Puzzle() = default;

	constexpr
	Puzzle(std::initializer_list<Symbol> init)
	{
		size_ = init.size();

		// Unfortunately, I have to access the buffer from data_ directly to make this constexpr.
		etl::copy_s(init.begin(), init.end(), data_._buffer, data_._buffer + size_);
	}

	constexpr
	auto size() const -> size_type { return size_; }

	auto begin() -> iterator { return data_.begin(); }
	constexpr
	auto begin() const -> const_iterator{ return data_.cbegin(); }
	constexpr
	auto cbegin() const -> const_iterator { return data_.cbegin(); }

	auto end() -> iterator { return begin() + size(); }
	constexpr
	auto end() const -> const_iterator { return begin() + size(); }
	constexpr
	auto cend() const -> const_iterator { return cbegin() + size(); }

	auto rend() -> reverse_iterator { return data_.rend(); }
	constexpr
	auto rend() const -> const_reverse_iterator { return data_.crend(); }
	constexpr
	auto crend() const -> const_reverse_iterator { return data_.crend(); }

	auto rbegin() -> reverse_iterator { return rend() - size(); }
	constexpr
	auto rbegin() const -> const_reverse_iterator { return rend() - size(); }
	constexpr
	auto crbegin() const -> const_reverse_iterator { return crend() - size(); }

	auto operator[](size_type i) -> reference { return data_[i]; }
	constexpr
	auto operator[](size_type i) const -> const_reference { return data_[i]; }

	void push_back(Symbol v) { data_[size_++] = v; }
};

auto apply_special_symbols(Puzzle const& p) -> Puzzle;

// Check whether the puzzle is in a solved position.
auto is_solved(Puzzle const& p) -> bool;

#endif
