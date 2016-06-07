#include "level.h"

Level::Level(Resource_manager& init_manager, Renderer& renderer){
	mousepicker = std::make_shared<Mousepicker>();


	for (int i = 0; i < 3; ++i) {
		glm::vec3 position;
		position.x = rand() % 2000 - 1000;
		position.y = rand() % 200 + 100;
		position.z = rand() % 2000 - 1000;
		glm::vec3 color = {10.0, 10.0, 10.0};
		Light_ptr point_light = std::make_shared<Point_light>(400, 
						position, 
						color, 
						true);
		add_light(point_light);
	}

	for (int i = 0; i < 0; ++i) {
		Light_ptr spot_light = std::make_shared<Spot_light>();
		add_light(spot_light);
	}

	Light_ptr dir_light = std::make_shared<Directional_light>(glm::vec3(-1.0, -1.0, -1.0), 
									glm::vec3(20.0, 20.0, 20.0), 
									false);
	add_light(dir_light);

	Prop_ptr prop = std::make_shared<Prop>(init_manager, 
										"BOX", 
										glm::vec3(0.0, 0.0, 0.0), 
										glm::vec3(1000.0, 2.0, 1000.0),
										glm::vec3(0.0, 0.0, -1.0),
										glm::vec4(0.2, 0.2, 0.2, 0.8),
										glm::vec4(1.0, 1.0, 1.0, 1.0),
										0.0f,
										BOX,
										glm::vec3(1000.0, 2.0, 1000.0));
	add_object(prop);

	for (GLuint i = 0; i < 10; ++i) {
		glm::vec3 position;
		position.x = rand() % 2000 - 1000;
		position.y = rand() % 500;
		position.z = rand() % 2000 - 1000;
		Prop_ptr prop = std::make_shared<Prop>(init_manager, 
										"BOX", 
										position, 
										glm::vec3(10.0, 130.0, 10.0),
										glm::vec3(0.0, 0.0, -1.0),
										glm::vec4(0.7, 0.0, 0.0, 0.8),
										glm::vec4(4.0, 0.0, 0.0, 1.0),
										1000.0f,
										BOX,
										glm::vec3(10.0, 130.0, 10.0));
		add_object(prop);
	}

	for (GLuint i = 0; i < 30; ++i) {
		glm::vec3 position;
		position.x = rand() % 2000 - 1000;
		position.y = rand() % 700;
		position.z = rand() % 2000 - 1000;
		Prop_ptr prop = std::make_shared<Prop>(init_manager, 
										"BOX", 
										position, 
										glm::vec3(20.0, 20.0, 20.0),
										glm::vec3(0.0, 0.0, -1.0),
										glm::vec4(0.0, 0.7, 0.0, 0.8),
										glm::vec4(0.0, 4.0, 0.0, 1.0),
										60.0f,
										BOX,
										glm::vec3(20.0, 20.0, 20.0));
		add_object(prop);
	}

	for (GLuint i = 0; i < 3; ++i) {
		glm::vec3 position;
		position.x = rand() % 2000 - 1000;
		position.y = rand() % 700;
		position.z = rand() % 2000 - 1000;
		Prop_ptr prop = std::make_shared<Prop>(init_manager, 
										"BOX", 
										position, 
										glm::vec3(50.0, 50.0, 50.0),
										glm::vec3(0.0, 0.0, -1.0),
										glm::vec4(0.7, 0.0, 0.0, 0.8),
										glm::vec4(0.1, 0.1, 0.1, 1.0),
										6000.0f,
										BOX,
										glm::vec3(50.0, 50.0, 50.0));
		add_object(prop);
	}

	for (GLuint i = 0; i < 20; ++i) {
		glm::vec3 position;
		position.x = rand() % 2000 - 1000;
		position.y = rand() % 400;
		position.z = rand() % 2000 - 1000;
		Prop_ptr prop = std::make_shared<Prop>(init_manager, "BOX", 
										position, 
										glm::vec3(40.0, 2.0, 40.0),
										glm::vec3(0.0, 0.0, -1.0),
										glm::vec4(0.7, 0.0, 0.0, 0.8),
										glm::vec4(0.0, 0.0, 4.0, 1.0),
										0.0f,
										BOX,
										glm::vec3(40.0, 2.0, 40.0));
		add_object(prop);
	}

	if (!add_objects_to_physics_world()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to add level objects to physics world!"<< std::endl;
		errorlogger("FATAL ERROR: Failed to add level objects to physics world!");
		exit(EXIT_FAILURE);
	}

	if (!add_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to add level contexts to renderer!"<< std::endl;
		errorlogger("FATAL ERROR: Failed to add level contexts to renderer!");
		exit(EXIT_FAILURE);
	}

#if ENABLE_BULLET_DEBUG
	physics_engine.set_debug_renderer(&renderer);
#endif


}

