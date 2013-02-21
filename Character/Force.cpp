#include "Force.hpp"

Force::Force(){
	fVector = make_vector<float>(0.0f,0.0f,0.0f);
}

Force::~Force(){}

//my edit start
void Force::add_force(Vector3f add){

	fVector += add;
}

void Force::mult_force(Vector3f mult){

	//fVector *= mult;
}

void Force::set_force(Vector3f tobe){
	fVector = tobe;
}
//my edit end