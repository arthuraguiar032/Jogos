#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include "../headers/list.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TIMEOUT 10
#define FPS 30
#define JUMP_VEL 8.5
#define TIME_ON_DEAD 1500

/*Compilação:
gcc main.c ../headers/list.c -g -lSDL2 -lSDL2_image -lSDL2_ttf -o main
gcc main2.c -g -lSDL2 -lSDL2_image -lSDL2_ttf -o main2
*/

//States Machine
typedef enum{
	STATE_MAIN_MENU,
	STATE_PLAY,
	STATE_PAUSE,
	STATE_GAME_OVER
}Game_State;

typedef enum{
	STATE_STANDING,
	STATE_DUCKING,
	STATE_JUMPING,
	STATE_DEAD
}Player_State;

//Control structures 
typedef struct{
	Game_State state;
	Uint32 previous_update;
	float speed;
	int x_pos_backgroung;
	int x_pos_track;
	int points;
}Game;

typedef struct{
	Player_State state;
	SDL_Point position;
	SDL_Rect sprites[2][7];
	int size;
	float jump_vel;
	int step_index;
}Player;

typedef struct{
	SDL_Point position;
	int width;
	int height;
	float speed;
}Cloud;

SDL_Texture* dino_running;
SDL_Texture* dino_dead;
SDL_Texture* scenario;
SDL_Texture* cloud_img;
SDL_Texture* cactus_img;
SDL_Texture* track_img;
SDL_Texture* bird;
SDL_Texture* txt;

//funcoes auxiliares SDL
void SDL_init();
SDL_Window* aux_SDLCreateWindow(char titulo[]);
SDL_Renderer* aux_SDLCreateRenderer(SDL_Window* win);
int aux_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms);
TTF_Font* aux_OpenFont(int size);

//game functions
void init_game(Game *game);
void load_textures(SDL_Renderer *ren);
void background(Game *game, SDL_Renderer *ren);
void background_draw(Game *game, SDL_Renderer *ren);
void score(Game *game, SDL_Renderer *ren, TTF_Font* fnt);

//player functions
void init_player(Player *player);
void player_standing(Player *player);
void player_ducking(Player *player);
void player_jumping(Player *player);
void player_eyeBlinkControl(Player *player);
void player_update(Player *player);
void player_draw(Player *player, SDL_Renderer *ren);

//cloud functions
void init_cloud(Cloud *cloud);
void cloud_update(Cloud *cloud, float game_speed);
void cloud_draw(Cloud *cloud, SDL_Renderer *ren);

//obstacle functions
void init_obstacle(Obstacle *obs);
void obstacle_update(List *obs, float game_speed);
void obstacle_draw(Obstacle *obs, SDL_Renderer *ren);
//int verify_colision(Player *player, Cactus *cactus);

