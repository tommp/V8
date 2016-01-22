#include "./headers/vector.h"

Vec3::Vec3(){
	for(int i = 0; i < 3; i++) {
		vector[i] = 0;
	}
}

GLfloat Vec3::operator()(int x) const{
	if((x) > 2){
		fprintf (stderr, "Vector error: Index out of range %d at %s, line %d.", x, __FILE__, __LINE__);
		exit(0);
	}
	return vector[x];
}

GLfloat& operator[](int x){
	if((x) > 2){
		fprintf (stderr, "Vector error: Index out of range %d at %s, line %d.", x, __FILE__, __LINE__);
		exit(0);
	}
	return &(vector[x]);
}