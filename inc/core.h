#ifndef __CORE_H
# define __CORE_H

//#include "minilibxwrapper.h"
#include "defines.h"
#include "sfmlwrapper.h"
#include <array>
#include <string>//TODO: delete this include
#include <sstream>//TODO: delete this include

//TODO: format code according to c++ styles

class GameCore
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// x, y of left top corner																	//
	//////////////////////////////////////////////////////////////////////////////////////////////
	struct Block
	{
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

		// ~Block() = default;
		
		//TODO: delete this func
		std::string toString()
		{
			std::stringstream ss;
			ss << x << " | " << y << " | " << isVisible << std::endl;
			return ss.str();
		}
	};

	//TODO: TMP OBJECT
	struct Snake {
		int len;//nubmer of blocks
		int x;
		int y;

		Snake() : len(1), x(BLOCKS_QUANTITY - 1 / 2), y(BLOCKS_QUANTITY - 1) { }
	}		snake;


	//можно запилить std::array для хранения uint8_t указателей на разные блоки пикселей
	//запилить энамчик для обозначения какой блок где...
	//можно хранить отдельно контейнер указателей на вид цели (такой же подход для препятствий)
	//в этом массиве будут массивы блоков. Индекс контейнера будет соответствовать уровню,
	//для которого эти цели и препятствия валидны.
	//каждый раунд брать новое количество блоков,целей,препятствий.
	uint8_t target_pixels_map[BLOCK_PIXELS_SIZE];
	uint8_t obstacle_pixels_map[BLOCK_PIXELS_SIZE];	

    std::array<Block*, BLOCKS_QUANTITY> allElenents;//TODO: tmp
	std::array<Block*, OBSTACLES_QUANTITY> obstacles;//TODO: tmp
	std::array<Block*, TARGETS_QUANTITY> targets;//TODO: tmp
	std::vector <Block*> Snake;//TODO: rename to snake
	
	void    fillPixelsToPixelsMap(uint8_t *px, int color);
	// void	fillBlocks();
	void	insertBlockToScene(int bx, int by, uint8_t *block, uint8_t *scene);
	//by default backgraund color
	void	setPixel(int x, int y, uint8_t *pixels, int rowLength, int color = 0x30D5C8);
	void	fillBackground(uint8_t *image);
	void	insertSnake(uint8_t *pixels);
	void	initElements();
	void	insertElements(uint8_t *pixels);
	uint8_t	*getImage(int event, uint8_t *pixels);

	// template<std::size_t ALLSIZE, std::size_t OBJSIZE, std::size_t TARGETSSIZE>
	// void	gameLoop(GUIDisplay &disp,
    //                  std::array<Block*, ALLSIZE> &allElements,
    //                  std::array<Block*, OBJSIZE> &obstacles,
    //                  std::array<Block*, TARGETSSIZE> &targets);

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
