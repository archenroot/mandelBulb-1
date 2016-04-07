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
#include "colorFloat.h"
#include "mandelboxFloat.h"
#include "camera.h"
#include "vec3dFloat.h"
#include "3d2.h"
#include "distance_estFloat.h"
#include "getcolorFloat.h"
#include "raymarchingFloat.h"

void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, MandelBoxParams &mandelBox_params, double *returnTotalNorm)
{
  float eps = pow(10.0f, renderer_params.detail);

  int height = renderer_params.height;
  int width  = renderer_params.width;
  int total = width*height;
  int i,j,k;

  vec3 *to = (vec3*)malloc(sizeof(vec3)*width*height);
  vec3 *from = (vec3*)malloc(sizeof(vec3)*width*height);
  vec3 *color = (vec3*)malloc(sizeof(vec3)*width*height);
  pixelData *pix_data = (pixelData*)malloc(sizeof(pixelData)*width*height);

  float in[4], out[4], result[3], farPoint[3];
#pragma acc data copy(image[:width*height*3], farPoint[0:3], camera_params[0:1], renderer_params[0:1], mandelBox_params[0:1], \
to[0:total], pix_data[0:total], color[0:total], from[0:total], result[0:3], in[0:4], out[0:4])
#pragma acc parallel loop private(result[0:3], in[0:4], out[0:4], farPoint[0:3])
 for(j = 0; j < height; j++){
  #pragma acc loop independent worker
	for(i = 0; i <width; i++){
		/*This is a physical inline of UnProject
		Problem: ACC did not allow certain values passed
		*/
	  SET_POINT(from[j*width+i],camera_params.camPos);
		in[0]=(i-(float)(camera_params.viewport[0]))/(float)(camera_params.viewport[2])*2.0-1.0;
		in[1]=(j-(float)(camera_params.viewport[1]))/(float)(camera_params.viewport[3])*2.0-1.0;
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
		/*End physical inline
		*/

	  SUBTRACT_POINT( to[j*width+i], farPoint,camera_params.camPos);
	  NORMALIZE( to[j*width+i] );

	  rayMarch(renderer_params, from[j*width+i], to[j*width+i], eps, pix_data[j*width+i], mandelBox_params);

	  getColour(pix_data[j*width+i], renderer_params, from[j*width+i], to[j*width+i], result);
    VEC(color[j*width+i], result[0], result[1], result[2]);
	  k = (j * width + i)*3;
	  image[k+2] = (unsigned char)(color[j*width+i].x * 255);
	  image[k+1] = (unsigned char)(color[j*width+i].y * 255);
	  image[k]   = (unsigned char)(color[j*width+i].z * 255);
	}
 }
}
