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
#include <stdlib.h>
#include <stdio.h>
#include "camera.h"
#include "renderer.h"
#include "mandelbox.h"
#include <math.h>

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
		   MandelBoxParams *mandelBox_paramsP);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, MandelBoxParams &mandelBox_params);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);
extern double getTime();
extern void   printProgress( double perc, double time );

#define PI 3.14159265

int main(int argc, char** argv)
{
	int i;
  CameraParams    camera_params;
  RenderParams    renderer_params;
  MandelBoxParams mandelBox_params;

  getParameters(argv[1], &camera_params, &renderer_params, &mandelBox_params);

  int image_size = renderer_params.width * renderer_params.height;
  unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
	double time = getTime();
	for (i=0; i<3600; i++) {
		camera_params.camPos[0] = 1.2 * cos(PI/3600 * i);
		camera_params.camPos[2] = 1.2 * sin(PI/3600 * i);
	  init3D(&camera_params, &renderer_params);

	  renderFractal(camera_params, renderer_params, image, mandelBox_params);

	  char fileName[80];
		sprintf(fileName, "./output/output_%04d.bmp", i);
	  saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/3600, getTime()-time );
		//printf("File %d saved\n",i);
	}

  free(image);

  return 0;
}
