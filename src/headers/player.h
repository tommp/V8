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
class Player : public Actor{
private:
	Animation* animation;
	SDL_Rect collision_box;
	int speed;
public:
	Player(Animation* animation);
	void render_player(SDL_Renderer *ren);
	void update_position(float timedelta);

};
/*=============================================*/

#endif