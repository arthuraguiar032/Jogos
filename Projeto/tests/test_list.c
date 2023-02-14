#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../headers/list.h"

/*Compilação:
gcc ../headers/queue.c test_queue.c -g -lSDL2 -o test
*/

int main(){
	List *lista = List_create();
	printf("A lista está vazia? %s\n", List_isEmpty(lista)?"sim":"nao");

	Obstacle obs;
	obs.type = TYPE_CACTUS;
	obs.position = (SDL_Point){700, 400};
	obs.width = 32;
	obs.height = 32;
	obs.quantity = 1;
	obs.step_index = 0;
	List_push(lista, obs);
	obs.type = TYPE_BIRD;
	List_push(lista, obs);
	// List_push(lista, 3, (SDL_Rect){32, 32, 32, 32});
	// List_push(lista, 4, (SDL_Rect){42, 42, 42, 42});
	// List_push(lista, 5, (SDL_Rect){52, 52, 52, 52});
	List_print(lista);
	printf("A lista está vazia? %s\n", List_isEmpty(lista)?"sim":"nao");


	//Node *aux;
	List_free(lista);

	//Queue_free(fila);
	List_print(lista);
}
