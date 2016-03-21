#ifndef OBJECT_H
#define OBJECT_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
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
	public:
		Object();
		~Object();
		virtual bool update_position(GLfloat timedelta) = 0;
		virtual bool touch_object(Object& object) = 0;
		btRigidBody* get_collision_body()const{return collision_body;};
};

Object::~Object() {
	delete motion_state;
	delete collision_shape;
	delete collision_body;
}

Object::Object() {
	mass = 0;
	fall_inertia = {0.0, 0.0, 0.0};
	/* TODO::Properly set this */
	collision_shape = new btSphereShape(20);
	collision_shape->calculateLocalInertia(mass, fall_inertia);
	motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
														btVector3(0, 0, 0)));
	
	btRigidBody::btRigidBodyConstructionInfo collision_body_CI(mass, 
															motion_state, 
															collision_shape, 
															btVector3(0, 0, 0));
	collision_body = new btRigidBody(collision_body_CI);
}

typedef std::shared_ptr<Object> Object_ptr;
/*=============================================*/

#endif