#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
#include "actor.h"
#include "renderer.h"
#include "animation_set.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <string>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Renderer;
class Animation_set;
class btRigidBody;

typedef std::shared_ptr<Animation_set> Animation_set_ptr;

class Debug_drawer: public btIDebugDraw {
	private:
		Renderer* renderer;
	public:
		Debug_drawer();
		void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
		void draw3dText(const btVector3 &location, const char* textString);
};
typedef std::shared_ptr<Object> Object_ptr;
/*=============================================*/

#endif