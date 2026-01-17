#include <stdio.h> // sscanf, printf, fprintf
#include <unistd.h>
#include <pthread.h>
#include <signal.h>


#include "network_types.h"
#include "print_func.h"
#include "constants.h"
#include "parsing.h"
#include "buffer_build.h"
#include "raw_socket.h"
#include "signal_handle.h"

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
const unsigned char ETH_BUFFER_SIZE_MIN = 60;

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
volatile sig_atomic_t running = true;

void * send_arp_reply_paquet(void * arg){

    (void) arg;
    while (running)
    {
        send_raw_paquets();
        sleep(2);
    }
    return NULL;

}

int main(int argc, char **argv)
{
    if (argc != 5)
        return (fprintf(stderr, "The program must have 4 arguments\n"));
    parse_args(argv);
    build_buffers();
    init_raw_socket_and_addr();
    setup_signal();
    pthread_t		sender_arp_packet;
    pthread_create(&sender_arp_packet, NULL, send_arp_reply_paquet, NULL);

    
    
    return 0;
}