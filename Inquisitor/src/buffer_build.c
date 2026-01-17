#include <string.h> 

#include "buffer_build.h"
#include "network_types.h"
#include "constants.h"

extern t_network_data nwdata;
extern t_buffer buff;

static void build_buffer_for_client(){

    memcpy(&buff.to_client[0], nwdata.mac_client, MAC_SIZE);
    memcpy(&buff.to_client[MAC_SIZE], nwdata.mac_local, MAC_SIZE);
    memcpy(&buff.to_client[MAC_SIZE * 2], ETH_TYPE_ARP, ETH_TYPE_SIZE);
    memcpy(&buff.to_client[ARP_HARDWARE_TYPE_OFFSET], HW_ETHERNET, HW_ETH_SIZE);
    memcpy(&buff.to_client[ARP_PROTOCOL_TYPE_OFFSET], PROTOCOL_TYPE_IPV4, PROTOCOL_TYPE_SIZE);
    buff.to_client[ARP_HARDWARE_SIZE_OFFSET] = MAC_SIZE;
    buff.to_client[ARP_PROTOCOL_SIZE_OFFSET] = IP_SIZE;
    memcpy(&buff.to_client[ARP_OPCODE_OFFSET], OPCODE_REPLY, OPCODE_SIZE);
    memcpy(&buff.to_client[ARP_SENDER_MAC_OFFSET], nwdata.mac_local, MAC_SIZE);
    memcpy(&buff.to_client[ARP_SENDER_IP_OFFSET], nwdata.ip_server, IP_SIZE);
    memcpy(&buff.to_client[ARP_TARGET_MAC_OFFSET], nwdata.mac_client, MAC_SIZE);
    memcpy(&buff.to_client[ARP_TARGET_IP_OFFSET], nwdata.ip_client, IP_SIZE);
}

static void build_buffer_for_server(){
    memcpy(&buff.to_server[0], nwdata.mac_server, MAC_SIZE);
    memcpy(&buff.to_server[MAC_SIZE], nwdata.mac_local, MAC_SIZE);
    memcpy(&buff.to_server[MAC_SIZE * 2], ETH_TYPE_ARP, ETH_TYPE_SIZE);
    memcpy(&buff.to_server[ARP_HARDWARE_TYPE_OFFSET], HW_ETHERNET, HW_ETH_SIZE);
    memcpy(&buff.to_server[ARP_PROTOCOL_TYPE_OFFSET], PROTOCOL_TYPE_IPV4, PROTOCOL_TYPE_SIZE);
    buff.to_server[ARP_HARDWARE_SIZE_OFFSET] = MAC_SIZE;
    buff.to_server[ARP_PROTOCOL_SIZE_OFFSET] = IP_SIZE;
    memcpy(&buff.to_server[ARP_OPCODE_OFFSET], OPCODE_REPLY, OPCODE_SIZE);
    memcpy(&buff.to_server[ARP_SENDER_MAC_OFFSET], nwdata.mac_local, MAC_SIZE);
    memcpy(&buff.to_server[ARP_SENDER_IP_OFFSET], nwdata.ip_client, IP_SIZE);
    memcpy(&buff.to_server[ARP_TARGET_MAC_OFFSET], nwdata.mac_server, MAC_SIZE);
    memcpy(&buff.to_server[ARP_TARGET_IP_OFFSET], nwdata.ip_server, IP_SIZE);
}

void build_buffers(){
    build_buffer_for_client();
    build_buffer_for_server();
}