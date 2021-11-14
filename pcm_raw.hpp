//
//  pcm_raw.hpp
//  mmbasic
//
//  Created by lubing.wan on 2021/11/12.
//

#ifndef pcm_raw_hpp
#define pcm_raw_hpp
#include <string>
#include <fstream>
#include <iostream>
#include <stdint.h>

int mm_pcm16le_split(const std::string &url){
    using namespace std;
    
    ifstream in(url, ifstream::in);
    ofstream outl("output_left.pcm", ofstream::out);
    ofstream outr("output_right.pcm", ofstream::out);
    
    if (!in.is_open() || !outl.is_open() || !outr.is_open()) {
        cout << "open file fail" << endl;
        return -1;
    }
    
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char data[len];
    in.read(data, len);

    int16_t *sample = (int16_t*)data;
    int16_t lsample[len / 4];
    int16_t rsample[len / 4];
    
    for (int i = 0; i < len / 4; i++)
    {
        //L
        lsample[i] = sample[i * 2]; // i * 2
        //R
        rsample[i] = sample[i * 2 + 1]; //  i * 2 + 1
    }
    
    outl.write((char*)lsample, len / 2);
    outr.write((char*)rsample, len / 2);

    in.close();
    outl.close();
    outr.close();
    
    return 0;
}

int mm_pcm16le_halfvolumeleft(const std::string url){
    using namespace std;

    ifstream in(url, ifstream::in);
    ofstream out("output_halfvolume_left.pcm", ofstream::out);
    if (!in.is_open() || !out.is_open()) {
        cout << "mm_pcm16le_halfvolumeleft open file fail" << endl;
        return -1;
    }
    
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char data[len];
    in.read(data, len);

    int16_t *sample = (int16_t*)data;
    size_t sampnum = len / 2;
    
    for (size_t i = 0; i < sampnum; i += 2) {
        sample[i] = sample[i] / 2;
    }
    out.write(data, len);
    
    in.close();
    out.close();
    
    return 0;
}

int16_t SafeDouble(int16_t& v) {
    if (v >= INT16_MAX >> 1) {
        return INT16_MAX;
    }
    else if (v <= INT16_MIN >> 1) {
        return INT16_MIN;
    }
    return v << 1;
}

int mm_pcm16le_doublevolumeleft(const std::string url){
    using namespace std;

    ifstream in(url, ifstream::in);
    ofstream out("output_doublevolume_left.pcm", ofstream::out);
    if (!in.is_open() || !out.is_open()) {
        cout << "mm_pcm16le_doublevolumeleft open file fail" << endl;
        return -1;
    }
    
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char data[len];
    in.read(data, len);

    int16_t *sample = (int16_t*)data;
    size_t sampnum = len / 2;
    
    for (size_t i = 0; i < sampnum; i += 2) {
        sample[i] = SafeDouble(sample[i]);
    }
    out.write(data, len);
    
    in.close();
    out.close();
    
    return 0;
}

int mm_pcm16le_doublespeed(const std::string &url){
    using namespace std;
    ifstream in(url, ifstream::in);
    ofstream out("output_double_speed.pcm", ofstream::out);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "file open fail" << endl;
        return -1;
    }
    
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char data[len];
    in.read(data, len);
    size_t outlen = (len / 4) & 0xFFFFFFFE;
    int16_t outdata[outlen];
    
//    cout << "data len: " << len << " output len: " << outlen << endl;
    
    int16_t *sample = (int16_t*)data;
    for (int i = 0; i < outlen; i += 2) {
        outdata[i] = sample[2 * i];
        outdata[i + 1] = sample[2 * i + 1];
    }
    out.write((char*)outdata, outlen * 2);

    in.close();
    out.close();
    return 0;
}

/* s16le 即short little endian, 采样点取值范围[INT16_MIN ~ INT16_MAX]
 * u8    即uint8, 采样点取值范围[0-255]
 */

int mm_pcm16le_to_pcm8(const std::string &url){
    using namespace std;
    ifstream in(url, ifstream::in);
    ofstream out("output_s8le.pcm", ofstream::out);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "file open fail" << endl;
        return -1;
    }

    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    char data[len];
    size_t sampnum = len / 2;
    int16_t *sample = (int16_t*)data;
    
    // s8le  sample value [0-255]
    uint8_t outdata[sampnum];
    
    for (size_t i = 0; i < sampnum; i++) {
        outdata[i] = (sample[i] >> 8) + 128;
    }

    out.write((char*)outdata, sampnum);
//    cout << "sampnum: " << sampnum << endl;
    in.close();
    out.close();
    return 0;
}

