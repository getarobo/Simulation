/*
 *  flo3.cpp
 *  3_cloth
 *
 *  Created by Gene Han on 11/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */




#include "Flo3.hpp"
#include "Den3.hpp"
#include "Velo3.hpp"
#include <time.h>

static Den3 den3;
static Velo3 vel3;
using std::cout;
using std::cerr;
using std::endl;

//#define IX(i,j) ((i)*(22)+(j))
#define IX(i,j,k) ((i)+(22)*(j)+(484)*(k))
#define SWAP(x0,x) {float* tmp=x0;x0=x;x=tmp;}


Flo3::Flo3(){
	size = 20;
	dt= 0.1f;
	diff = 0.1f;
	visc =0.1f;
	count =0;
}
Flo3::~Flo3(){}

void Flo3::flo3_step(){
	vel_step();
	dens_step();
//	add_gravity();
	//add_random_v();

}

void Flo3::draw(){
	den3.draw();
//	vel3.draw();
}

void Flo3::test1(){
	den3.test1();
	vel3.test1();
}




void Flo3::diffuse(int N, int b, float *x, float *x0, float diff, float dt){
	int i,j,k,G;
	float a = dt*diff*N*N*N;
	for(G=0; G<300;G++){
		for(i=1;i<=N;i++){
			for(j=1;j<=N;j++){
				for(k=1;k<=N;k++){
					x[IX(i,j,k)] = (x0[IX(i,j,k)] +a*(x[IX(i-1,j,k)]+x[IX(i+1,j,k)] +
					   		                          x[IX(i,j-1,k)]+x[IX(i,j+1,k)] +
												      x[IX(i,j,k-1)]+x[IX(i,j,k+1)]))/(1.f+6.f*a);
				}
			}
		}
		set_bnd(N,b,x);
	}
	
}


void Flo3::diffuse_b(int N, int b, float *x, float *x0, float diff, float dt){
	int i, j, k;
	float a=dt*diff*N*N;
	
	for( i=1; i <=N; i++){
		for(j=1; j<=N; j++){
			for(k=1; k<=N; k++){
				x[IX(i,j,k)] = x0[IX(i,j,k)] + a*( x0[IX(i-1,j,k)] + x0[IX(i+1,j,k)]
											 +     x0[IX(i,j-1,k)] + x0[IX(i,j+1,k)]
											 +     x0[IX(i,j,k-1)] + x0[IX(i,j,k+1)]
											 - 6.f*x0[IX(i,j,k)]);
			}
		}
	}
	set_bnd(N,b,x);
}

void Flo3::advect(int N, int b, float *d, float *d0, float *u, float *v, float *w, float dt){
	int i, j, k, i0, j0, i1, j1, k0, k1;
	float x, y, z, s0, s1, t0, t1, u0, u1, dt0;
	
	dt0 = dt*N;
	for(i=1; i<=N; i++){
		for(j=1; j<=N; j++){
			for(k=1; k<=N; k++){
				x = i-dt0*u[IX(i,j,k)];
				y = j-dt0*v[IX(i,j,k)];
				z = k-dt0*w[IX(i,j,k)];
				
				//x
				if(x<0.5f)
					x = 0.5f;
				if(x>(N+0.5f))
					x = N+0.5f;
				i0 = (int)x;
				i1=i0+1;
			
				//y
				if(y<0.5f)
					y = 0.5f;
				if(y>(N+0.5f))
					y = N+0.5f;
				j0 = (int)y;
				j1=j0+1;
			
				//z
				if(z<0.5f)
					z = 0.5f;
				if(z>(N+0.5f))
					z = N+0.5f;
				k0 = (int)z;
				k1=k0+1;
				
				s1 = x-(float)i0;
				s0 = 1.f-s1;
				t1 = y -(float)j0;
				t0 = 1.f-t1;
				u1 = z-(float)k0;
				u0 = 1.f-u1;
				
				d[IX(i,j,k)] = s0*(t0*u0*d0[IX(i0,j0,k0)] + t1*u0*d0[IX(i0, j1,k0)]) +
							   s0*(t0*u1*d0[IX(i0,j0,k1)] + t1*u1*d0[IX(i0, j1,k1)]) +
							   s1*(t0*u0*d0[IX(i1,j0,k0)] + t1*u0*d0[IX(i1, j1,k0)]) +
							   s1*(t0*u1*d0[IX(i1,j0,k1)] + t1*u1*d0[IX(i1, j1,k1)]) ;

				
			}
		}
	}
	set_bnd(N,b,d);
	
}

