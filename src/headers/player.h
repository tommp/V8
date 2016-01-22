#ifndef PLAYER_H
#define PLAYER_H


/*Included headers*/
/*---------------------------------------------*/
#include "state_handler.h"
#include "character.h"
#include "animation.h"
#include "world.h"
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
	glm::vec3 velocity;
	int speed;
	Button_mappings* button_mappings;
public:
	Player(Resource_manager& manager, Button_mappings& map);
	void render_frame(const glm::vec3& position);
	void update_position(float timedelta);
	void touch_character(Character& character);
};
/*=============================================*/

#endif