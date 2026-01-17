#include <stdio.h>
#include <stdint.h>
#include "print_func.h"

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"

static void print_ethernet_header(struct ether_header *eth_hdr){
    printf("-- ETHERNET HEADER --: \n");
    printf("MAC src: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth_hdr->ether_shost[0], eth_hdr->ether_shost[1], eth_hdr->ether_shost[2],
           eth_hdr->ether_shost[3], eth_hdr->ether_shost[4], eth_hdr->ether_shost[5]);
    printf("MAC dst: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth_hdr->ether_dhost[0], eth_hdr->ether_dhost[1], eth_hdr->ether_dhost[2],
           eth_hdr->ether_dhost[3], eth_hdr->ether_dhost[4], eth_hdr->ether_dhost[5]);
    if (ntohs(eth_hdr->ether_type) == ETHERTYPE_ARP)
        printf("ETHERTYPE: ARP\n");
    else if (ntohs(eth_hdr->ether_type) == ETHERTYPE_IP)
        printf("ETHERTYPE: IPV4\n");
}

static void print_ip_header(struct iphdr *ip_hdr, int payload_offset){
    printf("-- IP  HEADER --: \n");
    printf("IP src: %s\n", inet_ntoa(*(struct in_addr *)&ip_hdr->saddr));
    printf("IP dst: %s\n", inet_ntoa(*(struct in_addr *)&ip_hdr->daddr));
    printf("IP total length: %u\n", ntohs(ip_hdr->tot_len));
    printf("ID: %u\n", ntohs(ip_hdr->id));
    printf("offset payload: %d\n", payload_offset);
    uint16_t frag = ntohs(ip_hdr->frag_off);
    uint16_t offset = frag & 0x1FFF;
    uint8_t df = (frag & 0x4000) >> 14;
    uint8_t mf = (frag & 0x2000) >> 13;
    printf("Flags: DF=%u, MF=%u\n", df, mf);
    printf("Fragment offset: %u\n", offset);
}

static void print_tcp_header(struct tcphdr *tcp_hdr){
    printf("Protocol: TCP\n");
    printf("-- TCP HEADER --:\n");
    printf("source port: %d\n", ntohs(tcp_hdr->source));
    printf("dest port:   %d\n", ntohs(tcp_hdr->dest));
    printf("flags: ");
    if (tcp_hdr->syn)
        printf(" SYN ");
    if (tcp_hdr->ack)
        printf(" ACK ");
    if (tcp_hdr->psh)
        printf(" PSH ");
    if (tcp_hdr->fin)
        printf(" FIN ");
}

static void print_packet_hex(const struct pcap_pkthdr *h, const unsigned char *bytes){
    printf("PACKET RECEIVED:\n");
    for (unsigned int i = 0; i < h->caplen; i++)
        printf("%02x ", bytes[i]);
    printf("\n");
}

static void print_tcp_flags_only(struct tcphdr *tcp_hdr){
    printf("TCP: ");
    if (tcp_hdr->syn)
        printf("SYN ");
    if (tcp_hdr->ack)
        printf("ACK");
    if (tcp_hdr->psh)
        printf("PSH");
    if (tcp_hdr->fin)
        printf("PSH");
    printf("\n");
}

void print_arp_packet(const struct pcap_pkthdr *h, const unsigned char *bytes, struct ether_header *eth_hdr){
    struct ether_arp *arp = (struct ether_arp *)(bytes + sizeof(struct ether_header));
    printf(COLOR_CYAN "--------------------------------------------------------------------------------------------------------------\n" COLOR_RESET);
    printf(COLOR_BLUE);
    print_packet_hex(h, bytes);
    printf(COLOR_RESET);
    printf(COLOR_GREEN);
    print_ethernet_header(eth_hdr);
    printf("-- ARP PACKET --:\n");
    if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST)
        printf("OPCODE: REQUEST\n");
    else if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REPLY)
        printf("OPCODE: REPLY\n");
    printf("MAC sender: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2],
           arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
    printf("MAC target: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp->arp_tha[0], arp->arp_tha[1], arp->arp_tha[2],
           arp->arp_tha[3], arp->arp_tha[4], arp->arp_tha[5]);
    printf("IP sender: %s\n", inet_ntoa(*(struct in_addr *)&arp->arp_spa));
    printf("IP target: %s\n", inet_ntoa(*(struct in_addr *)&arp->arp_tpa));
    printf(COLOR_RESET);
}

