#ifndef CLIENT_C_
#define CLIENT_C_

#include "backEnd.h"

/*
 *
 */
static void sendAndReceive(Media request, int client_lb_fd);

void* backEnd(void *args) {
	struct client_to_servers_args* s_args = (struct client_to_servers_args*) args;
	char* server_ip = s_args->serverIP;
	Servers serv = s_args->server;
	free(s_args);

	struct sockaddr_in serv_address;
	int client_lb_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_lb_fd < 0) {
		fprintf(stderr, "Socket creation error \n");
		pthread_exit(NULL);
	}

	serv_address.sin_family = AF_INET;
	inet_pton(AF_INET, server_ip, &(serv_address.sin_addr));
	serv_address.sin_port = htons(atoi(PORT));;
	if (connect(client_lb_fd, (struct sockaddr*)&serv_address, sizeof(serv_address)) < 0) {
		fprintf(stderr, "Connection Failed\n");
		close(client_lb_fd);
		pthread_exit(NULL);
	}

	Queue requests_queue = servers_queue[serv];

	while(1) {
		pthread_mutex_lock(&mutexes[serv]);
		if(!queueIsEmpty(requests_queue)) {
			Media request = queueTop(requests_queue);
			pthread_mutex_unlock(&mutexes[serv]);
			sendAndReceive(request,client_lb_fd);
			pthread_mutex_lock(&mutexes[serv]);
			queueRemove(requests_queue, request);
			pthread_mutex_unlock(&mutexes[serv]);
			free(request);
		} else {
			pthread_mutex_unlock(&mutexes[serv]);
		}
	}

	pthread_exit(NULL);
	return NULL;//just to have no annoying warnings
}

static void sendAndReceive(Media request, int client_lb_fd) {
	char send_data[SIZE_OF_MESSAGE];
	send_data[0] = request->type;
	send_data[1] = request->length;
	int original_client_sock = request->client_socket;

	int sent_bytes = 0;
	do {
		sent_bytes += send(client_lb_fd, send_data+sent_bytes,SIZE_OF_MESSAGE-sent_bytes, 0);
	} while(sent_bytes<SIZE_OF_MESSAGE);

	char response[SIZE_OF_MESSAGE];
	int received_len = recv(client_lb_fd,response,SIZE_OF_MESSAGE*sizeof(char),MSG_WAITALL);

	sent_bytes = 0;
	do {
		sent_bytes += send(original_client_sock, response+sent_bytes,received_len-sent_bytes, 0);
	} while(sent_bytes<received_len);

	close(original_client_sock);
}

#endif /* CLIENT_C_ */
