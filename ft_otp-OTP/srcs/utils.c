#include "errors.h"
#include "utils.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>



bool is_hex_digit(char c){
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

bool is_hex_key(char *str){

    while (*str){
        if (!is_hex_digit(*str))
            return false;
        str++;
    }
    return true;
}

void check_is_hex_key(char **buffer){
    if (!is_hex_key(*buffer))
        error_exit("Error: key must be only hex value", *buffer, NO_ERRNO);
}

char *allocate_the_buffer(void){
    char *buffer = malloc(65 * sizeof(char));
    if (!buffer)
        error_exit("Error: malloc buffer failed", NULL, USE_ERRNO);
    return buffer;
}

void crypt_decrypt_key(char **buffer){

    for (int i = 0; i < 64; i++)
        (*buffer)[i] ^= i;
}

void create_counter_array(unsigned char *counter, uint64_t timestep){
    for (int i = 7; i >= 0; i--){
        counter[i] = timestep & 0xFF;
        timestep >>= 8;
    }
}

uint32_t create_sbytes(unsigned char *hmac){
    int offset = hmac[19] & 0x0F;
    uint32_t sbytes = (hmac[offset] & 0x7F) << 24 |
                      (hmac[offset + 1] & 0xFF) << 16 |
                      (hmac[offset + 2] & 0xFF) << 8 |
                      (hmac[offset + 3] & 0xFF);
    return sbytes;
}

char *parse_args(int argc, char **argv, char *opt_flag)
{
    int c;
    int kflg = 0, gflg = 0, errflg = 0;
    char *file = NULL;

    while ((c = getopt(argc, argv, ":g:k:")) != -1)
    {
        switch (c)
        {
        case 'k':
            if ((gflg) || (kflg))
                errflg++;
            else
            {
                file = optarg;
                kflg++;
            }
            break;
        case 'g':
            if ((kflg) || (gflg))
                errflg++;
            else
            {
                file = optarg;
                gflg++;
            }
            break;
        case '?':
            fprintf(stderr,
                    "Unrecognized option: '-%c'\n", optopt);
            exit(1);
        case ':':
            fprintf(stderr, "option -%c requires an argument\n", optopt);
            errflg++;
            break;
        }
    }
    if (errflg)
        error_exit("Error: invalid option", NULL, NO_ERRNO);

    return kflg   ? (opt_flag[0] = 'k', file)
           : gflg ? (opt_flag[0] = 'g', file)
                  : NULL;
}