int mm_pcm16le_to_wave(const std::string &url, int channels, int sample_rate, const std::string &wavepath)
{
    typedef struct WAVE_HEADER{
        uint8_t fccID[4];
        uint32_t chunksize;
        uint8_t fccType[4];
    } WAVE_HEADER;

    typedef struct WAVE_FMT{
        uint8_t  fccID[4];
        uint32_t wFmtchunksize;
        uint16_t wFormatTag;
        uint16_t wChannels;
        uint32_t wSamplerate;
        uint32_t wByterate;
        uint16_t wBlockAlign;
        uint16_t wBitsPerSample;
    } WAVE_FMT;

    typedef struct WAVE_DATA{
        uint8_t       fccID[4];
        uint32_t datachunksize;
    } WAVE_DATA;
    
    using namespace std;
    ifstream in(url, ifstream::in);
    ofstream out(wavepath, ofstream::out);

    if (!in.is_open() || !out.is_open()) {
        cout << "file open fail" << endl;
        return -1;
    }
    
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    size_t headerlen = sizeof(WAVE_HEADER) + sizeof(WAVE_FMT) + sizeof(WAVE_DATA);
    size_t totallen = len + headerlen;
    
    printf("len %lu, headerlen %lu, totallen %lu, WAVE_HEADER %lu, WAVE_FMT %lu, WAVE_DATA %lu\n",
           len, headerlen, totallen, sizeof(WAVE_HEADER), sizeof(WAVE_FMT), sizeof(WAVE_DATA));
    
    char wavedata[totallen];
    memset(wavedata, 0x0, totallen); // memset之后生成wav文件不正常了？？？
    
    size_t offset = 0;
    WAVE_HEADER *pcmHEADER = reinterpret_cast<WAVE_HEADER*>(wavedata);
    offset += sizeof(WAVE_HEADER);
    WAVE_FMT *pcmFMT = reinterpret_cast<WAVE_FMT*>(wavedata + offset);
    offset += sizeof(WAVE_FMT);
    WAVE_DATA *pcmDATA = reinterpret_cast<WAVE_DATA*>(wavedata + offset);
    
    //WAVE_HEADER
    memcpy(pcmHEADER->fccID, "RIFF", strlen("RIFF"));
    pcmHEADER->chunksize = static_cast<uint32_t>(totallen) - 8;
    memcpy(pcmHEADER->fccType, "WAVE", strlen("WAVE"));
    
    //WAVE_FMT
    memcpy(pcmFMT->fccID,"fmt ",strlen("fmt "));
    pcmFMT->wFmtchunksize = sizeof(WAVE_FMT) - 8;
    pcmFMT->wFormatTag = 1;
    pcmFMT->wChannels = channels;
    pcmFMT->wSamplerate = sample_rate;
    pcmFMT->wByterate = pcmFMT->wSamplerate * sizeof(int16_t) * channels;
    pcmFMT->wBlockAlign = 2;
    pcmFMT->wBitsPerSample = 16;


    //WAVE_DATA;
    memcpy(pcmDATA->fccID, "data", strlen("data"));
    pcmDATA->datachunksize = (uint32_t)len;


    in.read(wavedata + headerlen, len);
    out.write(wavedata, totallen);
    
    in.close();
    out.close();
    return 0;
}


int mm_pcm16le_to_wave2(const std::string &url, int channels, int sample_rate, const std::string &wavepath)
{
    using namespace std;
    typedef struct WAVE_HEADER{
        uint8_t fccID[4];
        uint32_t chunksize;
        uint8_t fccType[4];
    } WAVE_HEADER;

    typedef struct WAVE_FMT{
        uint8_t  fccID[4];
        uint32_t wFmtchunksize;
        uint16_t wFormatTag;
        uint16_t wChannels;
        uint32_t wSamplerate;
        uint32_t wByterate;
        uint16_t wBlockAlign;
        uint16_t wBitsPerSample;
        
    } WAVE_FMT;

    typedef struct WAVE_DATA{
        uint8_t       fccID[4];
        uint32_t datachunksize;
    } WAVE_DATA;
    

    ifstream in(url, ifstream::in);
    ofstream out(wavepath, ofstream::out);

    if (!in.is_open() || !out.is_open()) {
        cout << "file open fail" << endl;
        return -1;
    }
    
    
    WAVE_HEADER pcmHEADER;
    WAVE_FMT pcmFMT;
    WAVE_DATA pcmDATA;
    
    //WAVE_HEADER
    memcpy(pcmHEADER.fccID, "RIFF", strlen("RIFF"));
    pcmHEADER.chunksize = 0;
    memcpy(pcmHEADER.fccType, "WAVE", strlen("WAVE"));
    
    //WAVE_FMT
    memcpy(pcmFMT.fccID,"fmt ",strlen("fmt "));
    pcmFMT.wFmtchunksize = sizeof(WAVE_FMT) - 8;
    pcmFMT.wFormatTag = 1;
    pcmFMT.wChannels = channels;
    pcmFMT.wSamplerate = sample_rate;
    pcmFMT.wByterate = pcmFMT.wSamplerate * sizeof(int16_t) * channels;
    pcmFMT.wBlockAlign = 2;
    pcmFMT.wBitsPerSample = 16;


    //WAVE_DATA;
    memcpy(pcmDATA.fccID, "data", strlen("data"));
    
    in.seekg(0, in.end);
    uint32_t len = (uint32_t)in.tellg();
    in.seekg(0, in.beg);
    
    char samples[len];
    in.read(samples, len);
    pcmHEADER.chunksize = 44 + len;
    pcmDATA.datachunksize = len;
    
    out.write((char*)&pcmHEADER, sizeof(pcmHEADER));
    out.write((char*)&pcmFMT, sizeof(WAVE_FMT));
    out.write((char*)&pcmDATA, sizeof(WAVE_DATA));
    out.write(samples, len);
    
    in.close();
    out.close();
    return 0;
}

