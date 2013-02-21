/*
 *  Flo3.hpp
 *  3_cloth
 *
 *  Created by Gene Han on 11/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _Flo3_HPP_
#define _Flo3_HPP_

#include <vector>

class Flo3{
	
public:
	
	Flo3();
	~Flo3();
	
	int size;
	int count;
	float dt;
	float diff;
	float visc;
	
	void flo3_step();
	void test1();
	
	//diffuse
	void diffuse(int N, int b, float *x, float *x0, float diff, float dt);
	void diffuse_b(int N, int b, float *x, float *x0, float diff, float dt);
	
	void advect(int N, int b, float *d, float *d0, float *u, float *v, float *w, float dt);
	void project(int N, float *u, float *v, float *w, float *p, float *div);
	
	
	void set_bnd(int N, int b, float *x);
	void add_source(int N, float *x, float *s, float dt);
	void add_random_v();
  void add_gravity();

	
	void vel_step();
	void dens_step();
	
	void draw();
	void draw_v();
	
	void print_d(float* d, char* c);
	void print();
	
};

#endif