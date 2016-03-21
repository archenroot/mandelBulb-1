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
#ifndef GETCOLOR_H
#define GETCOLOR_H

#include "color.h"
#include "renderer.h"
#include "vec3d.h"
#include <cmath>

inline void getColour(const pixelData &pixData, const RenderParams &render_params, const vec3 &from, const vec3  &direction, double *result){
	vec3 backColor, hitColor;
  VEC(backColor, 0.4, 0.4, 0.4);
  VEC(hitColor, 1.0, 1.0, 1.0);
  double CamLightW = 1.8;// 1.27536;
	double CamLightMin = 0.3;// 0.48193;
  
  if (pixData.escaped == false){
      //apply lighting
      //lighting(pixData.normal, hitColor, pixData.hit, direction, hitColor);

			vec3 nn;
			VEC(nn, (pixData.normal.x - 1.0), (pixData.normal.y - 1.0), (pixData.normal.z - 1.0));
			double dotResult = (nn.x*direction.x) + (nn.y*direction.y) + (nn.z*direction.z);
			double ambient = (CamLightMin >= dotResult) ? CamLightMin*CamLightW : dotResult*CamLightW ; 
			VEC(hitColor, ambient*hitColor.x, ambient*hitColor.y, ambient*hitColor.z);
  
      //add normal based colouring
      if(render_params.colourType == 0 || render_params.colourType == 1){
      
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
				if (hitColor.x > 1.0) {hitColor.x = 1.0;}
				if (hitColor.x < 0.0) {hitColor.x = 0.0;}
				if (hitColor.y > 1.0) {hitColor.y = 1.0;}
				if (hitColor.y < 0.0) {hitColor.y = 0.0;}
				if (hitColor.z > 1.0) {hitColor.z = 1.0;}
				if (hitColor.z < 0.0) {hitColor.z = 0.0;}

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
  //return hitColor;
  result[0] = hitColor.x;
  result[1] = hitColor.y;
  result[2] = hitColor.z;
}

#endif

