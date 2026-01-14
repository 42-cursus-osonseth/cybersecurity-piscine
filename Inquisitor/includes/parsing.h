#pragma once 

#include <stdbool.h>
#include "network_types.h"

void parse_args(char **argv, t_network_data *nwdata);
bool is_valid_mac(const char *str);
bool is_valid_ipv4(const char *ip);
void args_to_bin(char **argv, t_network_data *nwdata);
void mac_str_to_bin(const char *mac_str,  t_network_data *nwdata, int code);
void ip_str_to_bin(const char *ip_str, t_network_data *nwdata, int code);
void get_local_mac(t_network_data *nwdata);