#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <math.h>


int main (int argc, char* args[]) {
    /* INICIALIZACAO */
    int tam_janela_x = 960;
    int tam_janela_y = 540;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Desenho com Formas Complexas",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         tam_janela_x, tam_janela_y, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 0x00);
    SDL_RenderClear(ren);
    
    //Desenhando as formas
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0xFF);
    SDL_RenderClear(ren);
       
    int trigon = filledTrigonRGBA(ren,150,300,300,300,225,200,0x43,0xff,0x64,0xd9);
    
    int circ = filledCircleColor(ren, 375, 245, 55, 0x980000b5);
    
    int line1 = lineRGBA(ren, 500, 300, 600, 200, 0x98, 0x56, 0xe2, 0xFF);
    int line2 = lineRGBA(ren, 500, 200, 600, 300, 0x98, 0x56, 0xe2, 0xFF);
    
    int quad =  boxRGBA(ren, 650, 300, 750, 200,0xFF, 0x45, 0xBA, 0xFF);

    
    //Rederizando em tela
    SDL_RenderPresent(ren);
    SDL_Delay(7000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
