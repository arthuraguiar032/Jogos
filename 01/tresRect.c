//#include <assert.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

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

	int quit = 0;
    SDL_Event e;
	
    /* EXECUÇÃO */
    SDL_Rect r_teclado = {40, 20, 30, 30};
    SDL_Rect r_mouse = {80, 40, 30, 30};
    SDL_Rect r_tempo = {120, 60, 30, 30};
    
    int espera = 500;
    while (!quit) {
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
        SDL_RenderClear(ren);
        //ffe4c4
        SDL_SetRenderDrawColor(ren, 0xFF,0xE4,0xC4,0x00);
        SDL_RenderFillRect(ren, &r_teclado);
        //87cefa
        SDL_SetRenderDrawColor(ren, 0x84,0xCE,0xFA,0x00);
        SDL_RenderFillRect(ren, &r_mouse);
        //c8a2c8
        SDL_SetRenderDrawColor(ren, 0xC8,0xA2,0xC8,0x00);
        SDL_RenderFillRect(ren, &r_tempo);
        
        SDL_RenderPresent(ren);

        SDL_Event evt;
        //Uint32 antes = SDL_GetTicks();
        int isevt = SDL_WaitEventTimeout(&evt, espera);
        if (isevt) {
            /*espera -= (SDL_GetTicks() - antes);
            if (espera < 0) {
                espera = 0;
            }*/
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
            //espera = 500;
            r_tempo.x += 2;
            r_tempo.y += 2;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
