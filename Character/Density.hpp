/*
 *  density.h
 *  3_cloth
 *
 *  Created by Gene Han on 11/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _DENSITY_HPP_
#define _DENSITY_HPP_

#include <Vector/Vector.hpp>
#include <Graphics/Graphics.hpp>


class Density{
	
public:
	
	Density();
	Density(int s, float d, float df);
	~Density();

	int count;
	
	int size;
	float *dense;
	float dt;  //time step
	float diff;//diffusion rate
		
	void reset();

	void draw();
	
	void test1();

};

#endif