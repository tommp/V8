#ifndef ACTOR_H
#define ACTOR_H


/*Included headers*/
/*---------------------------------------------*/

/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Actor{
protected:
	float x;
	float y;
	float z;
public:
	float get_x()const{return x;};
	float get_y()const{return y;};
	float get_z()const{return z;};

	void set_x(float new_x){x = new_x;};
	void set_y(float new_y){y = new_y;};
	void set_z(float new_z){z = new_z;};
};
/*=============================================*/

#endif