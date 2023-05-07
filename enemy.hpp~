#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <vector>
#include <tuple>
#include <utility>
#include <cstddef>
#include <ext/pb_ds/assoc_container.hpp>

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

#define LOOSE -1
#define DRAW 0
#define WIN 1
#define ONGOING 2
#define MAXN 4

using mt = std :: pair<char, char>;

class enemy {
	ht<size_t, mt> memoMAX, memoMIN;
	int col[MAXN], row[MAXN], m_diag, s_diag, status, npos, n;
	char table[MAXN * MAXN];
	bool checkUtility(int, int&, int&);
	bool mustPlace(int, int&, int&);
	bool canDoubleWin(int, int&, int&);
	void transpose();
	void reverseCols();
	void reverseRows();
	size_t addr();
	public:
	enemy();
	enemy(int);
	void put(int, int, char);
	void rem(int, int);
	bool can(int, int);
	mt MAX();
	mt MIN();
	void load();
	mt next();
	int getStatus();
	void dispose();
};

#endif
