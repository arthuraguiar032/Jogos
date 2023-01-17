#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "queue.h"

Queue* Queue_create(){
	Queue* queue = malloc(sizeof(Queue));

	queue->length = 0;
	queue->first = NULL;
	queue->last = NULL;

	return queue;
}

bool Queue_isEmpty(Queue *queue){
	return (queue->length==0);
}

void Queue_insert(Queue *queue, int vel, SDL_Rect pos){
	Node *node = (Node*)malloc(sizeof(Node));
	node->velocity = vel;
	node->position = (SDL_Rect) pos;
	node->next = NULL;
	if(Queue_isEmpty(queue)){
		queue->first = queue->last = node;
	}else{
		queue->last->next = (Node*)node;
		queue->last = node;
	}
	queue->length++;
}

Node* Queue_pop(Queue *queue){
	Node *aux;

	if (Queue_isEmpty(queue)){
		return NULL;
	}
	aux = queue->first;
	queue->first = aux->next;
	queue->length--;
	return aux;
}

void Queue_print(Queue *queue){
	if(Queue_isEmpty(queue)){
		printf("The Queue is empty. There's nothing to print.");
		return;
	}
	Node *aux = queue->first;
	printf("\n--- Printing Queue ---\n");
	while(aux!=NULL){
		printf("Position (x, y, w, h): (%d, %d, %d, %d), ", aux->position.x, aux->position.y, aux->position.w, aux->position.h);
		printf("Velocity: %d\n", aux->velocity);
		aux = aux->next;
	}
}

void Queue_free(Queue *queue){
	if(Queue_isEmpty(queue)){
		free(queue);
		return;
	}

	Node *aux;
	while(!Queue_isEmpty(queue)){
		aux = Queue_pop(queue);
		free(aux);
	}
}