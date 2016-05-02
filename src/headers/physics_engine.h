#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
#include "entity.h"
#include "mousepicker.h"

#if ENABLE_BULLET_DEBUG
	#include "debug_drawer.h"
#endif
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <memory>
#include <iostream>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

class Renderer;
class Entity;

typedef std::shared_ptr<Entity> Entity_ptr;

class Physics_engine{
	private:
		btScalar timestep;

		btDiscreteDynamicsWorld* physics_world;
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
#if ENABLE_BULLET_DEBUG
		Debug_drawer debugDrawer;
#endif

		glm::vec3 gravity;

		Mousepicker mousepicker;

	public:
		Physics_engine();
		~Physics_engine();

		void step_world(GLfloat timedelta);
		void set_timestep(GLfloat timestep){this->timestep = (btScalar)timestep;};

		bool init_physics();
		void set_gravity(const glm::vec3& gravity);
		glm::vec3 get_gravity()const{return gravity;};
		void update_gravity();

		bool add_to_physics_world(btRigidBody* object)const;
		bool remove_from_physics_world(btRigidBody* object)const;

		bool add_to_physics_world(Entity_ptr entity)const;
		bool remove_from_physics_world(Entity_ptr entity)const;

		void ray_trace(GLfloat distance, const Renderer& renderer);
};

/*=============================================*/

#endif