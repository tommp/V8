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
	fall_inertia = {0.0, 0.0, 0.0};
	init_rotation = {0.0, 0.0, 0.0, 1.0};
	/* TODO::Properly set this */
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

btRigidBody* Object::get_collision_body()const{
	return collision_body;
}

bool Object::generate_collision_volume(const std::string& modelname, Collision_shape type, const glm::vec3& scale){
	switch(type){
		case SPHERE:{
			GLuint radius = std::max(scale.x, std::max(scale.y, scale.z)); 
			collision_shape = new btSphereShape(radius);
			break;
		}
		case BOX:{
			btVector3 box_size = {scale.x, scale.y, scale.z};
			collision_shape = new btBoxShape(box_size);
			break;
		}
		case CONVEX_HULL:{
			std::vector<Vertex> vertices;
			load_model_vertices(modelname, vertices);
			btConvexHullShape* shape = new btConvexHullShape();

			for (auto vertex : vertices) {
				btVector3 vpos = btVector3(vertex.position[0], vertex.position[1], vertex.position[2]);
				shape->addPoint(vpos);
			}

			btShapeHull* hull = new btShapeHull(shape);
			btScalar margin = shape->getMargin();
			hull->buildHull(margin);

			collision_shape = new btConvexHullShape(reinterpret_cast<const btScalar*>(hull->getVertexPointer()), hull->numVertices());
			collision_shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
			break;
		}
		default:
			exit(EXIT_FAILURE);
	}

	return true;
}

bool Object::load_model_vertices(const std::string& modelname, std::vector<Vertex>& vertices){
	std::string model_path = Utility_vars::folder_path + MODEL_DATA_PATH + modelname + ".model";

	return true;
}

bool Object::generate_collision_body(GLfloat mass, 
				const glm::vec3& inertia, 
				const btQuaternion& rotation, 
				const glm::vec3& position){
	if (!collision_shape) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Collision shape not initialized, call generate_collision_volume(...) first." << std::endl;
		errorlogger("ERROR: Collision shape not initialized, call generate_collision_volume(...) first.");
		return false;
	}

	this->mass = mass;
	this->fall_inertia = btVector3(inertia.x, inertia.y, inertia.z);
	this->init_rotation = rotation;

	collision_shape->calculateLocalInertia(mass, fall_inertia);
	btTransform trans = btTransform(init_rotation, btVector3(position.x, position.y, position.z));
	motion_state = new btDefaultMotionState(trans);
	
	
	btRigidBody::btRigidBodyConstructionInfo collision_body_CI(mass, motion_state, collision_shape, btVector3(0.0, 0.0, 0.0));
	collision_body = new btRigidBody(collision_body_CI);

	return true;
}
