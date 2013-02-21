/*
 *  fluidflow.cpp
 *  3_cloth
 *
 *  Created by Gene Han on 11/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */




#include "Fluidflow.hpp"
#include "Density.hpp"
#include "Velocity.hpp"

static Density dens;
static Velocity velo;
using std::cout;
using std::cerr;
using std::endl;

//#define IX(i,j) ((i)*(22)+(j))
#define IX(i,j) ((i)+(22)*(j))
#define SWAP(x0,x) {float* tmp=x0;x0=x;x=tmp;}
	

Fluidflow::Fluidflow(){
	size = 20;
	dt= 0.01f;
	diff = 0.3f;
	visc =0.3f;
	count =0;
}
Fluidflow::~Fluidflow(){}

void Fluidflow::flow_step(){
	vel_step();
	dens_step();
	dens.count += 1;
}

void Fluidflow::test1(){
	dens.test1();
	velo.test1();
}

void Fluidflow::diffuse(int N, int b, float *x, float *x0, float diff, float dt){
	int i,j,k;
	float a = dt*diff*N*N;
	for(k=0; k<1000; k++){
		for(i=1;i<=N;i++){
			for(j=1;j<=N;j++){
				x[IX(i,j)] = (x0[IX(i,j)] +a*(x[IX(i-1,j)]+x[IX(i+1,j)] +
							                  x[IX(i,j-1)]+x[IX(i,j+1)]))/(1.f+4.f*a);
			}
		}
		set_bnd(N,b,x);
	}

}


void Fluidflow::diffuse_b(int N, int b, float *x, float *x0, float diff, float dt){
	int i, j;
	float a=dt*diff*N*N;
	
	for( i=1; i <=N; i++){
		for(j=1; j<=N; j++){
			x[IX(i,j)] = x0[IX(i,j)] + a*(x0[IX(i-1,j)] + x0[IX(i+1,j)]
										  +x0[IX(i,j-1)] +x0[IX(i,j+1)] -4*x0[IX(i,j)]);
		}
	}
	set_bnd(N,b,x);
}

void Fluidflow::advect(int N, int b, float *d, float *d0, float *u, float *v, float dt){
	int i, j, i0, j0, i1, j1;
	float x, y, s0, s1, t0, t1, dt0;
	
	dt0 = dt*N;
	for(i=1; i<=N; i++){
		for(j=1; j<=N; j++){
			x = i-dt0*u[IX(i,j)];
			y = j-dt0*v[IX(i,j)];
			if(x<0.5f)
				x = 0.5f;
			if(x>(N+0.5f))
				x = N+0.5f;
			i0 = (int)x;
			i1=i0+1;
			
			if(y<0.5f)
				y = 0.5f;
			if(y>(N+0.5f))
				y = N+0.5f;
			j0 = (int)y;
			j1=j0+1;
			
			s1 = x-(float)i0;
			s0 = 1.f-s1;
			t1 = y -(float)j0;
			t0 = 1.f-t1;
			
			d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)] + t1*d0[IX(i0, j1)]) +
						 s1*(t0*d0[IX(i1,j0)] + t1*d0[IX(i1, j1)]);
		}
	}
	set_bnd(N,b,d);
	
}

//void Fluidflow::dens_step(int N, float *x, float *x0, float *u, float *v, float diff, float dt){
void Fluidflow::dens_step(){

	float *x = new float[(size+2)*(size+2)];
	add_source(size,x,dens.dense,dt);

	diffuse(size,0,dens.dense,x,diff,dt);

	//print_d(dens.dense,"d before");
//	advect(size, 0,dens.dense,x,velo.u,velo.v,dt);
	advect(size, 0,x,dens.dense,velo.u,velo.v,dt);

	//print_d(dens.dense,"d after");

	
	dens.dense = x;
	dens.count += 1;

}

