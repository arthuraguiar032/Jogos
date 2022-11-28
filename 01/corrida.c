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

void rects_cores_aleatorias(Rect *retangulos, int n){
	srand(time(NULL));	
	for(int i =0; i<n; i++){
		retangulos[i].cor.r = rand() % 255;
		retangulos[i].cor.g = rand() % 255;
		retangulos[i].cor.b = rand() % 255;
		retangulos[i].cor.a = rand() % 255;
	}
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

	int qtd_corredores = 3;	
	Rect retangulos[qtd_corredores];
	
	rects_cores_aleatorias(retangulos, qtd_corredores);
	for(int i=0; i<qtd_corredores; i++){
		retangulos[i].fisico.h = retangulos[i].fisico.w = 20;	
		retangulos[i].fisico.x = 50;
		retangulos[i].fisico.y = 20*(1+i);			
	}
	
	Line linha_chegada;
	int tam_linha = 200;
	linha_chegada.inicial.x = linha_chegada.final.x = 900;
	linha_chegada.inicial.y = 10;
	linha_chegada.final.y = 10 + tam_linha;

    while (!quit) {
		//desenhando os quadrados
		SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
        SDL_RenderClear(ren);
		for(int i=0; i<qtd_corredores; i++){
			SDL_SetRenderDrawColor(ren, retangulos[i].cor.r, retangulos[i].cor.g, retangulos[i].cor.b, retangulos[i].cor.a);
			SDL_RenderFillRect(ren, &retangulos[i].fisico);
		}
		//desenhando a linha de chegada
		SDL_SetRenderDrawColor(ren, linha_chegada.cor.r, linha_chegada.cor.g, linha_chegada.cor.b, linha_chegada.cor.a);		
		SDL_RenderDrawLine(ren, linha_chegada.inicial.x, linha_chegada.inicial.y, linha_chegada.final.x, linha_chegada.final.y);		
        SDL_RenderPresent(ren);

        SDL_Event evt;
        int isevt = SDL_WaitEventTimeout(&evt, 50);
        if (isevt) {
            if (evt.type == SDL_QUIT) {
            	quit = 1;
            }
        } else {
			srand(time(NULL));
            for(int i=0; i<qtd_corredores; i++){
				retangulos[i].fisico.x += rand() % 10;
			}
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
