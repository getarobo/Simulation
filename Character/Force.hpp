#ifndef _FORCE_HPP_
#define _FORCE_HPP_


#include <Vector/Vector.hpp>
#include <Graphics/Graphics.hpp>

class Force{

public:

	Force();
	virtual ~Force();

	virtual void draw() = 0;
	virtual void apply_force() = 0;
	
	
	Vector3f fVector;
	void add_force(Vector3f add);

	void mult_force(Vector3f mult);

	
	void set_force(Vector3f tobe);
};

#endif
