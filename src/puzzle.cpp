#include "puzzle.hpp"

#include "symbol.hpp"

#include <etl/array.h>

auto apply_special_symbols(Puzzle const& p) -> Puzzle
{
	Puzzle result;

	// Copy over blocks, modified by special symbols.
	for (size_t i = 0; i < p.size(); ++i)
	{
		Symbol s = p[i];
		if (!s.is_block) { continue; }

		// Locked blocks aren't affected by special symbols.
		if (s.b.is_locked)
		{
			result.push_back(s);
			continue;
		}

		// List of indices to apply special operations from.
		etl::vector<size_t, 2> to_apply;

		if (i != 0 && !p[i - 1].is_block && p[i - 1].s.right)
		{
			to_apply.push_back(i - 1);
		}
		if (i + 1 != p.size() && !p[i + 1].is_block && p[i + 1].s.left)
		{
			to_apply.push_back(i + 1);
		}

		// Whether this symbol should be added to the result.
		bool should_add_to_result = true;
		for (size_t const si : to_apply)
		{
			switch (p[si].s.type)
			{
			case SymbolType::remove:
				should_add_to_result = false;
				break;
				
			case SymbolType::flip_up_down:
				for (int const c : {0, 1})
				{
					swap_block_bits(s, 0, c, 2, c);
				}
				break;

			case SymbolType::flip_left_right:
				for (int const r : {0, 1, 2})
				{
					swap_block_bits(s, r, 0, r, 1);
				}
				break;
			}
		}

		if (should_add_to_result)
		{
			result.push_back(s);
		}
	}

	return result;
}

// Left distances and widths of a row.
struct RowMetrics
{
	int left_dist; // Distance from left to first dot. -1 means the row is empty.
	int width; // Width of the content of the row.
};

static
auto block_metrics(Symbol s) -> etl::array<RowMetrics, 3>
{
	etl::array<RowMetrics, 3> result;
	for (size_t i = 0; i < 3; ++i)
	{
		// Extremely jank way of converting (left, right) into a number for switching.
		switch (2 * !!block_bit(s, static_cast<int>(i), 0)
			+ !!block_bit(s, static_cast<int>(i), 1))
		{
		case 0b00: result[i] = {-1, 0}; break;
		case 0b01: result[i] = {1, 1}; break;
		case 0b10: result[i] = {0, 1}; break;
		case 0b11: result[i] = {0, 2}; break;
		}
	}

	return result;
}

auto is_solved(Puzzle const& p) -> bool
{
	auto const normalized = apply_special_symbols(p);

	// Distances in each row to the left wall.
	int left_spaces[3] = {0, 0, 0};
	for (Symbol const s : p)
	{
	}

	return true;
}