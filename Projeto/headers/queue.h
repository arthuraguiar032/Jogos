#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdbool.h>
#include <SDL2/SDL.h>

struct Node{
	int velocity;
	SDL_Rect position;
	struct Node* next;
};

typedef struct Node Node;

typedef struct{
	int length;
	Node* first;
	Node* last;
}Queue;

Queue* Queue_create();

bool Queue_isEmpty(Queue *queue);

void Queue_insert(Queue *queue, int vel, SDL_Rect pos);

Node* Queue_pop(Queue *queue);

void Queue_print(Queue *queue);

void Queue_free(Queue *queue);

#endif
