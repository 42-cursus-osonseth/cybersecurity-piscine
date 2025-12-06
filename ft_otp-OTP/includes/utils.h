#ifndef UTILS_H
#define UTILS_H

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

bool is_hex_digit(char c);
bool is_hex_key(char * str);
void check_is_hex_key(char **buffer);
char *parse_args(int argc, char **argv, char *opt_flag);
char * allocate_the_buffer(void);
void crypt_decrypt_key(char **buffer);
void create_counter_array(unsigned char *counter, uint64_t timestep);
uint32_t create_sbytes(unsigned char *hmac);

#endif