#include "entity.h"

bool Entity::load_script(const std::string& script_name){
	std::ifstream contentf (Utility_vars::folder_path + SCRIPT_PATH + script_name + SCRIPT_POSTFIX, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for script: " << script_name << std::endl;
		errorlogger("ERROR: Failed to open content file for script: ", script_name.c_str());
		return false;
	}

	GLuint script_matrix_width = 0;
	GLuint script_matrix_height = 0;

	contentf.read(reinterpret_cast<char *>(&script_matrix_width), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(&script_matrix_height), sizeof(GLuint));

	if (script_matrix_height == 0 || script_matrix_width == 0){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid matrix dimensions for script: " << script_name << std::endl;
		errorlogger("ERROR: Invalid matrix dimensions for script: ", script_name.c_str());
		return false;
	}

	function_array.resize(script_matrix_width);
	for (auto& col : function_array){
		col.resize(script_matrix_height);
	}

	for (GLuint i = 0 ; i < script_matrix_width * script_matrix_height; ++i){
		if (!read_and_set_function_block(contentf)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to read function block for script: " << script_name << std::endl;
			errorlogger("ERROR: Failed to read function block for script: ", script_name.c_str());
			return false;
		}
	}

	return true;
}

bool Entity::read_and_set_function_block(std::ifstream& contentf){
	GLuint col_index;
	GLuint row_index;

	contentf.read(reinterpret_cast<char *>(&col_index), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(&row_index), sizeof(GLuint));

	GLint function_id = 0;

	contentf.read(reinterpret_cast<char *>(&function_id), sizeof(GLint));

	switch(function_id){
		case entity::CLEAN_ROT_N_SCALE: {
			
			break;
		}
		case entity::CHECK_IF_MAPPED_BUTTON_PRESSED: {
			std::string str_value;
			GLint key_value;
			if (!read_string_from_binary_file(contentf, str_value)){
				return false;
			}

			contentf.read(reinterpret_cast<char *>(&key_value), sizeof(GLint));

			string_buffer.push_back(str_value);
			int_buffer.push_back(key_value);

			function_array[col_index][row_index] = [&]()->GLboolean{
				return check_if_mapped_button_pressed(string_buffer[string_buffer.size() - 1], 
										static_cast<Key>(int_buffer[int_buffer.size() - 1]));
			};

			break;
		}			
		case SET_LINEAR_VELOCITY: {

			break;
		}
		case entity::INC_LINEAR_VELOCITY: {

			break;
		}
		case entity::SET_DIRECTION: {

			break;
		}
		case entity::RESIZE_VEC3_BUFFER: {

			break;
		}
		case entity::SET_VEC3_BUFFER: {

			break;
		}
		case entity::INC_VEC3_BUFFER: {

			break;
		}
		case entity::NORMALIZE_VEC3_IN_BUFFER: {

			break;
		}
		case entity::RESIZE_INT_BUFFER: {

			break;
		}
		case entity::SET_INT_BUFFER: {

			break;
		}
		case entity::INC_INT_BUFFER: {

			break;
		}
		case entity::RESIZE_FLOAT_BUFFER: {

			break;
		}
		case entity::SET_FLOAT_BUFFER: {

			break;
		}
		case entity::INC_FLOAT_BUFFER: {

			break;
		}
		case entity::RESIZE_BOOL_BUFFER: {

			break;
		}
		case entity::SET_BOOL_BUFFER: {

			break;
		}
		case entity::TOGGLE_BOOL_BUFFER: {

			break;
		}
		case entity::INT_COMPARE: {

			break;
		}	
		case entity::FLOAT_COMPARE: {

			break;
		}
		default: {
			return false;	
		}
	}
	return true;
}

bool Entity::execute_script_from_file(const std::string& script_name){
	std::ifstream contentf (Utility_vars::folder_path + SCRIPT_PATH + script_name + SCRIPT_POSTFIX, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for script: " << script_name << std::endl;
		errorlogger("ERROR: Failed to open content file for script: ", script_name.c_str());
		return false;
	}
	
	return true;
}

void Entity::init(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& dir){
	this->position = pos;
	this->init_position = pos;
	this->direction = dir;
	this->init_direction = dir;
	this->scale = scale;
	this->init_scale = scale;
	this->billboarded = false;
}

bool Entity::init_scripts(const std::string& init_script_name, const std::string& loop_script_name){
	(void(init_script_name)); (void(loop_script_name));


	return true;
}

bool Entity::init_model_context(const std::string& model_name){
	if (!model_context.load_model(*manager, model_name)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load model for entity: " << name << std::endl;
		errorlogger("ERROR: Failed to load model for entity: ", name.c_str());
		return false;
	}

	has_model = true;
	return true;
}

