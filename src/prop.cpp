#include "prop.h"

Prop::Prop(){
	std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: No default prop constructor!" << std::endl;
	errorlogger("FATAL ERROR: No default prop constructor!");
	exit(EXIT_FAILURE);
}

Prop::Prop(Resource_manager& manager, const std::string& model_name){
	if (!(model = manager.load_model(model_name))){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Prop constructor failed to load model: " << model_name << std::endl;
		errorlogger("FATAL ERROR: Prop constructor failed to load model: ", model_name.c_str());
		exit(EXIT_FAILURE);
	}

	model->bind_context(model_matrix, normal_model_matrix, context_name);

	position = {0, 0, 0};
	direction = {0.0f, 0.0f, -1.0f};
	scale = {10.0f, 10.0f, 10.0f};

	prev_position = {10.0, 0.0, 0.0};

	/* Physics */
	GLfloat mass = 10;
	glm::vec3 inertia = {0.0, 0.0, 0.0};
	btQuaternion rotation = {0.0, 0.0, 0.0, 1.0};
	generate_collision_volume(model_name, BOX, scale);
	generate_collision_body(mass, inertia, rotation, position);

	update_matrices();
}

Prop::Prop(Resource_manager& manager, 
		const std::string& model_name,
		const glm::vec3& position,
		const glm::vec3& scale,
		const glm::vec3& direction,
		GLfloat mass,
		const glm::vec3& inertia,
		Collision_shape shape){

	if (!(model = manager.load_model(model_name))){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Prop constructor failed to load model: " << model_name << std::endl;
		errorlogger("FATAL ERROR: Prop constructor failed to load model: ", model_name.c_str());
		exit(EXIT_FAILURE);
	}

	model->bind_context(model_matrix, normal_model_matrix, context_name);

	this->position = position;
	this->direction = glm::normalize(direction);
	this->scale = scale;

	prev_position = position + glm::vec3(10.0, 0.0, 0.0);

	/* Physics */
	btQuaternion rotation = {0.0, 0.0, 0.0, 1.0};
	generate_collision_volume(model_name, BOX, scale);
	generate_collision_body(mass, inertia, rotation, position);

	update_matrices();
}




Prop::~Prop(){
	if (!model->unbind_context(context_name)){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to unbind model context: " << context_name << std::endl;
		errorlogger("FATAL ERROR: Failed to unbind model context: ", context_name.c_str());
		exit(EXIT_FAILURE);
	}
}

bool Prop::update_position(GLfloat timedelta){
	btTransform transform;
    motion_state->getWorldTransform(transform);

    position[0] = transform.getOrigin().getX();
    position[1] = transform.getOrigin().getY();
    position[2] = transform.getOrigin().getZ();


    btQuaternion rotation = transform.getRotation();
    glm::quat glm_rot = {rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW()};

    glm::vec4 new_dir = glm::rotate	(glm_rot, glm::vec4(direction, 1.0));

    direction = {new_dir.x, new_dir.y, new_dir.z};	
    glm::normalize(direction);

    std::cout << direction.x << ":" << direction.y << ":" << direction.z << std::endl;

    if (!update_matrices()) {
    	std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to update matrices for prop!" << std::endl;
		errorlogger("ERROR: Failed to update matrices for prop!");
		return false;
    }

	return true;
}

bool Prop::touch_object(Object& object){
	return true;
}

bool Prop::add_contexts_to_renderer(Renderer& renderer)const{
	if (!model->add_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add model contexts to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add model contexts to renderer!");
		return false;
	}

	return true;
}

bool Prop::update_matrices(){
	GLboolean should_update_model = false;
	if (position != prev_position) {
		should_update_model = true;
	}
	else if (direction != prev_direction) {
		should_update_model = true;
	}

	else if (scale != prev_scale) {
		should_update_model = true;
	}

	if (should_update_model) {
		 
		glm::vec3 init_direction = model->get_init_direction();
		GLfloat dot = glm::dot(init_direction, direction);
		glm::vec3 rot_axis = glm::cross(init_direction, direction);
		GLfloat rot_angle = glm::acos(dot);

		model_matrix = glm::mat4();
		model_matrix = glm::translate(model_matrix, position); 
	    //model_matrix = glm::rotate(model_matrix, rot_angle, rot_axis); 
	    model_matrix = glm::scale(model_matrix, scale);

	    normal_model_matrix = glm::inverseTranspose(glm::mat3(model_matrix));

		prev_position = position;
		prev_scale = scale;
		prev_direction = direction;
	}
	return true;
}