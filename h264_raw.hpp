//
//  h264_raw.hpp
//  mmbasic
//
//  Created by lubing.wan on 2021/11/13.
//
/*
 *   nalu | nalu | nalu | ....
 *   nalu = startcode|nalu_head|nalu_paylod
 *   startcode = 0x000001(24bit) OR 0x00000001(32bit)
 *   nalu_head = forbidden_bit(1bit)|refence_idc(2bit)|nalu_type(5bit)   total 1byte.
 *   nalu_payload = from nalu_head to next startcode
 */
#ifndef h264_raw_hpp
#define h264_raw_hpp
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_SIZE 1024 * 1024 * 4 // 4M

typedef enum {
    NALU_TYPE_SLICE    = 1,
    NALU_TYPE_DPA      = 2,
    NALU_TYPE_DPB      = 3,
    NALU_TYPE_DPC      = 4,
    NALU_TYPE_IDR      = 5,
    NALU_TYPE_SEI      = 6,
    NALU_TYPE_SPS      = 7,
    NALU_TYPE_PPS      = 8,
    NALU_TYPE_AUD      = 9,
    NALU_TYPE_EOSEQ    = 10,
    NALU_TYPE_EOSTREAM = 11,
    NALU_TYPE_FILL     = 12,
} NaluType;

typedef enum {
    NALU_PRIORITY_DISPOSABLE = 0,
    NALU_PRIRITY_LOW         = 1,
    NALU_PRIORITY_HIGH       = 2,
    NALU_PRIORITY_HIGHEST    = 3
} NaluPriority;

typedef struct {
    uint32_t len;                   //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
    uint32_t max_size;              //! Nal Unit Buffer size
    int32_t  nal_forbidden_bit;     //! should be always FALSE
    int32_t  nal_reference_idc;     //! NALU_PRIORITY_xxxx
    int32_t  nal_unit_type;         //! NALU_TYPE_xxxx
    uint8_t  data[MAX_SIZE];        //! contains the first byte followed by the EBSP
} NALU_t;

//using NALU_SPLIT_FUNC = size_t (*)(uint8_t*);

//enum StartCodeType{
//    TYPE_UNKNOW = 0,
//    TYPE_24BIT = 1,
//    TYPE_32BIT = 2,
//};

using NALU_CTX_t = struct {
//    StartCodeType    code_type;
//    NALU_SPLIT_FUNC  nalu_split;
    size_t           code_pfix_len;
};


const size_t NALU_HEADER_SIZE = 1;

//size_t mm_nalu_start_32bit(uint8_t *buf) {
//    // start code 0x00000001.
//    if (buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 1)
//        return 4;
//    else
//        return 0;
//}
//
//size_t mm_nalu_start_24bit(uint8_t *buf) {
//    // start code  0x000001.
//    if (buf[0] == 0 && buf[1] == 0 && buf[2] == 1)
//        return 3;
//    else
//        return 0;
//}

// check nalu start, return start code type
bool mm_nalu_start_check(uint8_t *buf, NALU_CTX_t& ctx) {
    // start code  0x000001 or 0x00000001.
    if (buf[0] == 0 && buf[1] == 0 && buf[2] == 1) {
//        ctx.code_type = TYPE_24BIT;
//        ctx.nalu_split = &mm_nalu_start_24bit;
        ctx.code_pfix_len = 3;
        return true;
    }
    else if (buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 1) {
//        ctx.code_type = TYPE_32BIT;
//        ctx.nalu_split = &mm_nalu_start_32bit;
        ctx.code_pfix_len = 4;
        return true;
    }
    
    return false;
}

size_t mm_annexb_nalu(NALU_t &nalu, uint8_t *data, size_t len, NALU_CTX_t &ctx) {
    using namespace std;
    
    size_t rewind = ctx.code_pfix_len;
    size_t offset = ctx.code_pfix_len + NALU_HEADER_SIZE;
    // todo; use kmp.
    while (!mm_nalu_start_check(data + offset, ctx) && offset < len) {
        offset++;
    }
    
    if (offset >= len) {
        offset = len;
    }
    size_t epos = offset;
    
    nalu.len = uint32_t(epos - ctx.code_pfix_len);
    memset(nalu.data, 0x0, MAX_SIZE);
    memcpy(nalu.data, data + rewind, nalu.len);
    nalu.nal_forbidden_bit = nalu.data[0] & 0x80;
    nalu.nal_reference_idc = nalu.data[0] & 0x60;
    nalu.nal_unit_type = nalu.data[0] & 0x1f;
    
    if (nalu.nal_forbidden_bit) {
        cout << "nalu parse error" << endl;
        return 0;
    }

    return epos;
}


int mm_h264_parser(const std::string &url){
    using namespace std;
    ifstream in(url, ifstream::in);
    if (!in.is_open()) {
        cout << "h264 file open fail" << endl;
        return -1;
    }
    
    in.seekg(0, in.end);
    size_t len = in.tellg();
    in.seekg(0, in.beg);
    
    cout << "file byte size: " << len << endl;
    
    uint8_t data[len];
    in.read((char*)data, len);
    
    
    NALU_CTX_t ctx;
    NALU_t nalu;
    
    if (!mm_nalu_start_check(data, ctx)) {
        cout << "nalu start code check fail." << endl;
        return -1;
    }
    
    
    int nal_num = 0;
    cout << "-----+-------- NALU Table ------+---------+\n";
    cout << " NUM |    POS  |    IDC |  TYPE |   LEN   |\n";
    cout << "-----+---------+--------+-------+---------+\n";
    
    
    size_t pos = 0;
    while (pos < len && mm_annexb_nalu(nalu, data + pos, len - pos, ctx)) {
        nal_num++;
        char type_str[20] = {0};
        switch(nalu.nal_unit_type){
            case NALU_TYPE_SLICE:
                sprintf(type_str,"SLICE");break;
            case NALU_TYPE_DPA:
                sprintf(type_str,"DPA");break;
            case NALU_TYPE_DPB:
                sprintf(type_str,"DPB");break;
            case NALU_TYPE_DPC:
                sprintf(type_str,"DPC");break;
            case NALU_TYPE_IDR:
                sprintf(type_str,"IDR");break;
            case NALU_TYPE_SEI:
                sprintf(type_str,"SEI");break;
            case NALU_TYPE_SPS:
                sprintf(type_str,"SPS");break;
            case NALU_TYPE_PPS:
                sprintf(type_str,"PPS");break;
            case NALU_TYPE_AUD:
                sprintf(type_str,"AUD");break;
            case NALU_TYPE_EOSEQ:
                sprintf(type_str,"EOSEQ");break;
            case NALU_TYPE_EOSTREAM:
                sprintf(type_str,"EOSTREAM");break;
            case NALU_TYPE_FILL:
                sprintf(type_str,"FILL");break;
        }
        char idc_str[20] = {0};
        switch(nalu.nal_reference_idc >> 5){
            case NALU_PRIORITY_DISPOSABLE:
                sprintf(idc_str,"DISPOS");break;
            case NALU_PRIRITY_LOW:
                sprintf(idc_str,"LOW");break;
            case NALU_PRIORITY_HIGH:
                sprintf(idc_str,"HIGH");break;
            case NALU_PRIORITY_HIGHEST:
                sprintf(idc_str,"HIGHEST");break;
        }

        printf("%5d| %8lu| %7s| %6s| %8d|\n", nal_num, pos, idc_str, type_str, nalu.len);
        pos += (ctx.code_pfix_len + nalu.len);
    }
    
    return 0;
}



#endif /* h264_raw_hpp */
