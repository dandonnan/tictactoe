/*
	For more information on this, refer to game.cpp
*/

#include <3ds.h>
#include <cstring>
#include <cstdlib>
#include <stdio.h>

#include "Button.h"

#ifndef Game_H
#define Game_H

class Game{
public:
	Game();
	~Game();

	void reset();
	bool checkTied();
	bool checkWin(int);
	void mainUpdate();
	void playerUpdate();
	void cpuUpdate();
	bool isSlotFree(int);
	void drawImage(gfxScreen_t, gfx3dSide_t, u8*, u16, u16, s16, s16);
	void drawMarker(u8*, int);
	void render();

private:
	enum Modes { Player, CPU, Win, Loss, Tied };
	Modes currentMode;
	int slots[9];
	Button button[9];
};

#endif