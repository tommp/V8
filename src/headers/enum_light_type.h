#ifndef ENUM_LIGHT_TYPE_H
#define ENUM_LIGHT_TYPE_H


/*Included headers*/
/*---------------------------------------------*/
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
enum Light_type{
	NO_LIGHT = 0,
	DIRECTIONAL = 1,
	POINT = 2,
	SPOT = 3,
};

enum Light_dynamic{
	STAIC = 0,
	STATIONARY = 1,
	DYNAMIC = 2,
};
/*=============================================*/

#endif