#include "puzzle.hpp"

auto is_solved(Puzzle const& p) -> bool
{
	Puzzle result;

	// Copy over blocks, modified by special symbols.
	for (size_t i = 0; i < p.size(); ++i)
	{
		Symbol s = p[i];
		if (!s.is_block) { continue; }

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

	return false;
}
