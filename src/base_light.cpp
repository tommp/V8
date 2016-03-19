#include "base_light.h"

Light::Light(){
	glm::vec4 color = {100.0,1.0,1.0,0.0};
	rendering_context->active = true;
}

bool Light::update_position(float timedelta){
	return true;
}

bool Light::update_context(){
	return true;
}

bool Light::touch_object(Object& object){
	return true;
}

bool Light::add_bases_to_context(){
	base_geometry->add_base_to_context(rendering_context);
	return true;
}