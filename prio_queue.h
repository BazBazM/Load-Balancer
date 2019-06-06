#ifndef PRIO_QUEUE_H_
#define PRIO_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Queue_t* Queue;

typedef struct Media_t {
	char type; // 'M' for music, 'V' for video, 'P' for pictures
	char length; // number 0-9, as the time it supposed to take to process this media normally.
	int cost; // the time it will actually take to process this media - depends on the server
	int client_socket; // the socket, that we got this media request from, in the first place
	double priority; // The priority of this request. It should start with the value of cost and decreases to prevent starvation
}* Media;


// Allocates memory for a new queue and returns it
Queue queueCreate();

// Returns the number of elements in the queue
int queueGetSize(Queue queue);

// Returns the total time it will take to process the entire queue by the server.
int queueGetTotalCost(Queue queue);

// Returns the first element in the queue without removin it
Media queueTop(Queue queue);

// Removes the first element in the queue and returns it
Media queuePop(Queue queue);

// Removes @param "to_remove" from the queue
void queueRemove(Queue queue, Media to_remove);

// Inserts new element to the queue, keeping the queue sorted - head is the shortest one to process by server
void queuePush(Queue queue, Media data);

// Returns true if there are no elements in the queue.
bool queueIsEmpty(Queue queue);

// Frees all elements' memory (but not the data) and the queue's memory.
void queueDestroy(Queue queue);

// For debugging purposes. Prints all the elements in the queue, from the head to the tail.
void printQueue(Queue queue);


#endif /* PRIO_QUEUE_H_ */
