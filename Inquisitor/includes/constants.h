#pragma once

extern const unsigned char ETH_TYPE_ARP[2];
extern const unsigned char ETH_TYPE_IPv4[2];
extern const unsigned char HW_ETHERNET[2];
extern const unsigned char OPCODE_REPLY[2];
extern const unsigned char MAC_SIZE;
extern const unsigned char IP_SIZE;

typedef enum e_addr_field
{
    MAC_SRC,
    MAC_TARGET,
    IP_SRC,
    IP_TARGET,
    IP_LOCAL
} t_addr_field;
