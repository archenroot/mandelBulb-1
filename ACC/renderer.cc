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
#include <stdio.h>
#include "color.h"
#include "mandelbox.h"
#include "camera.h"
#include "vec3d.h"
//#include "3d.h"
#include "3d2.h"
#include "distance_est.h"
#include "getcolor.h"
#include "raymarching.h"

void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, MandelBoxParams mandelBox_params)
{
  double eps = pow(10.0f, renderer_params.detail); 
  double farPoint[3];
  double result[3];
  vec3 to, from;
  vec3 color;
  /*
  vec3 to[width*height];
  vec3 from[witdh*height];
  vec3 color[width*height];
  */
  SET_POINT(from,camera_params.camPos);
  
  int height = renderer_params.height;
  int width  = renderer_params.width;
  
  pixelData pix_data;

  int i,j,k;
  #pragma acc data copy(image[:width*height*3], farPoint[0:3], camera_params[0:1], pix_data[0:1], result[0:3])
  #pragma acc parallel loop 
  for(j = 0; j < height; j++){
      #pragma acc loop independent
	for(i = 0; i <width; i++){
	  //int hi = UnProject(i, j, camera_params, farPoint);
		double in[4], out[4];
		in[0]=(i-(double)(camera_params.viewport[0]))/(double)(camera_params.viewport[2])*2.0-1.0;
		in[1]=(j-(double)(camera_params.viewport[1]))/(double)(camera_params.viewport[3])*2.0-1.0;
		in[2]=2.0-1.0;
		in[3]=1.0;
		
		out[0]=camera_params.matInvProjModel[0]*in[0]+camera_params.matInvProjModel[4]*in[1]+camera_params.matInvProjModel[8]*in[2]+camera_params.matInvProjModel[12]*in[3];
  	out[1]=camera_params.matInvProjModel[1]*in[0]+camera_params.matInvProjModel[5]*in[1]+camera_params.matInvProjModel[9]*in[2]+camera_params.matInvProjModel[13]*in[3];
  	out[2]=camera_params.matInvProjModel[2]*in[0]+camera_params.matInvProjModel[6]*in[1]+camera_params.matInvProjModel[10]*in[2]+camera_params.matInvProjModel[14]*in[3];
  	out[3]=camera_params.matInvProjModel[3]*in[0]+camera_params.matInvProjModel[7]*in[1]+camera_params.matInvProjModel[11]*in[2]+camera_params.matInvProjModel[15]*in[3];
  
		out[3] = 1.0/out[3];
		farPoint[0] = out[0]*out[3];
		farPoint[1] = out[1]*out[3];
		farPoint[2] = out[2]*out[3];

	  SUBTRACT_POINT(to,farPoint,camera_params.camPos);
	  NORMALIZE(to);
	
	  rayMarch(renderer_params, from, to, eps, pix_data, mandelBox_params);
	  
	  getColour(pix_data, renderer_params, from, to, result);
    VEC(color, result[0], result[1], result[2]);
	  k = (j * width + i)*3;
	  image[k+2] = (unsigned char)(color.x * 255);
	  image[k+1] = (unsigned char)(color.y * 255);
	  image[k]   = (unsigned char)(color.z * 255);
	}
    }
  printf("\n rendering done:\n");
}
