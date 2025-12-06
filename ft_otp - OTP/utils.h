#ifndef UTILS_H
#define UTILS_H

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool is_hex_digit(char c);
bool is_hex_key(char * str);
void check_is_hex_key(char **buffer);
char *parse_args(int argc, char **argv, char *opt_flag);

#endif