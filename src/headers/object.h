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

namespace Object_vars{
	const GLfloat COLLISION_SLACK = 0.001;
}

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

		std::function<GLboolean(const glm::vec3& hit_coords)> mouse_pick_callback;

		bool delete_collision_data();
	public:
		Object();
		~Object();
		void set_mass(GLfloat new_mass){this->mass = (btScalar)new_mass;};
		GLfloat get_mass()const{return (GLfloat)mass;};
		void set_linear_velocity(const glm::vec3& velocity);
		void inc_linear_velocity(const glm::vec3& velocity);

		void set_mouse_pick_callback(const std::function<GLboolean(const glm::vec3& hit_coords)>& collision_callback);
		bool execute_mouse_pick_callback(const glm::vec3& hit_coords);

		bool generate_collision_volume(Collision_shape type, const std::string& modelname, const glm::vec3& scale);
		bool generate_collision_volume(Collision_shape type, const glm::vec3& scale);
		bool generate_collision_body(GLfloat mass, const btQuaternion& rotation, const glm::vec3& position);
		bool load_model_vertices(const std::string& modelname, std::vector<Vertex>& vertices);

		btRigidBody* get_collision_body()const;
		glm::vec3 get_volume_dimensions(const std::vector<Vertex>& vertices)const;
		glm::vec3 get_position()const;
		bool update_model_matrix();
		bool update_transform();
		bool fill_glm_matrix(glm::mat4& matrix)const;
		void disable_deactivation(){collision_body->setActivationState(DISABLE_DEACTIVATION);};
		void enable_deactivation(){collision_body->setActivationState(ACTIVE_TAG);};
};

typedef std::shared_ptr<Object> Object_ptr;
/*=============================================*/

#endif