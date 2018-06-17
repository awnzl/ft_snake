#include "minilibxwrapper.h"
#include <iostream>
extern "C" {
//#include "minilibx/mlx.h"
#include <mlx.h>
}

MlxLibWrapper::MlxLibWrapper() {
    window.width = 800;
    window.height = 600;
    window.mlx = mlx_init();
    window.win = mlx_new_window(window.mlx, window.width, window.height, (char*)"TEST");
    std::cout << "DEBUG: MlxLibWrapper constructor finished\n";
}
 
MlxLibWrapper::~MlxLibWrapper() {
}

void	MlxLibWrapper::setEventsListener( std::function<void(int)> f ) {
	function = f;
}

void	MlxLibWrapper::insertImage(int *image) {
	for (int y = 0; y < window.height; y++) {
		for (int x = 0; x < window.width; x++) {
			window.imdat[x + y * window.width] = image[x + y * window.width];
		}
	}
}

void	MlxLibWrapper::putImage(int *image) {
	window.img = mlx_new_image(window.mlx, window.width, window.height);
	window.imgdat = mlx_get_data_addr(window.img, &window.bspp, &window.ls,
			&window.endian);
	window.imdat = (int*)window.imgdat;

	insertImage(image);//should I copy from arg array to imdat??? Or just assign it?

	mlx_put_image_to_window(window.mlx, window.win, window.img, 0, 0);
	mlx_destroy_image(window.mlx, window.img);
	mlx_hook(window.win, 17, 0, &MlxLibWrapper::exit_buttons, NULL);
	// mlx_key_hook(window.win, &key_hook, w);
	// mlx_mouse_hook(window.win, &mouse_hook, w);
	mlx_loop(window.mlx);
}

MlxLibWrapper &MlxLibWrapper::instance() {
	static MlxLibWrapper instance;
	return instance;
}

void	MlxLibWrapper::go(int *img) {
	this->putImage(img);
}

int		MlxLibWrapper::exit_buttons()
{
	exit(1);
	return (0);
}
