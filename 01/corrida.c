#include <SDL2/SDL.h>
#include <time.h>

#define SCREEN_WIDTH 780
#define SCREEN_HEIGHT 350
#define WINDOW_POS_X  SDL_WINDOWPOS_UNDEFINED
#define WINDOW_POS_Y  SDL_WINDOWPOS_UNDEFINED
#define TIMEOUT 25

typedef struct{
	SDL_Rect fisico;
	SDL_Color cor;
	int terminouCorrida;
}Rect;

typedef struct{
	int qtd_corredores;
	int largada;
	SDL_Rect chegada;
	int podio[4];
	int posicao_proximo;
}Race;


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

void corrida_inicio(Rect *retangulos, Race *corrida){
	int anterior = 30;
	for(int i=0; i<corrida->qtd_corredores; i++){	
		retangulos[i].fisico.x = corrida->largada;
		retangulos[i].terminouCorrida = 0;
		retangulos[i].fisico.y = anterior+30;
		anterior = retangulos[i].fisico.y;
	}
	for(int i=0; i<= corrida->qtd_corredores; i++){
		corrida->podio[i] = -1;
	}
	corrida->posicao_proximo = 1;
}

void corrida_preenchePodio(Race *corrida, int corredor){
	int i = corrida->posicao_proximo;
	for (int i = 1; i <= corrida->qtd_corredores; ++i){
		if(corredor==corrida->podio[i]){
			return;
		}
	}
	corrida->podio[i] = corredor;
	corrida->posicao_proximo++;
}

void corrida_desenha(Race *corrida, SDL_Renderer *ren){
		SDL_SetRenderDrawColor(ren, 1, 42, 0, 1);
        SDL_RenderClear(ren);
		SDL_Rect pista = {0, 40, SCREEN_WIDTH, 130};
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 1);
		SDL_RenderFillRect(ren, &pista);
		//linha de chegada
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 1);
		SDL_RenderFillRect(ren, &corrida->chegada);
		SDL_RenderPresent(ren);
}

void rects_desenha(Rect *retangulos, int qtd, SDL_Renderer *ren){
	for(int i=0; i<qtd; i++){
		SDL_SetRenderDrawColor(ren, retangulos[i].cor.r, retangulos[i].cor.g, retangulos[i].cor.b, retangulos[i].cor.a);
		SDL_RenderFillRect(ren, &retangulos[i].fisico);
	}
	SDL_RenderPresent(ren);
}

