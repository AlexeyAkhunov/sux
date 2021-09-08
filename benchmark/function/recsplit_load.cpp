#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>
#include <sux/function/RecSplit.hpp>

#define SAMPLES (11)

using namespace std;
using namespace sux::function;

template <typename T> void benchmark(RecSplit<LEAF, ALLOC_TYPE> &rs, const vector<T> &keys) {
	printf("Benchmarking...\n");

	uint64_t sample[SAMPLES];
	uint64_t h = 0;

	for (int k = SAMPLES; k-- != 0;) {
		auto begin = chrono::high_resolution_clock::now();
		uint64_t h = 0;
		for (size_t i = 0; i < keys.size(); i += 2) {
			h ^= rs(keys[i ^ (h & 1)]);
		}
		for (size_t i = 1; i < keys.size(); i += 2) {
			h ^= rs(keys[i ^ (h & 1)]);
		}
		auto end = chrono::high_resolution_clock::now();
		const uint64_t elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
		sample[k] = elapsed;
		printf("Elapsed: %.3fs; %.3f ns/key\n", elapsed * 1E-9, elapsed / (double)keys.size());
	}

	const volatile uint64_t unused = h;
	auto end = chrono::high_resolution_clock::now();
	sort(sample, sample + SAMPLES);
	printf("\nMedian: %.3fs; %.3f ns/key\n", sample[SAMPLES / 2] * 1E-9, sample[SAMPLES / 2] / (double)keys.size());
}

	using Bytes = std::string;

static std::optional<unsigned> decode_hex_digit(char ch) noexcept {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    } else if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 10;
    } else if (ch >= 'A' && ch <= 'F') {
        return ch - 'A' + 10;
    }
    return std::nullopt;
}


std::optional<Bytes> from_hex(std::string_view hex) noexcept {
    if (hex.length() >= 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        hex.remove_prefix(2);
    }

    if (hex.length() % 2 != 0) {
        return std::nullopt;
    }

    Bytes out{};
    out.reserve(hex.length() / 2);

    unsigned carry{0};
    for (size_t i{0}; i < hex.size(); ++i) {
        std::optional<unsigned> v{decode_hex_digit(hex[i])};
        if (!v) {
            return std::nullopt;
        }
        if (i % 2 == 0) {
            carry = *v << 4;
        } else {
            out.push_back(static_cast<char>(carry | *v));
        }
    }

    return out;
}

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <keys> <mphf>\n", argv[0]);
		return 1;
	}

	ifstream fin(argv[1]);
	string str;
	vector<string> keys;
	while (getline(fin, str)) {
		Bytes key;
		key = from_hex(str).value();
		keys.push_back(key);
	}
	fin.close();

	fstream fs;
	RecSplit<LEAF, ALLOC_TYPE> rs;

	fs.exceptions(fstream::failbit | fstream::badbit);
	fs.open(argv[2], std::fstream::in | std::fstream::binary);
	fs >> rs;
	fs.close();

	benchmark(rs, keys);

	return 0;
}
