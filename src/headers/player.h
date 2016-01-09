#ifndef PLAYER_H
#define PLAYER_H


/*Included headers*/
/*---------------------------------------------*/
#include "./actor.h"
#include "./animation.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Player : public Character{
private:
	int speed;
public:
	Player(Animation* animation);
	void render_frame(SDL_Renderer *ren);
	void update_position(float timedelta);
	void live(SDL_Renderer *ren, float timedelta);
	void check_if_colliding(Actor* target);

};
/*=============================================*/

#endif