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
#include "3d.h"
#include <string.h>

#pragma acc routine
extern void rayMarch(const float maxDistance, const int maxRaySteps, const vec3 &from, const vec3  &direction, double eps, pixelData& pix_data, MandelBoxParams mandelBox_params);
#pragma acc routine
extern void getColour(double* temp, bool &escaped, double hx, double hy, double hz, double nx, double ny, double nz, int colourType, float brightness, double tx, double ty, double tz);
#pragma acc routine
int UnProject(double winX, double winY, double view[4], double matrix[16], double *obj);

void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, MandelBoxParams mandelBox_params)
{

  
  const double eps = pow(10.0, renderer_params.detail); 
  double farPoint[3];
  
  /* Literals as required in the next 3 function calls*/
  double view[4];
  memcpy(view, camera_params.viewport, 4);
  double Pos[3];
  memcpy(view, camera_params.camPos, 3);
  double matrixInvM[16]; 
  memcpy(matrixInvM, camera_params.matInvProjModel, 16);
  int maxSteps = renderer_params.maxRaySteps;
  float maxDist = renderer_params.maxDistance;
  int colorType = renderer_params.colourType;
  float Brightness = renderer_params.brightness;
	double * temp;
  vec3 from;
  SET_POINT(from,camera_params.camPos);
  
  const int height = renderer_params.height;
  const int width  = renderer_params.width;
  
  pixelData pix_data;
  
  //double time = getTime();
  vec3 color;
  int i,j,k;

#pragma acc data copyin(from, view[0:3], matrixInvM[0:15], Pos[0:2], maxSteps, maxDist, colorType, Brightness, width, height, color, pix_data) copyout(image[0:3*width*height]) 
#pragma acc kernels
#pragma acc loop private(image[0:3*width*height])
  for(j = 0; j < height; j++){       //for each column pixel in the row
			#pragma acc loop private(image[0:3*width*height])
      for(i = 0; i <width; i++){
	  // get point on the 'far' plane
	  // since we render one frame only, we can use the more specialized method
	  UnProject(i, j, view, matrixInvM, farPoint);
	  vec3 to, hit, normal;
	  // to = farPoint - camera_params.camPos
	  SUBTRACT_POINT(to,farPoint,Pos);
	  NORMALIZE(to);
	  VEC(hit, pix_data.hit.x, pix_data.hit.y, pix_data.hit.z)
	  VEC(normal, pix_data.normal.x, pix_data.normal.y, pix_data.normal.z)
		bool escape = pix_data.escaped;
	  rayMarch(maxDist, maxSteps, from, to, eps, pix_data, mandelBox_params);
	  
	  //get the colour at this pixel
	  getColour(temp, escape, hit.x, hit.y, hit.z, normal.x, normal.y, normal.z, colorType, Brightness, to.x , to.y, to.z);
    VEC(color, temp[0], temp[1], temp[2]);
	  //save colour into texture
	  k = (j * width + i)*3;
	  image[k+2] = (unsigned char)(color.x * 255);
	  image[k+1] = (unsigned char)(color.y * 255);
	  image[k]   = (unsigned char)(color.z * 255);
	}
      //printProgress((j+1)/(double)height,getTime()-time);
    }
  printf("\n rendering done:\n");
}