//void Fluidflow::dens_step(int N, float *x, float *x0, float *u, float *v, float diff, float dt){
void Flo3::dens_step(){

	float *x = new float[(size+2)*(size+2)*(size+2)];
	
	add_source(size,x,den3.dense,dt);
	advect(size, 0,x,den3.dense,vel3.u,vel3.v,vel3.w,dt);
    diffuse(size,0,den3.dense,x,diff,dt);

	den3.dense = x;
	den3.count += 1;

	
}

void Flo3::vel_step(){
	float *u = new float[(size+2)*(size+2)*(size+2)];
	float *v = new float[(size+2)*(size+2)*(size+2)];
	float *w = new float[(size+2)*(size+2)*(size+2)];

	add_source(size, u, vel3.u, dt);
	add_source(size, v, vel3.v, dt);
	add_source(size, w, vel3.w, dt);
  
  advect(size,1,u,vel3.u,vel3.u,vel3.v,vel3.w,dt);
  advect(size,2,v,vel3.v,vel3.u,vel3.v,vel3.w,dt);
  advect(size,3,w,vel3.w,vel3.u,vel3.v,vel3.w,dt);
  
	diffuse(size,1,vel3.u,u,visc,dt);
	diffuse(size,2,vel3.v,v,visc,dt);
	diffuse(size,3,vel3.w,w,visc,dt);

	project(size, vel3.u,vel3.v,vel3.w,u,v);

	
	//project(size,u,v,w,vel3.u,vel3.v);
	
	vel3.u = u;
	vel3.v = v;
	vel3.w = w;
}


void Flo3::project(int N, float *u, float *v, float *w, float *p, float *div){
	float h = 1.f/(N);
	int i,j,k,G;
	for(i =1; i<=N;i++){
		for( j =1; j <=N;j++){
			for(k=1;k<=N;k++){
				div[IX(i,j,k)] = -0.5f*h*(u[IX(i+1,j,k)] - u[IX(i-1,j,k)] + 
										  v[IX(i,j+1,k)] - v[IX(i,j-1,k)] +
										  w[IX(i,j,k+1)] - w[IX(i,j,k-1)] );
				p[IX(i,j,k)] = 0;
			}
		}
	}
	set_bnd(N,0,div);
	set_bnd(N,0,p);
	
	for(G=0;G<50; G++){
		for(i =1; i<=N;i++){
			for(j =1; j <=N;j++){
				for(k=1;k<=N;k++){

					p[IX(i,j,k)] = (div[IX(i,j,k)]+ p[IX(i-1,j,k)] + p[IX(i+1,j,k)] + 
										     		p[IX(i,j-1,k)] + p[IX(i,j+1,k)] +
											    	p[IX(i,j,k-1)] + p[IX(i,j,k+1)] )/6;
				}
			}
		}
		set_bnd(N,0,p);
	}
	
	for(i=1; i<=N; i++){
		for(j=1; j<=N; j++){
			for(k=1;k<=N;k++){
				u[IX(i,j,k)] -= 0.5f*(p[IX(i+1,j,k)]-p[IX(i-1,j,k)])*(N);
				v[IX(i,j,k)] -= 0.5f*(p[IX(i,j+1,k)]-p[IX(i,j-1,k)])*(N);
				w[IX(i,j,k)] -= 0.5f*(p[IX(i,j,k+1)]-p[IX(i,j,k-1)])*(N);
			}
		}
	}
	set_bnd(N,1,u);
	set_bnd(N,2,v);
	set_bnd(N,3,w);
	
}

