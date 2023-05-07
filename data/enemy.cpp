#include "enemy.hpp"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

#define DEBUG

size_t operator!(const std :: vector<char>& a) {
	size_t h = 0;
	for(char v : a) h = 3 * h + v + 1;
	return h;
}

ht<size_t, mt> enemy :: memoMAX;
ht<size_t, mt> enemy :: memoMIN;

template<int N>
enemy :: enemy() {}

enemy :: enemy(int n) {
	this->n = n;
	table.assign(n * n, 0);
	col.assign(n, 0);
	row.assign(n, 0);
	st = 2;
	npos = 0;
	fd[0] = fd[1] = 0;
	sd[0] = sd[1] = 0;
}

void enemy :: put(int i, int j, char v) {
	int id = v == 1;
	row[i] += v;
	col[j] += v;
	fd[id] += v * (i == j);
	sd[id] += v * (i == n - j - 1);
	st = abs(row[i]) == n || abs(col[j]) == n || (i == j && abs(fd[id]) == n) || (i == n - j - 1 && abs(sd[id]) == n) ? v : st;
	table[i * n + j] = v;
	st = ++npos == n * n && st == ONGOING ? DRAW : st;
}

void enemy :: rem(int i, int j) {
	int v = table[i * n + j];
	int id = v == 1;
	row[i] -= v;
	col[j] -= v;
	fd[id] -= v * (i == j);
	sd[id] -= v * (i == n - j - 1);
	table[i * n + j] = 0;
	--npos;
	st = ONGOING;
}

mt enemy :: MAX() {
	if(st != ONGOING) return memoMAX[!table] = {st, -1};

	for(int t = 0; t < 2; ++t) {
		for(int r = 0; r < 2; ++r) {
			for(int c = 0; c < 2; ++c) {
				if(auto it = memoMAX.find(!table); it != memoMAX.end()) {
					auto ans = it->second;
					auto [v, a] = ans;
					int x = a / n, y = a % n;
					if(c) {y = n - y - 1; reverseCols();}
					if(r) {x = n - x - 1; reverseRows();}
					if(t) {std::swap(x,y); transpose();}
					return {v, x * n + y};
				}
				reverseCols();
			}
			reverseRows();
		}
		transpose();
	}

	if(int x, y; mustPlace(1, x, y)) {
		put(x, y, 1);
		auto [v, _] = MIN();
		rem(x, y);
		return memoMAX[!table] = {v, x * n + y};
	}

	if(int x, y; checkUtility(-1, x, y)) return memoMAX[!table] = {LOOSE, x * n + y};
	
	if(int x, y; canDoubleWin(1, x, y)) {
		put(x, y, 1);
		auto [v, _] = MIN();
		rem(x, y);
		return memoMAX[!table] = {v, x * n + y};
	}

	if(int x, y; goDraw(x, y)) return memoMAX[!table] = {DRAW, x * n + y};
	
	mt ans(-ONGOING, 0);

	for(int i = 0; i < n && ans.first < 1; ++i) {
		for(int j = 0; j < n && ans.first < 1; ++j) {
			if(table[i * n + j]) continue;
			put(i, j, 1);
			auto [v, _] = MIN();
			rem(i, j);
			if(v > ans.first) ans = {v, i * n + j};
		}
	}

	return memoMAX[!table] = ans;
}

mt enemy :: MIN() {
	if(st != ONGOING) return memoMIN[!table] = {st, -1};

	for(int t = 0; t < 2; ++t) {
		for(int r = 0; r < 2; ++r) {
			for(int c = 0; c < 2; ++c) {
				if(auto it = memoMIN.find(!table); it != memoMIN.end()) {
					auto ans = it->second;
					auto [v, a] = ans;
					int x = a / n, y = a % n;
					if(c) {y = n - y - 1; reverseCols();}
					if(r) {x = n - x - 1; reverseRows();}
					if(t) {std::swap(x,y); transpose();}
					return {v, x * n + y};
				}
				reverseCols();
			}
			reverseRows();
		}
		transpose();
	}

	if(int x, y; mustPlace(-1,x, y)) {
		put(x, y, -1);
		auto [v, _] = MAX();
		rem(x, y);
		return memoMIN[!table] = {v, x * n + y};
	}

	if(int x, y; checkUtility(1, x, y)) return memoMIN[!table] = {WIN, x * n + y};
	
	if(int x, y; canDoubleWin(-1, x, y)) {
		put(x,y,-1);
		auto [v, _] = MAX();
		rem(x, y);
		return memoMIN[!table] = {v, x * n + y};
	}

	if(int x, y; goDraw(x, y)) return memoMIN[!table] = {DRAW, x * n + y};

	mt ans(2, 0);

	for(int i = 0; i < n && ans.first > -1; ++i) {
		for(int j = 0; j < n && ans.first > -1; ++j) {
			if(table[i * n + j]) continue;
			put(i, j, -1);
			auto [v, _] = MAX();
			rem(i, j);
			if(v < ans.first) ans = {v, i * n + j};
		}
	}

	return memoMIN[!table] = ans;
}

