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
	int spinFrames = 1;
	int zoomFrames = 0;
	if(argc == 3){
		spinFrames = atoi(argv[2]);
	} else if (argc == 4) {
		spinFrames = atoi(argv[2]);
		zoomFrames = atoi(argv[3]);
	}



	CameraParams    camera_params;
	RenderParams    renderer_params;
	MandelBoxParams mandelBox_params;
	getParameters(argv[1], &camera_params, &renderer_params, &mandelBox_params);



	int image_size = renderer_params.width * renderer_params.height;
	unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
	double time = getTime();
	printf("Rendering %d frames of zooming\n", zoomFrames);
	double zoomStepX = (5. - camera_params.camPos[0]) / (double)zoomFrames;
	double zoomStepZ = (5. - camera_params.camPos[2]) / (double)zoomFrames;
	double evolutions = (double)zoomFrames / 50.0;
	printf("power = %f\n", mandelBox_params.rFixed);
	mandelBox_params.rFixed -= evolutions;
	printf("power = %f\n", mandelBox_params.rFixed);
	for (i=0; i<zoomFrames; i++) {
		camera_params.camPos[0] = 5. - zoomStepX * i;
		camera_params.camPos[2] = 5. - zoomStepZ * i;

		init3D(&camera_params, &renderer_params);

		renderFractal(camera_params, renderer_params, image, mandelBox_params);

		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		mandelBox_params.rFixed += 0.02;
		printProgress( (double)i/zoomFrames, getTime()-time );
	}
	printf("power = %f\n", mandelBox_params.rFixed);
	printf("\n\n Zoomed to (%f,%f)", camera_params.camPos[0],camera_params.camPos[2]);

	double startX = camera_params.camPos[0];
	double startZ = camera_params.camPos[2];
	double radPerFrame = PI/spinFrames;
	printf("\n\nRendering %d frames of spinning\n", spinFrames);
	for (i=0; i<spinFrames; i++) {
		camera_params.camPos[0] = startX * cos(radPerFrame * i);
		camera_params.camPos[2] = startX * sin(radPerFrame * i);
		init3D(&camera_params, &renderer_params);

		renderFractal(camera_params, renderer_params, image, mandelBox_params);

		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i+zoomFrames);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/spinFrames, getTime()-time );
		//printf("File %d saved\n",i);
	}
	printf("\n\nAll frames rendered\n");

	free(image);

	return 0;
}
