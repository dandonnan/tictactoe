/*
	This is the main entry point for the program
*/

//Here we include some basic libraries that are required
#include <3ds.h>
#include <cstring>
#include <cstdlib>

//This includes the game.h and game.cpp files so we can call them from here
//All of the game logic is done in game.cpp
#include "Game.h"

int main()
{
	//Initialise stuff
	srvInit();
	aptInit();
	hidInit(NULL);
	gfxInitDefault();
	//gfxSet3D(true);	//Turn 3D on

	//Create a new instance of Game() and call reset() to set everything up
	Game* game = new Game();
	game->reset();

	//This is the main game loop
	while (aptMainLoop())
	{
		//Check input
		hidScanInput();
		u32 kDown = hidKeysDown();

		//If 'X' button is down, quit to the Homebrew Channel
		if (kDown & KEY_X)
			break;

		//Call update and render functions on Game
		game->mainUpdate();
		game->render();

		//flush the buffers so stuff is drawn
		gfxFlushBuffers();
		gfxSwapBuffers();

		//VBlank
		gspWaitForEvent(GSPEVENT_VBlank0, false);
	}
	
	//Before closing the game, shut everything down
	game->~Game();
	delete game;
	gfxExit();
	hidExit();
	aptExit();
	srvExit();

	return 0;
}