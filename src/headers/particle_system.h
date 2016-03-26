#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
#include "actor.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <string>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Particle{
	public:
		Model_ptr model;
		GLuint lifetime;
		GLuint age;

		/* TODO::Transform feedback */
		glm::mat4 model_matrix;
		glm::mat3 normal_model_matrix;
};

class Particle_system: public Actor{
	private:
		std::list<Particle> particles;

	public:
};


typedef std::shared_ptr<Particle_system> Particle_system_ptr;
/*=============================================*/

#endif