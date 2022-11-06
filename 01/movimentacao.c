#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>


const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;


//retorna true se tiver o vetor em alguma linha da matriz
int verifica_repeticao_linha(int matriz[10][3], int vetor[3], int indice_linha){
	int e_igual = 0;
	for (int i =0; i<indice_linha; i++){
		if (matriz[i][0] == vetor[0] && matriz[i][1] == vetor[1] && matriz[i][2] == vetor[2]){
			e_igual = 1;
		}
	}
	return e_igual;	
}

//preenche vetor de inteiros de tamanho n com numeros aleatorios
void vetor_numeros_aleatorios(int *vetor, int n){
        for (int i =0; i<n; i++){
        	vetor[i] = rand() % 255;
        }
}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    int lado_quad = 20;
    
    SDL_Rect r = { 50, 50, lado_quad, lado_quad};
    SDL_Event evt;    
    int quit = 0;
    
    int qtd_max_quad = 10;
    SDL_Rect r_fixos[10];
    int qtd_quad_atual =0;
    int cores_aleatorias[10][3];
    
    while (!quit) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        
        for (int i =0; i<qtd_quad_atual;i++){
        	SDL_SetRenderDrawColor(ren, cores_aleatorias[i][0],cores_aleatorias[i][1],cores_aleatorias[i][2],0x00);
        	SDL_RenderFillRect(ren, &r_fixos[i]);
        }
        
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);

        SDL_WaitEvent(&evt);
        if (evt.type == SDL_KEYDOWN) {
            int qtd_movimento = 2;
            int mov_futuro = 0;
            switch (evt.key.keysym.sym) {
            	case SDLK_UP:
            		mov_futuro = r.y - qtd_movimento;
                	if (mov_futuro >=0)
                		r.y = mov_futuro;
                    	break;
                case SDLK_DOWN:
                    	mov_futuro = r.y + qtd_movimento;
                    	if(mov_futuro<=SCREEN_HEIGHT-lado_quad)
                		r.y = mov_futuro;
                    	break;
                case SDLK_LEFT:
                    	mov_futuro = r.x - qtd_movimento;
                    	if(mov_futuro>=0)
                    		r.x = mov_futuro;
                    	break;
                case SDLK_RIGHT:
                	mov_futuro = r.x + qtd_movimento;
                	if(mov_futuro<=SCREEN_WIDTH - lado_quad)
                    		r.x = mov_futuro;
                    	break;
            }
        }
        if (evt.type == SDL_QUIT){
        	quit = 1;
        }
        if(evt.type == SDL_MOUSEBUTTONDOWN && qtd_quad_atual<qtd_max_quad){
        	int x,y;
        	SDL_GetMouseState(&x, &y);
        	
        	r_fixos[qtd_quad_atual].x = x;
        	r_fixos[qtd_quad_atual].y = y;
        	r_fixos[qtd_quad_atual].w= lado_quad;
        	r_fixos[qtd_quad_atual].h= lado_quad;
        	
        	//cores diferentes sempre
        	srand(time(NULL));
        	int *num_aleatorios;
        	num_aleatorios = (int *) malloc(3* sizeof(int));
        	vetor_numeros_aleatorios(num_aleatorios, 3);
        	
        	while(verifica_repeticao_linha(cores_aleatorias, num_aleatorios, qtd_quad_atual)){
        		vetor_numeros_aleatorios(num_aleatorios, 3);
        	}
        	
        	for(int i =0; i<3; i++){
        		cores_aleatorias[qtd_quad_atual][i] = num_aleatorios[i];
        	}
        	free(num_aleatorios);
        	qtd_quad_atual+=1;       	
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
