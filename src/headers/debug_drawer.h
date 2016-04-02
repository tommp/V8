#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
#include "renderer.h"
#include "line_data.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <string>
#include <cmath>
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
		GLint debug_mode;
	public:
		Debug_drawer();
		void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
		void draw3dText(const btVector3 &location, const char* textString);
		void drawContactPoint(const btVector3& PointOnB,
								const btVector3& normalOnB,
								btScalar distance,
								int lifeTime,
								const btVector3& color);
      	void reportErrorWarning(const char* warningString);
      	void setDebugMode(int debugMode);
      	int getDebugMode()const;
#if ENABLE_BULLET_DEBUG
      	void set_renderer(Renderer* renderer){this->renderer = renderer;};
#endif
};

typedef std::shared_ptr<Debug_drawer> Debug_drawer_ptr;
/*=============================================*/
#endif