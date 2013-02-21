/*
 *  density.cpp
 *  3_cloth
 *
 *  Created by Gene Han on 11/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Density.hpp"



using std::cout;
using std::endl;

//#define IX(i,j) ((i)*(22)+(j))
#define IX(i,j) ((i)+(22)*(j))

Density::Density(){
	size = 20;
	dense = new float[(size+2)*(size+2)];
	dt = 0.01f;
	diff= 0.1f;
	count = 0;
}

Density::Density(int s, float d, float df){
	size = s;
	dense = new float[(s+2)*(s+2)];
	dt= d;
	diff=df;
	count =0;
}

Density::~Density(){
	free(dense);
}
	
void Density::reset(){	
	dense = new float[(size+2)*(size+2)];
	
}
	
	
void Density::draw(){
//	cout<<"drawing"<<endl;
	int i,j;
	float mass = 0.f;
	Vector3f start = make_vector<float>(-10.0f, 4.0f, -10.0f);
	Vector3f to_x =  make_vector<float>(1.0f, 0.0f, 0.0f);
	Vector3f to_z =  make_vector<float>(0.0f, 0.0f, 1.0f);
	float color = 1.0f;
	for(i=1;i<=size;i++){
		for(j=1;j<=size;j++){
			color = dense[IX(i,j)]/20.0f;
		//	cout<< "dense[" << color <<"]"<<endl; 
		//	if(color < 0.f)
		//		color = 0.f;
			
		//	color = 9.5f/20.f;
			if(color > 1.2f){
				glColor3f(1.f,1.f,1.f);
			}else{
				glColor3f(0.f,color,color);
			}
			glBegin(GL_QUADS);
				glVertex3f(start.x, start.y, start.z+1.0f);
				glVertex3f(start.x+1.0f, start.y, start.z+1.0f);
				glVertex3f(start.x+1.0f, start.y, start.z);
				glVertex3f(start.x, start.y, start.z);
			glEnd();
			start+=to_x;
			mass += dense[IX(i,j)];
			cout<<"["<<dense[IX(i,j)]<<"]";
		}
		cout<<"==i"<<i<<endl;

		start.x=-10.f;
		start+=to_z;
	}
	cout<<"count:"<< count <<endl;
	cout<<"mass:"<<mass<<endl;
	cout<<"avg:"<<mass/400.f<<endl;
	//cout<<"mass: "<<mass<<endl;
}

void Density::test1(){
	int i,j;
	for( i=1; i<=size;i++){
		for(j=1; j<=size;j++){
			dense[IX(i,j)] = (float)(i-1);
		
		}
	} 
}














