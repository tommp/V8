#ifndef MATERIAL_H
#define MATERIAL_H


/*Included headers*/
/*---------------------------------------------*/
#include "texture.h"
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Resource_manager;
class Texture;
class Shader;

typedef std::shared_ptr<Texture> Texture_ptr;
typedef std::shared_ptr<Shader> Shader_ptr;

class Material{
private:
	std::string name;
	Texture_ptr diffuse;
	Texture_ptr specular;
	GLuint shininess;
public:
	Material();
	bool is_complete()const;
	bool load_from_file(Resource_manager& manager, const std::string& name);
	bool use(const Shader_ptr& shader);
};

typedef std::shared_ptr<Material> Material_ptr;
/*=============================================*/

#endif