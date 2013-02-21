#ifndef _CLOTH_HPP_
#define _CLOTH_HPP_

#include "Particle.hpp"
#include "Force.hpp"
#include "SpringForce.hpp"

#include <vector>

class Cloth{

public:

	Cloth();
	~Cloth();

	void reset();
	void draw();

	void simulation_step();
	void euler_step();
	
	//my edit start
	void apply_force();
	void clear_force();
};

#endif

