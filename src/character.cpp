#include "./headers/character.h"

bool Character::operator<(const Character& b){
	if (z < b.get_z() ){
		return true;
	}
	else if (this->z == b.get_z()){
		if ( (y+height) < (b.get_y() + b.get_height() ) ){
			return true;
		}
		return false;
	}
	else{
		return false;
	}
}