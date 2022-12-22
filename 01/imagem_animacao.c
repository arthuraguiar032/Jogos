#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540
#define TIMEOUT 35

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
    SDL_Window* win = SDL_criaJanela("1.7: Imagem animada");
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "sprite-sheet-bird.png");
    assert(img != NULL);

    /* EXECUCAO */
    int quit = 0;
    Uint32 tempo_espera = TIMEOUT;
    int indice_crop = 0;
    SDL_Rect r = {-100, SCREEN_HEIGHT/2 - 90, 90, 130};
    SDL_Rect c;
    while(!quit){
    	 SDL_Event evt;
    	int is_evt = aux_WaitEventTimeoutCount(&evt, &tempo_espera);
    	if(is_evt && evt.type==SDL_QUIT){
    		quit=1;
    	}else {
    		tempo_espera = TIMEOUT;
    		c = (SDL_Rect) {indice_crop*90, 0, 90, 130};
    		if(r.x > SCREEN_WIDTH+10){
    			r.x = -100;
    		}else{
    			r.x += 4;
    		}
    		
    		indice_crop++;
    		if(indice_crop>7){
    			indice_crop = 0;
    		}

    		SDL_SetRenderDrawColor(ren, 0x87, 0xCE, 0xEB, 1);
    		SDL_RenderClear(ren);
    		SDL_RenderCopy(ren, img, &c, &r);
    		SDL_RenderPresent(ren);
    	}
    }
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
