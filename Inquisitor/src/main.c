#include <stdio.h>  // sscanf
#include <string.h> // strlen
#include <stdbool.h>
#include <stdlib.h> // exit
#include <ctype.h>  // isdigit

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

    
    if (a < 0 || a > 255) return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);
    if (b < 0 || b > 255) return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);
    if (c < 0 || c > 255) return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);
    if (d < 0 || d > 255) return (fprintf(stderr, "invalid IP address format: %s\n", ip), false);

    return true;

}
void parse_args(char **argv)
{

    if (!is_valid_ipv4(argv[1]) ||
        !is_valid_mac(argv[2]) ||
        !is_valid_ipv4(argv[3]) ||
        !is_valid_mac(argv[4]))
    {
        exit(1);
    }
}

// void mac_str_to_bin(const char *mac_str)
// {

//     return 0;
// }

int main(int argc, char **argv)
{
    if (argc != 5)
        return (fprintf(stderr, "The program must have 4 arguments\n"));
    (void)argv;
    // parse_args(argv);
    // unsigned char binary_mac_target[6];
    // unsigned char binary_mac_src[6];
    char str[] = "08:00:27:26:c0:0c";
    char strip[] = "192.168.56.10";
    is_valid_ipv4(strip);
    unsigned int a, b, c, d, e, f;

    int n = sscanf(str, "%2x:%2x:%2x:%2x:%2x:%2x", &a, &b, &c, &d, &e, &f);
    if (n < 6)
        return 1;
    printf("%02x %02x %02x %02x %02x %02x\n", a, b, c, d, e, f);
    printf("%u %u %u %u %u %u\n", a, b, c, d, e, f);
    return 0;
}