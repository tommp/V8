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

		btScalar raw_model_matrix[16];

		btTransform transform;
		
		btScalar mass;
		btVector3 inertia;
		btQuaternion init_rotation;

		bool delete_collision_data();
	public:
		Object();
		~Object();
		virtual bool update_position(GLfloat timedelta, const glm::mat4& view_matrix) = 0;
		bool generate_collision_volume(const std::string& modelname, 
				Collision_shape type, 
				const glm::vec3& scale);
		bool generate_collision_body(GLfloat mass, 
				const btQuaternion& rotation, 
				const glm::vec3& position);
		bool load_model_vertices(const std::string& modelname, std::vector<Vertex>& vertices);

		btRigidBody* get_collision_body()const;
		glm::vec3 get_position()const;
		bool update_model_matrix();
		bool update_transform();
		bool fill_glm_matrix(glm::mat4& matrix)const;
};

typedef std::shared_ptr<Object> Object_ptr;
/*=============================================*/

#endif