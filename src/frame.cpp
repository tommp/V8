#include "frame.h"

void Frame::render(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, GLfloat rotate){
	mesh->render_mesh(position, size, color, rotate);
}