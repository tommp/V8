#ifndef LEVEL_H
#define LEVEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "errorlogger.h"
#include "utility.h"
#include "enum_light_type.h"
#include "mob.h"
#include "prop.h"
#include "resource_manager.h"
#include "mousepicker.h"
#include "renderer.h"
#include "camera.h"
#include "object.h"
#include "base_light.h"
#include "dir_light.h"
#include "point_light.h"
#include "spot_light.h"
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

class World;
class Object;
class Point_light;
class Spot_light;
class Directional_light;
class Light;
class Camera;
class Renderer;

typedef std::shared_ptr<Camera> Camera_ptr;
typedef std::shared_ptr<Base_light> Light_ptr;
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

		std::list<Prop_ptr> props;
		std::list<Mob_ptr> mobs;

		std::forward_list<Light_ptr> lights;

	public:
		Level(Resource_manager& init_manager, Renderer& renderer);
		~Level();

		void update_groups();

		bool add_object(const Prop_ptr& prop);
		bool add_object(const Mob_ptr& mob);

		bool add_light(const Light_ptr& light);

		bool add_contexts_to_renderer(Renderer& renderer)const;

		bool render_geometry(Renderer& renderer);
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