#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TIMEOUT 30
#define FPS 12

typedef struct{
	int high_score;
	int max_jump;
	int y_ground;
}Spiel;

typedef struct{
	int dy_solo;
	int hitted_max;
}Control_jump;

typedef struct{
	int is_down;
	int is_jumping;
	int lives;
	int height;
	Control_jump jump;	
}Trex;

//funcoes auxiliares SDL
void SDL_inicia();
SDL_Window* SDL_criaJanela(char titulo[]);
SDL_Renderer* SDL_criaRenderer(SDL_Window* win);

//utils
int aux_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms);
void init_game(Spiel *game, Trex *player);

int main (int argc, char* args[]) {
    /* INICIALIZACAO */
	SDL_inicia();
    SDL_Window* win = SDL_criaJanela("Dino Run");
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* trex_running = IMG_LoadTexture(ren, "../assets/images/up-trex-sprite.png");
    SDL_Texture* trex_down = IMG_LoadTexture(ren, "../assets/images/down-trex-sprite.png");
    assert(trex_running != NULL);
    assert(trex_down != NULL);

    Spiel game;
    Trex player;
    init_game(&game, &player);
    
    int quit = 0;
    Uint32 tempo_espera = TIMEOUT;
    int indice_crop = 0;
    
    Uint32 comeco = SDL_GetTicks();
    Uint32 anterior  = comeco;

    /* EXECUCAO */
    while(!quit){
    	SDL_Event evt;
    	int is_evt = aux_WaitEventTimeoutCount(&evt, &tempo_espera);
    	if(is_evt){
    		switch (evt.type){
    			case SDL_QUIT:
    				quit = 1;
    				break;
    			case SDL_KEYDOWN:
    				// if(evt.key.keysym.sym == SDLK_DOWN && !player.is_jumping && !player.is_down){
					// 	player.is_down = 1;
					// 	printf("Abaixou\n");
					// 	break;
					// }
					// if(evt.key.keysym.sym == SDLK_UP && player.is_down && !player.is_jumping){
					// 	player.is_down = 0;
					// 	printf("Levantou\n");
					// 	break;
					// }
					// if(evt.key.keysym.sym == SDLK_UP && !player.is_down && !player.is_jumping){
					// 	player.is_jumping = 1;
					// 	printf("Pulou\n");
					// 	break;
					// }
					switch (evt.key.keysym.sym){
						case SDLK_DOWN:
							if(!player.is_jumping && !player.is_down){
								player.is_down = 1;
								printf("Abaixou\n");
							}
							break;

						case SDLK_UP:
							
							break;
					}
    			case SDL_KEYUP:
    				if(evt.key.keysym.sym == SDLK_DOWN && !player.is_jumping && player.is_down){
						player.is_down = 0;
						printf("Levantou\n");
					}
    				break;
    		}
    	}else {
    		tempo_espera = TIMEOUT;
    	}
    	Uint32 agora = SDL_GetTicks();
    	SDL_SetRenderDrawColor(ren, 0x87, 0xCE, 0xEB, 1);
   		SDL_RenderClear(ren);
   		SDL_Rect rex_posicao;
    	SDL_Rect rex_crop; 
    	
    	//controla framerate do rex
    	if(!player.is_jumping){
    		if (agora-anterior>(1000/FPS)){
	    		indice_crop++;
	    		anterior = agora;
	    	}
	    	if(indice_crop==4){//probabilidade de piscar o olho
	    		srand(time(NULL));
	    		int num = rand() % 10; //entre 0 e 9
	    		if(num > 3) indice_crop = 5;
	    	}    		
    	}
    	if(indice_crop>7){
    		indice_crop = 0;
    	}

    	
    	if(player.is_down){
    		rex_posicao = (SDL_Rect){50, 536, player.height, player.height/2};
    		rex_crop = (SDL_Rect) {indice_crop*32, 0, 32, 16};
    		SDL_RenderCopy(ren, trex_down, &rex_crop, &rex_posicao);
    	}
    	if(!player.is_jumping && !player.is_down){
    		rex_posicao = (SDL_Rect){50, 472, player.height, player.height};
    		rex_crop = (SDL_Rect) {indice_crop*32, 0, 32, 32};
    		SDL_RenderCopy(ren, trex_running, &rex_crop, &rex_posicao);
    	}
    	if(player.is_jumping){
    		if(!player.jump.hitted_max){
    			if(player.jump.dy_solo>=game.max_jump){
    				player.jump.hitted_max = 1;
    			}else{
    				player.jump.dy_solo += 8;
    			}
    		}else{
    			if(player.jump.dy_solo==0){
    				player.jump.hitted_max = 0;
    				player.is_jumping = 0;
    			}else{
    				player.jump.dy_solo -= 8;
    			}
    		}
    		rex_posicao = (SDL_Rect){50, 472-player.jump.dy_solo, player.height, player.height};
    		rex_crop = (SDL_Rect) {indice_crop*32, 0, 32, 32};
    		SDL_RenderCopy(ren, trex_running, &rex_crop, &rex_posicao);
    	}
   		SDL_RenderPresent(ren);
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(trex_running);
    SDL_DestroyTexture(trex_down);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

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

void init_game(Spiel *game, Trex *player){
	player->lives = 3;
	player->is_jumping = 0;
	player->is_down = 0;
	player->height = 128;

	player->jump.dy_solo = 0;
	player->jump.hitted_max = 0;

	game->high_score = 0;
	game->max_jump = 128;
	game->y_ground = 600;
}
