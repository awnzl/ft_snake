#include <mlx.h>
#include <stdio.h>

#define WIDTH_HEIGTH 600

void fillImage(unsigned char *image) {
    
    //std::cout << (int)c[0] << " " << (int)c[1] << " " << (int)c[2] << " " << (int)c[3] << "\n";
    for (int i = 0; i < WIDTH_HEIGTH * WIDTH_HEIGTH * 4;) {
        image[i++] = 255;//c[3];
        image[i++] = 0;//c[2];
        image[i++] = 0;//c[1];
        image[i++] = 0;
    }
}

int main() {
    void			*mlx;
	void			*win;
	void			*img;
	unsigned char	*data;
    int				bspp;
	int				linesize;
	int				endian;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 600, 600, "tst");
    img = mlx_new_image(mlx, 600, 600);
    data = (unsigned char*)mlx_get_data_addr(img, &bspp, &linesize, &endian);

    fillImage(data);

    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_destroy_image(mlx, img);
    mlx_loop(mlx);
	mlx_destroy_window(mlx, win);
    return 0;
}

/*
void	put_image(t_win *win, t_peak *peaks)
{
	get_image_content(win, peaks);
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	mlx_destroy_image(win->mlx, win->img);
	win->img = mlx_new_image(win->mlx, win->width, win->height);
	win->img_data = (unsigned char*)mlx_get_data_addr(win->img,
			&(win->bspp), &(win->linesize), &(win->endian));
	win->imdat = (int*)win->img_data;
}

int		let_the_window_begin(t_win *win, t_peak *peaks)
{
	t_cont	cont;

	cont.w = win;
	cont.p = peaks;
	put_image(win, peaks);
	mlx_hook(win->win, 2, 5, key_hook, (void*)(&cont));
	mlx_mouse_hook(win->win, mouse_hook, (void*)(&cont));
	mlx_loop(win->mlx);
	mlx_destroy_window(win->mlx, win->win);
	return (0);
}



*/