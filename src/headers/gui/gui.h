#ifndef GUI_H
#define GUI_H


/*Included headers*/
/*---------------------------------------------*/
#include "widget.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <unistd.h>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class GUI{
private:
	std::vector<Widget_ptr> widgets;

	btDiscreteDynamicsWorld* physics_world;
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
public:
	virtual GUI();
	bool init_physics();
	bool render_widgets();
};

GUI::GUI(){
	if (!init_physics()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize physics!");
    	exit(EXIT_FAILURE);
	}
}

bool GUI::render_widgets(){
	for (auto widget : widgets) {
		widget->render();
	}
	return true;
}

bool GUI::init_physics(){
	broadphase = new btDbvtBroadphase();
	if (!broadphase){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize broadphase for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize broadphase for physics!");
		return false;
	}

	collisionConfiguration = new btDefaultCollisionConfiguration();
	if(!collisionConfiguration){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize collision configuration for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize collision configuration for physics!");
		return false;
	}

	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	if (!dispatcher){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize dispatcher for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize dispatcher for physics!");
		return false;
	}

	solver = new btSequentialImpulseConstraintSolver;
	if (!solver){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize constraint solver for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize constraint solver for physics!");
		return false;
	}

	physics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	if (!physics_world){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed initialize world for physics!"<< std::endl;
		errorlogger("ERROR: Failed initialize world for physics!");
		return false;
	}

	physics_world->setGravity(btVector3(0.0f, 0.0f, 0.0f));

	return true;
	}
}
/*=============================================*/

#endif