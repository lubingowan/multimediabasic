//
//  aac_raw.hpp
//  mmbasic
//
//  Created by lubing.wan on 2021/11/14.
//

#ifndef aac_raw_h
#define aac_raw_h
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fstream>

int mm_adts_frame(uint8_t* buffer, size_t buf_size, uint8_t* data, size_t &data_size) {
    size_t size = 0;

    if(!buffer || !data){
        return -1;
    }

    while(1){
        if(buf_size  < 7 ){
            return -1;
        }
        //Sync words
        if((buffer[0] == 0xff) && ((buffer[1] & 0xf0) == 0xf0) ){
            size |= ((buffer[3] & 0x03) << 11);     //high 2 bit
            size |= buffer[4] << 3;                //middle 8 bit
            size |= ((buffer[5] & 0xe0) >> 5);        //low 3bit
            break;
        }
        --buf_size;
        ++buffer;
    }

    if(buf_size < size){
        return 1;
    }

    memcpy(data, buffer, size);
    data_size = size;

    return 0;
}

int mm_aac_parser(const std::string &url)
{
    size_t size = 0;
    int cnt = 0;
    size_t offset = 0;

    using namespace std;
    ifstream in(url, ifstream::in);
    if (!in.is_open()) {
        cout << "aac test open fail." << endl;
        return -1;
    }
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    uint8_t aacframe[1024 * 5];
    uint8_t aacbuffer[len];
    
    cout << "aac file len: " << len << endl;
    in.read((char*)aacbuffer, len);

    printf("-----+- ADTS Frame Table -+------+\n");
    printf(" NUM | Profile | Frequency| Size |\n");
    printf("-----+---------+----------+------+\n");
    while(1)
    {
        int ret = mm_adts_frame(aacbuffer + offset, len - offset, aacframe, size);
        if (ret == -1) {
            break;
        } else if (ret == 1) {
            memcpy(aacframe, aacbuffer, size);
            offset += size;
            break;
        }

        char profile_str[10] = {0};
        char frequence_str[10] = {0};

        uint8_t profile = aacframe[2] & 0xC0;
        profile = profile >> 6;
        switch(profile) {
            case 0:
                sprintf(profile_str,"Main");break;
            case 1:
                sprintf(profile_str,"LC");break;
            case 2:
                sprintf(profile_str,"SSR");break;
            default:
                sprintf(profile_str,"unknown");break;
        }

        uint8_t sampling_frequency_index = aacframe[2] & 0x3C;
        sampling_frequency_index = sampling_frequency_index >> 2;
        switch(sampling_frequency_index) {
            case 0:
                sprintf(frequence_str,"96000Hz");break;
            case 1:
                sprintf(frequence_str,"88200Hz");break;
            case 2:
                sprintf(frequence_str,"64000Hz");break;
            case 3:
                sprintf(frequence_str,"48000Hz");break;
            case 4:
                sprintf(frequence_str,"44100Hz");break;
            case 5:
                sprintf(frequence_str,"32000Hz");break;
            case 6:
                sprintf(frequence_str,"24000Hz");break;
            case 7:
                sprintf(frequence_str,"22050Hz");break;
            case 8:
                sprintf(frequence_str,"16000Hz");break;
            case 9:
                sprintf(frequence_str,"12000Hz");break;
            case 10:
                sprintf(frequence_str,"11025Hz");break;
            case 11:
                sprintf(frequence_str,"8000Hz");break;
            default:
                sprintf(frequence_str,"unknown");break;
        }


        printf("%5d| %8s|  %8s| %5lu|\n",cnt, profile_str, frequence_str, size);
        
        offset += size;
        cnt++;
    }
    in.close();

    return 0;
}


#endif /* aac_raw_h */
