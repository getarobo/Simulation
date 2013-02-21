/*
 *  Fluidflow.hpp
 *  3_cloth
 *
 *  Created by Gene Han on 11/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Fluidflow_HPP_
#define _Fluidflow_HPP_

#include <vector>

class Fluidflow{
	
public:
	
	Fluidflow();
	~Fluidflow();
	
	int size;
	int count;
	float dt;
	float diff;
	float visc;
	
	void flow_step();
	void test1();
	
	//diffuse
	void diffuse(int N, int b, float *x, float *x0, float diff, float dt);
	void diffuse_b(int N, int b, float *x, float *x0, float diff, float dt);
	
	void advect(int N, int b, float *d, float *d0, float *u, float *v, float dt);
	void project(int N, float *u, float *v, float *p, float *div);

	
	void set_bnd(int N, int b, float *x);
	void add_source(int N, float *x, float *s, float dt);

	void vel_step();
	void dens_step();
	
	void draw();
	void draw_v();
	
	void print_d(float* d, char* c);
	void print();
	
};

#endif