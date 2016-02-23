#ifndef PROP_H
#define PROP_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "character.h"
#include "animation.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Animation;
class Character;
class btRigidBody;

class Prop: public Character{
public:
	Prop(Resource_manager& manager);
	~Prop();
	void render_frame(const Renderer& renderer)const;
	void update_position(float timedelta);
	void touch_character(Character& character);
};

typedef std::shared_ptr<Prop> Prop_ptr;
/*=============================================*/

#endif