bool Entity::init_model_context(const std::string& model_name, const glm::vec4& color, const glm::vec4& color_coeff, GLfloat gloss){
	if (!model_context.load_model(*manager, model_name, color, color_coeff, gloss)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load model with name: " << model_name << ", for entity: " << name << std::endl;
		errorlogger("ERROR: Failed to load model for entity: ", name.c_str());
		return false;
	}

	has_model = true;
	return true;
}

bool Entity::init_physics_context(Collision_shape shape, GLfloat mass, GLboolean allow_deactivation){
	physics_context = Object();
	btQuaternion rotation = {0.0, 1.0, 0.0, 0.0};
	if (has_model){
		if (!physics_context.generate_collision_volume(shape, model_context.model->get_name(), scale) || !physics_context.generate_collision_body(mass, rotation, position)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to generate collision data for entity: " << name << std::endl;
			errorlogger("ERROR: Failed to generate collision data for entity: ", name.c_str());
			return false;
		}
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: No model loaded for entity: " << name << ", if you want the collision volumes to sync properly, load the model first!" << std::endl;
		if (!physics_context.generate_collision_volume(shape, scale) || !physics_context.generate_collision_body(mass, rotation, position)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to generate collision data for entity: " << name << std::endl;
			errorlogger("ERROR: Failed to generate collision data for entity: ", name.c_str());
			return false;
		}
	}

	if (!allow_deactivation) {
		physics_context.disable_deactivation();
	}
	else{
		physics_context.enable_deactivation();
	}
	

	has_physics = true;
	return true;
}

bool Entity::update(GLfloat timedelta){
	(void(timedelta));

	if (!update_contexts()){
		return false;
	}

	return true;
}

bool Entity::update_physics_context(){
	if (!has_physics){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: No physics context to update for entity: " << name << std::endl;
		errorlogger("ERROR: No physics context to update for entity: ", name.c_str());
		return false;
	}

	if (!physics_context.update_transform() && physics_context.update_model_matrix()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update physics context for entity: " << name << std::endl;
		errorlogger("ERROR: Failed to update physics context for entity: ", name.c_str());
		return false;
	}
	
	return true;
}

bool Entity::update_model_context(){
	if (!has_model){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: No model context to update for entity: " << name << std::endl;
		errorlogger("ERROR: No model context to update for entity: ", name.c_str());
		return false;
	}

	if (has_physics) {
		if (!physics_context.fill_glm_matrix(model_context.model_matrix)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to fill model view matrix for entity: " << name << std::endl;
			errorlogger("ERROR: Failed to fill model view matrix for entity: ", name.c_str());
			return false;
		}
	}else{
		model_context.model_matrix = glm::mat4();
		model_context.model_matrix = glm::translate(model_context.model_matrix, position);
		/* TODO:: Rotate based on actor directions */
	}

	model_context.model_matrix = glm::scale(model_context.model_matrix, scale);

	return true;
}

bool Entity::update_contexts(){
	if (has_physics){
		if (!update_physics_context()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update physics context for entity: " << name << std::endl;
			errorlogger("ERROR: Failed to update physics context for entity: ", name.c_str());
			return false;
		}
	}

	if (has_model){
		if (!update_model_context()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update model context for entity: " << name << std::endl;
			errorlogger("ERROR: Failed to update model context for entity: ", name.c_str());
			return false;
		}
	}

	return true;
}

bool Entity::add_contexts_to_renderer(Renderer& renderer)const{
	if (!add_model_contexts_to_renderer(renderer)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add model contexts to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add model contexts to renderer!");
		return false;
	}

	if (!add_light_contexts_to_renderer(renderer)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add light contexts to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add light contexts to renderer!");
		return false;
	}

	return true;
}

