#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <vector>
#include <tuple>
#include <utility>
#include <cstdint>
#include <ext/pb_ds/assoc_container.hpp>
#include <map>

using namespace __gnu_pbds;

template <class K, class V>
using ht = gp_hash_table
<K, V, std :: hash<K>, std :: equal_to<K>,
direct_mask_range_hashing<>,
linear_probe_fn<>,
hash_standard_resize_policy<
hash_exponential_size_policy<>,
hash_load_check_resize_trigger
<>, true>>;

#define EMPTY -1
#define YOUR 0
#define MY	1

#define LOOSE -1
#define DRAW 0
#define WIN 1
#define ONGOING 2
#define MAXN 4

class Enemy {
	ht<uint64_t, char> memoMAX, memoMIN;
	int col[MAXN][2], row[MAXN][2], m_diag[2], s_diag[2], status, npos, n;
	int table[MAXN][MAXN];
	bool check(int, int);
	bool mustPlace(int, int&, int&);
	bool winAt(int, int&, int&);
	uint64_t addr();
	public:
	Enemy();
	Enemy(int);
	void put(int, int, int);
	void rem(int, int);
	bool can(int, int);
	int MAX(int, int);
	int MIN(int, int);
	void load();
	std :: pair<int, int> next();
	int getStatus();
	void dispose();
};

#endif
