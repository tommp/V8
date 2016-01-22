#include "./headers/matrix.h"

Matrix::Matrix(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(i == j) {
				matrix[i*4 + j] = 1.0f;
			}
			else{
				matrix[i*4 + j] = 0.0f;
			}
		}
	}
}

void Matrix::set_translator(const Vec3& trans){
	matrix[3] = trans(0);
	matrix[7] = trans(1);
	matrix[11] = trans(2);
}

void Matrix::set_rotator(const Quaternion& rot){

}

GLfloat Matrix::operator()(int x, int y) const{
	if((x*4 + y) > 15){
		fprintf (stderr, "Matrix error: Index out of range %d at %s, line %d.", (x*4 + y), __FILE__, __LINE__);
		exit(0);
	}
	return matrix[x*4 + y];
}