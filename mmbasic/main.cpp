//
//  main.cpp
//  mmbasic
//
//  Created by lubing.wan on 2021/11/9.
//

#include <iostream>
#include <string>
#include "yuv_raw.hpp"
#include "rgb_raw.hpp"
#include "pcm_raw.hpp"
#include "h264_raw.hpp"
#include "aac_raw.hpp"
#include "rtp_parse.hpp"

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
    
    {
        mm_rgb24_to_yuv420("../lena_256x256_rgb24.rgb", 256, 256);
    }
    
    {
        mm_rgb24_colorbar(640, 360, "output_colorbar.rgb");
    }
    
    
    // pcm tests
    {
        mm_pcm16le_split("../NocturneNo2inEflat_44.1k_s16le.pcm");
    }
    
    {
        mm_pcm16le_halfvolumeleft("../NocturneNo2inEflat_44.1k_s16le.pcm");
    }
    
    {
        mm_pcm16le_doublevolumeleft("../NocturneNo2inEflat_44.1k_s16le.pcm");
    }
    
    {
        mm_pcm16le_doublespeed("../NocturneNo2inEflat_44.1k_s16le.pcm");
    }
    
    {
        mm_pcm16le_to_pcm8("../NocturneNo2inEflat_44.1k_s16le.pcm");
    }
    
    {
        mm_pcm16le_to_wave("../NocturneNo2inEflat_44.1k_s16le.pcm", 2, 44100, "output_s16le.wav");
    }
    
    {
        mm_pcm16le_to_wave2("../NocturneNo2inEflat_44.1k_s16le.pcm", 2, 44100, "output_s16le2.wav");
    }
    
    // h264 parse nalu
    {
        mm_h264_parser("../my_desktop_capture.h264");
    }

//    {
//        mm_aac_parser("../nocturne.aac");
//    }

//    {
//        mm_rtp_parser(8888);
//    }
//
    return 0;
}
