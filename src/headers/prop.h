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
	bool update_position(float timedelta);
	bool update_context();
	bool touch_object(Object& Object);
	bool add_context_to_renderer(Renderer& renderer)const;
	bool add_bases_to_context();
};

typedef std::shared_ptr<Prop> Prop_ptr;
/*=============================================*/

#endif