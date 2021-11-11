//
//  rgb_raw.hpp
//  mmbasic
//
//  Created by 万露兵 on 2021/11/11.
//

#ifndef rgb_raw_hpp
#define rgb_raw_hpp

#include <string>
#include <fstream>
#include <iostream>
#include <stdint.h>

int mm_rgb24_split(const std::string& url, int w, int h) {
    using namespace std;
    
    ifstream in(url, ifstream::in);
    if (!in.is_open()) {
        cout << "open url fail" << endl;
        return -1;
    }
    
    ofstream out_r("output_r.y", ofstream::out);
    ofstream out_g("output_g.y", ofstream::out);
    ofstream out_b("output_b.y", ofstream::out);
    if (!out_r.is_open() || !out_g.is_open() || !out_b.is_open()) {
        cout << "open output file fail" << endl;
        return -1;
    }
    
    size_t len = w * h * 3;
    char pic[len];

    {

        in.read(pic, len);

        for(int i = 0; i < len; i += 3){
            //R
            out_r.write(pic + i, 1);
            //G
            out_g.write(pic + i + 1, 1);
            //B
            out_b.write(pic + i + 2, 1);
        }
    }

    in.close();
    out_r.close();
    out_g.close();
    out_b.close();

    return 0;
}

int mm_rgb24_cleanrgb(const std::string& url, int w, int h) {
    using namespace std;
    
    ifstream in(url, ifstream::in);
    if (!in.is_open()) {
        cout << "open url fail" << endl;
        return -1;
    }
    
    ofstream out_r("output_clean_r.rgb", ofstream::out);
    ofstream out_g("output_clean_g.rgb", ofstream::out);
    ofstream out_b("output_clean_b.rgb", ofstream::out);
    if (!out_r.is_open() || !out_g.is_open() || !out_b.is_open()) {
        cout << "open output file fail" << endl;
        return -1;
    }
    
    size_t len = w * h * 3;
    char pic[len];
    char tmp[len];
    
    const char val = char(128);

    {

        in.read(pic, len);

        // R
        memcpy(tmp, pic, len);
        for(int i = 0; i < len; i += 3){
            tmp[i] = val;
        }
        out_r.write(tmp, len);
        
        // G
        memcpy(tmp, pic, len);
        for(int i = 0; i < len; i += 3){
            tmp[i + 1] = val;
        }
        out_g.write(tmp, len);
        
        // B
        memcpy(tmp, pic, len);
        for(int i = 0; i < len; i += 3){
            tmp[i + 2] = val;
        }
        out_b.write(tmp, len);
    }

    in.close();
    out_r.close();
    out_g.close();
    out_b.close();

    return 0;
}

int mm_rgb24_onlyrgb(const std::string& url, int w, int h) {
    using namespace std;
    
    ifstream in(url, ifstream::in);
    if (!in.is_open()) {
        cout << "open url fail" << endl;
        return -1;
    }
    
    ofstream out_r("output_only_r.rgb", ofstream::out);
    ofstream out_g("output_only_g.rgb", ofstream::out);
    ofstream out_b("output_only_b.rgb", ofstream::out);
    if (!out_r.is_open() || !out_g.is_open() || !out_b.is_open()) {
        cout << "open output file fail" << endl;
        return -1;
    }
    
    size_t len = w * h * 3;
    char pic[len];
    char tmp[len];
    
    const char val = char(128);

    {

        in.read(pic, len);

        // R
        memcpy(tmp, pic, len);
        for(int i = 0; i < len; i += 3){
            tmp[i + 1] = val;
            tmp[i + 2] = val;
        }
        out_r.write(tmp, len);
        
        // G
        memcpy(tmp, pic, len);
        for(int i = 0; i < len; i += 3){
            tmp[i] = val;
            tmp[i + 2] = val;
        }
        out_g.write(tmp, len);
        
        // B
        memcpy(tmp, pic, len);
        for(int i = 0; i < len; i += 3){
            tmp[i] = val;
            tmp[i + 1] = val;
        }
        out_b.write(tmp, len);
    }

    in.close();
    out_r.close();
    out_g.close();
    out_b.close();

    return 0;
}

int mm_rgb24_to_bmp(const std::string &rgb, int w, int h, const std::string &bmp) {
    using namespace std;
    
    const char bfType[2] = {'B', 'M'};
    
    typedef struct {
        uint32_t imageSize;
        uint32_t blank;
        uint32_t startPosition;
    } BmpHead;

    typedef struct {
        uint32_t  Length;
        uint32_t  width;
        uint32_t  height;
        uint16_t  colorPlane;
        uint16_t  bitColor;
        uint32_t  zipFormat;
        uint32_t  realSize;
        uint32_t  xPels;
        uint32_t  yPels;
        uint32_t  colorUse;
        uint32_t  colorImportant;
    } InfoHead;

    int i = 0, j = 0;
    BmpHead bmphead = {0};
    InfoHead  infohead = {0};
    size_t headerlen = sizeof(bfType) + sizeof(BmpHead) + sizeof(InfoHead);

    ifstream in(rgb, ifstream::in);
    ofstream out(bmp, ofstream::out);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "open file fail" << endl;
        return -1;
    }

    size_t len = w * h * 3;
    char pic[len];
    in.read(pic, len);
    
    bmphead.imageSize = uint32_t(len + headerlen);
    bmphead.startPosition = (uint32_t)headerlen;
    
    infohead.Length = sizeof(InfoHead);
    infohead.width = w;
    
    //BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
    infohead.height = -h;
    infohead.colorPlane = 1;
    infohead.bitColor = 24;
    infohead.realSize = (uint32_t)len;

    out.write(bfType, 2);
    out.write((char*)(&bmphead), sizeof(BmpHead));
    out.write((char*)(&infohead), sizeof(InfoHead));

    //BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
    //It saves pixel data in Little Endian
    //So we change 'R' and 'B'
    for(j = 0; j < h; j++){
        for(i = 0; i < w; i++){
            char temp=pic[(j*w+i)*3+2];
            pic[(j*w+i)*3+2]=pic[(j*w+i)*3+0];
            pic[(j*w+i)*3+0]=temp;
        }
    }
    out.write(pic, len);
    in.close();
    out.close();
    
    printf("Finish generate %s!\n", bmp.c_str());
    return 0;
}


#endif /* rgb_raw_hpp */
