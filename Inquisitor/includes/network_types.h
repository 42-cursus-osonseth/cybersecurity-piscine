#pragma once

// typedef struct s_arp_reply_frame
// {
//     unsigned char ethertype[2];
//     unsigned char hw_type[2];
//     unsigned char proto_type[2];
//     unsigned char hw_size[1];
//     unsigned char proto_size[1];
//     unsigned char opcode[2];
//     unsigned char mac_src[6];
//     unsigned char ip_src[4];
//     unsigned char mac_dst[6];
//     unsigned char ip_dst[4];
// } t_arp_reply_frame;

typedef struct s_network_data{
    unsigned char mac_src[6];
    unsigned char ip_src[4];
    unsigned char mac_dst[6];
    unsigned char ip_dst[4];
    unsigned char mac_local[6];
    unsigned char ip_local[4];
} t_network_data;