#include "character.h"

bool Character::operator<(const Character& b){
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