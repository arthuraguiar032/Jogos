#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TIMEOUT 10
#define FPS 24
#define JUMP_VEL 8.5

/*Compilação:
gcc ../headers/queue.c test_queue.c -g -lSDL2 -o test
*/


//States Machine
typedef enum{
	STATE_MAIN_MENU,
	STATE_INSTRUCTIONS,
	STATE_SCORES,
	STATE_PLAY,
	STATE_PAUSE
}Game_State;

typedef enum{
	STATE_STANDING,
	STATE_DUCKING,
	STATE_JUMPING
}Player_State;


//Control structures 
typedef struct{
	Game_State state;
	int speed;
	Uint32 previous_update;
}Game;


typedef struct{
	Player_State state;
	SDL_Point position;
	SDL_Rect sprites[2][7];
	int size;
	float jump_vel;
	int step_index;
}Player;

//funcoes auxiliares SDL
void SDL_init();
SDL_Window* aux_SDLCreateWindow(char titulo[]);
SDL_Renderer* aux_SDLCreateRenderer(SDL_Window* win);
int aux_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms);

//utils
void init_game(Game *game);
void init_player(Player *player);

//controladoras de comportamentos e visual
void player_standing(Player *player);
void player_ducking(Player *player);
void player_jumping(Player *player);
void player_eyeBlinkControl(Player *player);
void player_update(Player *player);
void player_draw(Player *player, SDL_Renderer *ren);

SDL_Texture* dino_running;
SDL_Texture* dino_dead;

int main(int argc, char* args[]){
	SDL_init();
	SDL_Window *win = aux_SDLCreateWindow("Dino Runner");
	SDL_Renderer *ren = aux_SDLCreateRenderer(win);
	Game game;
	init_game(&game);
	Player player;
	init_player(&player);

	//carregando texturas
	dino_running = IMG_LoadTexture(ren, "../assets/images/trex.png");
	dino_dead = IMG_LoadTexture(ren, "../assets/images/dead-trex.png");
    SDL_Texture* scenario = IMG_LoadTexture(ren, "../assets/images/scenario.png");
    assert(dino_running != NULL);
    assert(dino_dead != NULL);
    assert(scenario != NULL);

    //variaveis de controle
    bool quit = false;
    Uint32 tempo_espera = TIMEOUT;

    while(!quit){
    	SDL_Event evt;
    	int is_evt = aux_WaitEventTimeoutCount(&evt, &tempo_espera);
    	if(is_evt){
    		switch(evt.type){
				case SDL_QUIT:
					quit = true;
					break;

				case SDL_KEYDOWN:
					if(evt.key.keysym.sym == SDLK_DOWN && player.state == STATE_STANDING){
						player.state = STATE_DUCKING;
					}else if(evt.key.keysym.sym == SDLK_UP && player.state == STATE_DUCKING){
						player.state = STATE_STANDING;
					}else if(evt.key.keysym.sym == SDLK_UP && player.state == STATE_STANDING){
						player.state = STATE_JUMPING;
					}
					break;

				case SDL_KEYUP:
					if(evt.key.keysym.sym == SDLK_DOWN && player.state == STATE_DUCKING){
						player.state = STATE_STANDING;
					}
					break;
			}
    	}else{
    		if(tempo_espera<=0){
    			tempo_espera = TIMEOUT;
    		}
    	}

    	SDL_Rect scenario_position = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_RenderCopy(ren, scenario, NULL, &scenario_position);

		Uint32 current_update = SDL_GetTicks();
		if((current_update-game.previous_update)>=1000/FPS){
			game.previous_update = current_update;
			player_update(&player);
		}
		player_draw(&player, ren);

		SDL_RenderPresent(ren);
    }

    //Finalizacao
    SDL_DestroyTexture(dino_running);
    SDL_DestroyTexture(dino_dead);
    SDL_DestroyTexture(scenario);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
	return 0;
}

void SDL_init(){
	if (SDL_Init(SDL_INIT_EVERYTHING))
		printf("Erro de inicialização no SDL.\nErro SDL: %s", SDL_GetError());
	return;
}

SDL_Window* aux_SDLCreateWindow(char titulo[]){
	SDL_Window* win = SDL_CreateWindow(titulo,
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
                      );
	if (win==NULL)
		printf("Erro na criação da Janela.\nErro SDL: %s", SDL_GetError());
	return win;
}

SDL_Renderer* aux_SDLCreateRenderer(SDL_Window* win){
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

void init_game(Game *game){
	game->state = STATE_PLAY;
	game->speed = 20;
	game->previous_update = SDL_GetTicks();
}

void init_player(Player *player){
	player->state = STATE_STANDING;
	player->position.x = 50;
	player->position.y = 472;
	player->size = 128;
	player->jump_vel = JUMP_VEL;
	player->step_index = 0;

	//set os retangulos de crop da animacao 
	for(int i=0; i<7; i++){
		//running
		player->sprites[0][i] = (SDL_Rect){i*32, 0, 32, 32};
		//ducking
		player->sprites[1][i] = (SDL_Rect){i*32, 48, 32, 16};
	}
}

void player_standing(Player *player){
	player->position = (SDL_Point){50, 472};
	player->step_index += 1;
	player_eyeBlinkControl(player);
}

void player_ducking(Player *player){
	player->position = (SDL_Point){50, 528};
	player->step_index += 1;
	player_eyeBlinkControl(player);
}

void player_jumping(Player *player){
	//testar uma nova formula
	player->position.y -= player->jump_vel*4;
	player->jump_vel -= 0.8;
	if(player->jump_vel< -JUMP_VEL){
		player->state = STATE_STANDING;
		player->jump_vel = JUMP_VEL;
	}
}

void player_eyeBlinkControl(Player *player){
	if(player->step_index==4){
		srand(time(NULL));
		//dinossauro vai piscar uma a cada 10 vezes
		int randint = rand() % 11;
		if(randint==10){
			return;
		}else{
			player->step_index+=1;
		}
	}
}

void player_update(Player *player){
	if(player->state == STATE_JUMPING){
		player_jumping(player);
	}else if(player->state == STATE_STANDING){
		player_standing(player);
	}else if(player->state == STATE_DUCKING){
		player_ducking(player);
	}
	if(player->step_index>=7){
		player->step_index = 0;
	}
}

void player_draw(Player *player, SDL_Renderer *ren){
	SDL_Rect position, crop;
	if(player->state == STATE_DUCKING){
		position = (SDL_Rect){player->position.x, player->position.y, player->size, player->size/2};
		crop = player->sprites[1][player->step_index];
	}else{
		position = (SDL_Rect){player->position.x, player->position.y, player->size, player->size};
		crop = player->sprites[0][player->step_index];
	}
	SDL_RenderCopy(ren, dino_running, &crop, &position);
}
