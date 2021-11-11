//
//  main.cpp
//  mmbasic
//
//  Created by 万露兵 on 2021/11/9.
//

#include <iostream>
#include <string>
#include "yuv_raw.hpp"
#include "rgb_raw.hpp"

int main(int argc, const char * argv[]) {
    
    {
        std::string url = "../lena_256x256_yuv420p.yuv";
        mm_yuv420_split(url, 256, 256);
    }
    
    {
        std::string url = "../lena_256x256_yuv444p.yuv";
        mm_yuv444_split(url, 256, 256);
    }
    
    {
        std::string url = "../lena_256x256_yuv420p.yuv";
        mm_yuv420_halfy(url, 256, 256);
    }
    
    {
        std::string url = "../lena_256x256_yuv420p.yuv";
        mm_yuv420_cleanyuv(url, 256, 256);
    }
    
    {
        std::string url = "../lena_256x256_yuv420p.yuv";
        mm_yuv420_only_yuv(url, 256, 256);
    }
    
    {
        std::string url = "../lena_256x256_yuv420p.yuv";
        mm_yuv420_gray(url, 256, 256);
    }
    
    {
        std::string url = "../lena_256x256_yuv420p.yuv";
        mm_yuv420_border(url, 256, 256, 10);
    }
    
    {
        mm_yuv420_graybar(256, 256, 0, 256, 10);
    }
    
    {
        mm_yuv420_psnr("../lena_256x256_yuv420p.yuv","../lena_distort_256x256_yuv420p.yuv",256,256);
    }
    
    {
        mm_rgb24_split("../cie1931_500x500.rgb", 500, 500);
    }
    
    {
        mm_rgb24_cleanrgb("../lena_256x256_rgb24.rgb", 256, 256);
    }
    
    {
        mm_rgb24_onlyrgb("../lena_256x256_rgb24.rgb", 256, 256);
    }
    
    {
        mm_rgb24_to_bmp("../lena_256x256_rgb24.rgb", 256, 256, "output_lena.bmp");
    }
    
    
    return 0;
}
