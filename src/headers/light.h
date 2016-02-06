#ifndef LIGHT_H
#define LIGHT_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "actor.h"
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
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
public:
	virtual void render(const Shader_ptr& shader) = 0;

};

struct Directional_light : public Light {
private:
	vec3 direction;
};

class Point_light : public Light {
private:
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
};

class Spot_light : public Light{
private:
	vec3 position;
	vec3 direction;

	float cut_off;
	float outer_cut_off;
  
	float constant;
	float linear;
	float quadratic;     
};
/*=============================================*/

#endif