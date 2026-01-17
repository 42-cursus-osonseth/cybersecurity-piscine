#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "network_types.h"
#include "constants.h"
#include "raw_socket.h"

extern t_network_data nwdata;
extern t_buffer buff;

static int socketfd;
static struct sockaddr_ll addr;
static int ifindex;

void init_raw_socket_and_addr(void){
    socketfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (socketfd == -1){
        fprintf(stderr, "socket failed: %s\n", strerror(errno));
        exit(1);
    }
    ifindex = if_nametoindex("enp0s8");
    if (ifindex == 0){
        fprintf(stderr, "if_nametoindex failed: %s\n", strerror(errno));
        exit(1);
    }

    addr = (struct sockaddr_ll){0};
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = ifindex;
    addr.sll_halen = ETH_ALEN;
    addr.sll_protocol = htons(ETH_P_ARP);
}

void send_raw_paquets(){
    ssize_t ret;
    memcpy(addr.sll_addr, nwdata.mac_client, MAC_SIZE);
    ret = sendto(socketfd, buff.to_client, ETH_BUFFER_SIZE_MIN, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1){
        fprintf(stderr, "sendto failed: %s\n", strerror(errno));
        if (errno != EAGAIN && errno != EWOULDBLOCK && errno != ENOBUFS)
            exit(1);
    }
    memcpy(addr.sll_addr, nwdata.mac_server, MAC_SIZE);
    ret = sendto(socketfd, buff.to_server, ETH_BUFFER_SIZE_MIN, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1){
        fprintf(stderr, "sendto failed: %s\n", strerror(errno));
        if (errno != EAGAIN && errno != EWOULDBLOCK && errno != ENOBUFS)
            exit(1);
    }
}

void close_socket(){
    close(socketfd);
}