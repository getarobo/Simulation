/*
 *  density.cpp
 *  3_cloth
 *
 *  Created by Gene Han on 11/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Den3.hpp"


using std::cout;
using std::endl;

//#define IX(i,j) ((i)*(22)+(j))
#define IX(i,j,k) ((i)+(22)*(j)+(484)*(k))

Den3::Den3(){
	size = 20;
	dense = new float[(size+2)*(size+2)*(size+2)];
	dt = 0.1f;
	diff= 0.3f;
	count = 0;
}

Den3::Den3(int s, float d, float df){
	size = s;
	dense = new float[(s+2)*(s+2)*(s+2)];
	dt= d;
	diff=df;
	count =0;
}

Den3::~Den3(){
	free(dense);
}

void Den3::reset(){	
	dense = new float[(size+2)*(size+2)*(size+2)];
	
}


void Den3::draw(){
	//	cout<<"drawing"<<endl;
	int i,j,k;
	float mass = 0.f;
	Vector3f start = make_vector<float>(-10.0f, 4.0f, -10.0f);
	Vector3f to_x =  make_vector<float>(1.0f, 0.0f, 0.0f);
	Vector3f to_y =  make_vector<float>(0.0f, 1.0f, 0.0f);
	Vector3f to_z =  make_vector<float>(0.0f, 0.0f, 1.0f);
	
  //boundary
	glBegin(GL_LINE_LOOP);
	glColor3f(0.f,0.f,0.f);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(start.x+20.f, start.y, start.z);
	glVertex3f(start.x+20.f, start.y+20.f, start.z);
	glVertex3f(start.x, start.y+20.f, start.z);
	glVertex3f(start.x, start.y+20.f, start.z+20.f);
	glVertex3f(start.x+20.f, start.y+20.f, start.z+20.f);
	glVertex3f(start.x+20.f, start.y+20.f, start.z);
	glVertex3f(start.x, start.y, start.z);

	glEnd();
	
	
	
	
	float color = 1.0f;
	for(i=1;i<=size;i++){
		for(j=1;j<=size;j++){
			for(k=1;k<=size;k++){
				color = dense[IX(i,j,k)]/35.0f;
			  if( dense[IX(i,j,k)] > max_d)
				max_d = dense[IX(i,j,k)];
				//	cout<< "dense[" << color <<"]"<<endl; 
			//	if(color < 0.f)
			//		color = 0.f;
			
			//	color = 9.5f/20.f;
				/*
				if(color > 1.f){
					glColor3f(1.f,1.f,1.f);
					color = 1.f;
				}else{
					glColor3f(0.f,color,color);
				}*/
			//	cout<<"color:"<<color<<endl;
		//f		color = 19.f/35.f;
				glColor3f(0.f,color,color);
				glBegin(GL_QUADS);
				glVertex3f(start.x, start.y, start.z+color);
				glVertex3f(start.x+color, start.y, start.z+color);
				glVertex3f(start.x+color, start.y, start.z);
				glVertex3f(start.x, start.y, start.z);
				glEnd();
				glBegin(GL_QUADS);
				glVertex3f(start.x, start.y+color, start.z);
				glVertex3f(start.x+color, start.y+color, start.z);
				glVertex3f(start.x+color, start.y, start.z);
				glVertex3f(start.x, start.y, start.z);
				glEnd();
				glBegin(GL_QUADS);
				glVertex3f(start.x, start.y+color, start.z);
				glVertex3f(start.x, start.y+color, start.z+color);
				glVertex3f(start.x, start.y, start.z+color);
				glVertex3f(start.x, start.y, start.z);
				glEnd();
				glBegin(GL_QUADS);
				glVertex3f(start.x+color, start.y, start.z);
				glVertex3f(start.x+color, start.y+color, start.z);
				glVertex3f(start.x+color, start.y+color, start.z+color);
				glVertex3f(start.x+color, start.y, start.z+color);
				glEnd();
				glBegin(GL_QUADS);
				glVertex3f(start.x, start.y+color, start.z);
				glVertex3f(start.x, start.y+color, start.z+color);
				glVertex3f(start.x+color, start.y+color, start.z+color);
				glVertex3f(start.x+color, start.y+color, start.z+color);
				glEnd();
				glBegin(GL_QUADS);
				glVertex3f(start.x, start.y+color, start.z+color);
				glVertex3f(start.x+color, start.y+color, start.z+color);
				glVertex3f(start.x+color, start.y, start.z+color);
				glVertex3f(start.x, start.y, start.z+color);
				glEnd();
				
				start+=to_y;
				mass += dense[IX(i,j,k)];
			}
			start.y = 4.f;
			start+=to_x;
		}
		
	
		
		start.x=-10.f;
		start+=to_z;
	}
	cout<<"count:"<< count <<endl;
	cout<<"mass:"<<mass<<endl;
	cout<<"avg:"<<mass/4000.f<<endl;
	cout<<"maxP:"<<max_d<<"\n"<<endl;
}

void Den3::test1(){
	int i,j,k;
  int max_d	=0;
	float mass = 0.f;
	for( i=0; i<=size+1;i++){
		for(j=0; j<=size+1;j++){
			for(k=0;k<size+1;k++){
				if(i == 0 || j == 0 || k ==0 || i==(size+1) || j==(size+1) || k==(size+1)){
					dense[IX(i,j,k)] = 0.f;
				}else{
					dense[IX(i,j,k)] = (float)(k-1);
					mass += (float)(k-1);
				}
			}
		}
	} 
	
	cout<<"test MASS:"<<mass<<endl;
	cout<<"test AVG :"<<mass/4000.f<<endl;
}


float * the(float * numbers, int count){
  float * average = new float[count];
  if( average == NULL)
	exit(0);
  if(count == 0)
	return average;
  if(count ==1){
	average[0]=numbers[0];
	return average;
  }
  
  average[0] = (numbers[0]+numbers[1])/2.f;
  average[count-1] = (numbers[count-2]+numbers[count-1])/2.f;
  for(int i=1; i<count-1; i++){
	average[i] = (numbers[i-1] + numbers[i] + numbers[i+1]) / 3.0f;
  }
  return average;

}













