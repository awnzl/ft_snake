#ifndef __MLXDL_H
# define __MLXDL_H

#include <functional>

class GUIDisplay_ {
    struct		s_win
    {   void	*mlx;
        void	*win;
        void	*img;
        char	*imgdat;
        int		*imdat;
        int		bspp;
        int		ls;
        int		endian;
        int		width;
        int		height;
    }			window;

    static int  exit_buttons();
    void        *thfun(void *v);
    void        insertImage(int*);

    GUIDisplay_(GUIDisplay_ const &) = delete;
    GUIDisplay_(GUIDisplay_ &&) = delete;
    GUIDisplay_ &operator=(GUIDisplay_ const &) = delete;
    GUIDisplay_ &operator=(GUIDisplay_ &&) = delete;

public:
    GUIDisplay_();
    ~GUIDisplay_();

    void render(uint8_t *rawImage);
    int getEvent();

};

#endif
