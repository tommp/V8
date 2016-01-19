#ifndef PLAYER_H
#define PLAYER_H


/*Included headers*/
/*---------------------------------------------*/
#include "./state_handler.h"
#include "./character.h"
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
	Button_mappings* button_mappings;
public:
	Player(SDL_Renderer& ren, Resource_manager& manager, Button_mappings& map);
	void render_frame(SDL_Renderer& ren, SDL_Rect* offset);
	void update_position(float timedelta);
	void touch_character(Character& character);
};
/*=============================================*/

#endif