void Flo3::set_bnd(int N, int b, float *x){
	int i,k;
	for(k=1;k<=N;k++) {
        for(i=1;i<=N;i++) {
            x[IX(i,k,0  )] = b == 3 ? -x[IX(i,k,1)] : x[IX(i,k,1)];
            x[IX(i,k,N+1)] = b == 3 ? -x[IX(i,k,N)] : x[IX(i,k,N)];
        }
    }
    for(k=1; k<=N; k++) {
        for(i=1; i<=N; i++) {
            x[IX(i,0  ,k)] = b == 2 ? -x[IX(i,1,k)] : x[IX(i,1,k)];
            x[IX(i,N+1,k)] = b == 2 ? -x[IX(i,N,k)] : x[IX(i,N,k)];
        }
    }
    for(k=1; k<=N; k++) {
        for(i=1; i<=N; i++) {
            x[IX(0  ,i,k)] = b == 1 ? -x[IX(1,i,k)] : x[IX(1,i,k)];
            x[IX(N+1,i,k)] = b == 1 ? -x[IX(N,i,k)] : x[IX(N,i,k)];
        }
    }
    
    x[IX(0,0,0)]       = 0.3333f * (x[IX(1, 0, 0)]
									+ x[IX(0, 1, 0)]
									+ x[IX(0, 0, 1)]);
    x[IX(0,N+1,0)]     = 0.3333f * (x[IX(1, N+1, 0)]
									+ x[IX(0, N,   0)]
									+ x[IX(0, N+1, 1)]);
    x[IX(0,0,N+1)]     = 0.3333f * (x[IX(1, 0, N+1)]
									+ x[IX(0, 1, N+1)]
									+ x[IX(0, 0, N)]);
    x[IX(0,N+1,N+1)]   = 0.3333f * (x[IX(1, N+1, N+1)]
									+ x[IX(0, N  , N+1)]
									+ x[IX(0, N+1, N  )]);
    x[IX(N+1,0,0)]     = 0.3333f * (x[IX(N  , 0, 0)]
									+ x[IX(N+1, 1, 0)]
									+ x[IX(N+1, 0, 1)]);
    x[IX(N+1,N+1,0)]   = 0.3333f * (x[IX(N  , N+1, 0)]
									+ x[IX(N+1, N  , 0)]
									+ x[IX(N+1, N+1, 1)]);
    x[IX(N+1,0,N+1)]   = 0.3333f * (x[IX(N  , 0, N+1)]
									+ x[IX(N+1, 1, N+1)]
									+ x[IX(N+1, 0, N  )]);
    x[IX(N+1,N+1,N+1)] = 0.3333f * (x[IX(N  , N+1, N+1)]
									+ x[IX(N+1, N  , N+1)]
									+ x[IX(N+1, N+1, N  )]);
	



}

void Flo3::add_source(int N, float *x, float *s, float dt){
	int i, size =(N+2)*(N+2)*(N+2);
	for(i=0; i<size;i++){
		//	x[i] = (1.f+dt)*s[i];
		//	x[i] = (dt)*s[i];
		x[i] = s[i];
		
	}
	
}

void Flo3::add_random_v(){
	/* initialize random seed: */
	int i,j,k;
	float ran;
	srand ( time(NULL) );
	
	/* generate secret number: */
	for(i=1;i<=size;i++){
		for(j=1;j<=size;j++){
			for(k=1;k<=size;k++){
				ran = ((float)(rand() % 200))/150.f;
				vel3.u[IX(i,j,k)] += ran;
				ran = ((float)(rand() % 200))/150.f;
				vel3.v[IX(i,j,k)] += ran;
				ran = ((float)(rand() % 200))/150.f;
				vel3.w[IX(i,j,k)] += ran;
			
			}
		}
	}
}


void Flo3::add_gravity(){

  int i,j,k;
  for(i=1;i<=size;i++){
	for(j=1;j<=size;j++){
	  for(k=1;k<=size;k++){
		vel3.w[IX(i,j,k)] += -1.f;
	  }
	}
  }

}







void Flo3::draw_v(){
	vel3.draw();
}

void Flo3::print(){
	int i,j,k;
	float mass =0.f;
	for(i=1; i<=size; i++){
		for(j=1; j<=size; j++){
			for(k=1;k<=size;k++){
				cout<<"["<<den3.dense[IX(i,j,k)]<<"]";
				mass +=den3.dense[IX(i,j,k)];
			}
			cout<<"==k:"<<k<<endl;
		}
	}
	cout<<"mass:"<<mass<<endl;
	cout<<"avg :"<<mass/(800.f)<<endl;
	
}

void Flo3::print_d(float* d, char* c){

}









































