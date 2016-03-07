#include "skeletal_node.h"

Skeletal_node::Skeletal_node(){
}

Skeletal_node::Skeletal_node(const std::string& name, 
				const glm::mat4& trans,
				Skeletal_node_ptr parent,
				std::vector<Skeletal_node_ptr> children) : name(name), trans(trans), parent(parent), children(children){
	
}

Skeletal_node_ptr Skeletal_node::get_parent()const{
	return parent;
}

std::string Skeletal_node::get_name()const{
	return name;
}