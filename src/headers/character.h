#ifndef CHARACTER_H
#define CHARACTER_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "actor.h"
#include "animation_set.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Character: public Actor{
	protected:
		Animation_set_ptr animations;
		std::string state;
		glm::vec3 collision_ellipse;
	public:
		virtual ~Character(){};
		virtual void update_position(float timedelta) = 0;
		virtual void render_frame()const = 0;
		virtual void touch_character(Character& character) = 0;
		bool operator<(const Character& b);
};
typedef std::shared_ptr<Character> Character_ptr;
/*=============================================*/

#endif