bool Level::add_light(const Light_ptr& light){
	if (light){
		lights.push_front(light);	
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null light in World" << std::endl;
		errorlogger("ERROR: Cannot add null light in World");
		return false;
	}
}

bool Level::add_object(const Prop_ptr& prop) {
	props.push_back(prop);
	return true;
}

bool Level::add_object(const Mob_ptr& mob) {
	mobs.push_back(mob);
	return true;
}

void Level::set_gravity(const glm::vec3& gravity){
	physics_engine.set_gravity(gravity);
}

void Level::update_gravity(){
	physics_engine.update_gravity();
}

bool Level::update_positions(GLfloat timedelta, Renderer& renderer){
	(void(renderer));
	for (auto& object : props) {
		if (!object->update_position(timedelta)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update object position!"<< std::endl;
			errorlogger("ERROR: Failed to update object position!");
			return false;
		}
	}

	for (auto& object : mobs) {
		if (!object->update_position(timedelta)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update object position!"<< std::endl;
			errorlogger("ERROR: Failed to update object position!");
			return false;
		}
	}

	physics_engine.ray_trace(2000.0, renderer);
	physics_engine.step_world(timedelta);

	return true;
}

bool Level::render_level(Renderer& renderer, Camera_ptr& camera)const{
	if (!renderer.render_all(camera)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Renderer failed to render level!"<< std::endl;
		errorlogger("ERROR: Renderer failed to render level!");
		return false;
	}

#if ENABLE_BULLET_DEBUG
	physics_engine.debug_draw_world();
#endif

	return true;
}

bool Level::add_to_physics_world(btRigidBody* object)const{
	if (!object) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add nullptr to physics world!" << std::endl;
		errorlogger("ERROR: Cannot add nullptr to physics world!");
		return false;
	}
	physics_engine.add_to_physics_world(object);
	return true;
}

bool Level::remove_from_physics_world(btRigidBody* object)const{
	if (!object) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot remove nullptr from physics world!" << std::endl;
		errorlogger("ERROR: Cannot remove nullptr from physics world!");
		return false;
	}
	physics_engine.remove_from_physics_world(object);
	return true;
}

bool Level::resolve_collisions(){
	return true;
}

bool Level::add_objects_to_physics_world()const{
	for (auto& prop : props) {
		physics_engine.add_to_physics_world(prop->get_collision_body());
	}

	for (auto& mob : mobs) {
		physics_engine.add_to_physics_world(mob->get_collision_body());
	}

	return true;
}

bool Level::add_contexts_to_renderer(Renderer& renderer)const{
	for (auto& light : lights) {
		if (!light->add_context(renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add light context to renderer!" << std::endl;
			errorlogger("ERROR: Failed to add light context to renderer!");
			return false;
		}
	}

	for (auto& prop : props) {
		if (!prop->add_contexts_to_renderer(renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add prop context to renderer!" << std::endl;
			errorlogger("ERROR: Failed to add prop context to renderer!");
			return false;
		}
	}

	for (auto& mob : mobs) {
		if (!mob->add_contexts_to_renderer(renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add mob context to renderer!" << std::endl;
			errorlogger("ERROR: Failed to add mob context to renderer!");
			return false;
		}
	}

	return true;
}