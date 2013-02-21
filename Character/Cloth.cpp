#include "Cloth.hpp"
#include "SpringForce.hpp"

static std::vector<Particle*> pVector;
static std::vector< std::vector<Force*> > fVector;

static float dt;

#define RAND (((rand()%2000)/1000.f)-1.f)
#define DAMP 0.98f


//my edit
using std::cout;
using std::cerr;
using std::endl;


Cloth::Cloth(){

	int i,j;
	double ks, kd;
	Vector3f start = make_vector<float>(-8.0f, 10.0f, 0.0f);
	Vector3f to_y = make_vector<float>(0.0f, 1.0f, 0.0f);
	Vector3f to_x = make_vector<float>(1.0f, 0.0f, 0.0f);
	
  
  /*****************
   TIME SETP
   *****************/
   dt = 0.01;
  /*****************
   Spirng Stiffness
   *****************/
	kd = 5;

	
	//16by16 particles
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			Particle *p = new Particle(start);
			p->m_ForceAccumulator += make_vector<float>(0.0f,-9.8f,0.0f); 
			pVector.push_back(p);
			start += to_x;
		}
		start = make_vector<float>(-8.0f,start.y, start.z);
		start += to_y;
		
	}
//	cout<<"pvec size: "<<pVector.size()<<endl;

	//add spring force
	//set kd,ks
	double dist= 1;

	ks =25;
	for(i = 0; i < 16; i++){
		
		for(j = 0; j < 16; j++){
			fVector.push_back(std::vector<Force*>());
			//structrue = 1 red
			if( i != 15){
	//			dist = length(pVector[16*i+j]->m_Position - pVector[16*(i+1)+j]->m_Position);
				fVector[(16*i)+j].push_back(new SpringForce(pVector[16*i+j], pVector[16*(i+1)+j],dist,ks,kd,0,1 ));
			}else{
				fVector[(16*i)+j].push_back(new SpringForce(pVector[0], pVector[0],dist,1.0,1.0,1,2 ));
			}
			if( j != 15){
	//			dist = length(pVector[16*i+j]->m_Position - pVector[16*i+j+1]->m_Position);
				fVector[(16*i)+j].push_back(new SpringForce(pVector[16*i+j], pVector[16*i+j+1],dist,ks,kd,0,1 ));
			}else{
				fVector[(16*i)+j].push_back(new SpringForce(pVector[i+j], pVector[i+j],dist,1.0,1.0,1,2 ));
			
			}
			//shear = 2 green
			
			if( i != 15 && j != 15){
	//			dist = length(pVector[16*i+j]->m_Position - pVector[16*(i+1)+j+1]->m_Position);
				fVector[(16*i)+j].push_back(new SpringForce(pVector[16*i+j], pVector[16*(i+1)+j+1],dist,ks,kd,0,2 ));
			}else{
				fVector[(16*i)+j].push_back(new SpringForce(pVector[0], pVector[0],dist,1.0,1.0,1,2 ));
			}
			if( i != 15 && j != 15){
	//			dist = length(pVector[16*(i+1)+j]->m_Position - pVector[16*i+j+1]->m_Position);
				fVector[(16*i)+j].push_back(new SpringForce(pVector[16*(i+1)+j], pVector[16*i+j+1],dist,ks,kd,0,2 ));
			}else{
				fVector[(16*i)+j].push_back(new SpringForce(pVector[i+j], pVector[i+j],dist,1.0,1.0,1,2 ));
			}
		//	dist = 2.0;
			//flexion =3 blue
			if( i < 14 && j < 14){
	//			dist = length(pVector[16*i+j]->m_Position - pVector[16*(i+2)+j]->m_Position);
				fVector[(16*i)+j].push_back(new SpringForce(pVector[16*i+j], pVector[16*(i+2)+j],dist,ks,kd,0,3 ));
			}else{
				fVector[(16*i)+j].push_back(new SpringForce(pVector[0], pVector[0],dist,1.0,1.0,1,2 ));
			}
			if( i < 14 && j < 14){
	//			dist = length(pVector[16*i+j]->m_Position - pVector[16*i+j+2]->m_Position);
				fVector[(16*i)+j].push_back(new SpringForce(pVector[16*i+j], pVector[16*i+j+2],dist,ks,kd,0,3 ));
			}else{
				fVector[(16*i)+j].push_back(new SpringForce(pVector[i+j], pVector[i+j],dist,1.0,1.0,1,2 ));
			}
			
		}
	}

	//cout<<"fvec size: "<< fVector.size()<<endl;
	
}

Cloth::~Cloth(){
	pVector.clear();
	fVector.clear();
}

void Cloth::reset(){
	int size = pVector.size();
	for(int ii=0; ii<size; ii++){
		pVector[ii]->reset();
	}
}

void Cloth::draw(){
	
	int size = pVector.size();
	
	for(int ii=0; ii<size; ii++){
		pVector[ii]->draw();
	}
	
	size = fVector.size();
	for(int ii=0; ii<size; ii++){
		fVector[ii][0]->draw();
		fVector[ii][1]->draw();
		fVector[ii][2]->draw();
		fVector[ii][3]->draw();
		fVector[ii][4]->draw();
		fVector[ii][5]->draw();
		
	}
}

void Cloth::simulation_step(){

	///TODO:
	
	///first, you need to clear force accumulators for all the particles

	///second, apply forces to them

	///if you want to implement hard constraints, the third step is to calculate constraint forces
	///for the basic cloth simulation, you can skip this.

	///Then, we can move forward
	///Change this to others if you want to implement RK2 or RK4 or other integration method
//	euler_step();

	///Finally, if you want to implement collisions, you could solve them here
	///for the basic cloth simulation, you can skip this.

	
	
	clear_force();
	apply_force();
//	for(int i =0; i< 256; i++)
//		cout<<"forces are :"<< pVector[i]->count_force<<endl;
	
	euler_step();
	
	
}


void Cloth::clear_force(){
	int i,size;
	size = pVector.size();
	for(i=0; i< size; i++){
		pVector[i]->clearForce();
	}
	
}

void Cloth::apply_force(){
	int i,size;
	size = fVector.size();
	for(i=0; i< size; i++){
		fVector[i][0]->apply_force();
		fVector[i][1]->apply_force();
		fVector[i][2]->apply_force();
		fVector[i][3]->apply_force();
		fVector[i][4]->apply_force();
		fVector[i][5]->apply_force();
	}

}




void Cloth::euler_step(){
	///TODO:
	///Modify things below to take a real euler step
	int ii, size = pVector.size();
	Vector3f an;
	for(ii=0; ii<size; ii++){
		if(ii == 240 || ii == 247 || ii ==255){
			pVector[ii]->m_Velocity += make_vector<float>(0.0f, 0.0f, 0.0f); 
			pVector[ii]->m_Position += make_vector<float>(0.0f, 0.0f, 0.0f);
		
		}else{
			an = (pVector[ii]->m_ForceAccumulator)/(pVector[ii]->m_Mass);
			pVector[ii]->m_Velocity += dt*an; 
			pVector[ii]->m_Position += pVector[ii]->m_Velocity*dt;
		}
	}
}

























