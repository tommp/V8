#ifndef OBJECT_H
#define OBJECT_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
#include "actor.h"
#include "rendering_context.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <string>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Animation_set;
class btRigidBody;
struct Rendering_context;

typedef std::weak_ptr<Rendering_context> Rendering_context_weak;
typedef std::shared_ptr<Animation_set> Animation_set_ptr;

class Object: public Actor{
	protected:
		std::string state;
		Rendering_context_ptr rendering_context;

		btRigidBody* collision_body;
		btCollisionShape* collision_shape;
		btDefaultMotionState* motion_state;
		
		btScalar mass;
		btVector3 fall_inertia;

	public:
		Object();
		virtual ~Object();
		virtual bool update_position(float timedelta) = 0;
		virtual bool update_context() = 0;
		virtual bool touch_object(Object& object) = 0;
		virtual bool add_bases_to_context() = 0;
		btRigidBody* get_collision_body()const{return collision_body;};
		bool operator<(const Object& b);
		Rendering_context_weak get_weak_context()const;
};
typedef std::shared_ptr<Object> Object_ptr;
/*=============================================*/

#endif