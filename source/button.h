/*
	See button.cpp for more info on these functions
*/

#include <3ds.h>

#ifndef Button_H
#define Button_H

class Button{
public:
	Button();
	Button(int, int, int, int);
	void movePosition(int, int);
	void resize(int, int);
	bool isTouched(touchPosition);

private:
	int width;
	int height;
	int x;
	int y;
};

#endif