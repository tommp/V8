#include "debug_drawer.h"

Debug_drawer::Debug_drawer(){
	debug_mode = 1;
}

void Debug_drawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color){
	glm::vec3 begin = {from.getX(), from.getY(), from.getZ()};
	glm::vec3 end = {to.getX(), to.getY(), to.getZ()};
	glm::vec3 draw_color = {color.getX(), color.getY(), color.getZ()};

	glm::vec3 line = end - begin;
	glm::vec3 line_dir = glm::normalize(line);
	glm::vec3 line_mesh = {1.0, 0.0, 0.0};

	glm::vec3 position = begin;
	glm::vec3 rot_axis = glm::cross(line_mesh, line_dir);
	GLfloat dot = glm::dot(line_mesh, line_dir);
	
	glm::vec3 scale = glm::vec3(glm::length(line));

	GLfloat rot_angle = glm::acos(dot);
	
	glm::mat4 model_matrix = glm::mat4();
	model_matrix = glm::translate(model_matrix, position);
	model_matrix = glm::rotate(model_matrix, rot_angle, rot_axis); 
	model_matrix = glm::scale(model_matrix, scale);

	if (!renderer) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Renderer not initialized in debug drawer!" << std::endl;
		errorlogger("FATAL ERROR: Renderer not initialized in debug drawer!");
		exit(EXIT_FAILURE);
	}
	renderer->add_context({model_matrix, draw_color});
}

void Debug_drawer::draw3dText(const btVector3 &location, const char* textString){

}

void Debug_drawer::drawContactPoint(const btVector3& PointOnB, 
	const btVector3& normalOnB, 
	btScalar distance,
	int lifeTime,
	const btVector3& color){

}
      
void Debug_drawer::reportErrorWarning(const char* warningString){
	std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Error in bullet3d debug drawer: " << warningString << std::endl;
	errorlogger("WARNING: Error in bullet3d debug drawer: ", warningString);
}

void Debug_drawer::setDebugMode(int debug_mode){
	this->debug_mode = debug_mode;
}

int Debug_drawer::getDebugMode()const{ 
	return debug_mode;
}