void enemy :: load() {
#ifdef DEBUG
	long long A = std :: chrono::steady_clock::now().time_since_epoch().count();
#endif
	(void)MAX();
#ifdef DEBUG
	long long B = std :: chrono::steady_clock::now().time_since_epoch().count();
	auto p = 1.0L * std :: chrono :: system_clock::period::num / std :: chrono :: system_clock::period::den;
	std::cout.precision(5);
	std::cout.setf(std::ios_base::fixed);
	std::cout<<"load time: " << p*(B-A) << " seconds\n";
#endif
}

mt enemy :: next() {
	if(npos == 0) {
		srand(time(0));
		return {rand() % n, rand() % n};
	}
	auto [_, v] = MAX();
	return {v/n, v%n};
}

int enemy :: curState() {
	return st;
}

bool enemy :: can(int i, int j) {
	return st == ONGOING && table[i * n + j] == 0;
}

bool enemy :: checkUtility(int p, int& x, int& y) {
	if(npos >= n * n - 2) return false;
	int f = -1, s = -1;
	for(int j = 0; j < n; ++j) {
		if(col[j] == p * (n - 1)) {
			if(f != -1) s = j;
			else f = j;
		}
	}
	for(int i = 0; i < n; ++i) {
		if(row[i] != p * (n - 1)) continue;
		if(table[i*n+f]||(s!=-1&&table[i*n+s])) {
			int c = table[i*n+f] ? f : s;
			for(int a=0;a<n;++a) {
				if(a==i) continue;
				for(int b=0;b<n;++b) {
					if(b == c) continue;
					x = a, y = b;
					return true;
				}
			}
		}
	}
	return false;
}

bool enemy :: mustPlace(int p, int& x, int& y) {
	for(int i = 0; i < n; ++i) {
		if(row[i] == p * (n - 1)) {
			x = i;
			for(int j = 0; j < n; ++j) if(!table[i * n + j]) y = j;
			return true;
		}
	}
	for(int j = 0; j < n; ++j) {
		if(col[j] == p * (n - 1)) {
			y = j;
			for(int i = 0; i < n; ++i) if(table[i * n + j] == 0) x = i;
			return true;
		}
	}
	for(int i = 0; i < n; ++i) {
		if(row[i] == p * (1 - n)) {
			x = i;
			for(int j = 0; j < n; ++j) if(table[i * n + j] == 0) y = j;
			return true;
		}
	}
	for(int j = 0; j < n; ++j) {
		if(col[j] == p * (1 - n)) {
			y = j;
			for(int i = 0; i < n; ++i) if(table[i * n + j] == 0) x = i;
			return true;
		}
	}
	return false;
}

bool enemy :: canDoubleWin(int p, int& x, int& y) {
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			if(table[i*n+j]) continue;
			put(i, j, p);
			if(checkUtility(p,x,y)) {
				x = i, y = j;
				rem(i, j);
				return true;
			}
			rem(i, j);
		}
	}
	return false;
}

bool enemy :: goDraw(int& x, int& y) {
	for(int p : {-1, 1}) {
		for(int i = 0; i < n; ++i) {
			int r = 0;
			for(int j = 0; j < n; ++j) {
				r += table[i*n+j]==-p;
				if(!table[i*n+j]) x = i, y = j;
			}
			if(!r) return false;
		}
		for(int j = 0; j < n; ++j) {
			int c = 0;
			for(int i = 0; i < n; ++i) c += table[i*n+j]==-p;
			if(!c) return false;
		}
	}
	return true;
}

void enemy :: dispose() {
	memoMAX.clear();
	memoMIN.clear();
}

void enemy :: transpose() {
	for(int i = 0; i < n; ++i)
		for(int j = i; j < n; ++j)
			std :: swap(table[i*n+j],table[j*n+i]);
}

void enemy :: reverseCols() {
	for(int i = 0; i < n; ++i)
		for(int j = 0; 2 * j < n; ++j)
			std :: swap(table[i*n+j],table[i*n+n-j-1]);
}

void enemy :: reverseRows() {
	for(int i = 0; 2 * i < n; ++i)
		for(int j = 0; j < n; ++j)
			std :: swap(table[i*n+j],table[(n-i-1)*n+j]);
}
