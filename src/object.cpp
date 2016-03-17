#include "object.h"

Object::Object(){
	motion_state = nullptr;
	collision_shape = nullptr;
	collision_body = nullptr;
	rendering_context = std::make_shared<Rendering_context>();
}

Object::~Object(){
	delete motion_state;
	delete collision_shape;
	delete collision_body;
}

bool Object::operator<(const Object& b){
	if (position[2] < b.get_z() ){
		return true;
	}
	else if (this->position[2] == b.get_z()){
		if ( (position[1]+size[1]) < (b.get_y() + b.get_height() ) ){
			return true;
		}
		return false;
	}
	else{
		return false;
	}
}

Rendering_context_weak Object::get_weak_context()const{
	Rendering_context_weak weak_context = rendering_context;
	return weak_context;
}