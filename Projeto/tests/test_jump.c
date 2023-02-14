#include <stdio.h>
#include <stdlib.h>

#define VEL_JUMP 8.5
#define Y_GROUND 472

int main(){
	int y = Y_GROUND;
	float vel = VEL_JUMP;
	int isJumping = 1;

	while(isJumping){
		printf("Está pulando? %d. Position: %d, Velocity: %f\n", isJumping, y, vel);
		y -= vel*4;
		vel -= 0.8;
		if(y>=Y_GROUND){
			isJumping = 0;
			vel = VEL_JUMP;
		}	
	}
	printf("Está pulando? %d. Position: %d, Velocity: %f\n", isJumping, y, vel);
	
	return 0;
}