// try if we can set multy data chunks.
//int mm_pcm16le_to_wave3(const std::string &url, int channels, int sample_rate, const std::string &wavepath)
//{
//    using namespace std;
//    typedef struct WAVE_HEADER{
//        uint8_t fccID[4];
//        uint32_t chunksize;
//        uint8_t fccType[4];
//    } WAVE_HEADER;
//
//    typedef struct WAVE_FMT{
//        uint8_t  fccID[4];
//        uint32_t wFmtchunksize;
//        uint16_t wFormatTag;
//        uint16_t wChannels;
//        uint32_t wSamplerate;
//        uint32_t wByterate;
//        uint16_t wBlockAlign;
//        uint16_t wBitsPerSample;
//        
//    } WAVE_FMT;
//
//    const uint32_t MAX_DATA_CHUNK_SIZE = 1500 - 8;
//    typedef struct WAVE_DATA{
//        uint8_t       fccID[4];
//        uint32_t datachunksize;
////        uint8_t  data[MAX_DATA_CHUNK_SIZE];
//    } WAVE_DATA;
//    
//
//    ifstream in(url, ifstream::in);
//    ofstream out(wavepath, ofstream::out);
//
//    if (!in.is_open() || !out.is_open()) {
//        cout << "file open fail" << endl;
//        return -1;
//    }
//    
//    
//    WAVE_HEADER pcmHEADER;
//    WAVE_FMT pcmFMT;
//    WAVE_DATA pcmDATA;
//    
//    //WAVE_HEADER
//    memcpy(pcmHEADER.fccID, "RIFF", strlen("RIFF"));
//    pcmHEADER.chunksize = 0;
//    memcpy(pcmHEADER.fccType, "WAVE", strlen("WAVE"));
//    
//    //WAVE_FMT
//    memcpy(pcmFMT.fccID,"fmt ",strlen("fmt "));
//    pcmFMT.wFmtchunksize = sizeof(WAVE_FMT) - 8;
//    pcmFMT.wFormatTag = 1;
//    pcmFMT.wChannels = channels;
//    pcmFMT.wSamplerate = sample_rate;
//    pcmFMT.wByterate = pcmFMT.wSamplerate * sizeof(int16_t) * channels;
//    pcmFMT.wBlockAlign = 2;
//    pcmFMT.wBitsPerSample = 16;
//
//
//    in.seekg(0, in.end);
//    uint32_t len = (uint32_t)in.tellg();
//    in.seekg(0, in.beg);
//    
//    char samples[len];
//    in.read(samples, len);
//    
//    // WAVE_DATA;
//    memcpy(pcmDATA.fccID, "data", strlen("data"));
//    
//    pcmHEADER.chunksize = 44 + len + (len/MAX_DATA_CHUNK_SIZE + 1) * 8;
//    pcmDATA.datachunksize = len;
//    
//    out.write((char*)&pcmHEADER, sizeof(pcmHEADER));
//    out.write((char*)&pcmFMT, sizeof(WAVE_FMT));
//    
//    uint32_t num = 0;
//    do {
//        pcmDATA.datachunksize = MAX_DATA_CHUNK_SIZE;
//        out.write((char*)&pcmDATA, sizeof(WAVE_DATA));
//        out.write(samples + num, MAX_DATA_CHUNK_SIZE);
//    }
//    while ((num += MAX_DATA_CHUNK_SIZE) < len);
//
//    num -= MAX_DATA_CHUNK_SIZE;
//    pcmDATA.datachunksize = len - num;
//    out.write((char*)&pcmDATA, sizeof(WAVE_DATA));
//    out.write(samples + num, len - num);
//    
//    in.close();
//    out.close();
//    return 0;
//}



#endif /* pcm_raw_hpp */
