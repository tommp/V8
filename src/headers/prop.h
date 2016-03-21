#ifndef PROP_H
#define PROP_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
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
class btRigidBody;

class Prop{
private:
public:
	Prop(Resource_manager& manager);
	~Prop();
	bool update_position(float timedelta);
	bool update_context();
	bool touch_object();
	bool add_bases_to_context();
};

typedef std::shared_ptr<Prop> Prop_ptr;
/*=============================================*/

#endif