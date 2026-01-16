#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>    // pour ntohs() et inet_ntoa()
#include <net/ethernet.h> // pour struct ether_header, ETHERTYPE_IP
#include <netinet/ip.h>   // pour struct ip

void callback(unsigned char *user, const struct pcap_pkthdr *h, const unsigned char *bytes)
{
    (void)user;
    printf("PAQUET :\n");
    for (unsigned int i = 0; i < h->caplen; i++)
        printf("%02x ", bytes[i]);
    printf("\n");
    struct ether_header *eth = (struct ether_header *)bytes;
    printf("MAC src: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->ether_shost[0], eth->ether_shost[1], eth->ether_shost[2],
           eth->ether_shost[3], eth->ether_shost[4], eth->ether_shost[5]);
    printf("MAC dst: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->ether_dhost[0], eth->ether_dhost[1], eth->ether_dhost[2],
           eth->ether_dhost[3], eth->ether_dhost[4], eth->ether_dhost[5]);

    if (ntohs(eth->ether_type) == ETHERTYPE_IP)
    {
        struct iphdr *ip_hdr = (struct iphdr *)(bytes + sizeof(struct ether_header));
        printf("IP src: %s\n", inet_ntoa(*(struct in_addr *)&ip_hdr->saddr));
        printf("IP dst: %s\n", inet_ntoa(*(struct in_addr *)&ip_hdr->daddr));
    }

    printf("\n");
}

int main()
{

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live("wlp0s20f3", 65535, 0, 1000, errbuf);
    if (!handle)
    {
        printf("%s\n", errbuf);
        return 1;
    }

    struct bpf_program filter;
    pcap_compile(handle, &filter, "tcp", 1, PCAP_NETMASK_UNKNOWN);
    pcap_loop(handle, -1, callback, NULL);

    return 0;
}