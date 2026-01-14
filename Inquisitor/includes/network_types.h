#pragma once

typedef struct s_network_data{
    unsigned char mac_server[6];
    unsigned char ip_server[4];
    unsigned char mac_client[6];
    unsigned char ip_client[4];
    unsigned char mac_local[6];
    unsigned char ip_local[4];
} t_network_data;

typedef struct s_buffer{

    unsigned char to_client[60];
    unsigned char to_server[60];
}t_buffer;