#ifndef vec3_h
#define vec3_h

#ifdef _OPENACC
#include <accelmath.h>
#else
#include <math.h>
#endif

//Floating point operations, this is most apparent in computing time reductions
typedef struct 
{
  float x, y, z;
}  vec3;

#define SET_POINT(p,v) { p.x=v[0]; p.y=v[1]; p.z=v[2]; }

#define SUBTRACT_POINT(p,v,u)			\
  {						\
  p.x=(v[0])-(u[0]);				\
  p.y=(v[1])-(u[1]);				\
  p.z=(v[2])-(u[2]);				\
}

#define NORMALIZE(p) {					\
    double fMag = ( p.x*p.x + p.y*p.y + p.z*p.z );	\
    if (fMag != 0)					\
      {							\
	double fMult = 1.0/sqrt(fMag);			\
	p.x *= fMult;					\
	p.y *= fMult;					\
	p.z *= fMult;					\
      }							\
  }


#define MAGNITUDE(m,p) 	({ m=sqrt( p.x*p.x + p.y*p.y + p.z*p.z ); })

#define DOT(c,d,p) {c=( d.x*p.x + d.y*p.y + d.z*p.z ); }

#define MAX(a,b) ( ((a)>(b))? (a):(b))

#define VEC(v,a,b,c) {v.x = a; v.y = b; v.z = c; }


#endif
