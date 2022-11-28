#include <SDL2/SDL.h>
#include <time.h>

#define SCREEN_WIDTH 960
#define  SCREEN_HEIGHT 540
#define WINDOW_POS_X  SDL_WINDOWPOS_UNDEFINED
#define WINDOW_POS_Y  SDL_WINDOWPOS_UNDEFINED

typedef struct{
	SDL_Rect fisico;
	SDL_Color cor;
	int terminouCorrida;
}Rect;

typedef struct{
	int qtd_corredores;
	int largada;
	int chegada;
	int vencedor;
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

void inicioCorrida(Rect *retangulos, Race *corrida){
	for(int i=0; i<corrida->qtd_corredores; i++){	
		retangulos[i].fisico.x = corrida->largada;
		retangulos[i].terminouCorrida = 0;		
	}
	corrida->vencedor = -1;
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


int main (int argc, char* args[]) {
    /* INICIALIZACAO */
    SDL_inicia();
    SDL_Window* win = SDL_criaJanela("1.6.1: Corrida");
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
	Race corrida;
	corrida.qtd_corredores = 3;
	corrida.largada = 50;
	corrida.chegada = 700;
		
	Rect retangulos[corrida.qtd_corredores];
	rects_coresAleatorias(retangulos, corrida.qtd_corredores);
	for(int i=0; i<corrida.qtd_corredores; i++){
		retangulos[i].fisico.h = retangulos[i].fisico.w = 20;	
		retangulos[i].fisico.y = 20*(1+i*1.5);		
	}
	
	inicioCorrida(retangulos, &corrida);
	int quit = 0;
	
    while (!quit) {
    	SDL_Color white = {255, 255, 255, 1};
		SDL_Color black = {0, 0, 0, 1};
		
		SDL_SetRenderDrawColor(ren, black.r, black.g, black.b, black.a);
        SDL_RenderClear(ren);
		
		//desenhando a linha de chegada
        SDL_Rect quadrado_chegada = {corrida.chegada, 10, 25, 200}; 
        SDL_SetRenderDrawColor(ren, white.r, white.g, white.b, white.a);
		SDL_RenderFillRect(ren, &quadrado_chegada);
		
		//desenhando os quadrados
		for(int i=0; i<corrida.qtd_corredores; i++){
			SDL_SetRenderDrawColor(ren, retangulos[i].cor.r, retangulos[i].cor.g, retangulos[i].cor.b, retangulos[i].cor.a);
			SDL_RenderFillRect(ren, &retangulos[i].fisico);
		}
		SDL_RenderPresent(ren);

        SDL_Event evt;
        int isevt = SDL_WaitEventTimeout(&evt, 25);
        if (isevt) {
        	int qtd_movimento = 4;		
			int mov_futuro = 0;
            switch(evt.type){
            	case SDL_QUIT:
            		quit = 1;
            		break;
            	case SDL_MOUSEMOTION:
            		if(!retangulos[0].terminouCorrida){
            			int x, y;
            			SDL_GetMouseState(&x, &y);
            			if(x<corrida.largada){
            				retangulos[0].fisico.x = corrida.largada;
            			}else if(x>=corrida.chegada){
            				retangulos[0].fisico.x = corrida.chegada;
            				//retangulos[0].terminouCorrida = 1;
            			}else{
            				retangulos[0].fisico.x = x;
            			}
            		}
            		break;
            		
            	case SDL_KEYDOWN:
            		switch(evt.key.keysym.sym){
            			case SDLK_LEFT:
            				if(!retangulos[1].terminouCorrida){
            					mov_futuro = retangulos[1].fisico.x - qtd_movimento;
            					if (mov_futuro<=corrida.largada){
            						retangulos[1].fisico.x = corrida.largada;
            					}else{
            						retangulos[1].fisico.x = mov_futuro;
            					}
            				}
            				break;
            				
            			case SDLK_RIGHT:
            				if(!retangulos[1].terminouCorrida){
            					mov_futuro = retangulos[1].fisico.x + qtd_movimento;
            					if (mov_futuro>=corrida.chegada){
            						retangulos[1].fisico.x = corrida.chegada;
            					}else{
            						retangulos[1].fisico.x = mov_futuro;
            					}	
            				}
            				break;
            		}
            		break;
            }
        } else {
			srand(time(NULL));
            int aux = rand() %7 + 1;
            if (retangulos[2].fisico.x + aux >=corrida.chegada){
            	retangulos[2].fisico.x = corrida.chegada;
            }else{
            	retangulos[2].fisico.x += aux;
            }	
        }
        //verificando se cada quadrado terminou a corrida
        for(int i=0; i<corrida.qtd_corredores; i++){
        	if (SDL_HasIntersection(&quadrado_chegada, &retangulos[i].fisico)){
        		retangulos[i].terminouCorrida = 1;
        		if (corrida.vencedor == -1){
        			corrida.vencedor = i;
        		}
        	}
        	
        	SDL_SetRenderDrawColor(ren, retangulos[i].cor.r, retangulos[i].cor.g, retangulos[i].cor.b, retangulos[i].cor.a);
			SDL_RenderFillRect(ren, &retangulos[i].fisico);
			SDL_Delay(5);
        }
        
        //anunciar vencedor e resetar programa
        if(SDL_HasIntersection(&quadrado_chegada, &retangulos[0].fisico) && SDL_HasIntersection(&quadrado_chegada, &retangulos[1].fisico) &&
			 SDL_HasIntersection(&quadrado_chegada, &retangulos[2].fisico)){
			 	printf("%d\n", corrida.vencedor);
			 	SDL_Delay(10);
				inicioCorrida(retangulos, &corrida);
		}
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
