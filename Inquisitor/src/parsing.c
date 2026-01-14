#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>   
#include <sys/types.h>  
#include <sys/socket.h> 
#include <net/if.h> 
#include <sys/ioctl.h>


#include "parsing.h"
#include "constants.h"


void parse_args(char **argv, t_network_data *nwdata)
{

    if (!is_valid_ipv4(argv[1]) ||
        !is_valid_mac(argv[2]) ||
        !is_valid_ipv4(argv[3]) ||
        !is_valid_mac(argv[4]))
    {
        exit(1);
    }
    args_to_bin(argv, nwdata);
    get_local_mac(nwdata);
    ip_str_to_bin("192.168.56.12", nwdata, IP_LOCAL);
    
}

bool is_valid_mac(const char *str)
{
    if (strlen(str) != 17)
        return (fprintf(stderr, "invalid MAC address format: %s\n", str), false);
    for (int i = 0; str[i]; i++)
    {
        if ((i - 2) % 3 == 0 ? str[i] != ':' : !isxdigit(str[i]))
            return (fprintf(stderr, "invalid MAC address format: %s\n", str), false);
    }
    return true;
}

bool is_valid_ipv4(const char *ip)
{

    if (strlen(ip) < 7 || strlen(ip) > 15)
        return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);

    int a, b, c, d;
    char extra;

    int n = sscanf(ip, "%d.%d.%d.%d%c", &a, &b, &c, &d, &extra);

    if (n != 4)
        return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);

    if (a < 0 || a > 255)
        return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);
    if (b < 0 || b > 255)
        return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);
    if (c < 0 || c > 255)
        return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);
    if (d < 0 || d > 255)
        return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);

    return true;
}

void args_to_bin(char **argv, t_network_data *nwdata)
{
    ip_str_to_bin(argv[1], nwdata, IP_SRC);
    mac_str_to_bin(argv[2], nwdata, MAC_SRC);
    ip_str_to_bin(argv[3], nwdata, IP_TARGET);
    mac_str_to_bin(argv[4], nwdata, MAC_TARGET);
}

void mac_str_to_bin(const char *mac_str,  t_network_data *nwdata, int code)
{
    unsigned char buff[MAC_SIZE];
    unsigned int a, b, c, d, e, f;
    int n = sscanf(mac_str, "%2x:%2x:%2x:%2x:%2x:%2x", &a, &b, &c, &d, &e, &f);
    if (n < 6)
    {
        fprintf(stderr, "sscanf failed to scan mac%s\n", mac_str);
        exit(1);
    }
    buff[0] = (unsigned char)a;
    buff[1] = (unsigned char)b;
    buff[2] = (unsigned char)c;
    buff[3] = (unsigned char)d;
    buff[4] = (unsigned char)e;
    buff[5] = (unsigned char)f;
    if (code == MAC_TARGET)
        memcpy(nwdata->mac_dst, buff, MAC_SIZE);
    else
        memcpy(nwdata->mac_src, buff, MAC_SIZE);
}

void ip_str_to_bin(const char *ip_str, t_network_data *nwdata, int code){

    unsigned char buff[IP_SIZE];
    unsigned int a, b, c, d;
    int n = sscanf(ip_str, "%d.%d.%d.%d", &a, &b, &c, &d);
    if (n < 4)
    {
        fprintf(stderr, "sscanf failed to scan ip%s\n", ip_str);
        exit(1);
    }
    buff[0] = (unsigned char)a;
    buff[1] = (unsigned char)b;
    buff[2] = (unsigned char)c;
    buff[3] = (unsigned char)d;

    if (code == IP_TARGET)
        memcpy(nwdata->ip_dst, buff, IP_SIZE);
    else if (code == IP_SRC)
        memcpy(nwdata->ip_src, buff, IP_SIZE);
    else
        memcpy(nwdata->ip_local, buff, IP_SIZE);

}

void get_local_mac(t_network_data *nwdata){
    
    int s;
    struct ifreq ifr;
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s==-1)
      exit(1);
    snprintf(ifr.ifr_name, IFNAMSIZ-1, "%s", "enp0s8" );
    if (ioctl(s, SIOCGIFFLAGS, &ifr)==0) {
      if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0)
        memcpy( nwdata->mac_local, ifr.ifr_hwaddr.sa_data, 6);
    close(s);
}

}