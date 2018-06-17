#include <iostream>

void setPix(int x, int y, unsigned char *pixels, int color) {
    int idx = y * 2 + x;
    pixels[idx + 3] = color >> 24;        //a
    pixels[idx + 2] = color >> 16 & 0xFF; //r
    pixels[idx + 1] = color >> 8 & 0xFF;  //g
    pixels[idx + 0] = color & 0xFF;       //b

}

//done
int ret(int x, int y, unsigned char *pixels) {
    int idx = y * 2 + x;
    return (pixels[idx + 3] << 24) + (pixels[idx + 2] << 16) + (pixels[idx + 1] << 8) + pixels[idx];
}

int main() {
    union {
        int i = 3600528;
        unsigned char c[4];
    };

    int res = 0;
    // res = (c[3] << 24) + (c[2] << 16) + (c[1] << 8) + c[0];
    // std::cout << "i   = " << i << "\n"
    //              "res = " << res << std::endl;

    // res = ret(0,0,c);
    // std::cout << "i   = " << i << "\n"
    //              "res = " << res << std::endl;

    unsigned char test[8];
    int *arr = (int*)test;

    arr[0] = 362514893;
    arr[1] = 12356789;
    // res = ret(0, 0, test);
    // std::cout << "i   = " << arr[0] << "\n"
    //              "res = " << res << std::endl;
    // res = ret(4, 0, test);
    // std::cout << "i   = " << arr[1] << "\n"
    //              "res = " << res << std::endl;

    int color = 62354324;
    int idx = 0;
    setPix(0,0,test, color);
    std::cout << "control: " << 62354324 << std::endl;
    std::cout << "ret res: " << ret(0, 0, test) << std::endl;

    color = 5131124;
    setPix(0,0,test, color);
    // b[0] = val>>24; b[1] = (val>>16)&0xFF; b[2] = (val>>8)&0xFF; b[3] = val&0xFF;
    std::cout << "control: " << 5131124 << std::endl;
    std::cout << "ret res: " << ret(0, 0, test) << std::endl;

    return 0;
}
