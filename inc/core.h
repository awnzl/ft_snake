#ifndef __CORE_H
# define __CORE_H

//#include "minilibxwrapper.h"
#include "defines.h"
#include "sfmlwrapper.h"
#include <array>
#include <string>//TODO: delete this include
#include <sstream>//TODO: delete this include

class GameCore {
	//////////////////////////////////////////////////////////////////////////////////////////////
	// x, y of left top corner																	//
	//////////////////////////////////////////////////////////////////////////////////////////////
	struct Block {
		int		x;
		int		y;
		bool	isVisible;
		uint8_t	*pxls;
		//int direction; //for snake ?

		Block(int x, int y, bool isVisible, uint8_t *px)
			: x(x),
			  y(y),
			  isVisible(isVisible),
			  pxls(px)
		{
		}
		//TODO: delete this func
		std::string toString() {
			std::stringstream ss;
			ss << x << " | " << y << " | " << isVisible << std::endl;
			return ss.str();
		}
	};

	struct Snake {
		int len;//nubmer of blocks
		int x;
		int y;

		//need to implement snake direction logic
		// Snake() : len(3), x((len + BLOCKS_QUANTITY) / 2), y(BLOCKS_QUANTITY / 2)
		// {
		// }

		//TODO: temporary constructor, till snake direction logic isn't implemented
		Snake() : len(1), x(BLOCKS_QUANTITY - 1 / 2), y(BLOCKS_QUANTITY - 1) { }
	}		snake;

	uint8_t background_pxls[BLOCK_SIZE * BLOCK_SIZE * 4];

	std::array<Block*, BLOCKS_QUANTITY> blocks;
	
	void    fillPixelBlock(uint8_t *px, int color);
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
