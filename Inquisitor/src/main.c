#include <stdio.h>  // sscanf, printf, fprintf
#include <string.h> // memcpy
#include <stdlib.h>

#include "network_types.h"
#include "print_func.h"
#include "constants.h"
#include "parsing.h"

const unsigned char ETH_TYPE_ARP[2] = {0x08, 0x06};
const unsigned char PROTCOL_TYPE_IPv4[2] = {0x08, 0x00};
const unsigned char HW_ETHERNET[2] = {0x00, 0x01};
const unsigned char OPCODE_REPLY[2] = {0x00, 0x02};
const unsigned char MAC_SIZE = 6;
const unsigned char IP_SIZE = 4;

unsigned char buff[60] = {0};

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

    t_network_data nwdata = {0};

    parse_args(argv, &nwdata);
    int s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (s == -1)
        return (fprintf(stderr, "socket failed: %s\n", strerror(errno)));
    memcpy(buff, nwdata.mac_dst, MAC_SIZE);
    memcpy(&buff[MAC_SIZE], nwdata.mac_local, MAC_SIZE);
    memcpy(&buff[MAC_SIZE * 2], ETH_TYPE_ARP, 2);
    memcpy(&buff[14], HW_ETHERNET, 2);
    memcpy(&buff[16], PROTCOL_TYPE_IPv4, 2);
    buff[18] = MAC_SIZE;
    buff[19] = IP_SIZE;
    memcpy(&buff[20], OPCODE_REPLY, 2);
    memcpy(&buff[22], nwdata.mac_local, MAC_SIZE);
    memcpy(&buff[28], nwdata.ip_src, IP_SIZE);
    memcpy(&buff[32], nwdata.mac_dst, MAC_SIZE);
    memcpy(&buff[38], nwdata.ip_dst, IP_SIZE);
    print_arp_frame(buff);
    //-----------------------------------------------------
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
    memcpy(addr.sll_addr, nwdata.mac_dst, 6);
    //----------------------------------------------------------------------
    while (1){
        sendto(s, buff, 60, 0, (struct sockaddr *)&addr, sizeof(addr));
        sleep(2);
    }
    return 0;
}