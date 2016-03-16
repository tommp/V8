#ifndef TERRAIN_H
#define TERRAIN_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "utility.h"
#include "object.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <string>
#include <memory>
#include <cstdlib>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

class Terrain : public Object{
    private:
        Base_render_context_ptr base_context;
    public:
        bool update_position(float timedelta);
        bool update_context();
        bool touch_object(Object& object);
        bool add_bases_to_context();
};

typedef std::shared_ptr<Terrain> Terrain_ptr;
/*=============================================*/
#endif