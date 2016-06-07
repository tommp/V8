#include "physics_engine.h"

Physics_engine::Physics_engine(){
	if (!init_physics()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed initialize physics engine!"<< std::endl;
		errorlogger("FATAL ERROR: Failed initialize physics engine!");
		exit(EXIT_FAILURE);
	}

	this->timestep = 7;
}

Physics_engine::~Physics_engine(){
	delete physics_world;
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
}

bool Physics_engine::init_physics(){
	broadphase = new btDbvtBroadphase();
	if (!broadphase){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize broadphase for physics engine!"<< std::endl;
		errorlogger("ERROR: Failed initialize broadphase for physics engine!");
		return false;
	}

	collisionConfiguration = new btDefaultCollisionConfiguration();
	if(!collisionConfiguration){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize collision configuration for physics engine!"<< std::endl;
		errorlogger("ERROR: Failed initialize collision configuration for physics engine!");
		return false;
	}

	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	if (!dispatcher){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize dispatcher for physics engine!"<< std::endl;
		errorlogger("ERROR: Failed initialize dispatcher for physics engine!");
		return false;
	}

	solver = new btSequentialImpulseConstraintSolver;
	if (!solver){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize constraint solver for physics engine!"<< std::endl;
		errorlogger("ERROR: Failed initialize constraint solver for physics engine!");
		return false;
	}

	physics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	if (!physics_world){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize world for physics engine!"<< std::endl;
		errorlogger("ERROR: Failed initialize world for physics engine!");
		return false;
	}

	gravity = {0.0f, -100.0f, 0.0f};
	update_gravity();

#if ENABLE_BULLET_DEBUG
	physics_world->setDebugDrawer(&debugDrawer);
#endif
	return true;
}

void Physics_engine::step_world(GLfloat timedelta){
	physics_world->stepSimulation(timedelta, timestep);
}

bool Physics_engine::add_to_physics_world(btRigidBody* object)const{
	if (!object) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add nullptr to physics world!" << std::endl;
		errorlogger("ERROR: Cannot add nullptr to physics world!");
		return false;
	}
	
	physics_world->addRigidBody(object);
	return true;
}

bool Physics_engine::remove_from_physics_world(btRigidBody* object)const{
	if (!object) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot remove nullptr from physics world!" << std::endl;
		errorlogger("ERROR: Cannot remove nullptr from physics world!");
		return false;
	}

	physics_world->removeRigidBody(object);
	return true;
}

bool Physics_engine::add_to_physics_world(Entity_ptr entity)const{
	if (!add_to_physics_world(entity->get_collision_body())){
		return false;
	}

	return true;
}
bool Physics_engine::remove_from_physics_world(Entity_ptr entity)const{
	if (!remove_from_physics_world(entity->get_collision_body())){
		return false;
	}

	return true;
}

void Physics_engine::set_gravity(const glm::vec3& gravity){
	this->gravity = gravity;
}

void Physics_engine::update_gravity(){
	physics_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void Physics_engine::ray_trace(GLfloat distance, const Renderer& renderer){
	mousepicker.calculate_ray(distance, renderer);
	mousepicker.check_for_intersection(physics_world);

}