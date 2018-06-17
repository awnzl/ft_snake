#ifndef __CORE_H
# define __CORE_H

//#include "minilibxwrapper.h"
#include "defines.h"
#include "sfmlwrapper.h"

class GameCore {
	struct Snake {
		int x;
		int y;
		int len;
	}		snake;
	
	void	setPixel(int x, int y, uint8_t *pixels, int color);
	void	fillImage(uint8_t *image);
	void	insertSnake(uint8_t *pixels);
	uint8_t	*getImage(int event, uint8_t *pixels);

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Debug functions																			//
	//////////////////////////////////////////////////////////////////////////////////////////////
	int pixToInt(int x, int y, uint8_t *pixels);

public:
	GameCore();
	~GameCore();
	void run();
};

#endif
