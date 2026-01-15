#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "signal_handle.h"
#include "constants.h"
#include "network_types.h"
#include "raw_socket.h"

extern t_network_data nwdata;
extern t_buffer buff;


void sigint_handler(int signal)
{
    (void) signal;
    memcpy(&buff.to_client[MAC_SIZE], nwdata.mac_server, MAC_SIZE);
    memcpy(&buff.to_client[ARP_SENDER_MAC_OFFSET], nwdata.mac_server, MAC_SIZE);
    memcpy(&buff.to_client[ARP_TARGET_IP_OFFSET], nwdata.ip_server, IP_SIZE);

    memcpy(&buff.to_server[MAC_SIZE], nwdata.mac_client, MAC_SIZE);
    memcpy(&buff.to_server[ARP_SENDER_MAC_OFFSET], nwdata.mac_client, MAC_SIZE);
    memcpy(&buff.to_server[ARP_TARGET_IP_OFFSET], nwdata.ip_client, IP_SIZE);
    send_raw_paquets();
    close_socket();
    exit(0);
}

void setup_signal(){

    struct sigaction act = {0};;
    act.sa_handler = sigint_handler;
    sigaction(SIGINT, &act, NULL);

}