#include <SDL2/SDL.h>
#include <time.h>
	
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 600
#define WINDOW_POS_X  SDL_WINDOWPOS_UNDEFINED
#define WINDOW_POS_Y  SDL_WINDOWPOS_UNDEFINED
#define TIMEOUT 15
	
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

int aux_pointInRect(SDL_Point point, SDL_Rect rect){
	if(point.x>=rect.x && point.x<=rect.x+rect.w && point.y>=rect.y && point.y<=rect.y+rect.h){
		return 1;
	}
	return 0;
}

void atualizaTela(SDL_Rect* quadrado,SDL_Renderer *ren){
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 1);
	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren, 0, 0, 255, 1);
	SDL_RenderFillRect(ren, quadrado);
	SDL_RenderPresent(ren);
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
	int tam_quad = 50;		
	SDL_Rect quadrado = {SCREEN_WIDTH/2 - tam_quad/2, SCREEN_HEIGHT/2 - tam_quad/2, tam_quad, tam_quad};
	atualizaTela(&quadrado, ren);
	int arrastado = 0;
	int selecionado = 0;
	SDL_Point drag_origem;

	int quit=0;
	while(!quit){
		int dx, dy;
		int x,y;
		int inRect =0;
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
					//SDL_PointInRect(&pos_mouse, &quadrado) - nao tem no lab
					if(evt.button.button == SDL_BUTTON_LEFT && aux_pointInRect(pos_mouse, quadrado)){
							drag_origem = (SDL_Point){x, y};
							dx = quadrado.x - x;
							dy = quadrado.y - y;
							drag_origem = (SDL_Point){quadrado.x, quadrado.y};
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
						quadrado.x = x+dx;
						quadrado.y = y+dy;
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
					if(evt.key.keysym.sym == SDLK_ESCAPE && selecionado){
						arrastado = selecionado = 0;
						printf("Cancelled!\n");
						quadrado.x = drag_origem.x;
						quadrado.y = drag_origem.y;
					}
					break;
			}
		}else{
			tempo_espera = TIMEOUT;
		}
		atualizaTela(&quadrado, ren);
	}
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
