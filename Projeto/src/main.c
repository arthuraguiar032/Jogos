#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TIMEOUT 30
#define FPS 24

typedef enum{
	false = 0,
	true
}boolean;

//States Machine
typedef enum{
	STATE_MAIN_MENU,
	STATE_INSTRUCTIONS,
	STATE_SCORES,
	STATE_PLAY
}Game_State;

typedef enum{
	STATE_STANDING,
	STATE_DUCKING,
	STATE_JUMPING
}Player_State;

//Control structures 
typedef struct{
	Game_State state;
	int high_score;
	int max_jump;
	int y_ground;
	int current_score;
}Game;

typedef struct{
	int dy_solo;
	boolean hitted_max;
}Control_jump;

typedef struct{
	int index_crop;
	Uint32 previous_update;
}Control_Visual;

typedef struct{
	Player_State state;
	SDL_Point position;
	Control_jump jump;
	Control_Visual visual;
	int height;
}Player;

//funcoes auxiliares SDL
void SDL_init();
SDL_Window* SDL_aux_CreateWindow(char titulo[]);
SDL_Renderer* SDL_aux_createRenderer(SDL_Window* win);

//utils
int aux_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms);
void init_game(Game *game);
void init_player(Player *player);

//controladoras de comportamentos e visual
void player_standing(Player *player, SDL_Renderer *ren);
void player_ducking(Player *player, SDL_Renderer *ren);
void player_jumping(Player *player,  int max_jump, SDL_Renderer *ren);

SDL_Texture* trex_standing;
SDL_Texture* trex_ducking;

int main(int argc, char* args[]){
	//Inicialização
	SDL_init();
	SDL_Window *win = SDL_aux_CreateWindow("Dino Runner");
	SDL_Renderer *ren = SDL_aux_createRenderer(win);
	Game game;
	init_game(&game);
	Player player;
	init_player(&player);

	//carregando texturas
	trex_standing = IMG_LoadTexture(ren, "../assets/images/up-trex-sprite.png");
    trex_ducking = IMG_LoadTexture(ren, "../assets/images/down-trex-sprite.png");
    SDL_Texture* scenario = IMG_LoadTexture(ren, "../assets/images/scenario.png");
    assert(trex_standing != NULL);
    assert(trex_ducking != NULL);
    assert(scenario != NULL);

    //variaveis de controle
    int quit = false;
    //Control_Visual visual;
    //control_FR.previous_update = SDL_GetTicks();

	//Execução
    //loop principal
    while(!quit){
    	//capturando os eventos desejados
    	SDL_Event evt;
    	while( SDL_PollEvent( &evt ) != 0 ){
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
		}
		SDL_Rect scenario_position = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_RenderCopy(ren, scenario, NULL, &scenario_position);

		if(player.state == STATE_JUMPING){
			player_jumping(&player, game.max_jump, ren);
		}else if(player.state == STATE_STANDING){
			player_standing(&player, ren);
		}else if(player.state == STATE_DUCKING){
			player_ducking(&player, ren);
		}
		Uint32 current_update = SDL_GetTicks();
		if((current_update-player.visual.previous_update)>=1000/FPS && player.state != STATE_JUMPING){
			player.visual.previous_update = current_update;
			player.visual.index_crop = (player.visual.index_crop+1)%7;
		}

		SDL_RenderPresent(ren);
    }

	//Finalização
	SDL_DestroyTexture(trex_standing);
    SDL_DestroyTexture(trex_ducking);
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

SDL_Window* SDL_aux_CreateWindow(char titulo[]){
	SDL_Window* win = SDL_CreateWindow(titulo,
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
                      );
	if (win==NULL)
		printf("Erro na criação da Janela.\nErro SDL: %s", SDL_GetError());
	return win;
}

SDL_Renderer* SDL_aux_createRenderer(SDL_Window* win){
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
	game->high_score = 0;
	game->max_jump = 128;
	game->y_ground = 600;

	game->state = STATE_PLAY;
}

void init_player(Player *player){
	player->state = STATE_STANDING;
	player->position.x = 50;
	player->position.y = 472;

	player->jump.dy_solo = 0;
	player->jump.hitted_max = false;

	player->height = 128;

	player->visual.index_crop = 0;
	player->visual.previous_update = SDL_GetTicks();
}

void player_standing(Player *player, SDL_Renderer *ren){
	SDL_Rect standing_position = {50, 472, player->height, player->height};
	player->position = (SDL_Point){50, 472};
	player->jump.dy_solo = 0;
	player->jump.hitted_max = false;

	SDL_Rect rex_crop = {player->visual.index_crop*32, 0, 32, 32};
	SDL_RenderCopy(ren , trex_standing, &rex_crop, &standing_position);
}

void player_ducking(Player *player, SDL_Renderer *ren){
	SDL_Rect ducking_position = {50, 528, player->height, player->height/2};

	SDL_Rect rex_crop = {player->visual.index_crop*32, 0, 32, 16};
	SDL_RenderCopy(ren , trex_ducking, &rex_crop, &ducking_position);
}

void player_jumping(Player *player, int max_jump, SDL_Renderer *ren){
	if(player->jump.hitted_max){
		if(player->jump.dy_solo<=0){
			player->state = STATE_STANDING;
		}else{
			//int aux = player.position.y*1,2;
			//player.position.y = aux;
			player->jump.dy_solo -= 8;
		}
	}else{
		if(player->jump.dy_solo>=max_jump){
			player->jump.hitted_max=true;
		}else{
			player->jump.dy_solo += 6;
		}
	}
	SDL_Rect rex_posicao = {50, 472-player->jump.dy_solo, player->height, player->height};
    SDL_Rect rex_crop = {player->visual.index_crop*32, 0, 32, 32};
    SDL_RenderCopy(ren, trex_standing, &rex_crop, &rex_posicao);
}
