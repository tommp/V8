#include "frame.h"

Frame::Frame(const Model_ptr frame_model, GLuint duration){
	model = frame_model;
	frame_duration = duration;
}

void Frame::render(const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const{
	model->render_model(position, size, direction);
}