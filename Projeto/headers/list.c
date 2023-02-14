#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "list.h"

List* List_create(){
	List* list = malloc(sizeof(List));

	list->length = 0;
	list->first = NULL;
	list->last = NULL;

	return list;
}

bool List_isEmpty(List *list){
	return (list->length==0);
}

void List_push(List *list, Obstacle obs){
	Node *node = (Node*)malloc(sizeof(Node));
	
	node->data.type = obs.type;
	node->data.position = obs.position;
	node->data.width = obs.width;
	node->data.height = obs.height;
	node->data.quantity = obs.quantity;
	node->data.step_index = obs.step_index;

	node->next = NULL;
	if(List_isEmpty(list)){
		list->first = list->last = node;
	}else{
		list->last->next = (Node*)node;
		list->last = node;
	}
	list->length++;
}

Obstacle List_search(List *list, int index){
	// if(List_isEmpty(list)){
	// 	return NULL;
	// }
	// if(index>=list->length){
	// 	return NULL;
	// }

	Node *aux;
	aux = list->first;
	for(int i=0; i<index; i++){
		aux = aux->next;
	}
	return aux->data;
}

void List_delete(List *list, int index){
	if(List_isEmpty(list)){
		return;
	}
	if(index>=list->length){
		return;
	}
	if(index==0){
		List_pop(list);
	}

	Node *aux;
	aux = list->first;

	for(int i=0; i<index-1; i++){
		aux = aux->next;
	}

	Node *del;
	del = aux->next;
	aux->next = del->next;
	free(del);
	list->length--;
}

void List_pop(List *list){
	Node *aux;

	if (List_isEmpty(list)){
		return;
	}
	aux = list->first;
	list->first = list->first->next;
	list->length--;
	free(aux);
}

void List_free(List *lista){
	if(List_isEmpty(lista)){
		return;
	}
	while(!List_isEmpty(lista)){
		List_pop(lista);
	}
}

void List_print(List *list){
	if(List_isEmpty(list)){
		printf("The list is empty!\n");
		return;
	}
	printf("--- Printing list ---\n");
	Node *aux = list->first;
	while(aux){
		printf("Type: ");
		switch (aux->data.type){
			case TYPE_CACTUS:
				printf("Cactus. ");
				break;

			case TYPE_BIRD:
				printf("Bird. ");
				break;
		}
		printf("Position: (%d, %d).\n", aux->data.position.x, aux->data.position.y);
		aux = aux->next;
	}

}

