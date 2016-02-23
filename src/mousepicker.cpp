#include "mousepicker.h"

void Mousepicker::calculate_ray(GLfloat length, const Renderer& renderer){
	GLint x;
	GLint y;
	SDL_GetMouseState(&x, &y);

	glm::vec4 ray_start_s(((float)x/renderer.get_window_width()  - 0.5f) * 2.0f, 
							((float)y/renderer.get_window_height() - 0.5f) * 2.0f, 
							-1.0,
							1.0f
	);
	glm::vec4 ray_end_s(((float)x/renderer.get_window_width()  - 0.5f) * 2.0f,
						((float)y/renderer.get_window_height() - 0.5f) * 2.0f,
						0.0, 
						1.0f);

	glm::mat4 trans = glm::inverse(renderer.get_projection_matrix() * renderer.get_view_matrix());
	
	ray_start = trans * ray_start_s; 
	ray_start/= ray_start.w;

	ray_end = trans * ray_end_s; 
	ray_end /= ray_end.w;

	ray_end = (ray_end - ray_start) * length;

	std::cout << ray_start.x << ":" << ray_start.y << std::endl;
	std::cout << ray_end.x << ":" << ray_end.y << std::endl;
}

void Mousepicker::check_for_intersection(const btDiscreteDynamicsWorld* physics_world){

	btCollisionWorld::ClosestRayResultCallback ray_callback(
							btVector3(ray_start.x, ray_start.y, ray_start.z), 
							btVector3(ray_end.x, ray_end.y, ray_end.z));

	physics_world->rayTest(btVector3(ray_start.x, ray_start.y, ray_start.z), 
							btVector3(ray_end.x, ray_end.y, ray_end.z), 
							ray_callback);

	if(ray_callback.hasHit()) {
		std::cout << "mesh " << ray_callback.m_collisionObject->getUserPointer() << std::endl;
	}
	else{
		std::cout << "background" << std::endl;
	}
}