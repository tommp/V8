#ifndef CHARACTER_H
#define CHARACTER_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
#include "actor.h"
#include "renderer.h"
#include "animation_set.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Renderer;
class Animation_set;
class btRigidBody;

typedef std::shared_ptr<Animation_set> Animation_set_ptr;

class Character: public Actor{
	protected:
		Animation_set_ptr animations;
		std::string state;

		btRigidBody* collision_body;
		btCollisionShape* collision_shape;
		btDefaultMotionState* motion_state;
		
		btScalar mass;
		btVector3 fall_inertia;

	public:
		Character();
		virtual ~Character();
		virtual void update_position(float timedelta) = 0;
		virtual void render_frame(const Renderer& renderer)const = 0;
		virtual void touch_character(Character& character) = 0;

		btRigidBody* get_collision_body()const{return collision_body;};
		bool operator<(const Character& b);
};
typedef std::shared_ptr<Character> Character_ptr;
/*=============================================*/

#endif