bool Entity::add_light_contexts_to_renderer(Renderer& renderer)const{
	for (auto& light : lights) {
		if (!light->add_context(renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add light context to renderer!" << std::endl;
			errorlogger("ERROR: Failed to add light context to renderer!");
			return false;
		}
	}

	return true;
}

bool Entity::add_model_contexts_to_renderer(Renderer& renderer)const{
	if(has_model){
		if (!model_context.model->add_contexts_to_renderer(renderer)) {
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add model contexts to renderer!" << std::endl;
			errorlogger("ERROR: Failed to add model contexts to renderer!");
			return false;
		}
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: No model loaded for entity: " << name << ", cant add contexts to renderer!" << std::endl;
	}
	
	return true;
}

bool Entity::add_physics_context_to_physics_engine(Physics_engine& physics_engine){
	if (!physics_engine.add_to_physics_world(physics_context.get_collision_body())){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add physics context to physics engine!" << std::endl;
		errorlogger("ERROR: Failed to add physics context to physics engine!");
		return false;
	}

	return true;
}

btRigidBody* Entity::get_collision_body()const{
	if (has_physics){
		return physics_context.get_collision_body();
	}

	return nullptr;
}

/* Script funcs */

bool Entity::clean_rot_n_scale(glm::mat4& matrix)const{
	matrix[0] = glm::vec4(1.0, 0.0, 0.0, matrix[0][3]);
	matrix[1] = glm::vec4(0.0, 1.0, 0.0, matrix[1][3]);
	matrix[2] = glm::vec4(0.0, 0.0, 1.0, matrix[2][3]);

	return true;
}

bool Entity::check_if_mapped_button_pressed(const std::string& map, Key key)const{
	const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
	if(current_key_states[manager->get_button_map_key(map, key)]){
		return true;
	}

	return false;
}

bool Entity::set_linear_velocity(const glm::vec3& velocity){
	if (has_physics){
		physics_context.set_linear_velocity(velocity);
		return true;
	}
	
	std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: No physics context loaded for entity: " << name << std::endl;
	return false;
}

bool Entity::inc_linear_velocity(const glm::vec3& velocity){
	if (has_physics){
		physics_context.inc_linear_velocity(velocity);
		return true;
	}
	
	std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: No physics context loaded for entity: " << name << std::endl;
	return false;
}

bool Entity::set_direction(const glm::vec3& direction){
	this->direction = glm::normalize(direction);

	return true;
}

bool Entity::set_vec3_buffer(GLuint index, const glm::vec3& value){
	if (index > vec3_buffer.size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Vec3_buffer index out of range for entity: " << name << std::endl;
		return false;
	}
	
	vec3_buffer[index] = value;

	return false;
}

bool Entity::inc_vec3_buffer(GLuint index, const glm::vec3& value){
	if (index > vec3_buffer.size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Vec3_buffer index out of range for entity: " << name << std::endl;
		return false;
	}
	
	vec3_buffer[index] += value;

	return false;
}

bool Entity::resize_vec3_buffer(GLuint buffer_size){
	vec3_buffer.resize(buffer_size);

	return true;
}

bool Entity::normalize_vec3_in_buffer(GLuint index){
	if (index > vec3_buffer.size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Vec3_buffer index out of range for entity: " << name << std::endl;
		return false;
	}

	glm::normalize(vec3_buffer[index]);

	return true;
}

bool Entity::int_compare(GLint a, GLint b){
	if (a > b){
		return true;
	}

	return false;
}

bool Entity::float_compare(GLfloat a, GLfloat b){
	if (a > b){
		return true;
	}

	return false;
}

bool Entity::resize_int_buffer(GLuint buffer_size){
	int_buffer.resize(buffer_size);

	return true;
}

bool Entity::set_int_buffer(GLuint index, GLint data){
	if (index > int_buffer.size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: int_buffer index out of range for entity: " << name << std::endl;
		return false;
	}

	int_buffer[index] = data;

	return true;
}

bool Entity::inc_int_buffer(GLuint index, GLint data){
	if (index > int_buffer.size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: int_buffer index out of range for entity: " << name << std::endl;
		return false;
	}

	int_buffer[index] += data;

	return true;
}

bool Entity::resize_float_buffer(GLuint buffer_size){
	float_buffer.resize(buffer_size);

	return true;
}

bool Entity::set_float_buffer(GLuint index, GLfloat data){
	if (index > float_buffer.size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: float_buffer index out of range for entity: " << name << std::endl;
		return false;
	}

	float_buffer[index] = data;

	return true;
}

bool Entity::inc_float_buffer(GLuint index, GLfloat data){
	if (index > float_buffer.size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: float_buffer index out of range for entity: " << name << std::endl;
		return false;
	}

	float_buffer[index] += data;

	return true;
}

bool Entity::resize_bool_buffer(GLuint buffer_size){
	bool_buffer.resize(buffer_size);

	return true;
}

bool Entity::set_bool_buffer(GLuint index, GLboolean data){
	if (index > bool_buffer.size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: bool_buffer index out of range for entity: " << name << std::endl;
		return false;
	}

	bool_buffer[index] = data;

	return true;
}

bool Entity::toggle_bool_buffer(GLuint index){
	if (index > bool_buffer.size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: bool_buffer index out of range for entity: " << name << std::endl;
		return false;
	}

	if (bool_buffer[index] == true){
		bool_buffer[index] = false;
	}
	else{
		bool_buffer[index] = true;
	}

	return true;
}