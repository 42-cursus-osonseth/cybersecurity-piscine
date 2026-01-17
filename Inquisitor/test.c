#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> // pour ntohs() et inet_ntoa()
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

#include <string.h>

void callback(unsigned char *user, const struct pcap_pkthdr *h, const unsigned char *bytes)
{
    unsigned char payload_ip[1500] = {0};
    unsigned char payload_tcp[1500] = {0};
    (void)user;
    printf("PAQUET RECEIVED:\n");
    for (unsigned int i = 0; i < h->caplen; i++)
        printf("%02x ", bytes[i]);
    printf("\n\n");
    struct ether_header *eth = (struct ether_header *)bytes;
    printf("---------- ETHERNET HEADER------------------------\n");
    printf("MAC src: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->ether_shost[0], eth->ether_shost[1], eth->ether_shost[2],
           eth->ether_shost[3], eth->ether_shost[4], eth->ether_shost[5]);
    printf("MAC dst: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->ether_dhost[0], eth->ether_dhost[1], eth->ether_dhost[2],
           eth->ether_dhost[3], eth->ether_dhost[4], eth->ether_dhost[5]);
    if (ntohs(eth->ether_type) == ETHERTYPE_ARP)
        printf("ETHERTYPE: ARP\n");
    else if (ntohs(eth->ether_type) == ETHERTYPE_IP)
        printf("ETHERTYPE: IPV4\n\n");

    if (ntohs(eth->ether_type) == ETHERTYPE_IP)
    {
        printf("---------- IP  HEADER------------------------\n");
        struct iphdr *ip_hdr = (struct iphdr *)(bytes + sizeof(struct ether_header));
        printf("IP src: %s\n", inet_ntoa(*(struct in_addr *)&ip_hdr->saddr));
        printf("IP dst: %s\n", inet_ntoa(*(struct in_addr *)&ip_hdr->daddr));
        printf("IP total length: %u\n", ntohs(ip_hdr->tot_len));
        printf("ID: %u\n", ntohs(ip_hdr->id));
        int payload_offset = sizeof(struct ether_header) + ip_hdr->ihl * 4;
        printf("offset payload: %d\n", payload_offset);
        uint16_t frag = ntohs(ip_hdr->frag_off);
        uint16_t offset = frag & 0x1FFF;
        uint8_t df = (frag & 0x4000) >> 14;
        uint8_t mf = (frag & 0x2000) >> 13;

        printf("Flags: DF=%u, MF=%u\n", df, mf);
        printf("Fragment offset: %u\n", offset);

        int payload_ip_len = ntohs(ip_hdr->tot_len) - ip_hdr->ihl * 4;
        memcpy(payload_ip, &bytes[payload_offset], payload_ip_len);

        switch (ip_hdr->protocol)
        {
        case 1:
            printf("Protocol: ICMP\n");
            printf("---------- ICMP  HEADER------------------------\n");
            struct icmphdr *icmp_hdr = (struct icmphdr *)(payload_ip);
            if (icmp_hdr->type == ICMP_ECHO)
                printf("type: ECHO REQUEST\n");
            else if (icmp_hdr->type == ICMP_ECHOREPLY)
                printf("type: ECHO REPLY\n");
            break;
        case 6:
            printf("Protocol: TCP\n");
            printf("---------- TCP HEADER------------------------\n");
            struct tcphdr *tcp_hdr = (struct tcphdr *)(payload_ip);
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
            printf("\n\ndata TCP : ");
            int payload_tcp_len = payload_ip_len - tcp_hdr->doff * 4;
            if (payload_tcp_len > 0)
            {
                memcpy(payload_tcp, &payload_ip[tcp_hdr->doff * 4], payload_tcp_len);
                printf("\033[32m");
                for (int i = 0; i < payload_tcp_len; i++)
                {
                    if (payload_tcp[i] >= 32 && payload_tcp[i] <= 126)
                        putchar(payload_tcp[i]);
                    else
                        putchar('.');
                }
                printf("\033[0m");
            }
            printf("\n");
            break;
        case 17:
            printf("Protocol: UDP\n");
            break;
        default:
            printf("Protocol: %u\n", ip_hdr->protocol);
        }

        printf("--------------------------------------------------\n");
    }

    printf("\n\n\n");
}

int main()
{

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live("enp0s8", 65535, 0, 1000, errbuf);
    if (!handle)
    {
        printf("%s\n", errbuf);
        return 1;
    }

    struct bpf_program filter;
    pcap_compile(handle, &filter, "tcp or arp or icmp", 1, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(handle, &filter);
    pcap_loop(handle, -1, callback, NULL);

    return 0;
}