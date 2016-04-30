#ifndef BASE_CONTROLLER_H
#define BASE_CONTROLLER_H

/*Included headers*/
/*---------------------------------------------*/
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <unistd.h>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

class Base_controller{
	private:
		
	public:
		virtual bool update();

};

typedef std::shared_ptr<Base_controller> Base_controller_ptr;
/*=============================================*/

#endif