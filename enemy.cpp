#include "enemy.hpp"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstring>
#include <climits>
#include <cassert>

Enemy :: Enemy() {}

Enemy :: Enemy(int _n) : status {ONGOING}, npos {}, n {_n}  {
	memset(&table[0][0], EMPTY, sizeof table);
	memset(&col[0][0], 0, sizeof col);
	memset(&row[0][0], 0, sizeof row);
	memset(m_diag, 0, sizeof m_diag);
	memset(s_diag, 0, sizeof s_diag);
}

bool Enemy :: check(int x, int y) {
	return (table[x][y]!=EMPTY)&&(row[x][0]&&row[x][1])&&(col[y][0]&&col[y][1])&&(x!=y||(m_diag[0]&&m_diag[1]))&&(x!=n-y-1||(s_diag[0]&&s_diag[1]));
}

uint64_t Enemy :: addr() {
	uint64_t h[8], ans {};
	int origin[][2] = {{0, 0}, {0, n - 1}, {n - 1, 0}, {n - 1, n - 1}};
	int checked[MAXN][MAXN];

	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			checked[i][j] = check(i, j);

	memset(h, 0, sizeof h);

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			for(int k = 0; k < 4; ++k) {
				int x0 = origin[k][0], y0 = origin[k][1];
				int dx = x0 ? -1 : 1, dy = y0 ? -1 : 1;
				for(int t = 0; t < 2; ++t) {
					int x = x0 + dx * i, y = y0 + dy * j;
					if(t) std :: swap(x, y);
					#ifdef DEBUG
					assert(x >= 0 && x < n && y >= 0 && y < n);
					#endif
					h[2 * k + t] *= 5;
					h[2 * k + t] += checked[x][y] ? 3 : 1 + table[x][y];
				}
			}
		}
	}

	for(int i = 0; i < 8; ++i) if(ans < h[i]) ans = h[i];
	
	return ans;
}

void Enemy :: put(int i, int j, int pi) {
#ifdef DEBUG
	assert(i >= 0 && i < n && j >= 0 && j < n && pi >= 0 && pi <= 1);
#endif

	++row[i][pi];

	++col[j][pi];

	if(i == j) ++m_diag[pi];

	if(i == n - j - 1) ++s_diag[pi];

	++npos;

	table[i][j] = pi;

	if(row[i][pi] == n || col[j][pi] == n || m_diag[pi] == n || s_diag[pi] == n)
		status = pi ? WIN : LOOSE;
	else if(npos == n * n)
		status = DRAW;
	else
		status = ONGOING;
}

void Enemy :: rem(int i, int j) {
	int pi = table[i][j];

#ifdef DEBUG
	assert(pi >= 0 && pi <= 1);
#endif

	--row[i][pi];

	--col[j][pi];

	if(i == j) --m_diag[pi];

	if(i == n - j - 1) --s_diag[pi];

	--npos;

	table[i][j] = EMPTY;

	status = ONGOING;
}

int Enemy :: MAX(int a, int b) {
	if(status != ONGOING) return memoMAX[addr()] = status;

	if(auto it = memoMAX.find(addr()); it != memoMAX.end()) return it->second;

	if(int x, y; mustPlace(MY, x, y)) {
		put(x, y, MY);

		int result = MIN(a, b);

		rem(x, y);

		return memoMAX[addr()] = result;
	}

	if(int x, y; winAt(MY, x, y)) {
		put(x, y, MY);

		int result = MIN(a, b);

		rem(x, y);

		return memoMAX[addr()] = result;
	}

	int best = INT_MIN;

	for(int i = 0; i < n && best <= 0; ++i) {
		for(int j = 0; j < n && best <= 0; ++j) {
			if(table[i][j] != EMPTY) continue;
			
			put(i, j, MY);

			int result = MIN(a, b);
			
			rem(i, j);

			if(result > b) return result;

			if(result > best) best = result;

			if(result > a) a = result;
		}
	}

	return memoMAX[addr()] = best;
}