void rects_coresAleatorias(Rect *retangulos, int qtd){
	srand(time(NULL));	
	for(int i =0; i<qtd; i++){
		retangulos[i].cor.r = rand() % 255;
		retangulos[i].cor.g = rand() % 255;
		retangulos[i].cor.b = rand() % 255;
		retangulos[i].cor.a = 1;
	}
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
    SDL_Window* win = SDL_criaJanela("1.6.1: Corrida");
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    /* EXECUÇÃO */
	Race corrida;
	corrida.qtd_corredores = 3;
	corrida.largada = 50;
	corrida.chegada = (SDL_Rect){730, 40, 80, 130};
	
	Rect retangulos[corrida.qtd_corredores];
	rects_coresAleatorias(retangulos, corrida.qtd_corredores);
	for(int i=0; i<corrida.qtd_corredores; i++){
		retangulos[i].fisico.h = retangulos[i].fisico.w = 20;			
	}

	corrida_inicio(retangulos, &corrida);
	corrida_desenha(&corrida, ren);
	rects_desenha(retangulos, corrida.qtd_corredores, ren);
	int quit = 0;
    
	while(!quit){
		//verifica se possui vencedores
		int todos_terminaram=1;
		for(int i=0; i<corrida.qtd_corredores; i++){
        	if (SDL_HasIntersection(&corrida.chegada, &retangulos[i].fisico)){
        		retangulos[i].terminouCorrida = 1;
        		corrida_preenchePodio(&corrida, i);
        	}else{
        		todos_terminaram=0;
        	}
        }
        //mostrar podio
        if(todos_terminaram==1){
        	SDL_Color gold = {255, 215, 0, 1};
			SDL_Color silver = {169, 169, 169, 1};
			SDL_Color bronze = {205, 127, 50, 1};
				
			int largura_podio = 40;
			SDL_Rect quadrados_podio[3];
			quadrados_podio[0] = (SDL_Rect){SCREEN_WIDTH/2 - largura_podio/2, 250, largura_podio, 50};
			quadrados_podio[1] = (SDL_Rect){quadrados_podio[0].x - largura_podio, 275, largura_podio, 25};
			quadrados_podio[2] = (SDL_Rect){quadrados_podio[0].x + largura_podio, 285, largura_podio, 15};
			SDL_Rect retangulos_copia[corrida.qtd_corredores];
			for(int i=0; i<corrida.qtd_corredores; i++){
				retangulos_copia[i].w = retangulos_copia[i].h = 20;
			}
							
			SDL_SetRenderDrawColor(ren, gold.r, gold.g, gold.b, gold.a);
			SDL_RenderFillRect(ren, &quadrados_podio[0]);
			SDL_SetRenderDrawColor(ren, silver.r, silver.g, silver.b, silver.a);
			SDL_RenderFillRect(ren, &quadrados_podio[1]);
			SDL_SetRenderDrawColor(ren, bronze.r, bronze.g, bronze.b, bronze.a);
			SDL_RenderFillRect(ren, &quadrados_podio[2]);
			SDL_RenderPresent(ren);

			for (int i = 1; i <=corrida.qtd_corredores; i++){
				int aux = corrida.podio[i];
				retangulos_copia[aux].x = quadrados_podio[i-1].x + retangulos[aux].fisico.w/2;
				retangulos_copia[aux].y = quadrados_podio[i-1].y - retangulos[aux].fisico.w -2;
				SDL_SetRenderDrawColor(ren, retangulos[aux].cor.r, retangulos[aux].cor.g, retangulos[aux].cor.b, retangulos[aux].cor.a);
				SDL_RenderFillRect(ren, &retangulos_copia[aux]);
			}
			SDL_RenderPresent(ren);
        }
        //capturando os eventos
        SDL_Event evt;
        Uint32 tempo_espera = TIMEOUT;
        int isevt = aux_WaitEventTimeoutCount(&evt, &tempo_espera);
        if (isevt) {
        	int qtd_movimento = 6;		
			int mov_futuro = 0;
            switch(evt.type){
            	case SDL_QUIT:
            		quit = 1;
            		break;
				//retangulo movido pelo mouse
            	case SDL_MOUSEMOTION:
            		if(!retangulos[0].terminouCorrida){
            			int x, y;
            			SDL_GetMouseState(&x, &y);
            			if(x<corrida.largada){
            				retangulos[0].fisico.x = corrida.largada;
            			}else if(x>=corrida.chegada.x){
            				retangulos[0].fisico.x = corrida.chegada.x;
            			}else{
            				retangulos[0].fisico.x = x;
            			}
            		}
            		break;
            	//reiniciar corrida
            	case SDL_MOUSEBUTTONDOWN:
            		if(todos_terminaram==1){
            			corrida_inicio(retangulos, &corrida);
            		}
            		break;
				//retangulo movido pelo teclado
            	case SDL_KEYDOWN:
					if(!retangulos[1].terminouCorrida){
            			switch(evt.key.keysym.sym){
            				case SDLK_LEFT:
            						mov_futuro = retangulos[1].fisico.x - qtd_movimento;
            						if (mov_futuro<=corrida.largada){
            							retangulos[1].fisico.x = corrida.largada;
            						}else{
            							retangulos[1].fisico.x = mov_futuro;
            						}
            					break;
            				
            				case SDLK_RIGHT:
            						mov_futuro = retangulos[1].fisico.x + qtd_movimento;
            						if (mov_futuro>=corrida.chegada.x){
            							retangulos[1].fisico.x = corrida.chegada.x;
            						}else{
            							retangulos[1].fisico.x = mov_futuro;
            						}	
            				break;
            			}
					}
            		break;
            }
        } else {
			//retangulo movido pelo tempo
			if(!retangulos[2].terminouCorrida){				
				srand(time(NULL));
            	int aux = rand() %7 + 1;
				retangulos[2].fisico.x += aux;
			}
			tempo_espera = TIMEOUT;
        }
        if(!todos_terminaram){
        	corrida_desenha(&corrida, ren);
			rects_desenha(retangulos, corrida.qtd_corredores, ren);	
        }
	}
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
