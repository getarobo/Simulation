/*
 *  velocity.h
 *  3_cloth
 *
 *  Created by Gene Han on 11/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VELOCITY_HPP_
#define _VELOCITY_HPP_

#include <Vector/Vector.hpp>
#include <Graphics/Graphics.hpp>


class Velocity{
	
public:
	
	Velocity();
	~Velocity();
	
	int size;
	float *u;
	float *v;
	float visc;
	float dt;  //time step
//	float diff;//diffusion rate
	
	void reset();
	
	void draw();
	
	void test1();
	void test2();

	
};

#endif