int main(int argc, char* args[]){
	SDL_init();
	SDL_Window *win = aux_SDLCreateWindow("Dino Runner");
	SDL_Renderer *ren = aux_SDLCreateRenderer(win);
	
	TTF_Init();
	TTF_Font* fnt;
	fnt = aux_OpenFont(20);

	Game game;
	init_game(&game);
	load_textures(ren);

	Player player;
	init_player(&player);
	Cloud cloud;
	init_cloud(&cloud);

	List *obstacles = List_create();

    //variaveis de controle
    bool quit = false;
    Uint32 time_dead = 0;
    Uint32 tempo_espera = TIMEOUT;
	
	// SDL_Rect scenario_position = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	// SDL_RenderCopy(ren, scenario, NULL, &scenario_position);

    while(!quit){
    	SDL_Event evt;
    	int is_evt = aux_WaitEventTimeoutCount(&evt, &tempo_espera);
    	if(is_evt){
    		switch(evt.type){
				case SDL_QUIT:
					quit = true;
					break;

				case SDL_KEYDOWN:
					if(game.state == STATE_PLAY){
						if(evt.key.keysym.sym == SDLK_DOWN && player.state == STATE_STANDING){
							player.state = STATE_DUCKING;
						}else if(evt.key.keysym.sym == SDLK_UP && player.state == STATE_DUCKING){
							player.state = STATE_STANDING;
						}else if(evt.key.keysym.sym == SDLK_UP && player.state == STATE_STANDING){
							player.state = STATE_JUMPING;
						}
					}else if(game.state==STATE_MAIN_MENU || game.state==STATE_GAME_OVER){
						init_game(&game);
						game.state = STATE_PLAY;
						init_player(&player);
						init_cloud(&cloud);
						List_free(obstacles);
					}
					break;

				case SDL_KEYUP:
					if(game.state = STATE_PLAY){
						if(evt.key.keysym.sym == SDLK_DOWN && player.state == STATE_DUCKING){
							player.state = STATE_STANDING;
						}
					}
					break;
			}
    	}else{
    		if(tempo_espera<=0){
    			tempo_espera = TIMEOUT;
    		}
    	}
    	Uint32 current_update = SDL_GetTicks();

    	if(game.state==STATE_PLAY){
			if((current_update-game.previous_update)>=1000/FPS && player.state!=STATE_DEAD){
				game.previous_update = current_update;
				
				player_update(&player);
				cloud_update(&cloud, game.speed*0.5);

				//desenhando cenario
				background(&game, ren);
				cloud_draw(&cloud, ren);
				score(&game, ren, fnt);
				player_draw(&player, ren);

				if(obstacles->length==0){
					Obstacle obs;
					init_obstacle(&obs);
					List_push(obstacles, obs);
				}
				for(int i=0; i<obstacles->length; i++){
					Obstacle aux;
					aux = List_search(obstacles, i);

					if(aux.position.x <= -aux.width*aux.quantity){
						List_pop(obstacles);
					}else{
						obstacle_update(obstacles, game.speed);
						obstacle_draw(&aux, ren);
					}

					int aux_height = 128;
					if(player.state == STATE_DUCKING){
						aux_height = 64;
					} 

					SDL_Rect playerpos = {player.position.x, player.position.y, player.size, aux_height};
					SDL_Rect auxpos = {aux.position.x, aux.position.y, aux.width*aux.quantity-10, aux.height};
					if(SDL_HasIntersection(&playerpos, &auxpos)==SDL_TRUE){
						player.state = STATE_DEAD;
						time_dead = SDL_GetTicks();
					}
				}
				SDL_RenderPresent(ren);
			}

			if(player.state == STATE_DEAD){
				Uint32 now = SDL_GetTicks();
				background_draw(&game, ren);
				cloud_draw(&cloud, ren);
				Obstacle aux = obstacles->first->data;
				obstacle_draw(&aux, ren);
				player_draw(&player, ren);
				if(now-time_dead>=TIME_ON_DEAD){
					game.state = STATE_GAME_OVER;
					time_dead = 0;
				}
				SDL_RenderPresent(ren);
			}
		}

		if(game.state == STATE_MAIN_MENU){
			background_draw(&game, ren);
			player_draw(&player, ren);

			char start[] = {"Press any Key to Start"};

			SDL_Color clr = {0x00, 0x00, 0x00, 0xFF};
			
			TTF_Font* fnt_menu;
			fnt_menu = aux_OpenFont(40);

			SDL_Surface *sfc = TTF_RenderText_Blended(fnt_menu, start, clr);
			assert(sfc!=NULL);
			txt = SDL_CreateTextureFromSurface(ren, sfc);
			assert(txt!=NULL);
			SDL_FreeSurface(sfc);
			SDL_Rect pos = {SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2-100, 600, 70};
			SDL_RenderCopy(ren, txt, NULL, &pos);

			TTF_CloseFont(fnt_menu);
			SDL_RenderPresent(ren);
		}

		if(game.state == STATE_GAME_OVER){
			//#e7fbff
			SDL_SetRenderDrawColor(ren, 0xe7, 0xfb, 0xff, 255);
			SDL_RenderClear(ren);
			

			char str_score[12];
			sprintf(str_score, "%06d", game.points);
			char str_text[20] = {"Your score: "};

			strcat(str_text, str_score);
			char reset[35] = {"Press any Key to Restart"};
			char gameover[10] = {"GAME OVER"};

			SDL_Color clr = {0x00, 0x00, 0x00, 0xFF};
			
			//Points
			SDL_Surface *sfc = TTF_RenderText_Blended(fnt, str_text, clr);
			assert(sfc!=NULL);
			txt = SDL_CreateTextureFromSurface(ren, sfc);
			assert(txt!=NULL);
			SDL_FreeSurface(sfc);
			SDL_Rect pos = {SCREEN_WIDTH/2 -100, SCREEN_HEIGHT/2, 200, 25};
			SDL_RenderCopy(ren, txt, NULL, &pos);

			//Restart
			TTF_Font* fnt_restart;
			fnt_restart = aux_OpenFont(30);

			sfc = TTF_RenderText_Blended(fnt_restart, reset, clr);
			assert(sfc!=NULL);
			txt = SDL_CreateTextureFromSurface(ren, sfc);
			assert(txt!=NULL);
			SDL_FreeSurface(sfc);
			pos = (SDL_Rect){SCREEN_WIDTH/2 -170, SCREEN_HEIGHT/2+200, 340, 40};
			SDL_RenderCopy(ren, txt, NULL, &pos);
			TTF_CloseFont(fnt_restart);

			//Game Over
			TTF_Font* fnt_over;
			fnt_over = aux_OpenFont(60);

			sfc = TTF_RenderText_Blended(fnt_over, gameover, clr);
			assert(sfc!=NULL);
			txt = SDL_CreateTextureFromSurface(ren, sfc);
			assert(txt!=NULL);
			SDL_FreeSurface(sfc);
			pos = (SDL_Rect){SCREEN_WIDTH/2-200, SCREEN_HEIGHT/2-300, 400, 60};
			SDL_RenderCopy(ren, txt, NULL, &pos);
			TTF_CloseFont(fnt_over);


			//Dino
			player.state = STATE_DEAD;
			player.position = (SDL_Point){SCREEN_WIDTH/2-64, SCREEN_HEIGHT/2-150};
			player_draw(&player, ren);


			SDL_RenderPresent(ren);
		}
    }

    //Finalizacao
    SDL_DestroyTexture(dino_running);
    SDL_DestroyTexture(dino_dead);
    SDL_DestroyTexture(scenario);
	SDL_DestroyTexture(cloud_img);
	SDL_DestroyTexture(cactus_img);
	SDL_DestroyTexture(track_img);
	SDL_DestroyTexture(bird);

	List_free(obstacles);

	SDL_DestroyTexture(txt);
	TTF_CloseFont(fnt);
	TTF_Quit();

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

TTF_Font* aux_OpenFont(int size){
	int font_size = size;//20
	TTF_Font* font = TTF_OpenFont("../assets/fonts/PressStart2P.ttf", font_size);
	assert(font!=NULL);
	return font; 
}


void load_textures(SDL_Renderer *ren){
	dino_running = IMG_LoadTexture(ren, "../assets/images/trex.png");
	dino_dead = IMG_LoadTexture(ren, "../assets/images/dead-trex.png");
    scenario = IMG_LoadTexture(ren, "../assets/images/scenario.png");
	cloud_img = IMG_LoadTexture(ren, "../assets/images/cloud.png");
	cactus_img = IMG_LoadTexture(ren, "../assets/images/cactus.png");
	track_img = IMG_LoadTexture(ren, "../assets/images/track.png");
	bird = IMG_LoadTexture(ren, "../assets/images/bird.png");
	assert(bird != NULL);
	assert(track_img != NULL);
	assert(cactus_img != NULL);
	assert(cloud_img != NULL);	
    assert(dino_running != NULL);
    assert(dino_dead != NULL);
    assert(scenario != NULL);
}

void init_game(Game *game){
	game->state = STATE_MAIN_MENU;
	game->speed = 14;
	game->previous_update = SDL_GetTicks();
	game->x_pos_backgroung = 0;
	game->x_pos_track = 0;
	game->points = 0;
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
	if(player->position.y >= 455){
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
	position = (SDL_Rect){player->position.x, player->position.y, player->size, player->size};
	if(player->state == STATE_DEAD){
		SDL_RenderCopy(ren, dino_dead, NULL, &position);

		return;
	}

	if(player->state == STATE_DUCKING){
		position = (SDL_Rect){player->position.x, player->position.y, player->size, player->size/2};
		crop = player->sprites[1][player->step_index];
	}else{
		crop = player->sprites[0][player->step_index];
	}
	SDL_RenderCopy(ren, dino_running, &crop, &position);
}

void init_cloud(Cloud *cloud){
	srand(time(NULL));
	cloud->position.x = SCREEN_WIDTH + (rand()%1500) + 10;
	cloud->position.y = (rand()%150)+50;
	cloud->width = 100;
	cloud->height = 50;
}

void cloud_update(Cloud *cloud, float game_speed){
	cloud->position.x -= game_speed;
	if(cloud->position.x<-cloud->width){
		srand(time(NULL));
		cloud->position.x = SCREEN_WIDTH + (rand()%1500) + 10;
		cloud->position.y = (rand()%200)+25;
	}

}
void cloud_draw(Cloud *cloud, SDL_Renderer *ren){
	SDL_Rect pos = {cloud->position.x, cloud->position.y, cloud->width, cloud->height};
	SDL_RenderCopy(ren, cloud_img, NULL, &pos);
}

void background_draw(Game *game, SDL_Renderer *ren){
	SDL_Rect pos = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_Rect crop = {game->x_pos_backgroung, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_RenderCopy(ren, scenario, &crop, &pos);

	//track
	SDL_RenderCopy(ren, track_img, &crop, &pos);
}

void background(Game *game, SDL_Renderer *ren){
	//SDL_RenderCopy(ren, scenario, NULL, &scenario_position);
	game->x_pos_backgroung += game->speed*0.5;
	if(game->x_pos_backgroung>=SCREEN_WIDTH){
		game->x_pos_backgroung = 0;
	}
	SDL_Rect pos = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_Rect crop = {game->x_pos_backgroung, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_RenderCopy(ren, scenario, &crop, &pos);

	//track
	game->x_pos_track += game->speed;
	if(game->x_pos_track>=SCREEN_WIDTH){
		game->x_pos_track = 0;
	}
	SDL_Rect crop_t = {game->x_pos_track, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_RenderCopy(ren, track_img, &crop_t, &pos);
}

void score(Game *game, SDL_Renderer *ren, TTF_Font* fnt){
	game->points+=1;
	if(game->points%100==0){
		game->speed+=1;
	}
	char str_score[7];
	sprintf(str_score, "%06d", game->points);
	char str_text[15] = {"Points: "};
	strcat(str_text, str_score);

	SDL_Color clr = {0x00, 0x00, 0x00, 0xFF};
	SDL_Surface *sfc = TTF_RenderText_Blended(fnt, str_text, clr);
	assert(sfc!=NULL);
	txt = SDL_CreateTextureFromSurface(ren, sfc);
	assert(txt!=NULL);
	SDL_FreeSurface(sfc);

	SDL_Rect pos = {1050, 25, 180, 25};
	SDL_RenderCopy(ren, txt, NULL, &pos);
}

void init_obstacle(Obstacle *obs){
	//Obstacle obs;

	srand(time(NULL));
	int randint = rand()%4;
	if(randint<=2){
		obs->type = TYPE_CACTUS;
	}else if(randint==3){
		obs->type = TYPE_BIRD;
	}

	obs->position = (SDL_Point){SCREEN_WIDTH+10, 536};
	obs->width = 56;//14x4
	obs->height = 64;//16x4
	obs->quantity = 1;
	obs->step_index = 0;

	if(obs->type==TYPE_CACTUS){
		obs->quantity = (rand()%3)+1;
	}
	if(obs->type==TYPE_BIRD){
		//groun 600
		obs->width = 96; //64*1,5
		obs->height = 72;//48*1,5
		randint = rand()%2;
		if(randint==0){
			obs->position.y = 456;
		}else{
			obs->position.y = 520;
		}
	}
}

void obstacle_update(List *obs, float game_speed){
	//printf("Alterando posicao\n");
	obs->first->data.position.x -= game_speed;
	if(obs->first->data.type == TYPE_BIRD){
		obs->first->data.step_index += 1;
		if(obs->first->data.step_index>=18){// o msm crop permanece a cada 3 frames
			obs->first->data.step_index = 0;
		}
	}
}

void obstacle_draw(Obstacle *obs, SDL_Renderer *ren){
	//printf("Desenhando\n");
	SDL_Rect pos = {obs->position.x, obs->position.y, obs->width*obs->quantity, obs->height};
	// SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	// SDL_RenderFillRect(ren, &pos);
	SDL_Rect crop;
	if(obs->type == TYPE_BIRD){
		int aux = obs->step_index/3;
		crop = (SDL_Rect) {aux*64, 0, 64, 48};
		SDL_RenderCopy(ren, bird, &crop, &pos);
	}else if(obs->type == TYPE_CACTUS){
		int i = obs->quantity -1;
		crop = (SDL_Rect) {0, 16*i, 14*obs->quantity, 16};
		SDL_RenderCopy(ren, cactus_img, &crop, &pos);
	}
}
