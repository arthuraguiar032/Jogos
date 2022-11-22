#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;
const int TAM_RECT = 30;

//comportamentos dos objetos ao bater nas quinas e nas bordas
void define_comportamento(SDL_Rect *r, int matriz[3][2], int linha){
	//se o objeto bater em uma barreira, inverto a direção
	if( (r->x==0 && r->y==0) || (r->x==SCREEN_WIDTH-TAM_RECT  && r->y==0) || (r->x==SCREEN_WIDTH-TAM_RECT && r->y==SCREEN_HEIGHT-TAM_RECT) || (r->x==0 && r->y==SCREEN_HEIGHT-TAM_RECT)){
		matriz[linha][0] *= -1;
		matriz[linha][1] *= -1;
		r->x +=1;
	} else if(r->x<=0 || r->x>=SCREEN_WIDTH-TAM_RECT){
		matriz[linha][0] *= -1;
	} else if(r->y<=0 || r->y>=SCREEN_HEIGHT-TAM_RECT){
		matriz[linha][1] *= -1;
	}
	int vel = 2;
	r->x += vel*matriz[linha][0];
	r->y += vel*matriz[linha][1];
}


int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animacao Cíclica",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r1 = { 1, 1, TAM_RECT, TAM_RECT};
    SDL_Rect r2 = { SCREEN_WIDTH-TAM_RECT-8, SCREEN_HEIGHT-TAM_RECT-8, TAM_RECT, TAM_RECT};
    SDL_Rect r3 = { 20, 500, TAM_RECT, TAM_RECT};
	   
    int quit = 0;
    SDL_Event e;
    int espera = 500;
    
    //direcao dos objetos
    int delta_r[3][2];
    for(int i =0; i<3; i++){
    	delta_r[i][0] = 1;
    	delta_r[i][1] = 1;
    }
    
    while (!quit){ 
		SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 0x87, 0xce, 0xfa, 0x00);
		SDL_RenderFillRect(ren, &r1);
		SDL_SetRenderDrawColor(ren, 0xf5, 0xde, 0xb3, 0x00);
		SDL_RenderFillRect(ren, &r2);
		SDL_SetRenderDrawColor(ren, 0xc8, 0xa2, 0xc8, 0x00);
		SDL_RenderFillRect(ren, &r3);
		
		//escutando evento
		int isevt = SDL_WaitEventTimeout(&e, espera);
		if (isevt) {
            if (e.type == SDL_QUIT) {
            	quit = 1;
            }else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        r1.y -= 5;
                        break;
                    case SDLK_DOWN:
                        r1.y += 5;
                        break;
                    case SDLK_LEFT:
                        r1.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        r1.x += 5;
                        break;
                }
        	}
        } else {
        	//refefine o tempo de espera e mexe linearmente os quadrados
            define_comportamento(&r1, delta_r, 0);
			define_comportamento(&r2, delta_r, 1);
			define_comportamento(&r3, delta_r, 2);
			SDL_Delay(200);
        }
		SDL_RenderPresent(ren);
    }
	
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
