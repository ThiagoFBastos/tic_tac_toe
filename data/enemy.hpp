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

using mt = std :: pair<char, char>;

template<int N>
class enemy {
	static ht<size_t, mt> memoMAX, memoMIN;
	int col[N], row[N];
	char table[N * N];
	int fd[2], sd[2], st, npos, n;
	bool checkUtility(int, int&, int&);
	bool mustPlace(int, int&, int&);
	bool canDoubleWin(int, int&, int&);	
	bool goDraw(int&, int&);
	void transpose();
	void reverseCols();
	void reverseRows();
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
	int curState();
	void dispose();
};

#endif
