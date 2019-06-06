#ifndef PRIOQUEUE_C_
#define PRIOQUEUE_C_

#include "prio_queue.h"

typedef struct Node_t {
	Media data;
	struct Node_t* prev;
	struct Node_t* next;
}* Node;

typedef struct Queue_t {
	Node head;
	Node tail;
	int size;
	int total_cost;
}* Queue;

Queue queueCreate() {
	Queue q = malloc(sizeof(*q));
	q->head=NULL;
	q->tail=NULL;
	q->size=0;
	q->total_cost=0;
	return q;
}

int queueGetSize(Queue queue) {
	return queue->size;
}

int queueGetTotalCost(Queue queue) {
	return queue->total_cost;
}

Media queueTop(Queue queue) {
	if(!queueIsEmpty(queue))
		return queue->head->data;
	return NULL;
}

Media queuePop(Queue queue) {
	Media data = NULL;
	if(queue->size > 0) {
		Node old_head = queue->head;
		if(queue->size == 1) queue->tail = NULL;
		queue->head = queue->head->next;
		data = old_head->data;
		free(old_head);
		queue->total_cost-= data->cost;
		queue->size--;
		Node temp = queue->head;
		//update the priority of all elements. It doesn't change the sorting, because it's linear change.
		while(temp) {
			temp->data->priority -= 0.5*data->cost;
			temp = temp->next;
		}
	}
	return data;
}

void queueRemove(Queue queue, Media to_remove) {
	if(queue->head->data == to_remove) {
		queuePop(queue);
		return;
	}
	Node temp = queue->head->next;
	while(temp) {
		if(temp->data == to_remove) {
			Node t_prev = temp->prev;
			Node t_next = temp->next;
			t_prev->next = temp->next;
			if(temp == queue->tail) {
				queue->tail = t_prev;
			} else {
				t_next->prev = t_prev;
			}
			queue->total_cost-= to_remove->cost;
			free(temp);
			queue->size--;
			temp = queue->head;
			while(temp) {
				temp->data->priority -= 0.5*to_remove->cost;
				temp = temp->next;
			}
			return;
		}
		temp = temp->next;
	}
}

void queuePush(Queue queue, Media data) {
	Node new_element = malloc(sizeof(*new_element));
	new_element->data = data;
	new_element->next = NULL;
	new_element->prev = NULL;
	if(queueIsEmpty(queue)) {
		queue->head = new_element;
		queue->tail = new_element;
	} else {
		Node temp = queue->head;
		while(temp && (temp->data->priority < data->priority))
			temp = temp->next;
		if(!temp) {
			queue->tail->next = new_element;
			new_element->prev = queue->tail;
			queue->tail = new_element;
		} else {
			if(temp != queue->head) {
				Node before = temp->prev;
				before->next = new_element;
				new_element->prev = before;
			}
			temp->prev = new_element;
			new_element->next = temp;
			if(temp == queue->head) {
				queue->head = new_element;
			}
		}
	}
	queue->size++;
	queue->total_cost+=data->cost;
}

bool queueIsEmpty(Queue queue) {
	return queue->size==0;
}

void printQueue(Queue queue) {
	Node temp = queue->head;
	Media data = temp->data;
	while(temp) {
		data = temp->data;
		printf("(%c%c|%d)", data->type,data->length,data->cost);
		temp = temp->next;
		if(temp) printf(", ");
		else printf(".");
	}
	printf("\n");
}

void queueDestroy(Queue queue) {
	while(!queueIsEmpty(queue)) {
		free(queuePop(queue));
	}
	free(queue);
}

#endif /* PRIOQUEUE_C_ */
