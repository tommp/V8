#ifndef ENUM_SHADER_TYPE_H
#define ENUM_SHADER_TYPE_H


/*Included headers*/
/*---------------------------------------------*/
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
enum Shader_type{
	NO_SHADER = 0,
	GEOMETRY_ANIMATED = 1,
	GEOMETRY_STATIC = 2,
	GEOMETRY_ANIMATED_COLORED = 3,
	GEOMETRY_STATIC_COLORED = 4,
	LIGHT_POINT = 5,
	LIGHT_SPOT = 6,
	LIGHT_DIRECTIONAL = 7,
};
/*=============================================*/

#endif