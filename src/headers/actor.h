#ifndef ACTOR_H
#define ACTOR_H


/*Included headers*/
/*---------------------------------------------*/
#include "./animation.h"
#include "./resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Actor{
	protected:
		float x;
		float y;
		float z;
		float width;
		float height;
		float depth;
	public:
		float get_x()const{return x;};
		float get_y()const{return y;};
		float get_z()const{return z;};

		float get_width()const{return width;};
		float get_height()const{return height;};
		float get_depth()const{return depth;};

		void set_x(float new_x){x = new_x;};
		void set_y(float new_y){y = new_y;};
		void set_z(float new_z){z = new_z;};

		void set_width(float new_width){width = new_width;};
		void set_height(float new_height){height = new_height;};
		void set_depth(float new_depth){depth = new_depth;};
};

class Character: public Actor{
	protected:
		Animation_set_ptr animations;
		std::string state;
	public:
		virtual void update_position(float timedelta) = 0;
		virtual void render_frame(SDL_Renderer& ren, SDL_Rect* offset) = 0;
		virtual void influence_world(std::list<Character*>& characters) = 0;
		virtual void touch_character(Character& character) = 0;
		bool operator<(const Character& b);
};



class Descriptor{

};

typedef std::shared_ptr<Actor> Actor_ptr;
typedef std::shared_ptr<Character> Character_ptr;
/*=============================================*/

#endif