/*
   This file is part of the Mandelbox program developed for the course
    CS/SE  Distributed Computer Systems taught by N. Nedialkov in the
    Winter of 2015-2016 at McMaster University.

    Copyright (C) 2015-2016 T. Gwosdz and N. Nedialkov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef RAY_H
#define RAY_H

#include <assert.h>
//#include <algorithm>
#include <stdio.h>
#include "color.h"
#include "renderer.h"
#include "vec3d.h"
#include "mandelbox.h"
#include "distance_est.h"

inline void normal(const vec3 & p, vec3 & normal, MandelBoxParams &mandelBox_params)
{
  // compute the normal at p
  const double sqrt_mach_eps = 1.4901e-08;
	double Mag = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
  //double eps = std::max( Mag, 1.0 )*sqrt_mach_eps;
  double eps = (Mag > 1.0) ? Mag*sqrt_mach_eps : sqrt_mach_eps ;
  vec3 e1, e2, e3;
  vec3 pp1, pm1, pp2, pm2, pp3, pm3; //Couldn't find the DE function
  VEC(e1, eps, 0,   0);
  VEC(e2, 0  , eps, 0);
  VEC(e3, 0  , 0, eps);
  VEC(pm1, p.x - e1.x, p.y - e1.y, p.z - e1.z);
  VEC(pp1, p.x + e1.x, p.y + e1.y, p.z + e1.z);
  VEC(pm2, p.x - e2.x, p.y - e2.y, p.z - e2.z);
  VEC(pp2, p.x + e2.x, p.y + e2.y, p.z + e2.z);
  VEC(pm3, p.x - e3.x, p.y - e3.y, p.z - e3.z);
  VEC(pp3, p.x + e3.x, p.y + e3.y, p.z + e3.z);
  //normal = vec3(DE(p+e1)-DE(p-e1), DE(p+e2)-DE(p-e2), DE(p+e3)-DE(p-e3));
  VEC(normal, MandelBulbDistanceEstimator(pp1,mandelBox_params)-MandelBulbDistanceEstimator(pm1,mandelBox_params), MandelBulbDistanceEstimator(pp2,mandelBox_params)-MandelBulbDistanceEstimator(pm2,mandelBox_params), MandelBulbDistanceEstimator(pp3,mandelBox_params)-MandelBulbDistanceEstimator(pm3,mandelBox_params));
  //normal.Normalize();
  NORMALIZE(normal);
}

inline void rayMarch(const RenderParams &render_params, const vec3 &from, const vec3  &direction, double eps, pixelData& pix_data, MandelBoxParams &mandelBox_params)
{
  double dist = 0.0;
  double totalDist = 0.0;

  // We will adjust the minimum distance based on the current zoom

  double epsModified = 0.0;

  int steps=0;
  vec3 p;
  do
    {
			p.x = from.x+direction.x*totalDist;
		  p.y = from.y+direction.y*totalDist;
		  p.z = from.z+direction.z*totalDist;
      dist = MandelBulbDistanceEstimator(p,mandelBox_params);

      totalDist += .95*dist;

      epsModified = totalDist;
      epsModified*=eps;
      steps++;
    }
  while (dist > epsModified && totalDist <= render_params.maxDistance && steps < render_params.maxRaySteps);

  if (dist < epsModified)
    {
      //we didnt escape
      pix_data.escaped = false;

      // We hit something, or reached MaxRaySteps
      pix_data.hit = p;

      //figure out the normal of the surface at this point
      vec3 normPos;
      //const vec3 normPos = p - direction * epsModified;
      VEC(normPos, p.x - (direction.x * epsModified), p.y - (direction.y * epsModified), p.z - (direction.z * epsModified));
      normal(normPos, pix_data.normal, mandelBox_params);
    }
  else
    //we have the background colour
    pix_data.escaped = true;
}



#endif
