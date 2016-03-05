#ifndef MOBS_H
#define MOBS_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "object.h"
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
class Object;
class btRigidBody;

class Cube: public Object{
private:
	Animation_set_ptr animations;
	glm::vec3 last_pos;
	glm::vec3 velocity;

	unsigned int speed;
	unsigned int last_move;
	unsigned int move_duration;
public:
	Cube(Resource_manager& manager);
	~Cube();
	void render_frame(const Renderer& renderer)const;
	bool update_position(float timedelta);
	bool touch_object(Object& Object);
};

typedef std::shared_ptr<Cube> Cube_ptr;
/*=============================================*/

#endif