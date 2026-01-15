#pragma once 

#include <stdbool.h>
#include "network_types.h"

void parse_args(char **argv);
bool is_valid_mac(const char *str);
bool is_valid_ipv4(const char *ip);
void args_to_bin(char **argv);
void mac_str_to_bin(const char *mac_str, int code);
void ip_str_to_bin(const char *ip_str, int code);
void get_local_mac();