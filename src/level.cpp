#include "level.h"

Level::Level(Resource_manager& init_manager, Renderer& renderer){
	mousepicker = std::make_shared<Mousepicker>();

	camera = std::make_shared<Camera>();

	if (!init_physics()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to initialize physics!"<< std::endl;
		errorlogger("ERROR: Failed to initialize physics!");
		exit(EXIT_FAILURE);
	}

	
	for (int i = 0; i < 2; ++i) {
		Object_ptr cube = std::make_shared<Cube>(init_manager);
		add_active_object(cube);
	}
	
	for (int i = 0; i < 2; ++i) {
		Light_ptr point_light = std::make_shared<Point_light>();
		add_point_light(point_light);
	}

	Light_ptr dir_light = std::make_shared<Directional_light>();
	add_dir_light(dir_light);

	Object_ptr prop = std::make_shared<Prop>(init_manager);
	add_active_object(prop);

	add_context_to_renderer(renderer);
}

Level::~Level(){
	delete physics_world;
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
}

bool Level::add_dir_light(const Light_ptr& light){
	if (light){
		dir_lights.push_front(light);	
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null directional light in World" << std::endl;
		errorlogger("ERROR: Cannot add null directional light in World");
		return false;
	}
}

bool Level::add_point_light(const Light_ptr& light){
	if (light){
		point_lights.push_front(light);	
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null point light in World" << std::endl;
		errorlogger("ERROR: Cannot add null point light in World");
		return false;
	}
}

bool Level::add_spot_light(const Light_ptr& light){
	if (light){
		spot_lights.push_front(light);	
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null point light in World" << std::endl;
		errorlogger("ERROR: Cannot add null point light in World");
		return false;
	}
}

bool Level::add_active_object(const Object_ptr& object){
	if (object){
		objects.push_back(object);
		add_to_physics_world(object->get_collision_body());
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null object to level" << std::endl;
		errorlogger("ERROR: Cannot add null object to level");
		return false;
	}
}

bool Level::add_dormant_object(const Object_ptr& object){
	if(object){
		dormant_objects.push_front(object);
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null object to level" << std::endl;
		errorlogger("ERROR: Cannot add null object to level");
		return false;
	}
}

bool Level::add_object(const Object_ptr& object){
	if(object) {
		if(check_if_offscreen(object)) {
			return add_active_object(object);
		}
		else{
			return add_dormant_object(object);
		}
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null object to level" << std::endl;
		errorlogger("ERROR: Cannot add null object to level");
		return false;
	}
}

void Level::update_groups(){
	auto first_it_dormant = dormant_objects.begin();
	auto before_it_dormant = dormant_objects.before_begin();

	for (auto it = objects.begin(); it != objects.end(); ++it){
		if( !check_if_offscreen(*it)){
			add_dormant_object(*it);
			remove_from_physics_world((*it)->get_collision_body());
			it = objects.erase(it);
		}
	}

	for (auto it = first_it_dormant; it != dormant_objects.end(); ++it){
		if(check_if_offscreen(*it)){
			add_active_object(*it);
			add_to_physics_world((*it)->get_collision_body());
			dormant_objects.erase_after(before_it_dormant);
			it = before_it_dormant;
		}
		else{
			++before_it_dormant;
		}
	}
}

bool Level::init_physics(){
	broadphase = new btDbvtBroadphase();
	if (!broadphase){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize broadphase for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize broadphase for physics!");
		return false;
	}

	collisionConfiguration = new btDefaultCollisionConfiguration();
	if(!collisionConfiguration){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize collision configuration for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize collision configuration for physics!");
		return false;
	}

	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	if (!dispatcher){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize dispatcher for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize dispatcher for physics!");
		return false;
	}

	solver = new btSequentialImpulseConstraintSolver;
	if (!solver){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize constraint solver for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize constraint solver for physics!");
		return false;
	}

	physics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	if (!physics_world){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize world for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize world for physics!");
		return false;
	}

	gravity = {0.0f, -10.0f, 0.0f};
	update_gravity();

	return true;
}

void Level::set_gravity(const glm::vec3& gravity){
	this->gravity = gravity;
}

void Level::update_gravity(){
	physics_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

bool Level::check_if_offscreen(const Object_ptr& a)const{
	return false;
}

bool Level::update_positions(GLfloat timedelta, Renderer& renderer){
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if (!(*it)->update_position(timedelta)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update object position!"<< std::endl;
			errorlogger("ERROR: Failed to update object position!");
			return false;
		}
	}

	physics_world->stepSimulation(timedelta, 7);

	mousepicker->calculate_ray(2000.0f, renderer);
	mousepicker->check_for_intersection(physics_world);

	return true;
}

bool Level::add_context_to_renderer(Renderer& renderer)const{
	for (auto target : objects) {
		if (!target->add_context_to_renderer(renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add context to renderer for object!"<< std::endl;
			errorlogger("ERROR: Failed to add context to renderer for object!");
			return false;
		}
	}

	for (auto target : dormant_objects) {
		if (!target->add_context_to_renderer(renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add context to renderer for object!"<< std::endl;
			errorlogger("ERROR: Failed to add context to renderer for object!");
			return false;
		}
	}

	return true;
}

bool Level::render_geometry(Renderer& renderer) {
	renderer.render_geometry(camera);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render geometry!"<< std::endl;
		errorlogger("ERROR: Failed to render geometry!");
		return false;
	}
	return true;
}

void Level::render_lights(const Renderer& renderer)const{
	renderer.render_dir_lights(dir_lights, camera->get_position_refrence());
	renderer.render_point_lights(point_lights, camera->get_position_refrence());
	renderer.render_spot_lights(spot_lights, camera->get_position_refrence());
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render lights!" << std::endl;
		errorlogger("ERROR: Failed to render lights!");
		exit(EXIT_FAILURE);
	}
}

void Level::render_level(Renderer& renderer){
	render_geometry(renderer);
	render_lights(renderer);
}

bool Level::add_to_physics_world(btRigidBody* object)const{
	if (!object) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add nullptr to physics world!" << std::endl;
		errorlogger("ERROR: Cannot add nullptr to physics world!");
		return false;
	}
	physics_world->addRigidBody(object);
	return true;
}

bool Level::remove_from_physics_world(btRigidBody* object)const{
	if (!object) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot remove nullptr from physics world!" << std::endl;
		errorlogger("ERROR: Cannot remove nullptr from physics world!");
		return false;
	}
	physics_world->removeRigidBody(object);
	return true;
}

bool Level::resolve_collisions(){
	return true;
}