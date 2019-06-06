#include "backEnd.h"

int main(void) {
	for(int i=0; i<NUM_OF_SERVERS; i++){
		// priority queue of requests for each server
		servers_queue[i] = queueCreate();
		// locks to handle reading and writing to the priority queues
		pthread_mutex_init(&mutexes[i], NULL);
	}


	// Open 3 connections with the 3 backstage servers (the backend)
	// connection to the 1st server
	pthread_t client_thread1;
	struct client_to_servers_args* s_args1 = malloc(sizeof(*s_args1));
	s_args1->server = Serv1;
	s_args1->serverIP = SERV1_IP;
	if (pthread_create(&client_thread1, NULL, backEnd, s_args1) != 0) {
		fprintf(stderr, "ERROR on client pthread");
		return -1;
	}
	// connection to the 2nd server
	pthread_t client_thread2;
	struct client_to_servers_args* s_args2 = malloc(sizeof(*s_args2));
	s_args2->server = Serv2;
	s_args2->serverIP = SERV2_IP;
	if (pthread_create(&client_thread2, NULL, backEnd, s_args2) != 0) {
		fprintf(stderr, "ERROR on client pthread");
		return -1;
	}
	// connection to the 3rd server
	pthread_t client_thread3;
	struct client_to_servers_args* s_args3 = malloc(sizeof(*s_args3));
	s_args3->server = Serv3;
	s_args3->serverIP = SERV3_IP;
	if (pthread_create(&client_thread3, NULL, backEnd, s_args3) != 0) {
		fprintf(stderr, "ERROR on client pthread");
		return -1;
	}

	// Open the server part of the LB (the frontend) - the one the real clients see
	pthread_t server_thread;
	if (pthread_create(&server_thread, NULL, frontEnd, NULL) != 0) {
		fprintf(stderr, "ERROR on server pthread");
		return -1;
	}

	// wait for something that will never accure actually...
	pthread_join(server_thread,NULL);
	pthread_join(client_thread1,NULL);
	pthread_join(client_thread2,NULL);
	pthread_join(client_thread3,NULL);

	// destroy all mutexes and queues
	for(int i=0; i<NUM_OF_SERVERS; i++){
		queueDestroy(servers_queue[i]);
		pthread_mutex_destroy(&mutexes[i]);
	}

	return 0;
}