int Enemy :: MIN(int a, int b) {
	if(status != ONGOING) return memoMIN[addr()] = status;

	if(auto it = memoMIN.find(addr()); it != memoMIN.end()) return it->second;

	if(int x, y; mustPlace(YOUR, x, y)) {
		put(x, y, YOUR);

		int result = MAX(a, b);

		rem(x, y);

		return memoMIN[addr()] = result;
	}

	if(int x, y; winAt(YOUR, x, y)) {
		put(x, y, YOUR);

		int result = MAX(a, b);

		rem(x, y);

		return memoMIN[addr()] = result;
	}

	int best = INT_MAX;

	for(int i = 0; i < n && best >= 0; ++i) {
		for(int j = 0; j < n && best >= 0; ++j) {
			if(table[i][j] != EMPTY) continue;
			
			put(i, j, YOUR);

			int result = MAX(a, b);
			
			rem(i, j);

			if(result < a) return result;

			if(result < b) b = result;

			if(result < best) best = result;
		}
	}

	return memoMIN[addr()] = best;
}

void Enemy :: load() {
#ifdef DEBUG
	long long A = std :: chrono::steady_clock::now().time_since_epoch().count();
#endif
	(void)MAX(-1, 1);
#ifdef DEBUG
	long long B = std :: chrono::steady_clock::now().time_since_epoch().count();
	auto p = 1.0L * std :: chrono :: system_clock::period::num / std :: chrono :: system_clock::period::den;
	std :: cout.precision(5);
	std :: cout.setf(std :: ios_base :: fixed);
	std :: cout << "Load time: " << p * (B - A) << " seconds\n";
	std :: cout << "Hash table size: " << memoMAX.size() + memoMIN.size() << '\n';
#endif
}

std :: pair<int, int> Enemy :: next() {
	if(npos == 0) {
		srand(time(0));
		return {rand() % n, rand() % n};
	}

	int result = MAX(-1, 1);

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			if(table[i][j] != EMPTY) continue;

			put(i, j, MY);

			auto it = memoMIN.find(addr());

			rem(i, j);

			if(it != memoMIN.end() && it->second == result) return {i, j};
		}
	}

	std :: cout << "erro: o próximo movimento não foi encontrado\n";

	exit(1);
	
	return {-1, -1};
}

int Enemy :: getStatus() {
	return status;
}

bool Enemy :: can(int i, int j) {
	return status == ONGOING && table[i][j] == EMPTY;
}

bool Enemy :: mustPlace(int pi, int& x, int& y) {
	for(int k = 0; k < 2; ++k) {

		if(m_diag[pi] == n - 1 && !m_diag[pi ^ 1]) {
			x = y = 0;
			while(table[x][y] != EMPTY) ++x, ++y;
			return true;
		}

		if(s_diag[pi] == n - 1 && !s_diag[pi ^ 1]) {
			x = 0, y = n - 1;
			while(table[x][y] != EMPTY) ++x, --y;
			return true;
		}

		for(int i = 0; i < n; ++i) {
			if(row[i][pi] == n - 1 && !row[i][pi ^ 1]) {
				x = i, y = 0;
				while(table[x][y] != EMPTY) ++y;
				return true;
			}

			if(col[i][pi] == n - 1 && !col[i][pi ^ 1]) {
				x = 0, y = i;
				while(table[x][y] != EMPTY) ++x;
				return true;
			}
		}

		pi ^= 1;
	}

	return false;
}

bool Enemy :: winAt(int pi, int& x, int& y) {
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			if(table[i][j] != EMPTY) continue;
			bool ok = false;

			put(i, j, pi);

			for(int s = 0; s < 16; ++s) {
				if(__builtin_popcount(s) < 2) continue;
				bool flag = true;
				flag = flag && ((~s&1)||(row[i][pi]==n-1&&!row[i][pi^1]));
				flag = flag && ((~s&2)||(col[j][pi]==n-1&&!col[j][pi^1]));
				flag = flag && ((~s&4)||(m_diag[pi]==n-1&&!m_diag[pi^1]));
				flag = flag && ((~s&8)||(s_diag[pi]==n-1&&!s_diag[pi^1]));
				ok = ok || flag;
			}

			rem(i, j);

			if(ok) {
				x = i, y = j;
				return true;
			}	
		}
	}

	return false;
}

void Enemy :: dispose() {
	memoMAX.clear();
	memoMIN.clear();
}
