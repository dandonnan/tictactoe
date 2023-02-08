/*
	For the most part, this is a copy of the code that I wrote for
	the Tic Tac Toe game for Android.

	All of the functions that handle the game states exist within here.

	How the game worked is that it each section of the grid is held in
	array at the corresponding index:

				_|_|_		0|1|2
				_|_|_		3|4|5
				_|_|_		6|7|8

	The array stores integers which are then devised based on the following:
		0 - The grid space is empty, and free to be overwritten
		1 - The grid space is occupied by a O
		2 - The grid space is occupied by a X
*/


#include "Game.h"

//These are references to the image (.bin) files stored within the /data folder
//When building the project, .h and .o files for each .bin will be created in the /build folder
#include "bg_top_bin.h"
#include "bg_bottom_bin.h"
#include "nought_bin.h"
#include "cross_bin.h"
#include "win_bin.h"
#include "lose_bin.h"
#include "tie_bin.h"

/*
	The constructor sets up all the buttons in the array
	For 3DS, rather than going from top to bottom (like on Android),
	the buttons go from bottom to top
*/
Game::Game()
{
	button[0] = Button(10, 160, 100, 80);
	button[1] = Button(110, 160, 100, 80);
	button[2] = Button(210, 160, 100, 80);

	button[3] = Button(10, 80, 100, 80);
	button[4] = Button(110, 80, 100, 80);
	button[5] = Button(210, 80, 100, 80);

	button[6] = Button(10, 0, 100, 80);
	button[7] = Button(110, 0, 100, 80);
	button[8] = Button(210, 0, 100, 80);
}

//destructor
Game::~Game()
{
}

/*	Reset
	This starts by going through each grid space and clearing it
	before setting it to be the player's turn
*/
void Game::reset()
{
	for (int i = 0; i < 9; i++)
		slots[i] = 0;

	//had a random function to determine if CPU went first on Android
	//I didn't implement this on 3DS (a bit of an oversight in that I
	//completely forgot), but it was essentially a 50% chance that
	//we would set the mode to CPU instead

	currentMode = Player;
}

/*	CheckTied
	This checks to see if the player has tied with the AI
	It goes through each grid space first and checks if a space is empty.
	If a space is still empty, it is not possible to call a tie until the
	space is used so it returns false.

	It then calls the CheckWin function for both player 1 and 2 to make
	sure that no player has actually won.

	Once satisfied all checks have been done, it then returns true to say
	the player has tied.
*/
bool Game::checkTied()
{
	//if slot free, return false
	for (int i = 0; i < 9; i++)
	{
		if (slots[i] == 0)
			return false;
	}

	if (checkWin(1))
		return false;

	if (checkWin(2))
		return false;

	return true;
}

/*	CheckWin
	Using the integer that is passed in, it checks to see if the
	given player has won by systematically checking each combination
	Note: 3DS goes from bottom to top

						6|7|8	O|O|O
						3|4|5	X|X|X
						0|1|2	O|O|O

						6|7|8	O|X|O
						3|4|5	O|X|O
						0|1|2	O|X|O

						6|7|8	O|X|O
						3|4|5	X|O|X
						0|1|2	O|X|O
*/
bool Game::checkWin(int n)
{
	//check across the top
	if (slots[0] == n && slots[1] == n && slots[2] == n)
		return true;

	//check across middle
	if (slots[3] == n && slots[4] == n && slots[5] == n)
		return true;

	//check across bottom
	if (slots[6] == n && slots[7] == n && slots[8] == n)
		return true;

	//check down left
	if (slots[0] == n && slots[3] == n && slots[6] == n)
		return true;

	//check down middle
	if (slots[1] == n && slots[4] == n && slots[7] == n)
		return true;

	//check down right
	if (slots[2] == n && slots[5] == n && slots[8] == n)
		return true;

	//check diagonal top-left to bottom-right
	if (slots[0] == n && slots[4] == n && slots[8] == n)
		return true;

	//check diagonal top-right to bottom-left
	if (slots[2] == n && slots[4] == n && slots[6] == n)
		return true;

	return false;
}

/*	MainUpdate
	This is the main update function. It switches between each
	state and calls appropriate functions for each one.

	If it is the player's turn, then playerUpdate() is called.

	During the CPU's turn, cpuUpdate() is called, before checks
	to make sure the CPU hasn't won, and then giving control back
	to the player.

	In the event of a Win/Loss/Tie, we check the input until 'Y'
	is pressed to reset the game.
*/
void Game::mainUpdate()
{
	switch (currentMode)
	{
	case Player:
		playerUpdate();
		break;

	case CPU:
		cpuUpdate();

		if (checkWin(1))
		{
			currentMode = Win;
			return;
		}
		else if (checkWin(2))
		{
			currentMode = Loss;
			return;
		}

		currentMode = Player;

		break;

	case Win:
	case Loss:
	case Tied:
		
		u32 kDown = hidKeysDown();

		if (kDown & KEY_Y)
			reset();

		break;
	}
}

/*	PlayerUpdate
	This handles player functionality

	The first thing it does is check for a tie situation
	and changes the state if a tie occurs.

	Input then gets check to see if 'Y' is pressed. Doing
	so will reset the game.

	Each slot is then looped through to check if touch input
	has been registered on the button corresponding to that
	slot. If the slot is free, it then sets the slot to '1'
	and checks win and loss conditions before giving control
	to the CPU.
*/
void Game::playerUpdate()
{
	if (checkTied())
	{
		currentMode = Tied;
	}

	touchPosition touch;
	hidTouchRead(&touch);

	u32 kDown = hidKeysDown();

	if (kDown & KEY_Y)
		reset();


	for (int i = 0; i < 9; i++)
	{
		if (button[i].isTouched(touch) && isSlotFree(i))
		{
			slots[i] = 1;

			if (checkWin(1))
			{
				currentMode = Win;
				break;
			}
			else if (checkWin(2))
			{
				currentMode = Loss;
				break;
			}

			currentMode = CPU;
		}
	}
}

