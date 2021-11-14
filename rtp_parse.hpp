//
//  rtp_parse.hpp
//  mmbasic
//
//  Created by lubing.wan on 2021/11/14.
//

#ifndef rtp_parse_h
#define rtp_parse_h
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <uv.h>
//#pragma pack(1)

/*
 * [memo] FFmpeg stream Command:
 * ffmpeg -re -i sintel.ts -f mpegts udp://127.0.0.1:8880
 * ffmpeg -re -i sintel.ts -f rtp_mpegts udp://127.0.0.1:8880
 */

typedef struct RTP_FIXED_HEADER{
    /* byte 0 */
    uint8_t csrc_len:4;       /* expect 0 */
    uint8_t extension:1;      /* expect 1 */
    uint8_t padding:1;        /* expect 0 */
    uint8_t version:2;        /* expect 2 */
    /* byte 1 */
    uint8_t payload:7;
    uint8_t marker:1;        /* expect 1 */
    /* bytes 2, 3 */
    uint16_t seq_no;
    /* bytes 4-7 */
    uint32_t timestamp;
    /* bytes 8-11 */
    uint32_t ssrc;            /* stream number is used here. */
} RTP_FIXED_HEADER;

typedef struct MPEGTS_FIXED_HEADER {
    uint32_t sync_byte: 8;
    uint32_t transport_error_indicator: 1;
    uint32_t payload_unit_start_indicator: 1;
    uint32_t transport_priority: 1;
    uint32_t PID: 13;
    uint32_t scrambling_control: 2;
    uint32_t adaptation_field_exist: 2;
    uint32_t continuity_counter: 4;
} MPEGTS_FIXED_HEADER;

uv_loop_t *loop;
uv_udp_t send_socket;
uv_udp_t recv_socket;

void uv_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    uint8_t const_buf[1024 * 1024];
    *buf = uv_buf_init((char*)const_buf, (unsigned int)suggested_size);
}

static uint32_t cnt = 0;
void udp_parse(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags) {
    using namespace std;
    cnt++;
    cout << "nread: " << nread << " flags: " << flags << endl;
    if (nread > 0){
        //printf("Addr:%s\r\n",inet_ntoa(remoteAddr.sin_addr));
        //printf("packet size:%d\r\n",pktsize);
        //Parse RTP
        //

        char payload_str[10] = {0};
        RTP_FIXED_HEADER rtp_header;
        int rtp_header_size = sizeof(RTP_FIXED_HEADER);
        //RTP Header
        memcpy((void *)&rtp_header, buf->base, rtp_header_size);

        //RFC3351
        char payload = rtp_header.payload;
        switch(payload) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
                sprintf(payload_str,"Audio");break;
            case 31:
                sprintf(payload_str,"H.261");break;
            case 32:
                sprintf(payload_str,"MPV");break;
            case 33:
                sprintf(payload_str,"MP2T");break;
            case 34:
                sprintf(payload_str,"H.263");break;
            case 96:
                sprintf(payload_str,"H.264");break;
            default:
                sprintf(payload_str,"other");break;
        }

        uint32_t timestamp = ntohl(rtp_header.timestamp);
        uint32_t seq_no = ntohs(rtp_header.seq_no);

        printf("[RTP Pkt] %5d| %5s| %10u| %5d| %5ld|\n", cnt, payload_str, timestamp, seq_no, nread);

        //RTP Data
        char *rtp_data = buf->base + rtp_header_size;
        size_t rtp_data_size= nread - rtp_header_size;

        //Parse MPEGTS
        if(payload == 33){
//            MPEGTS_FIXED_HEADER mpegts_header;
            for(int i = 0; i < rtp_data_size; i = i + 188){
                if(rtp_data[i]!=0x47)
                    break;
                //MPEGTS Header
//                memcpy((void *)&mpegts_header, rtp_data + i, sizeof(MPEGTS_FIXED_HEADER));
                printf("   [MPEGTS Pkt]\n");
            }
        }

        cnt++;
    }
}

int uv_init(int port) {
    loop = uv_default_loop();

    uv_udp_init(loop, &recv_socket);
    struct sockaddr_in recv_addr;
    uv_ip4_addr("0.0.0.0", port, &recv_addr);
    uv_udp_bind(&recv_socket, (const struct sockaddr *)&recv_addr, UV_UDP_REUSEADDR);
    uv_udp_recv_start(&recv_socket, uv_alloc, udp_parse);

    return uv_run(loop, UV_RUN_DEFAULT);
}

int mm_rtp_parser(int port)
{

    uv_init(port);

    return 0;
}


#endif /* rtp_parse_h */
