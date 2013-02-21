/*
 *  velocity.cpp
 *  3_cloth
 *
 *  Created by Gene Han on 11/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Velocity.hpp"

using std::cout;
using std::endl;

//#define IX(i,j) ((i)*(22)+(j))
#define IX(i,j) ((i)+(22)*(j))

Velocity::Velocity(){
	size = 20;
	visc = 0.01f;
	u = new float[(size+2)*(size+2)];
	v = new float[(size+2)*(size+2)];

	dt = 0.01;
//	diff= 0.1f;
}

Velocity::~Velocity(){
	free(u);
	free(v);
}

void Velocity::reset(){	
	u = new float[(size+2)*(size+2)];
	v = new float[(size+2)*(size+2)];
	
}


void Velocity::draw(){
	int i,j;
	Vector3f start = make_vector<float>(-10.0f, 4.1f, -10.0f);
	Vector3f to_x =  make_vector<float>(1.0f, 0.0f, 0.0f);
	Vector3f to_z =  make_vector<float>(0.0f, 0.0f, 1.0f);
	for(i=1;i<=size;i++){
		for(j=1;j<=size;j++){
			
			glBegin(GL_LINES);
				glColor3f(1.f,1.f,1.f);
				glVertex3f(start.x+0.5f, start.y, start.z+0.5f);
				glVertex3f(start.x+0.5f+(u[IX(i,j)]/2.f), start.y, start.z+0.5f+(v[IX(i,j)]/2.f));
			glEnd();
			
			glBegin(GL_LINES);
				glColor3f(1.f,1.f,1.f);
				glVertex3f(start.x+0.5f, start.y, start.z+0.5f);
				glVertex3f(start.x+0.5f-(u[IX(i,j)]/2.f), start.y, start.z+0.5f-(v[IX(i,j)]/2.f));
			glEnd();
			
			start+=to_x;
		}
		start.x = -10.f;
		start+=to_z;
	}
}

void Velocity::test1(){
	int i,j;
	
	for(i=1;i<=size;i++){
		for(j=1;j<=size;j++){
			if(j < 10.f){
				u[IX(i,j)] =((float)i/10.f);
				v[IX(i,j)] =((float)j/20.f);
			}else{
				u[IX(i,j)] =((float)i/20.f);
				v[IX(i,j)] =((float)-j/20.f);
			}
		}
	}
}
void Velocity::test2(){
	int i,j;
	
	for(i=1;i<=size;i++){
		for(j=1;j<=size;j++){
			if(j < 10.f){
				u[IX(i,j)] =0.f;
				v[IX(i,j)] =0.f;
			}else{
				u[IX(i,j)] =0.f;
				v[IX(i,j)] =0.f;
			}
		}
	}
}


