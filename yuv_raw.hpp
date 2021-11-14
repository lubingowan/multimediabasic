//
//  mediadata_raw.hpp
//  mmbasic
//
//  Created by lubing.wan on 2021/11/9.
//

#ifndef mediadata_raw_hpp
#define mediadata_raw_hpp
#include <string>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <math.h>

int mm_yuv420_split(const std::string& url, int w, int h){
    using namespace std;
    
    ifstream in(url,  std::ifstream::in);
    ofstream outy("output_420_y.y", ofstream::out);
    ofstream outu("output_420_u.y", ofstream::out);
    ofstream outv("output_420_v.y", ofstream::out);
    
    if (!in.is_open() || !outy.is_open() || !outu.is_open() || !outv.is_open()) {
        cout << "in is open: " << in.is_open()
        << ", outy is open: " << outy.is_open()
        << ", outu is open: " << outu.is_open()
        << ", outv is open: " << outv.is_open() << endl;
        return -1;
    }
    
    // File size. equals to 256x256x((4 + 2 + 2) / 2)
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    
//    cout << "len of yuv420 256x256: " << len << endl
//    << "len of y: " << w * h << endl
//    << "len of u: " << (w * h >> 2) << endl
//    << "len of v: " << (w * h >> 2) << endl;
    
    char pic[len];
    memset(pic, 0x0, len);

    {
        in.read(pic, len);
        
        // Y
        outy.write(pic, w * h);
        // U
        outu.write(pic + w * h, w * h >> 2);
        // V
        outv.write(pic + (w * h * 5 >> 2), w * h >> 2);
    }

    in.close();
    outy.flush();
    outy.close();
    
    outu.flush();
    outu.close();
    
    outv.flush();
    outv.close();
    
    return 0;
}


int mm_yuv444_split(const std::string &url, int w, int h){
    using namespace std;
    
    ifstream in(url, ifstream::in);
    ofstream outy("output_444_y.y", ofstream::out);
    ofstream outu("output_444_u.y", ofstream::out);
    ofstream outv("output_444_v.y", ofstream::out);
    
    if (!in.is_open() || !outy.is_open() || !outu.is_open() || !outv.is_open()) {
        std::cout << "in is open: " << in.is_open()
        << ", outy is open: " << outy.is_open()
        << ", outu is open: " << outu.is_open()
        << ", outv is open: " << outv.is_open() << std::endl;
        return -1;
    }

    
    // File size. equals to 256x256x((4 + 2 + 2) / 2)
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
//    cout << "len of yuv444 256x256: " << len << endl
//    << "len of y: " << w * h << endl
//    << "len of u: " << (w * h) << endl
//    << "len of v: " << (w * h) << endl;
    
    char pic[len];
    {
        in.read(pic, w * h * 3);
        
        //Y
        outy.write(pic, w * h);
        //U
        outu.write(pic + w * h, w * h);
        //V
        outv.write(pic + w * h * 2, w * h);
    }

    in.close();
    outy.flush();
    outy.close();
    
    outu.flush();
    outu.close();
    
    outv.flush();
    outv.close();

    return 0;
}

int mm_yuv420_halfy(const std::string& url, int w, int h){
    using namespace std;
    
    fstream in;
    in.open(url, std::fstream::in);
    
    ofstream out("output_half.yuv", ofstream::out);
    
    if (!in.is_open() || !out.is_open()) {
        std::cout << "in is open: " << in.is_open()
        << " out is open: " << out.is_open() << std::endl;
        return -1;
    }

    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char pic[len];
    {
        in.read(pic, len);
        
        //Half
        for(int j = 0;j < w * h; j++){
            unsigned char temp = pic[j] / 2;
            pic[j] = temp;
        }
        
        out.write(pic, len);
    }

    in.close();
    out.close();

    return 0;
}

int mm_yuv420_cleanyuv(const std::string& url, int w, int h){
    using namespace std;
    
    ifstream in(url,  std::ifstream::in);
    ofstream outy("output_420_no_y.yuv", ofstream::out);
    ofstream outu("output_420_no_u.yuv", ofstream::out);
    ofstream outv("output_420_no_v.yuv", ofstream::out);
    
    if (!in.is_open() || !outy.is_open() || !outu.is_open() || !outv.is_open()) {
        cout << "in is open: " << in.is_open()
        << ", outy is open: " << outy.is_open()
        << ", outu is open: " << outu.is_open()
        << ", outv is open: " << outv.is_open() << endl;
        return -1;
    }
    
    // File size. equals to 256x256x((4 + 2 + 2) / 2)
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char pic[len];
    memset(pic, 0x0, len);

    {
        in.read(pic, len);
        
        char tmp[len];
        // clean up Y
        memcpy(tmp, pic, len);
        memset(tmp, 128, w * h);
        outy.write(tmp, len);
        // clean up U
        memcpy(tmp, pic, len);
        memset(tmp + w * h, 128, w * h >> 2);
        outu.write(tmp, len);
        // clean up V
        memcpy(tmp, pic, len);
        memset(tmp + (w * h * 5 >> 2), 128, w * h >> 2);
        outv.write(tmp, len);
    }

    in.close();
    outy.flush();
    outy.close();
    
    outu.flush();
    outu.close();
    
    outv.flush();
    outv.close();
    
    return 0;
}

