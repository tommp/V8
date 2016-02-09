#ifndef CONTACT_H
#define CONTACT_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "actor.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <forward_list>
#include <list>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Actor;

typedef std::shared_ptr<Actor> Actor_ptr;

class Contact{
	private:
		Actor_ptr a;
		Actor_ptr b;
	public:
		Contact(Actor_ptr c_a, Actor_ptr c_b);
};
/*=============================================*/

#endif