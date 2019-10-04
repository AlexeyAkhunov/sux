/*
 * Sux: Succinct data structures
 *
 * Copyright (C) 2007-2013 Sebastiano Vigna
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the Free
 *  Software Foundation; either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  This library is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef rank9sel_h
#define rank9sel_h
#include <cstdint>
#include "../Rank.hpp"
#include "../Select.hpp"
#include "../SelectZero.hpp"

namespace sux {

class Rank9Sel : public Rank, public Select, public SelectZero {
private:
  const uint64_t *bits;
  uint64_t *counts, *inventory, *subinventory;
  uint64_t num_words, num_counts, inventory_size, ones_per_inventory, log2_ones_per_inventory,
      num_ones;

public:
  Rank9Sel(const uint64_t *const bits, const uint64_t num_bits);
  uint64_t rank(const size_t pos);
  uint64_t select(const size_t rank);
  uint64_t bitCount();
};

}
#endif
