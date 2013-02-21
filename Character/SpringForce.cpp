#include "SpringForce.hpp"

//my edit
#include <math.h>
#include <stdio.h>
using std::cout;
using std::cerr;
using std::endl;


SpringForce::SpringForce(Particle *p1, Particle *p2, double dist, double ks, double kd, int it, int what) : 
m_p1(p1), m_p2(p2), m_dist(dist), m_ks(ks), m_kd(kd), itself(it), kind(what){
}




void SpringForce::draw()
{
	if(!itself){
		if(kind == 1){
			glBegin( GL_LINES );
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f( m_p1->m_Position.x, m_p1->m_Position.y, m_p1->m_Position.z);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f( m_p2->m_Position.x, m_p2->m_Position.y, m_p2->m_Position.z);
			glEnd();
		}else if( kind == 2){
			glBegin( GL_LINES );
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f( m_p1->m_Position.x, m_p1->m_Position.y, m_p1->m_Position.z);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f( m_p2->m_Position.x, m_p2->m_Position.y, m_p2->m_Position.z);
			glEnd();
		}else{
			glBegin( GL_LINES );
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f( m_p1->m_Position.x, m_p1->m_Position.y, m_p1->m_Position.z);
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f( m_p2->m_Position.x, m_p2->m_Position.y, m_p2->m_Position.z);
			glEnd();
		}
	}
}

void SpringForce::apply_force()
{
	if(itself){
	//	cout<<"im myself"<<endl;
	}else{
	//just to look little nicer
	Vector3f v1 = m_p1->m_Velocity;
	Vector3f v2 = m_p2->m_Velocity;
	Vector3f p1 = m_p1->m_Position;
	Vector3f p2 = m_p2->m_Position;

	//calc spring-Dampers
	float tmp1 = m_ks * ( length(p1 - p2) - (float)m_dist) + m_kd * ( (v1-v2)*(p1 - p2)/length(p1 - p2) );
	Vector3f damp = -(tmp1)*( (p1-p2)/length(p1-p2)  );
	
	//calc g force assuming all particle weighs the same
	//also soft wind is added
	  Vector3f g = make_vector(0.0f,-0.1f,0.0f);
		//m_p1->m_Mass*make_vector<float>(0.0f,-0.1f,-0.05f); 
//		cout<<"damp: "<< damp.x <<", "<<damp.y<<", "<<damp.z<<" length:"<< (length(damp)) <<endl;
	//apply force to particles
	m_p1->m_ForceAccumulator += damp +g;
	m_p2->m_ForceAccumulator += g - damp;
	
//	cout<<"fcount 1-1: "<< m_p1->count_force<<endl;
	m_p1->count_force +=1;
//	cout<<"fcount 1-2: "<< m_p1->count_force<<endl;
	
//	cout<<"fcount 2-1: "<< m_p2->count_force<<endl;
	m_p2->count_force +=1;
//	cout<<"fcount 2-2: "<< m_p2->count_force<<endl;
	
	//save spring damping force
	s_force = damp + g;
	}
	
}