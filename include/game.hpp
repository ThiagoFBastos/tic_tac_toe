#ifndef GAME_HPP
#define GAME_HPP

#include "info.hpp"
#include "table.hpp"
#include "text.hpp"
#include <vector>
#include "enemy.hpp"
#include "button.hpp"

class Game {
	static Table table;
	static int turnPlayer;
	static Enemy adv;
	static Text notif;
	static TTF_Font* textFont;
	static bool isRunning;
	static Button btnReset;
	static int n;
	public:
	static void build(int);
	static GAME_STATUS run();
	static void dispose();
};

#endif