static void print_icmp_payload(struct ether_header *eth_hdr, struct iphdr *ip_hdr, unsigned char * payload_ip, int payload_offset){
    print_ethernet_header(eth_hdr);
    print_ip_header(ip_hdr, payload_offset);
    printf("Protocol: ICMP\n");
    printf("---------- ICMP  HEADER------------------------\n");
    struct icmphdr *icmp_hdr = (struct icmphdr *)(payload_ip);
    if (icmp_hdr->type == ICMP_ECHO)
        printf("type: ECHO REQUEST\n\n");
    else if (icmp_hdr->type == ICMP_ECHOREPLY)
        printf("type: ECHO REPLY\n\n");
}

void print_ip_packet(const struct pcap_pkthdr *h, const unsigned char *bytes, struct ether_header *eth_hdr){
    unsigned char payload_ip[1500] = {0};
    unsigned char payload_tcp[1500] = {0};
    struct iphdr *ip_hdr = (struct iphdr *)(bytes + sizeof(struct ether_header));
    int payload_offset = sizeof(struct ether_header) + ip_hdr->ihl * 4;
    int payload_ip_len = ntohs(ip_hdr->tot_len) - ip_hdr->ihl * 4;
    memcpy(payload_ip, &bytes[payload_offset], payload_ip_len);
    switch (ip_hdr->protocol){
    case IPPROTO_ICMP:
        print_icmp_payload(eth_hdr, ip_hdr, payload_ip, payload_offset);
        break;
    case IPPROTO_TCP:
        struct tcphdr *tcp_hdr = (struct tcphdr *)(payload_ip);
        int payload_tcp_len = payload_ip_len - tcp_hdr->doff * 4;
        if (payload_tcp_len == 0){
            printf(COLOR_CYAN "--------------------------------------------------------------------------------------------------------------\n" COLOR_RESET);
            printf(COLOR_BLUE);
            print_packet_hex(h, bytes);
            printf(COLOR_RESET);
            printf(COLOR_MAGENTA);
            print_tcp_flags_only(tcp_hdr);
            printf(COLOR_RESET);
            printf(COLOR_CYAN "--------------------------------------------------------------------------------------------------------------\n\n\n" COLOR_RESET);
            return;
        }
        if (payload_tcp_len > 0){
            printf(COLOR_CYAN "--------------------------------------------------------------------------------------------------------------\n" COLOR_RESET);
            printf(COLOR_BLUE);
            print_packet_hex(h, bytes);
            printf(COLOR_RESET "\n");
            printf(COLOR_GREEN);
            print_ethernet_header(eth_hdr);
            print_ip_header(ip_hdr, payload_offset);
            print_tcp_header(tcp_hdr);
            printf(COLOR_RESET);
            printf(COLOR_RED);
            printf("\n\ndata TCP : ");
            memcpy(payload_tcp, &payload_ip[tcp_hdr->doff * 4], payload_tcp_len);
            for (int i = 0; i < payload_tcp_len; i++){
                if (payload_tcp[i] >= 32 && payload_tcp[i] <= 126)
                    putchar(payload_tcp[i]);
                else
                    putchar('.');
            }
            printf("\n" COLOR_RESET);
        }
        printf(COLOR_CYAN "--------------------------------------------------------------------------------------------------------------\n\n\n" COLOR_RESET);
        break;
    case IPPROTO_UDP:
        printf("Protocol: UDP\n");
        break;
    default:
        printf("Protocol: %u\n", ip_hdr->protocol);
    }
}
