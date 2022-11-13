#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

int aux_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms){
	int antes = SDL_GetTicks();
	int is_evt = SDL_WaitEventTimeout(evt, *ms);
	int d_tempo = SDL_GetTicks() - antes;
	
	if (is_evt){
		//protecao para caso haja algum ruido na contagem de tempo
		if(d_tempo>*ms){
			d_tempo = *ms;
		}
		*ms -= d_tempo;
	}	
	return is_evt;
}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("3 Quadrados: 3 Eventos",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	
    /* EXECUÇÃO */
    SDL_Rect r_teclado = {40, 20, 30, 30};
    SDL_Rect r_mouse = {80, 40, 30, 30};
    SDL_Rect r_tempo = {120, 60, 30, 30};
    
    int quit = 0;
    SDL_Event e;
    int espera_inicial = 5000;
    Uint32 espera = espera_inicial;
    
    while (!quit) {
    	//desenhando os retangulos
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0xFF,0xE4,0xC4,0x00);
        SDL_RenderFillRect(ren, &r_teclado);
        SDL_SetRenderDrawColor(ren, 0x84,0xCE,0xFA,0x00);
        SDL_RenderFillRect(ren, &r_mouse);
        SDL_SetRenderDrawColor(ren, 0xC8,0xA2,0xC8,0x00);
        SDL_RenderFillRect(ren, &r_tempo);
        SDL_RenderPresent(ren);

        SDL_Event evt;
        int isevt = aux_WaitEventTimeoutCount(&evt, &espera);
        
        if (isevt) {
            if (evt.type == SDL_QUIT) {
            	quit = 1;
            }else if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        r_teclado.y -= 5;
                        break;
                    case SDLK_DOWN:
                        r_teclado.y += 5;
                        break;
                    case SDLK_LEFT:
                        r_teclado.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        r_teclado.x += 5;
                        break;
                }
            }else if (evt.type == SDL_MOUSEMOTION) {
            	int x,y;
            	SDL_GetMouseState(&x, &y);
            	r_mouse.x = x;
            	r_mouse.y = y;
            }
        } else {
        	//refefine o tempo de espera
            espera = espera_inicial;
            r_tempo.x += 2;
            r_tempo.y += 2;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
