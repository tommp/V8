#ifndef WIDGET_H
#define WIDGET_H


/*Included headers*/
/*---------------------------------------------*/
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Widget{
protected:
	GLuint width;
	GLuint height;
	GLuint texture;
	glm::vec2 pos;
	glm::vec3 hover_boost;

	GLuint VBO;
	GLuint VAO;
public:
	virtual bool on_hover() = 0;
	virtual bool on_click() = 0;
	virtual bool on_release() = 0;
	virtual bool render() = 0;

	virtual void update() = 0;
};

typedef std::shared_ptr<Widget> Widget_ptr;
/*=============================================*/

#endif