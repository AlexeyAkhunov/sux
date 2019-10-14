/*
 * Sux: Succinct data structures
 *
 * Copyright (C) 2019 Emmanuel Esposito and Sebastiano Vigna
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

#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>
#include "../support/common.hpp"

using namespace std;

namespace sux::function {

#define DEFAULT_VECTSIZE (1 << 2)

class RiceBitvector {
  private:
	uint64_t *data;
	std::size_t data_bytes;
	std::size_t bit_count;

	size_t curr_fixed_offset;
	uint64_t *curr_ptr_unary;
	uint64_t curr_window_unary;
	int valid_lower_bits_unary;

  public:
	RiceBitvector(const size_t alloc_words = DEFAULT_VECTSIZE);
	~RiceBitvector();
	uint64_t read_next(const int log2golomb);
	void skip_subtree(const size_t nodes, const size_t fixed_len);
	void read_reset(const size_t bit_pos = 0, const size_t unary_offset = 0);
	void append_fixed(const uint64_t v, const int log2golomb);
	void append_unary_all(const vector<uint32_t> unary);
	size_t get_bits() const;
	void fit_data();
	void print_bits() const;
	int dump(FILE *fp) const;
	void load(FILE *fp);
};

} // namespace sux
