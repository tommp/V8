#ifndef SKELETAL_NODE_H
#define SKELETAL_NODE_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <string>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Skeletal_node;
typedef std::shared_ptr<Skeletal_node> Skeletal_node_ptr;

class Skeletal_node{
	public:
		std::string name;
		GLuint id;
		glm::mat4 trans;
		Skeletal_node_ptr parent;
		std::vector<Skeletal_node_ptr> children;
	public:
		Skeletal_node();
		Skeletal_node(const std::string& name, 
						GLuint id, 
						const glm::mat4& trans,
						Skeletal_node_ptr parent,
						std::vector<Skeletal_node_ptr> children);
		Skeletal_node_ptr get_parent()const;
		std::string get_name()const;
};
/*=============================================*/

#endif