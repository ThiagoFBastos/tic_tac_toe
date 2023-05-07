#include "enemy.hpp"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstring>

#define DEBUG

enemy :: enemy() {}

enemy :: enemy(int n) {
	this->n = n;
	memset(table, 0, sizeof table);
	memset(col, 0, sizeof col);
	memset(row, 0, sizeof row);
	m_diag = s_diag = 0;
	status = ONGOING;
	npos = 0;
}

size_t enemy :: addr() {
	size_t h = 0;
	for(int i = 0; i < n * n; ++i) h = 3 * h + table[i] + 1;
	return h;
}

void enemy :: put(int i, int j, char v) {
	row[i] += v;
	col[j] += v;
	m_diag += v * (i == j);
	s_diag += v * (i == n - j - 1);
	status = abs(row[i]) == n || abs(col[j]) == n || (i == j && abs(m_diag) == n) || (i == n - j - 1 && abs(s_diag) == n) ? v : status;
	table[i * n + j] = v;
	status = ++npos == n * n && status == ONGOING ? DRAW : status;
}

void enemy :: rem(int i, int j) {
	int v = table[i * n + j];
	row[i] -= v;
	col[j] -= v;
	m_diag -= v * (i == j);
	s_diag -= v * (i == n - j - 1);
	table[i * n + j] = 0;
	--npos;
	status = ONGOING;
}

mt enemy :: MAX() {
	if(status != ONGOING) return memoMAX[addr()] = {status, -1};

	for(int t = 0; t < 2; ++t) {
		for(int r = 0; r < 2; ++r) {
			for(int c = 0; c < 2; ++c) {
				if(auto it = memoMAX.find(addr()); it != memoMAX.end()) {
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
		return memoMAX[addr()] = {v, x * n + y};
	}

	if(int x, y; canDoubleWin(1, x, y)) {
		put(x, y, 1);
		auto [v, _] = MIN();
		rem(x, y);
		return memoMAX[addr()] = {v, x * n + y};
	}

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

	return memoMAX[addr()] = ans;
}

mt enemy :: MIN() {
	if(status != ONGOING) return memoMIN[addr()] = {status, -1};

	for(int t = 0; t < 2; ++t) {
		for(int r = 0; r < 2; ++r) {
			for(int c = 0; c < 2; ++c) {
				if(auto it = memoMIN.find(addr()); it != memoMIN.end()) {
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
		return memoMIN[addr()] = {v, x * n + y};
	}

	if(int x, y; canDoubleWin(-1, x, y)) {
		put(x,y,-1);
		auto [v, _] = MAX();
		rem(x, y);
		return memoMIN[addr()] = {v, x * n + y};
	}

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

	return memoMIN[addr()] = ans;
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

int enemy :: getStatus() {
	return status;
}

bool enemy :: can(int i, int j) {
	return status == ONGOING && table[i * n + j] == 0;
}

bool enemy :: checkUtility(int p, int& x, int& y) {
	int fc, sc;
	bool util = false;

	fc = sc = -1;

	for(int i = 0; i < n; ++i) {
		if(col[i] == p * (n - 1)) {
			if(fc != -1) sc = i;
			else fc = i;
		}
	}

	for(int i = 0; i < n; ++i) {
		util = util || (row[i] == p * (n - 1) && m_diag == p * (n - 1) && table[i * n + i])
		|| (row[i] == p * (n - 1) && s_diag == p * (n - 1) && table[i * n + n - i - 1])
		|| (row[i] == p * (n - 1) && ((fc != -1 && table[i * n + fc]) || (sc != -1 && table[i * n + sc])))
		|| (col[i] == p * (n - 1) && m_diag == p * (n - 1) && table[i * n + i])
		|| (col[i] == p * (n - 1) && s_diag == p * (n - 1) && table[(n - i - 1) * n + i]);
	}

	if(util) {
		for(int i = 0; i < n; ++i) {
			for(int j = 0; j < n; ++j) {
				if(!table[i * n + j]) {
					x = i, y = j;
					return true;
				}
			}
		}
	}

	return false;
}

bool enemy :: mustPlace(int p, int& x, int& y) {
	for(int k : {1, -1}) {
		for(int i = 0; i < n; ++i) {
			if(row[i] == k * p * (n - 1)) {
				x = i;
				for(int j = 0; j < n; ++j) if(!table[i * n + j]) y = j;
				return true;
			}
		}
		for(int j = 0; j < n; ++j) {
			if(col[j] == k * p * (n - 1)) {
				y = j;
				for(int i = 0; i < n; ++i) if(table[i * n + j] == 0) x = i;
				return true;
			}
		}
		if(m_diag == k * p * (n - 1)) {
			for(int i = 0; i < n; ++i) {
				if(!table[i*n+i]) {
					x = y = i;
					return true;
				}
			}
		}
		if(s_diag == k * p * (n - 1)) {
			for(int i = 0; i < n; ++i) {
				if(!table[i*n+n-i-1]) {
					x = i, y = n - i - 1;
					return true;
				}
			}
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
