#ifndef FRAME_H
#define FRAME_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "model.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Model;

typedef std::shared_ptr<Model> Model_ptr;

class Frame{
private:
	Model_ptr model;
	GLuint frame_duration;
public:
	Frame(const Model_ptr frame_model, GLuint duration);
	GLuint get_duration()const{return frame_duration;};
	void render(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const;
};
/*=============================================*/

#endif