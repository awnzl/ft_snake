#include "core.h"

int main()
{
	GameCore *gc = new GameCore;
	gc->run();
	delete gc;

	return (0);
}