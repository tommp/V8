#ifndef DEFINES_H
#define DEFINES_H

#define DISPLAY_SETTINGS_FILE_PATH "assets/config/display.co"
#define ANIMATION_DATA_PATH "assets/animations/"
#define IMAGE_DATA_PATH "assets/raw_images/"
#define TEXTURE_DATA_PATH "assets/textures/"
#define MESH_DATA_PATH "assets/meshes/"
#define MODEL_DATA_PATH "assets/models/"
#define RAW_MODEL_DATA_PATH "assets/raw_models/"
#define MATERIAL_DATA_PATH "assets/materials/"
#define SHADER_PATH "src/shaders/"

#define SCRIPT_PATH "scr/scripts/"
#define SCRIPT_POSTFIX ".mol"

#define ENGINE_DEFAULT_MATERIAL_NAME "engine_default_material"
#define MAX_FILENAME_LENGTH 500
#define REBUILD_ASSETS 1
#define DISABLE_VSYNC 0
#define ENABLE_BULLET_DEBUG 0
#define SHADOW_FACTOR 3

/* STATIC LINKAGE, CHANGE TO DYNAMIC LATER */
#define GLEW_STATIC 1

/* Scripting defines */
#define S_BOOL 			  1
#define S_INT 			  2
#define S_FLOAT 		  3
#define S_VEC3  		  4
#define S_STRING 		  5
#define S_B_BOOL 		 -1
#define S_B_INT 		 -2
#define S_B_FLOAT 		 -3
#define S_B_VEC3  		 -4
#define S_B_STRING 		 -5

#endif