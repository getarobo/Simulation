/*
 *  density.h
 *  3_cloth
 *
 *  Created by Gene Han on 11/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _DENS3_HPP_
#define _DEN3_HPP_

#include <Vector/Vector.hpp>
#include <Graphics/Graphics.hpp>


class Den3{
	
public:
	
	Den3();
	Den3(int s, float d, float df);
	~Den3();
	
	int count;
	
	int size;
	float max_d;
	float *dense;
	float dt;  //time step
	float diff;//diffusion rate
	
	void reset();
	
	void draw();
	
	void test1();
	
};

#endif