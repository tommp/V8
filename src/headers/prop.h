#ifndef PROP_H
#define PROP_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "object.h"
#include "model.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Model;
class Object;
class btRigidBody;

class Prop: public Object{
private:
	Model_ptr model;
public:
	Prop(Resource_manager& manager);
	~Prop();
	void render_frame(const Renderer& renderer)const;
	bool update_position(float timedelta);
	bool touch_object(Object& Object);
};

typedef std::shared_ptr<Prop> Prop_ptr;
/*=============================================*/

#endif