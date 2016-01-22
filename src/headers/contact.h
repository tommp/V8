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
class Contact{
	private:
		Actor* a;
		Actor* b;
	public:
		Contact(Actor* c_a, Actor* c_b);
};
/*=============================================*/

#endif