int mm_yuv420_only_yuv(const std::string& url, int w, int h){
    using namespace std;
    
    ifstream in(url,  std::ifstream::in);
    ofstream outy("output_420_only_y.yuv", ofstream::out);
    ofstream outu("output_420_only_u.yuv", ofstream::out);
    ofstream outv("output_420_only_v.yuv", ofstream::out);
    
    if (!in.is_open() || !outy.is_open() || !outu.is_open() || !outv.is_open()) {
        cout << "in is open: " << in.is_open()
        << ", outy is open: " << outy.is_open()
        << ", outu is open: " << outu.is_open()
        << ", outv is open: " << outv.is_open() << endl;
        return -1;
    }
    
    // File size. equals to 256x256x((4 + 2 + 2) / 2)
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char pic[len];
    memset(pic, 0x0, len);

    {
        in.read(pic, len);
        
        char tmp[len];
        // only store Y
        memset(tmp, 128, len);
        memcpy(tmp, pic, w * h);
        outy.write(tmp, len);
        // only store U
        memset(tmp, 128, len);
        memcpy(tmp + w * h, pic + w * h, w * h >> 2);
        outu.write(tmp, len);
        // V
        memset(tmp, 128, len);
        memcpy(tmp + (w * h * 5 >> 2), pic + (w * h * 5 >> 2), w * h >> 2);
        outv.write(tmp, len);
    }

    in.close();
    outy.flush();
    outy.close();
    
    outu.flush();
    outu.close();
    
    outv.flush();
    outv.close();
    
    return 0;
}

int mm_yuv420_gray(const std::string &url, int w, int h){
    using namespace std;
    ifstream in(url, ifstream::in);
    ofstream out("output_gray.yuv", ofstream::out);
    if (!in.is_open() || !out.is_open()) {
        cout << "in is open: " << in.is_open()
        << "out is open: " << out.is_open() << endl;
        return -1;
    }
    
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char pic[len];
    {
        in.read(pic, len);
        
        //Gray
        memset(pic + w * h, 128, w * h / 2);
        out.write(pic, len);
    }

    in.close();
    out.close();
    return 0;
}

int mm_yuv420_border(const std::string &url, int w, int h, int border){
    using namespace std;
    
    ifstream in(url, ifstream::in);
    ofstream out("output_yuv_border.yuv", ofstream::out);
    
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char pic[len];

    {
        in.read(pic, len);
        
        //Y
        for(int j = 0; j < h; j++){
            for(int k = 0; k < w; k++){
                if(k < border || k > (w-border) || j < border || j > (h-border)){
//                    pic[j * w + k] = char(255);
                    pic[j * w + k] = char(0);
                }
            }
        }
        out.write(pic, len);
    }

    in.close();
    out.close();

    return 0;
}

int mm_yuv420_graybar(int width, int height, int ymin, int ymax, int barnum) {
    using namespace std;

    int barwidth = width / barnum;
    int luminc = (ymax - ymin) / barnum;

    ofstream out("output_yuv420p_graybar.yuv", ofstream::out);
    if(!out.is_open()){
        cout << "output file is open: " << out.is_open() << endl;
        return -1;
    }
    
    size_t len = width * height * 3 >> 1;
    char pic[len];
    
    // set Y U V to 128
    memset(pic, 128, len);

    //Gen Y Data
    int tmpy = ymin;
    for (int i = 0; i < width; i++) {
        tmpy = ymin + i / barwidth * luminc;
        for (int j = 0; j < height; j++) {
            pic[j * height + i] = tmpy;
        }
    }

    out.write(pic, len);
    out.close();
    return 0;
}

int mm_yuv420_psnr(const std::string &oriurl, const std::string &url, int w, int h) {
    using namespace std;
    ifstream ori(oriurl, ifstream::in);
    ifstream in(url, ifstream::in);
    
    if (!ori.is_open() || !in.is_open()) {
        cout << "open fail" << endl;
        return -1;
    }
    
    size_t len = w * h * 3 >> 1;
    
    char pic1[len];
    char pic2[len];
    
    ori.read(pic1, len);
    in.read(pic2, len);

    {

        double mse_sum = 0, mse = 0, psnr = 0;
        for(int j = 0; j < w * h * 3 >> 1; j++) {
        // recast to uint8 or unsigned char for psnr carculate.
            mse_sum += pow((double)((uint8_t)pic1[j] - (uint8_t)pic2[j]), 2);
        }
        mse = mse_sum / ( w * h);
        psnr = 10 * log10(255.0 * 255.0 / mse);
//        cout << "mse: " << mse << " mse_sum: " << mse_sum << endl;
        cout << "psnr: "<< setw(5) << psnr << endl;

    }

    ori.close();
    in.close();
    return 0;
}

#endif /* mediadata_raw_hpp */
