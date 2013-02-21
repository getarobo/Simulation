/*
 *  Velo3.hpp
 *  3_cloth
 *
 *  Created by Gene Han on 11/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _VELO3_HPP_
#define _VELO3_HPP_

#include <Vector/Vector.hpp>
#include <Graphics/Graphics.hpp>


class Velo3{
	
public:
	
	Velo3();
	~Velo3();
	
	int size;
	float *u;
	float *v;
	float *w;
	float visc;
	float dt;  //time step
	//	float diff;//diffusion rate
	
	void reset();
	
	void draw();
	
	void test1();
	void test2();
	
	
};

#endif