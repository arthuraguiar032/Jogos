#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>
#include <SDL2/SDL.h>


typedef enum{
	TYPE_CACTUS,
	TYPE_BIRD
}Obstacle_Type;

typedef struct{
	Obstacle_Type type;
	SDL_Point position;
	int width;
	int height;
	int quantity;
	int step_index;
}Obstacle;

struct Node{
	Obstacle data;
	struct Node* next;
};

typedef struct Node Node;

typedef struct{
	int length;
	Node* first;
	Node* last;
}List;

List* List_create();

bool List_isEmpty(List *list);

void List_push(List *list, Obstacle obs);

Obstacle List_search(List *list, int index);

void List_delete(List *list, int index);

void List_pop(List *list);

void List_free(List *list);

void List_print(List *list);

#endif