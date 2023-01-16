#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct{
	int velocity;
	SDL_Rect position;
	struct Node* next;
}Node;

typedef struct{
	int length;
	Node* first;
	Node* last;
}Queue;

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
		queue->last->next = node;
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
	queue->first = queue->first->next;
	fila->length--;
	return aux;
}

void Queue_print(Queue *queue){
	if(Queue_isEmpty(queue))
		return;
	Node *aux = queue->first;
	printf("\n--- Printing Queue ---\n");
	while(aux!=NULL){
		printf("Position (x, y, w, h): (%d, %d, %d, %d), ", aux->position.x, aux->position.y, aux->position.w, h);
		printf("Velocity: %d\n", aux->velocity);
		aux = aux->next;
	}
}