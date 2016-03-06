#ifndef VERTEX_H
#define VERTEX_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
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
typedef struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    /* TODO::Split into bonestruct later, GPU mem opt. */
    glm::i16vec4 bone_ids;
    glm::vec4 bone_weights;
}Vertex;

void initialize_vertex(Vertex& vertex);

bool add_bone_to_vertex(Vertex& vertex, GLuint index);

bool add_weight_to_vertex(Vertex& vertex, GLfloat weight);

/*=============================================*/

#endif