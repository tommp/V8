#ifndef MATERIAL_H
#define MATERIAL_H


/*Included headers*/
/*---------------------------------------------*/
#include "material_map.h"
#include "texture.h"
#include "resource_manager.h"
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
class Resource_manager;

class Material{
private:
	Shader_ptr shader;
	Texture_ptr diffuse;
	Texture_ptr specular;
	GLuint shininess;
public:
	Material();
	Shader_ptr get_shader()const{return shader;};
	bool load_from_file(Resource_manager& manager, const std::string& name);
	void use();
};

typedef std::shared_ptr<Material> Material_ptr;
/*=============================================*/

#endif