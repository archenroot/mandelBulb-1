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
#include "color.h"
#include "renderer.h"
#include "vec3d.h"
#include <cmath>
#include <algorithm>

using namespace std;
double clamp(double d, double min, double max);


void lighting(const vec3 &n, const vec3 &color, const vec3 &pos, const vec3 &direction,  vec3 &outV)
{
	static double CamLightW = 1.8;// 1.27536;
	static double CamLightMin = 0.3;// 0.48193;
	vec3 nn;
  VEC(nn, (n.x - 1.0), (n.y - 1.0), (n.z - 1.0));
  double dotResult = (nn.x*direction.x) + (nn.y*direction.y) + (nn.z*direction.z);
  double ambient = max( CamLightMin, dotResult )*CamLightW;
  //  outV = CamLight*ambient*color; Camlight= 1 anyways...
  VEC(outV, ambient*color.x, ambient*color.y, ambient*color.z);
}

vec3 getColour(const pixelData &pixData, const RenderParams &render_params, const vec3 &from, const vec3  &direction){
	vec3 backColor, hitColor;
  VEC(backColor, 0.4, 0.4, 0.4);
  //colouring and lightning
  VEC(hitColor, 1.0, 1.0, 1.0);

  if (pixData.escaped == false){
      //apply lighting
      lighting(pixData.normal, hitColor, pixData.hit, direction, hitColor);
      //add normal based colouring
      if(render_params.colourType == 0 || render_params.colourType == 1){

				/*hitColor = hitColor * pixData.normal;
	  		hitColor = (hitColor + 1.0)/2.0;
	  		hitColor = hitColor*render_params.brightness;*/
	  		hitColor.x *= pixData.normal.x;
	  		hitColor.y *= pixData.normal.y;
	  		hitColor.z *= pixData.normal.z;
	  		hitColor.x = (hitColor.x+1.0)/2.0;
	  		hitColor.y = (hitColor.y+1.0)/2.0;
	  		hitColor.z = (hitColor.z+1.0)/2.0;
	  		hitColor.x *= render_params.brightness;
	  		hitColor.y *= render_params.brightness;
	  		hitColor.z *= render_params.brightness;

				//gamma correction
				hitColor.x = clamp(hitColor.x, 0.0, 1.0);
				hitColor.y = clamp(hitColor.y, 0.0, 1.0);
				hitColor.z = clamp(hitColor.z, 0.0, 1.0);
				VEC(hitColor, hitColor.x*hitColor.x, hitColor.y*hitColor.y, hitColor.z*hitColor.z);
			}
			double t;
      if(render_params.colourType == 1){
				//"swap" colors
				t = hitColor.x;
				hitColor.x = hitColor.z;
				hitColor.z = t;
			}
 }else{
    //we have the background colour
    hitColor = backColor;
 }
  return hitColor;
}

double clamp(double d, double min, double max) {
  double t;
  if (d< min){
  	t = min;
  }else{
  	t = d;
  }
  if (d > max){
  	t = max;
  }else{
  	t = d;
  }

	return t;
}
