#ifndef OBJECT_H
#define OBJECT_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <string>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Object{
	protected:
		btRigidBody* collision_body;
		btCollisionShape* collision_shape;
		btDefaultMotionState* motion_state;
		
		btScalar mass;
		btVector3 fall_inertia;

		bool delete_collision_data();
	public:
		Object();
		~Object();
		virtual bool update_position(GLfloat timedelta) = 0;
		virtual bool touch_object(Object& object) = 0;

		btRigidBody* get_collision_body()const;
};

typedef std::shared_ptr<Object> Object_ptr;
/*=============================================*/

#endif