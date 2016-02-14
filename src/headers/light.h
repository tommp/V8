#ifndef LIGHT_H
#define LIGHT_H

/* TODO: Tons of optimization possible here, fix later */
/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "actor.h"
#include "shader.h"
#include "renderer.h"
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
class Shader;
class Actor;
class Renderer;

enum Light_type{
	DIRECTIONAL = 0,
	POINT = 1,
	SPOT = 2
};

class Light : public Actor {
	protected:
		GLuint clip_VAO;
		Light_type type;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	public:
		virtual void render_light(const Renderer& renderer)const = 0;
		Light_type get_type(){return type;};
};

class Directional_light : public Light {
	private:
		glm::vec3 direction;
	public:
		Directional_light();
		bool init_light_quad();
		bool render_light_quad()const;
		void render_light(const Renderer& renderer)const;
};

class Point_light : public Light {
	private:
		glm::vec3 position;
		
		GLfloat linear;
		GLfloat quadratic;
	public:
		Point_light();
		bool init_light_quad();
		bool render_light_quad()const;
		void render_light(const Renderer& renderer)const;
	
};

class Spot_light : public Light{
	private:
		glm::vec3 position;
		glm::vec3 direction;

		GLfloat cut_off;
		GLfloat outer_cut_off;
	  
		GLfloat linear;
		GLfloat quadratic;
	public:
		Spot_light();
		bool init_light_quad();
		bool render_light_quad()const;
		void render_light(const Renderer& renderer)const;  
};

typedef std::shared_ptr<Light> Light_ptr;
typedef std::shared_ptr<Point_light> Pont_light_ptr;
typedef std::shared_ptr<Spot_light> Spot_light_ptr;
typedef std::shared_ptr<Directional_light> Directional_light_ptr;
/*=============================================*/

#endif