void Fluidflow::vel_step(){
	float *u = new float[(size+2)*(size+2)];
	float *v = new float[(size+2)*(size+2)];

	
	add_source(size, u, velo.u, dt);
	add_source(size, v, velo.v, dt);
//	SWAP(velo.velo_u,u);
	diffuse(size,1,velo.u,u,visc,dt);
//	SWAP(velo.velo_v,v);
//	velo.velo_u=u;
	
	diffuse(size,2,velo.v,v,visc,dt);
//	velo.velo_v=v;
	
	project(size, velo.u,velo.v,u,v);
	//project(size,u,v, velo.velo_u,velo.velo_v);
//	velo.velo_u = u;velo.velo_v = v;
	//SWAP(velo.velo_u,u);SWAP(velo.velo_v,v);
//	velo.velo_u=u;velo.velo_v=v;

	advect(size,1,u,velo.u,velo.u,velo.v,dt);
//	velo.velo_u=u;
	
	advect(size,2,v,velo.v,velo.u,velo.v,dt);
//	velo.velo_v=v;
	
	project(size,u,v,velo.u,velo.v);
	velo.u = u;velo.v = v;

}


void Fluidflow::project(int N, float *u, float *v, float *p, float *div){
	float h = 1.f/N;
	int i,j,k;
	for(i =1; i<=N;i++){
		for( j =1; j <=N;j++){
			div[IX(i,j)] = -0.5f*h*(u[IX(i+1,j)]-u[IX(i-1,j)] + 
								    v[IX(i,j+1)] -v[IX(i,j-1)]);
			p[IX(i,j)] =0;
		}
	}
	set_bnd(N,0,div);
	set_bnd(N,0,p);
	
	for( k=0; k<100; k++){
		for(i =1; i<=N;i++){
			for(j =1; j <=N;j++){
				p[IX(i,j)] = (div[IX(i,j)]+p[IX(i-1,j)] + p[IX(i+1,j)] + 
										   p[IX(i,j-1)] +p[IX(i,j+1)])/4;
			}
		}
		set_bnd(N,0,p);
	}
	
	for(i=1; i<=N; i++){
		for(j=1; j<=N; j++){
			u[IX(i,j)] -= 0.5f*(p[IX(i+1,j)]-p[IX(i-1,j)])/h;
			v[IX(i,j)] -= 0.5f*(p[IX(i,j+1)]-p[IX(i,j-1)])/h;
		}
	
	}
	set_bnd(N,1,u);
	set_bnd(N,2,v);

}

void Fluidflow::set_bnd(int N, int b, float *x){
	int i;
	for(i=1; i<=N; i++){ 
		x[IX(0,i)]   = b==1 ? -x[IX(1,i)] : x[IX(1,i)];
		x[IX(N+1,i)] = b==1 ? -x[IX(N,i)] : x[IX(N,i)];
		x[IX(i,0)]   = b==2 ? -x[IX(i,1)] : x[IX(i,1)];
		x[IX(i,N+1)] = b==2 ? -x[IX(i,N)] : x[IX(i,N)];
	}
	x[IX(0,0)]     = 0.5*(x[IX(1,0)] + x[IX(0,1)]);
	x[IX(0,N+1)]   = 0.5*(x[IX(1,N+1)] + x[IX(0,N)]);
	x[IX(N+1,0)]   = 0.5*(x[IX(N,0)] + x[IX(N+1,1)]);
	x[IX(N+1,N+1)] = 0.5*(x[IX(N,N+1)] + x[IX(N+1,N)]);
}

void Fluidflow::add_source(int N, float *x, float *s, float dt){
	int i, size =(N+2)*(N+2);
	for(i=0; i<size;i++){
		x[i] = s[i];
	}
}


void Fluidflow::draw(){
	dens.draw();
	velo.draw();
//	print_d(velo.velo_u);
}

void Fluidflow::draw_v(){
	velo.draw();
}

void Fluidflow::print(){
	float count=0.f;
	int i,j;
	for(i=1;i<=20;i++){
		for(j=1;j<=20;j++){
			cout<<"["<<dens.dense[IX(i,j)]<<"]";
			count += dens.dense[IX(i,j)];
		}
		cout<<"==i"<<i<<endl;
	}
	cout<<"count"<<dens.count <<endl;
	cout<<"mass:"<<count<< endl;
	cout<<"avg:"<<count/400.f<<endl;
}

void Fluidflow::print_d(float* d, char* c){
	int i,j;
	float count=0.f;
	cout<<c<<endl;
	for(i=1;i<=20;i++){
		for(j=1;j<=20;j++){
			cout<<"["<<d[IX(i,j)]<<"]";
			count += dens.dense[IX(i,j)];

		}
		cout<<"==i"<<i<<endl;

	}

	cout<<"mass:"<<count<<endl;

}









































