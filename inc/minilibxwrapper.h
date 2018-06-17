#ifndef __MLXDL_H
# define __MLXDL_H

#include <functional>

class GameCore;

class MlxLibWrapper {
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
    std::function<void(int)> function;
    MlxLibWrapper();
    ~MlxLibWrapper();

    MlxLibWrapper(MlxLibWrapper const &) = delete;
    MlxLibWrapper(MlxLibWrapper &&) = delete;
    MlxLibWrapper &operator=(MlxLibWrapper const &) = delete;
    MlxLibWrapper &operator=(MlxLibWrapper &&) = delete;

public:
    static MlxLibWrapper *wrapperimpl;
    static MlxLibWrapper &instance();
    //take a callback function
    void setEventsListener(std::function<void(int)>);
    //take an image and put it into window
    void putImage(int* img);
    //take default image for start
    void go(int* img);

};

#endif
