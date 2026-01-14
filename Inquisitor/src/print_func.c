#include <stdio.h>
#include <stdint.h>

void print_buffer_bin_hex(const unsigned char *buf, size_t len)
{
    size_t i, bit;

    for (i = 0; i < len; i++)
    {
        for (bit = 0; bit < 8; bit++)
            putchar((buf[i] & (1 << (7 - bit))) ? '1' : '0');
        putchar(' ');
    }
    putchar('\n');
    for (i = 0; i < len; i++)
        printf("%02x ", buf[i]);
    putchar('\n');
}

void print_buffer_bin_dec(const unsigned char *buf, size_t len)
{
    size_t i, bit;

    for (i = 0; i < len; i++)
    {
        for (bit = 0; bit < 8; bit++)
            putchar((buf[i] & (1 << (7 - bit))) ? '1' : '0');
        putchar(' ');
    }
    putchar('\n');

    for (i = 0; i < len; i++)
        printf("%3u ", buf[i]);
    putchar('\n');
}
static void print_mac(const unsigned char *m)
{
    printf("%02x:%02x:%02x:%02x:%02x:%02x",
           m[0], m[1], m[2], m[3], m[4], m[5]);
}

static void print_ip(const unsigned char *ip)
{
    printf("%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
}

static uint16_t read_u16(const unsigned char *b)
{
    return (b[0] << 8) | b[1];
}

void print_arp_frame(const unsigned char *buf)
{
    printf("=== Ethernet ===\n");
    printf("Dest MAC    : ");
    print_mac(buf);
    printf("\n");
    printf("Source MAC  : ");
    print_mac(buf + 6);
    printf("\n");
    printf("Ethertype   : 0x%04x\n\n", read_u16(buf + 12));

    printf("=== ARP ===\n");
    printf("HW type     : %u\n", read_u16(buf + 14));
    printf("Proto type  : 0x%04x\n", read_u16(buf + 16));
    printf("HW size     : %u\n", buf[18]);
    printf("Proto size  : %u\n", buf[19]);
    printf("Opcode      : %u\n", read_u16(buf + 20));

    printf("Sender MAC  : ");
    print_mac(buf + 22);
    printf("\n");
    printf("Sender IP   : ");
    print_ip(buf + 28);
    printf("\n");
    printf("Target MAC  : ");
    print_mac(buf + 32);
    printf("\n");
    printf("Target IP   : ");
    print_ip(buf + 38);
    printf("\n");
}