#ifndef __CORE_H
# define __CORE_H

//#include "minilibxwrapper.h"
#include "defines.h"
#include "sfmlwrapper.h"
#include <vector>

class GameCore {
	struct Block {
		int		x;
		int		y;
		bool	isVisible;
		uint8_t	*pxls;

		Block(int x, int y, bool isVisible, uint8_t *px)
			: x(x),
			  y(y),
			  isVisible(isVisible),
			  pxls(px)
		{
		}
	};

	struct Snake {
		int x;
		int y;
		int len;
	}		snake;

	std::vector<Block> blocks;
	
	void	initBlocks();
	void	setBlocks();
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
