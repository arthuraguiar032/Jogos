#include <SDL2/SDL.h>
#include <stdlib.h>

void engrossar_linha(SDL_Renderer * renderer, int x1, int y1, int x2, int y2, int grossura){
	for (int i =0; i<grossura; i++){
		if (x1==x2){
			SDL_RenderDrawLine(renderer, x1+i, y1, x2+i, y2);
		}else if (y1==y2){
			SDL_RenderDrawLine(renderer, x1, y1+i, x2, y2+i);
		}else{
			SDL_RenderDrawLine(renderer, x1+i, y1+i, x2+i, y2+i);
		}
	}
}


int main (int argc, char* args[]) {
    /* INICIALIZACAO */
    int tam_janela_x = 960;
    int tam_janela_y = 540;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Desenho Cubista",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         tam_janela_x, tam_janela_y, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 199, 222, 224, 0x00);
    SDL_RenderClear(ren);
    
    //Desenhando Retangulos
    int rect_tam = 150;
    int qtd_rects = 3;
    int rect_x = 235;
    int rect_h = 180;
    
    SDL_Rect * rects;
    rects = (SDL_Rect *) malloc( qtd_rects * sizeof(SDL_Rect));
    SDL_SetRenderDrawColor(ren, 255, 0, 0,0x00);   
    for (int i =0; i<3; i++){
    	//centralizar no eixo x
    	if (i == 0) {
    		rects[i].x= rect_x;
    	}else{
    		rects[i].x= 160 + rects[i-1].x;
    	}
    	//centralizar no eixo y
    	rects[i].y= rect_h;
    	rects[i].w= rect_tam;
    	rects[i].h= rect_tam;
    	SDL_RenderFillRect(ren, &rects[i]);
    }  
    
    //Desenhando linhas   (242,90,64), (128,0,0)
    SDL_SetRenderDrawColor(ren, 242, 195, 64, 0.5);
    int tam_linha = 120;
    engrossar_linha(ren, 40, rect_h + rect_tam/2, 40+tam_linha , rect_h + rect_tam/2, 5);
    engrossar_linha(ren, 780, rect_h + rect_tam/2, 780+tam_linha , rect_h + rect_tam/2, 5);
    
    //Rederizando em tela
    SDL_RenderPresent(ren);
    SDL_Delay(7000);

    /* FINALIZACAO */
    free(rects);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
