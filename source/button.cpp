/*
	This is a partial copy of GUIButton from my Android engine,
	as the image functions are not present here.

	The movePosition and resize function are fairly redundant here
	too, but I included them just for the sake of copying them across.
*/

#include "Button.h"

//blank constructor
Button::Button() { }

/*
	Creates an invisible button at point (x,y) with scale (w, h)
*/
Button::Button(int X, int Y, int W, int H)
{
	x = X;
	y = Y;
	width = W;
	height = H;
}

/*
	Moves the button to (x,y)
*/
void Button::movePosition(int X, int Y)
{
	x = X;
	y = Y;
}

/*
	Resizes the button to (w, h)
*/
void Button::resize(int W, int H)
{
	width = W;
	height = H;
}

/*
	Checks if the button is touched and returns true

	The check here detects if the touch position was
	within the bounds of the button (greater than the
	button's x and y, but smaller than x+width and
	y+height)
*/
bool Button::isTouched(touchPosition touch)
{
	if (touch.px > x && touch.px < x + width
		&& touch.py > y && touch.py < y + height)
		return true;

	return false;
}