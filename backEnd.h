#ifndef BACKEND_H_
#define BACKEND_H_

#include "frontEnd.h"

/*
 * wrapper of parameters to send to client as a thread's first function
 */
struct client_to_servers_args {
    char* serverIP; //The IP address of the server which we will connect to
    Servers server; //The server that we will connect to
};

#define SERV1_IP "192.168.0.101"
#define SERV2_IP "192.168.0.102"
#define SERV3_IP "192.168.0.103"

/*
 *	@param args - assumed to be struct client_to_servers_args
 *	Runs one thread, and handles the connection with one back end server. It takes the first request of
 *	the priority queue and send it to the server. When it receives a response, it sends it back to the
 *	client that sent this request in the first place.
 */
void* backEnd(void *args);

#endif /* BACKEND_H_ */
