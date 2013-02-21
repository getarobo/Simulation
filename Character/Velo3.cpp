/*
 *  Velo3.cpp
 *  3_cloth
 *
 *  Created by Gene Han on 11/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Velo3.hpp"


using std::cout;
using std::endl;

//#define IX(i,j) ((i)*(22)+(j))
#define IX(i,j,k) ((i)+(22)*(j)+(484)*(k))

Velo3::Velo3(){
	size = 20;
	visc = 0.3f;
	u = new float[(size+2)*(size+2)*(size+2)];
	v = new float[(size+2)*(size+2)*(size+2)];
	w = new float[(size+2)*(size+2)*(size+2)];
	dt = 0.1;
	//	diff= 0.1f;
}

Velo3::~Velo3(){
	free(u);
	free(v);
	free(w);
}

void Velo3::reset(){	
	u = new float[(size+2)*(size+2)*(size+2)];
	v = new float[(size+2)*(size+2)*(size+2)];
	w = new float[(size+2)*(size+2)*(size+2)];

	
}


void Velo3::draw(){
	
	int i,j,k;
	Vector3f start = make_vector<float>(-10.0f, 4.1f, -10.0f);
	Vector3f to_x =  make_vector<float>(1.0f, 0.0f, 0.0f);
	Vector3f to_y =  make_vector<float>(0.0f, 1.0f, 0.0f);
	Vector3f to_z =  make_vector<float>(0.0f, 0.0f, 1.0f);
	for(i=1;i<=size;i++){
		for(j=1;j<=size;j++){
			for(k=1;k<=size;k++){
				glBegin(GL_LINES);
				glColor3f(1.f,1.f,1.f);
				glVertex3f(start.x+0.5f, start.y, start.z+0.5f);
				glVertex3f(start.x+0.5f+(u[IX(i,j,k)]/2.f), start.y+(w[IX(i,j,k)]/2.f), start.z+0.5f+(v[IX(i,j,k)]/2.f));
				glEnd();
			
				glBegin(GL_LINES);
				glColor3f(1.f,1.f,1.f);
				glVertex3f(start.x+0.5f, start.y, start.z+0.5f);
				glVertex3f(start.x+0.5f-(u[IX(i,j,k)]/2.f), start.y-(w[IX(i,j,k)]/2.f), start.z+0.5f-(v[IX(i,j,k)]/2.f));
				glEnd();
			
				start+=to_y;
			}
			start.y = 4.f;
			start+=to_x;
		}
		start.x = -10.f;
		start+=to_z;
	}
}

void Velo3::test1(){
	int i,j,k;
	
	for(i=1;i<=size;i++){
		for(j=1;j<=size;j++){
			for(k=1;k<=size;k++){
				if(j < 10.f){
					u[IX(i,j,k)] = ((float)i/1.f);
					v[IX(i,j,k)] = ((float)j/2.f);
					w[IX(i,j,k)] = ((float)j/2.f);
				}else{
					u[IX(i,j,k)] = ((float)-i/20.f);
					v[IX(i,j,k)] = ((float)-j/20.f);
					w[IX(i,j,k)] = ((float)-j/20.f);
				}
			}
		}
	}
}
void Velo3::test2(){
//	int i,j;
	/*
	for(i=1;i<=size;i++){
		for(j=1;j<=size;j++){
			if(j < 10.f){
				u[IX(i,j)] =0.f;
				v[IX(i,j)] =0.f;
				w[IX(i,j)] =((float)j/20.f);

			}else{
				u[IX(i,j)] =0.f;
				v[IX(i,j)] =0.f;
				w[IX(i,j)] =((float)j/20.f);

			}
		}
	}*/
}


