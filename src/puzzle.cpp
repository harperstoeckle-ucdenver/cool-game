#include "puzzle.hpp"

#include "symbol.hpp"

#include <etl/array.h>
#include <etl/optional.h>

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
	etl::optional<int> left_dist; // Distance from left to first dot. Empty means no blocks.
	int width; // Width of the content of the row.
};

/*
 * For each row in a block symbol, get the distance from the left edge to the first dot and the
 * width of the blocks in the row. For
 * example, in the block
 *
 * ##
 *  #
 *
 *
 * The first row is right against the edge, so has distance 0. The second has distance 1. The third
 * doesn't have a distance because this whole block can move as far as it wants until one of the
 * first two rows hit something, so its distance is given a value of etl::nullopt.
 */
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
		case 0b00: result[i] = {etl::nullopt, 0}; break;
		case 0b01: result[i] = {1, 1}; break;
		case 0b10: result[i] = {0, 1}; break;
		case 0b11: result[i] = {0, 2}; break;
		}
	}

	return result;
}

// Check if all values in a range are equal.
template <typename It>
auto all_equal(It b, It e) -> bool
{
	if (b == e) { return true; }
	return etl::all_of(b, e, [=](auto v) { return v == *b; });
}

auto is_solved(Puzzle const& p) -> bool
{
	auto const normalized = apply_special_symbols(p);

	// Width of the built-up block in each row.
	etl::array<int, 3> built_widths = {0, 0, 0};
	for (Symbol const s : p)
	{
		// Don't bother trying to do anything with empty blocks.
		if (s.b.bits == 0) { continue; }

		// Metrics for the current block.
		auto const s_metrics = block_metrics(s);

		/*
		 * Differences between left widths and right distances. If all of the rows line up properly,
		 * then they should all be equal, ignoring null rows (since null rows can be moved any
		 * distance).
		 */
		etl::array<etl::optional<int>, 3> offsets;

		etl::transform_n(built_widths.cbegin(), s_metrics.cbegin(), 3, offsets.begin(),
			[](auto l, auto r) {
				return r.left_dist ? etl::optional<int>(*r.left_dist - l) : etl::nullopt;
			});

		// End of range of non-null offsets.
		auto const off_end_it =
			etl::remove(offsets.begin(), offsets.end(), etl::nullopt);

		// All offsets other than the null ones must be equal, or this is not valid.
		if (!all_equal(offsets.begin(), off_end_it))
		{
			return false;
		}

		// If the offsets are all equal, we can safely add the widths to the total built widths.
		etl::transform_n(built_widths.cbegin(), s_metrics.cbegin(), 3, built_widths.begin(),
			[](auto l, auto r) { return l + r.width; });
	}

	// The right side of the block must also be flat.
	return all_equal(built_widths.cbegin(), built_widths.cend());
}
