#ifndef LIGHT_H
#define LIGHT_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "actor.h"
#include "shader.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
enum Light_type{
	DIRECTIONAL = 0,
	POINT = 1,
	SPOT = 2
};

class Light : public Actor {
protected:
	Light_type type;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
public:
	virtual void render_light();

};

struct Directional_light : public Light {
private:
	glm::vec3 direction;
};

class Point_light : public Light {
private:
	glm::vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
};

class Spot_light : public Light{
private:
	glm::vec3 position;
	glm::vec3 direction;

	float cut_off;
	float outer_cut_off;
  
	float constant;
	float linear;
	float quadratic;     
};
typedef std::shared_ptr<Light> Light_ptr;
/*=============================================*/

#endif