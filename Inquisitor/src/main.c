#include <stdio.h>  // sscanf, printf, fprintf
#include <string.h> // memcpy
#include <stdlib.h>

#include "network_types.h"
#include "print_func.h"
#include "constants.h"
#include "parsing.h"
#include "buffer_build.h"

const unsigned char ETH_TYPE_ARP[2] = {0x08, 0x06};
const unsigned char PROTOCOL_TYPE_IPV4[2] = {0x08, 0x00};
const unsigned char HW_ETHERNET[2] = {0x00, 0x01};
const unsigned char OPCODE_REPLY[2] = {0x00, 0x02};
const unsigned char MAC_SIZE = 6;
const unsigned char IP_SIZE = 4;
const unsigned char ETH_TYPE_SIZE = 2;
const unsigned char ETH_HEADER_SIZE = (MAC_SIZE * 2) + ETH_TYPE_SIZE;
const unsigned char OPCODE_SIZE = 2;
const unsigned char HW_ETH_SIZE = 2;
const unsigned char PROTOCOL_TYPE_SIZE = 2;

const unsigned char ARP_HARDWARE_TYPE_OFFSET = ETH_HEADER_SIZE + 0;
const unsigned char ARP_PROTOCOL_TYPE_OFFSET = ETH_HEADER_SIZE + 2;
const unsigned char ARP_HARDWARE_SIZE_OFFSET = ETH_HEADER_SIZE + 4;
const unsigned char ARP_PROTOCOL_SIZE_OFFSET = ETH_HEADER_SIZE + 5;
const unsigned char ARP_OPCODE_OFFSET = ETH_HEADER_SIZE + 6;
const unsigned char ARP_SENDER_MAC_OFFSET = ETH_HEADER_SIZE + 8;
const unsigned char ARP_SENDER_IP_OFFSET = ETH_HEADER_SIZE + 14;
const unsigned char ARP_TARGET_MAC_OFFSET = ETH_HEADER_SIZE + 18;
const unsigned char ARP_TARGET_IP_OFFSET = ETH_HEADER_SIZE + 24;

t_network_data nwdata = {0};
t_buffer buff = {0};

//----------------------------------------------------------
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h> /* protocoles L2 */
#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <unistd.h>

//--------------------------------------------------------

#include <stdio.h>
#include <stdint.h>



int main(int argc, char **argv)
{
    if (argc != 5)
        return (fprintf(stderr, "The program must have 4 arguments\n"));

    parse_args(argv);
    build_buffers();


    printf("---------CLIENT----------------");
    print_arp_frame(buff.to_client);
    printf("-------------------------------");
     printf("---------SERVER----------------");
    print_arp_frame(buff.to_server);
    printf("-------------------------------");
    //------------------------ envoie de du buffer sur l interface reseau -----------------------------

    int s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (s == -1)
        return (fprintf(stderr, "socket failed: %s\n", strerror(errno)));
    unsigned int ifindex = if_nametoindex("enp0s8");
    if (ifindex == 0)
    {
        perror("if_nametoindex");
        exit(1);
    }
    struct sockaddr_ll addr;
    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = ifindex;
    addr.sll_halen = ETH_ALEN;
    memcpy(addr.sll_addr, nwdata.mac_client, MAC_SIZE);
    //----------------------------------------------------------------------
    while (1)
    {
        sendto(s, buff.to_client, 60, 0, (struct sockaddr *)&addr, sizeof(addr));
        sendto(s, buff.to_server, 60, 0, (struct sockaddr *)&addr, sizeof(addr));
        sleep(2);
    }
    return 0;
}