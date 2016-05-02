#include "actor.h"

Actor::Actor(): position({0.0f, 0.0f, 0.0f}), 
				init_position({0.0f, 0.0f, 0.0f}), 
				scale({10.0f, 10.0f, 10.0f}), 
				init_scale({10.0f, 10.0f, 10.0f}), 
				direction({0.0f, 0.0f, -1.0f}),
				init_direction({0.0f, 0.0f, -1.0f}){

}