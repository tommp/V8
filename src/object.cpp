#include "object.h"

Object::~Object() {
	if (!delete_collision_data()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to delete collision data!" << std::endl;
		errorlogger("FATAL ERROR: Failed to delete collision data!");
		exit(EXIT_FAILURE);
	}
}

Object::Object() {
	mass = 0;
	inertia = {0.0, 0.0, 0.0};
	init_rotation = {0.0, 0.0, 0.0, 1.0};
	collision_shape = nullptr;
	motion_state = nullptr;
	collision_body = nullptr;
}

bool Object::delete_collision_data() {
	delete motion_state;
	delete collision_shape;
	delete collision_body;

	return true;
}

void Object::set_linear_velocity(const glm::vec3& velocity){
	collision_body->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
}

void Object::inc_linear_velocity(const glm::vec3& velocity){
	btVector3 increment = btVector3(velocity.x,velocity.y,velocity.z);
	btVector3 linear_velocity = collision_body->getLinearVelocity();
	collision_body->setLinearVelocity(linear_velocity + increment);
}
void Object::set_mouse_pick_callback(const std::function<GLboolean(const glm::vec3& hit_coords)>& collision_callback){
	this->mouse_pick_callback = collision_callback;
}

bool Object::execute_mouse_pick_callback(const glm::vec3& hit_coords){
	if(mouse_pick_callback){
		if(!mouse_pick_callback(hit_coords)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Mouse picking callback failed!" << std::endl;
			errorlogger("ERROR: Mouse picking callback failed!");
			return false;
		}
	}
	
	return true;
}

btRigidBody* Object::get_collision_body()const{
	return collision_body;
}

bool Object::generate_collision_volume(Collision_shape type, const std::string& modelname, const glm::vec3& scale){
	std::vector<Vertex> vertices;
	load_model_vertices(modelname, vertices);
	glm::vec3 dimensions = get_volume_dimensions(vertices);
	dimensions *= scale;
	switch(type){
		case SPHERE:{
			GLuint radius = std::max(dimensions.x, std::max(dimensions.y, dimensions.z)) + 1; 
			collision_shape = new btSphereShape(radius);
			break;
		}
		case BOX:{
			btVector3 box_size = {dimensions.x + 1, dimensions.y + 1, dimensions.z + 1};
			collision_shape = new btBoxShape(box_size);
			break;
		}
		case CONVEX_HULL:{
			btConvexHullShape* shape = new btConvexHullShape();

			for (auto& vertex : vertices) {
				btVector3 vpos = btVector3(vertex.position[0], vertex.position[1], vertex.position[2]);
				shape->addPoint(vpos);
			}

			btShapeHull* hull = new btShapeHull(shape);
			btScalar margin = shape->getMargin();
			hull->buildHull(margin);

			collision_shape = new btConvexHullShape(reinterpret_cast<const btScalar*>(hull->getVertexPointer()), hull->numVertices());
			collision_shape->setLocalScaling(btVector3(dimensions.x, dimensions.y, dimensions.z));
			break;
		}
		default:{
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid collision volume type!" << std::endl;
			errorlogger("ERROR: Invalid collision volume type!");
			return false;
		}
	}

	return true;
}

bool Object::generate_collision_volume(Collision_shape type, const glm::vec3& scale){
	switch(type){
		case SPHERE:{
			GLuint radius = std::max(scale.x, std::max(scale.y, scale.z)) + 1; 
			collision_shape = new btSphereShape(radius);
			break;
		}
		case BOX:{
			btVector3 box_size = {scale.x + 1, scale.y + 1, scale.z + 1};
			collision_shape = new btBoxShape(box_size);
			break;
		}
		default:{
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid collision volume type!" << std::endl;
			errorlogger("ERROR: Invalid collision volume type!");
			return false;
		}
	}

	return true;
}

bool Object::load_model_vertices(const std::string& modelname, std::vector<Vertex>& vertices){
	(void(vertices));
	std::string model_path = Utility_vars::folder_path + MODEL_DATA_PATH + modelname + ".model";

	return true;
}

bool Object::generate_collision_body(GLfloat mass, 
				const btQuaternion& rotation, 
				const glm::vec3& position){
	if (!collision_shape) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Collision shape not initialized, call generate_collision_volume(...) first." << std::endl;
		errorlogger("ERROR: Collision shape not initialized, call generate_collision_volume(...) first.");
		return false;
	}

	this->mass = mass;
	this->init_rotation = rotation;

	collision_shape->calculateLocalInertia(mass, inertia);
	btTransform trans = btTransform(init_rotation, btVector3(position.x, position.y, position.z));
	motion_state = new btDefaultMotionState(trans);
	
	
	btRigidBody::btRigidBodyConstructionInfo collision_body_CI(mass, motion_state, collision_shape, inertia);
	collision_body_CI.m_friction = 10.0;
	collision_body_CI.m_rollingFriction = 10.0;
	collision_body = new btRigidBody(collision_body_CI);
	collision_body->setUserPointer(static_cast<void*>(this));

	trans.getOpenGLMatrix(&raw_model_matrix[0]);

	return true;
}

bool Object::update_model_matrix(){
	transform.getOpenGLMatrix(&raw_model_matrix[0]);
	return true;
}

bool Object::update_transform(){
	motion_state->getWorldTransform(transform);
	return true;
}

bool Object::fill_glm_matrix(glm::mat4& matrix)const{
	for (GLuint i = 0; i < 4; ++i) {
		for (GLuint j = 0; j < 4; ++j) {
			matrix[i][j] = raw_model_matrix[i * 4 + j];
		}
	}
	return true;
}

glm::vec3 Object::get_position()const{
	btVector3 origin = transform.getOrigin();
	glm::vec3 position = glm::vec3(origin.getX(), origin.getY(), origin.getZ());

	return position;
}

glm::vec3 Object::get_volume_dimensions(const std::vector<Vertex>& vertices)const{
	GLfloat max_x = -10.0f;
	GLfloat max_y = -10.0f;
	GLfloat max_z = -10.0f;

	GLfloat min_x = 10.0f;
	GLfloat min_y = 10.0f;
	GLfloat min_z = 10.0f;

	for (auto vertex : vertices) {
		max_x = vertex.position.x > max_x ? vertex.position.x : max_x;
		max_y = vertex.position.y > max_y ? vertex.position.y : max_y;
		max_z = vertex.position.z > max_z ? vertex.position.z : max_z;

		min_x = vertex.position.x < min_x ? vertex.position.x : min_x;
		min_y = vertex.position.y < min_y ? vertex.position.x : min_y;
		min_z = vertex.position.z < min_z ? vertex.position.x : min_z;
	}

	GLfloat delta_x = max_x + glm::abs(min_x);
	GLfloat delta_y = max_y + glm::abs(min_y);
	GLfloat delta_z = max_z + glm::abs(min_z);

	delta_x /= 2;
	delta_y /= 2;
	delta_z /= 2;

	return glm::vec3(delta_x, delta_y, delta_z);
}
