#include "tile.h"

Tile::Tile(): num_lights(0){
	/* void */	
}

void Tile::increment_num_lights() {
	++num_lights;
}