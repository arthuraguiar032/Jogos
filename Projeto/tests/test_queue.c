#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../headers/queue.h"

/*Compilação:
gcc ../headers/queue.c test_queue.c -g -lSDL2 -o test
*/

int main(){
	Queue *fila = Queue_create();
	printf("A fila está vazia? %s\n", Queue_isEmpty(fila)?"sim":"nao");

	Queue_insert(fila, 2, (SDL_Rect){22, 22, 22, 22});
	Queue_insert(fila, 3, (SDL_Rect){32, 32, 32, 32});
	Queue_insert(fila, 4, (SDL_Rect){42, 42, 42, 42});
	Queue_insert(fila, 5, (SDL_Rect){52, 52, 52, 52});
	Queue_print(fila);

	Node *aux;
	while(!Queue_isEmpty(fila)){
		aux = Queue_pop(fila);
		aux->next = NULL;
		free(aux);
	}

	//Queue_free(fila);
	Queue_print(fila);
}
