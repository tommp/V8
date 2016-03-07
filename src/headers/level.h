#ifndef LEVEL_H
#define LEVEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
#include "utility.h"
#include "enum_light_type.h"
#include "mobs.h"
#include "prop.h"
#include "resource_manager.h"
#include "mousepicker.h"
#include "camera.h"
#include "object.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <vector>
#include <memory>
#include <forward_list>
#include <list>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
using std::vector;

class Tile;
class World;
class Object;
class Point_light;
class Spot_light;
class Directional_light;
class Light;
class Camera;

typedef std::shared_ptr<Camera> Camera_ptr;
typedef std::shared_ptr<Light> Light_ptr;
typedef std::shared_ptr<Object> Object_ptr;
typedef std::shared_ptr<Point_light> Point_light_ptr;
typedef std::shared_ptr<Spot_light> Spot_light_ptr;
typedef std::shared_ptr<Directional_light> Directional_light_ptr;

class Level {
	friend class World;
	private:
		std::string name;
		GLuint width;
		GLuint height;
		GLuint depth;

		Camera_ptr camera;
		Mousepicker_ptr mousepicker;

		/* ==================== Physics stuff ===================== */
		btDiscreteDynamicsWorld* physics_world;
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;

		glm::vec3 gravity;
		/* ======================================================== */

		std::vector<const std::list<Object_ptr>*> rendering_targets;

		std::list<Object_ptr> objects;
		std::forward_list<Object_ptr> dormant_objects;

		std::forward_list<Light_ptr> dir_lights;
		std::forward_list<Light_ptr> point_lights;
		std::forward_list<Light_ptr> spot_lights;
	public:
		Level(Resource_manager& init_manager);
		~Level();

		void update_groups();

		bool add_active_object(const Object_ptr& Object);
		bool add_dormant_object(const Object_ptr& Object);
		bool add_object(const Object_ptr& Object);

		bool add_dir_light(const Light_ptr& light);
		bool add_point_light(const Light_ptr& light);
		bool add_spot_light(const Light_ptr& light);

		void render_geometry(Renderer& renderer);
		void render_lights(const Renderer& renderer)const;
		void render_level(Renderer& renderer);

		void load_level();
		void unload_level();
		void save_level();
		void generate_level();

		bool init_physics();
		void set_gravity(const glm::vec3& gravity);
		void update_gravity();
		bool add_to_physics_world(btRigidBody* Object)const;
		bool remove_from_physics_world(btRigidBody* Object)const;
		
		bool resolve_collisions();
		bool check_if_offscreen(const Object_ptr& a)const;
		bool update_positions(float timedelta, Renderer& renderer);
};

typedef std::shared_ptr<Level> Level_ptr;
/*=============================================*/

#endif