/*	CPUUpdate
	First the AI checks to see if the game is tied, otherwise it
	would not be able to take it's turn

	It then picks a random number, and provided that slot in the
	grid is free, it takes that space
*/
void Game::cpuUpdate()
{
	if (checkTied())
	{
		currentMode = Tied;
		return;
	}

	int newVal = rand() % 8;

	while (!isSlotFree(newVal))
		newVal = rand() % 9;

	slots[newVal] = 2;
}

/*	IsSlotFree
	This checks to see if the given slot is free (set to 0)
*/
bool Game::isSlotFree(int slot)
{
	if (slots[slot] == 0)
		return true;

	return false;
}

/*	DrawMarker
	This draws the given image on the bottom screen at the
	given slot
*/
void Game::drawMarker(u8* spriteData, int slot)
{
	if (slot == 0)
	{
		drawImage(GFX_BOTTOM, GFX_LEFT, spriteData, 70, 70, 5, 15);
	}
	else if (slot == 1)
	{
		drawImage(GFX_BOTTOM, GFX_LEFT, spriteData, 70, 70, 5, 115);
	}
	else if (slot == 2)
	{
		drawImage(GFX_BOTTOM, GFX_LEFT, spriteData, 70, 70, 5, 215);
	}
	else if (slot == 3)
	{
		drawImage(GFX_BOTTOM, GFX_LEFT, spriteData, 70, 70, 85, 15);
	}
	else if (slot == 4)
	{
		drawImage(GFX_BOTTOM, GFX_LEFT, spriteData, 70, 70, 85, 115);
	}
	else if (slot == 5)
	{
		drawImage(GFX_BOTTOM, GFX_LEFT, spriteData, 70, 70, 85, 215);
	}
	else if (slot == 6)
	{
		drawImage(GFX_BOTTOM, GFX_LEFT, spriteData, 70, 70, 165, 15);
	}
	else if (slot == 7)
	{
		drawImage(GFX_BOTTOM, GFX_LEFT, spriteData, 70, 70, 165, 115);
	}
	else if (slot == 8)
	{
		drawImage(GFX_BOTTOM, GFX_LEFT, spriteData, 70, 70, 165, 215);
	}
}

/*	DrawImage
	This code was used in 2048

	gfxScreen_t screen		-	The screen we want to draw on (GFX_TOP, GFX_BOTTOM)
	gfx3dSide_t side		-	The side we want to draw from (GFX_LEFT)
	u8* spriteData			-	The image we want to use
	u16 width				-	The width of the image (images are rotated, so this would normally be the height)
	u16 height				-	The height of the image (images are rotated, so this would normally be the width)
	s16 x					-	The x pos (normally y)
	s16 y					-	The y pos (normally x)
*/
void Game::drawImage(gfxScreen_t screen, gfx3dSide_t side, u8* spriteData, u16 width, u16 height, s16 x, s16 y)
{
	//Don't draw if we don't have any images
	if (!spriteData)
		return;

	//Sets up the framebuffer
	u16 fbWidth, fbHeight;
	u8* fbAdr = gfxGetFramebuffer(screen, side, &fbWidth, &fbHeight);

	if (x + width<0 || x >= fbWidth)return;
	if (y + height<0 || y >= fbHeight)return;

	u16 xOffset = 0, yOffset = 0;
	u16 widthDrawn = width, heightDrawn = height;

	if (x<0)xOffset = -x;
	if (y<0)yOffset = -y;
	if (x + width >= fbWidth)widthDrawn = fbWidth - x;
	if (y + height >= fbHeight)heightDrawn = fbHeight - y;
	widthDrawn -= xOffset;
	heightDrawn -= yOffset;

	int j;
	for (j = yOffset; j<yOffset + heightDrawn; j++)
	{
		memcpy(&fbAdr[((x + xOffset) + (y + j)*fbWidth) * 3], &spriteData[((xOffset)+(j)*width) * 3], widthDrawn * 3);
	}
}

/*	Render
	This renders images to the screen

	If we want to draw images over other images, we draw them last

	First the backgrounds get drawn, then each slot is checked to
	see if it contains a O or X and draw accordingly, and last
	Win, Loss and Tie conditions are checked for the correct message
	to be displayed.
*/
void Game::render()
{
	drawImage(GFX_TOP, GFX_LEFT, (u8*)bg_top_bin, 240, 400, 0, 0);
	drawImage(GFX_BOTTOM, GFX_LEFT, (u8*)bg_bottom_bin, 240, 320, 0, 0);	
	
	for (int i = 0; i < 9; i++)
	{
		if (slots[i] == 1)
			drawMarker((u8*)nought_bin, i);	//Nought
		else if (slots[i] == 2)
			drawMarker((u8*)cross_bin, i);	//Cross
	}

	if (currentMode == Win)
		drawImage(GFX_BOTTOM, GFX_LEFT, (u8*)win_bin, 60, 160, 90, 80);

	if (currentMode == Loss)
		drawImage(GFX_BOTTOM, GFX_LEFT, (u8*)lose_bin, 60, 160, 90, 80);

	if (currentMode == Tied)
		drawImage(GFX_BOTTOM, GFX_LEFT, (u8*)tie_bin, 60, 160, 90, 80);
}