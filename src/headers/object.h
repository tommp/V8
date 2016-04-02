#ifndef OBJECT_H
#define OBJECT_H


/*Included headers*/
/*---------------------------------------------*/
#include "enum_collision_shape_type.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "errorlogger.h"
#include "vertex.h"
#include "utility.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Object{
	protected:
		btRigidBody* collision_body;
		btCollisionShape* collision_shape;
		btDefaultMotionState* motion_state;
		
		btScalar mass;
		btVector3 inertia;
		btQuaternion init_rotation;

		bool delete_collision_data();
	public:
		Object();
		~Object();
		virtual bool update_position(GLfloat timedelta) = 0;
		virtual bool touch_object(Object& object) = 0;
		bool generate_collision_volume(const std::string& modelname, 
				Collision_shape type, 
				const glm::vec3& scale);
		bool generate_collision_body(GLfloat mass, 
				const glm::vec3& inertia, 
				const btQuaternion& rotation, 
				const glm::vec3& position);
		bool load_model_vertices(const std::string& modelname, std::vector<Vertex>& vertices);

		btRigidBody* get_collision_body()const;
};

typedef std::shared_ptr<Object> Object_ptr;
/*=============================================*/

#endif