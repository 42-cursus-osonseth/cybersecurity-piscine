#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <pcap.h>

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
pcap_t *handle;


void *send_arp_reply_paquet(void *arg){
    (void)arg;
    while (running){
        send_raw_paquets();
        sleep(2);
    }
    return NULL;
}

void callback(unsigned char *user, const struct pcap_pkthdr *h, const unsigned char *bytes){
    (void)user;
    struct ether_header *eth_hdr = (struct ether_header *)bytes;
    if (ntohs(eth_hdr->ether_type) == ETHERTYPE_ARP)
        print_arp_packet(h, bytes, eth_hdr);
    else if (ntohs(eth_hdr->ether_type) == ETHERTYPE_IP)
        print_ip_packet(h, bytes, eth_hdr);
}

int main(int argc, char **argv)
{
    if (argc != 5)
        return (fprintf(stderr, "The program must have 4 arguments\n"));
    parse_args(argv);
    build_buffers();
    init_raw_socket_and_addr();
    setup_signal();

    pthread_t sender_arp_packet;
    pthread_create(&sender_arp_packet, NULL, send_arp_reply_paquet, NULL);

    char errbuf[PCAP_ERRBUF_SIZE];
    handle = pcap_open_live("enp0s8", 65535, 0, 1000, errbuf);
    if (!handle)
    {
        printf("%s\n", errbuf);
        return 1;
    }

    struct bpf_program filter;
    pcap_compile(handle, &filter, "(tcp or arp or icmp) and ether dst 08:00:27:3e:9f:83", 1, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(handle, &filter);
    pcap_loop(handle, -1, callback, NULL);
    pcap_freecode(&filter);
    pcap_close(handle);
    pthread_join(sender_arp_packet, NULL);
    return 0;
}