#include <SDL2/SDL.h>
	
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 600
#define WINDOW_POS_X  SDL_WINDOWPOS_UNDEFINED
#define WINDOW_POS_Y  SDL_WINDOWPOS_UNDEFINED
#define TIMEOUT 15
	
typedef struct{
	SDL_Rect fisico;
	SDL_Color cor;
}Rect;
	
void SDL_inicia(){
	if (SDL_Init(SDL_INIT_EVERYTHING))
		printf("Erro de inicialização no SDL.\nErro SDL: %s", SDL_GetError());
	return;
}

SDL_Window* SDL_criaJanela(char titulo[]){
	SDL_Window* win = SDL_CreateWindow(titulo,
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
                      );
	if (win==NULL)
		printf("Erro na criação da Janela.\nErro SDL: %s", SDL_GetError());
	return win;
}

SDL_Renderer* SDL_criaRenderer(SDL_Window* win){
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	if (ren==NULL)
		printf("Erro na criação do Renderizador.\nErro SDL: %s", SDL_GetError());
	return ren;
}

int aux_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms){
	int antes = SDL_GetTicks();
	int is_evt = SDL_WaitEventTimeout(evt, *ms);
	int d_tempo = SDL_GetTicks() - antes;
	if (is_evt){
		//protecao para caso haja algum ruido na contagem de tempo
		if(d_tempo>*ms){
			d_tempo = 0;
		}
		*ms -= d_tempo;
	}	
	return is_evt;
}
	
int main (int argc, char* args[]) {
    /* INICIALIZACAO */
    SDL_inicia();
    SDL_Window* win = SDL_criaJanela("2.X: Drag and Drop");
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    
    /* EXECUÇÃO */
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 1);
	SDL_RenderClear(ren);
	Rect quadrado;
	int tam_quad = 50;
	quadrado.fisico = (SDL_Rect){SCREEN_WIDTH/2 - tam_quad/2, SCREEN_HEIGHT/2 - tam_quad/2, tam_quad, tam_quad};
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 1);
	SDL_RenderFillRect(ren, &quadrado.fisico);
	SDL_RenderPresent(ren);
	
	int arrastado = 0;
	int selecionado = 0;
	SDL_Point pos_origem;
	SDL_Point drag_origem;
	pos_origem = (SDL_Point){};

	int quit=0;
	while(!quit){
		int dx, dy;
		int x,y;
		SDL_Event evt;
		Uint32 tempo_espera = TIMEOUT;
		int isevt = aux_WaitEventTimeoutCount(&evt, &tempo_espera);
		if(isevt){
			switch(evt.type){
          	 	case SDL_QUIT:
					quit=1;
					break;
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&x, &y);
					SDL_Point pos_mouse = {x, y};
					if(evt.button.button == SDL_BUTTON_LEFT && SDL_PointInRect(&pos_mouse, &quadrado.fisico)){
							drag_origem = (SDL_Point){x, y};
							dx = quadrado.fisico.x - x;
							dy = quadrado.fisico.y - y;
							selecionado = 1;
							arrastado = 0;
					}
					break;
				case SDL_MOUSEMOTION:
					if(selecionado==1){
						if(!arrastado){
							printf("Dragging...\n");
						}
						arrastado = 1;
						SDL_GetMouseState(&x, &y);
						quadrado.fisico.x = x+dx;
						quadrado.fisico.y = y+dy;
					}					
					break;
				case SDL_MOUSEBUTTONUP:
					if(selecionado){
						if(arrastado==1){
							printf("Dropped\n");
						}else {
							printf("Clicked\n");
						}
					}
					selecionado=arrastado= 0;
					break;
				case SDL_KEYDOWN:
					if(evt.key.keysym.sym == SDLK_ESCAPE && arrastado){
						arrastado = selecionado = 0;
						printf("Cancelled!\n");
						
					}
					break;
			}
		}else{
			tempo_espera = TIMEOUT;
		}
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 1);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 1);
		SDL_RenderFillRect(ren, &quadrado.fisico);
		SDL_RenderPresent(ren);
	}
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
