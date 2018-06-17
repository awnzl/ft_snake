#ifndef __GRAPHLIB_H
# define __GRAPHLIB_H

#include "minilibxwrapper.h"

class GraphLib {
public:
	bool createWindow(int w, int h);
	void setEventsListener((void)(*f)(int events));
	void drawImage(int *image);
};

bool GraphLib::createWindow(int w, int h) {
	MlxLibWrapper *mlx = new MlxLibWrapper();

	
}

#endif
