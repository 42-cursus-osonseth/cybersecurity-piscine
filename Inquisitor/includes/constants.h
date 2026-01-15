#pragma once

extern const unsigned char ETH_TYPE_ARP[2];
extern const unsigned char PROTOCOL_TYPE_IPV4[2];
extern const unsigned char HW_ETHERNET[2];
extern const unsigned char OPCODE_REPLY[2];
extern const unsigned char MAC_SIZE;
extern const unsigned char IP_SIZE;
extern const unsigned char ETH_TYPE_SIZE;
extern const unsigned char ETH_HEADER_SIZE;
extern const unsigned char OPCODE_SIZE;
extern const unsigned char HW_ETH_SIZE;
extern const unsigned char PROTOCOL_TYPE_SIZE;
extern const unsigned char ETH_BUFFER_SIZE_MIN;

extern const unsigned char ARP_HARDWARE_TYPE_OFFSET;
extern const unsigned char ARP_PROTOCOL_TYPE_OFFSET;
extern const unsigned char ARP_HARDWARE_SIZE_OFFSET;
extern const unsigned char ARP_PROTOCOL_SIZE_OFFSET;
extern const unsigned char ARP_OPCODE_OFFSET;
extern const unsigned char ARP_SENDER_MAC_OFFSET;
extern const unsigned char ARP_SENDER_IP_OFFSET;
extern const unsigned char ARP_TARGET_MAC_OFFSET;
extern const unsigned char ARP_TARGET_IP_OFFSET;

typedef enum e_addr_field
{
    MAC_SERVER,
    MAC_CLIENT,
    IP_SERVER,
    IP_CLIENT,
    IP_LOCAL
} t_addr_field;
