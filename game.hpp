#ifndef GAME_HPP
#define GAME_HPP

#include "info.hpp"
#include "table.hpp"
#include "text.hpp"
#include <vector>
#include "enemy.hpp"

class Game {
	static Table table;
	static int turnPlayer;
	static enemy adv;
	static Text notif;
	static TTF_Font* textFont;
	static bool isRunning;
	public:
	static void build(int);
	static GAME_STATUS run();
	static void dispose();
};

#endif

