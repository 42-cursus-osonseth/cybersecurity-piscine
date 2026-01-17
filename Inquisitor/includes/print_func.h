#pragma once

#include <pcap.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>
#include <string.h>

void print_arp_packet(const struct pcap_pkthdr *h, const unsigned char *bytes, struct ether_header *eth_hdr);
void print_ip_packet(const struct pcap_pkthdr *h, const unsigned char *bytes, struct ether_header *eth_hdr);