#include "level.h"

Level::Level(Resource_manager& init_manager){
	mousepicker = new Mousepicker();

	camera = std::make_shared<Camera>();

	if (!init_physics()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize physics!");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 10; ++i) {
		Character_ptr cube = std::make_shared<Cube>(init_manager);
		add_active_character(cube);
	}
	
	for (int i = 0; i < 20; ++i) {
		Light_ptr point_light = std::make_shared<Point_light>();
		add_point_light(point_light);
	}

	Light_ptr dir_light = std::make_shared<Directional_light>();
	add_dir_light(dir_light);

	Character_ptr prop = std::make_shared<Prop>(init_manager);
	add_active_character(prop);
	rendering_targets.push_back(&characters);
}

Level::~Level(){
	delete physics_world;
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete mousepicker;
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

bool Level::add_active_character(const Character_ptr& character){
	if (character){
		characters.push_back(character);
		add_to_physics_world(character->get_collision_body());
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null Character in world" << std::endl;
		errorlogger("ERROR: Cannot add null Character in world");
		return false;
	}
}

bool Level::add_dormant_character(const Character_ptr& character){
	if(character){
		dormant_characters.push_front(character);
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null character in world" << std::endl;
		errorlogger("ERROR: Cannot add null character in world");
		return false;
	}
}

bool Level::add_character(const Character_ptr& character){
	if(character) {
		if(check_if_offscreen(character)) {
			return add_active_character(character);
		}
		else{
			return add_dormant_character(character);
		}
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null character in world" << std::endl;
		errorlogger("ERROR: Cannot add null character in world");
		return false;
	}
}

void Level::update_groups(){
	auto first_it_dormant = dormant_characters.begin();
	auto before_it_dormant = dormant_characters.before_begin();

	for (auto it = characters.begin(); it != characters.end(); ++it){
		if( !check_if_offscreen(*it)){
			add_dormant_character(*it);
			remove_from_physics_world((*it)->get_collision_body());
			it = characters.erase(it);
		}
	}

	for (auto it = first_it_dormant; it != dormant_characters.end(); ++it){
		if(check_if_offscreen(*it)){
			add_active_character(*it);
			add_to_physics_world((*it)->get_collision_body());
			dormant_characters.erase_after(before_it_dormant);
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

bool Level::check_if_offscreen(const Character_ptr& a)const{
	return false;
}

bool Level::update_positions(GLfloat timedelta, Renderer& renderer){
	for (auto it = characters.begin(); it != characters.end(); ++it) {
		if (!(*it)->update_position(timedelta)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update character position!"<< std::endl;
			errorlogger("ERROR: Failed to update character position!");
			return false;
		}
	}

	physics_world->stepSimulation(timedelta, 7);

	mousepicker->calculate_ray(2000.0f, renderer);
	mousepicker->check_for_intersection(physics_world);

	return true;
}

void Level::render_geometry(Renderer& renderer){
	renderer.render_geometry(rendering_targets, camera);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render geometry!"<< std::endl;
		errorlogger("ERROR: Failed to render geometry!");
		exit(EXIT_FAILURE);
	}
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

bool Level::add_to_physics_world(btRigidBody* character)const{
	if (!character) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add nullptr to physics world!" << std::endl;
		errorlogger("ERROR: Cannot add nullptr to physics world!");
		return false;
	}
	physics_world->addRigidBody(character);
	return true;
}

bool Level::remove_from_physics_world(btRigidBody* character)const{
	if (!character) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot remove nullptr from physics world!" << std::endl;
		errorlogger("ERROR: Cannot remove nullptr from physics world!");
		return false;
	}
	physics_world->removeRigidBody(character);
	return true;
}

bool Level::resolve_collisions(){
	return true;
}