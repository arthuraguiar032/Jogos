#include <SDL2/SDL.h>
#include <time.h>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

typedef struct{
	SDL_Rect fisico;
	SDL_Color cor;
}Rect;

typedef struct{
	SDL_Point inicial;
	SDL_Point final;
	SDL_Color cor;
}Line;

void rects_inicioCorrida(Rect *retangulos, int qtd, int x_inicial){
	for(int i=0; i<qtd; i++){
		//retangulos[i].fisico.h = retangulos[i].fisico.w = 20;	
		retangulos[i].fisico.x = x_inicial;
		//retangulos[i].fisico.y = 20*(1+i*1.5);		
	}
}

void rects_cores_aleatorias(Rect *retangulos, int qtd){
	srand(time(NULL));	
	for(int i =0; i<qtd; i++){
		retangulos[i].cor.r = rand() % 255;
		retangulos[i].cor.g = rand() % 255;
		retangulos[i].cor.b = rand() % 255;
		retangulos[i].cor.a = 1;
	}
}

Uint32 colourToUint(SDL_Color cor){
	return (Uint32)((cor.r << 16) + (cor.g << 8) + (cor.b << 0));
}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Corrida",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
	int quit = 0;
	SDL_Color white = {255, 255, 255, 1};
	SDL_Color black = {0, 0, 0, 1};

	int qtd_corredores = 3;	
	Rect retangulos[qtd_corredores];
	
	rects_cores_aleatorias(retangulos, qtd_corredores);
	rects_inicioCorrida(retangulos, qtd_corredores, 50);
	for(int i=0; i<qtd_corredores; i++){
		retangulos[i].fisico.h = retangulos[i].fisico.w = 20;	
		//retangulos[i].fisico.x = 50;
		retangulos[i].fisico.y = 20*(1+i*1.5);		
	}
	
	int tam_linha = 200;
	Line linha_chegada;
	linha_chegada.cor = white;
	linha_chegada.inicial.x = linha_chegada.final.x = 700;
	linha_chegada.inicial.y = 10;
	linha_chegada.final.y = 10 + tam_linha;
	
	
	int primeiro = -1;
    while (!quit) {
		SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
        SDL_RenderClear(ren);
		
		//desenhando a linha de chegada
        SDL_Rect quadrado_chegada = {700, 10, 25, 200}; 
        SDL_SetRenderDrawColor(ren, white.r, white.g, white.b, white.a);
		SDL_RenderFillRect(ren, &quadrado_chegada);
		
		//desenhando os quadrados
		for(int i=0; i<qtd_corredores; i++){
			SDL_SetRenderDrawColor(ren, retangulos[i].cor.r, retangulos[i].cor.g, retangulos[i].cor.b, retangulos[i].cor.a);
			SDL_RenderFillRect(ren, &retangulos[i].fisico);
		}
		SDL_RenderPresent(ren);

        SDL_Event evt;
        int isevt = SDL_WaitEventTimeout(&evt, 25);
        if (isevt) {
            if (evt.type == SDL_QUIT) {
            	quit = 1;
            }
        } else {
			srand(time(NULL));
            for(int i=0; i<qtd_corredores; i++){
				if(!SDL_HasIntersection(&quadrado_chegada, &retangulos[i].fisico)){
					retangulos[i].fisico.x += rand() % 5 + 1;
					SDL_SetRenderDrawColor(ren, retangulos[i].cor.r, retangulos[i].cor.g, retangulos[i].cor.b, retangulos[i].cor.a);
					SDL_RenderFillRect(ren, &retangulos[i].fisico);
					SDL_Delay(5);
				}else{
					if(primeiro==-1){
						primeiro = i;
					}
				}
			}
        }
        //anunciar vencedor e resetar programa
        if(SDL_HasIntersection(&quadrado_chegada, &retangulos[0].fisico) && SDL_HasIntersection(&quadrado_chegada, &retangulos[1].fisico) &&
			 SDL_HasIntersection(&quadrado_chegada, &retangulos[2].fisico)){
			 	SDL_Delay(50);
			 	printf("%d\n", primeiro);
				rects_inicioCorrida(retangulos, qtd_corredores, 50);
				primeiro = -1;
		}
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
