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
#include "color.h"

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
			MandelBoxParams *mandelBox_paramsP);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
pixelData renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, MandelBoxParams &mandelBox_params);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);
extern double getTime();
extern void   printProgress( double perc, double time );
void autoMove(CameraParams &camera_params, RenderParams &renderer_params, MandelBoxParams &mandelBox_params, int primary, int secondary, double velocity, int start);

int main(int argc, char** argv){
	CameraParams    camera_params;
	RenderParams    renderer_params;
	MandelBoxParams mandelBox_params;
	getParameters("parm.dat", &camera_params, &renderer_params, &mandelBox_params);


	double Velocity = 0.0005; // 0.0001 Cartesian Movement Per Frame
	//for (i=0; i<300; i++){}
	int primary = 0;	//Primary is X
	int secondary = 1;	//Secondary is Y
	autoMove(camera_params, renderer_params, mandelBox_params, primary, secondary, Velocity, 0);


	printf("\n\nAll frames rendered\n");

	printf("\n\nMaking Video\n");

	return 0;
}


void autoMove(CameraParams &camera_params, RenderParams &renderer_params, MandelBoxParams &mandelBox_params, int primary, int secondary, double velocity, int start){
	int i, t, releaseCounter, release;
	i = 0;
	pixelData farPixel; //Check Wall Hit
	int image_size = renderer_params.width * renderer_params.height;
	unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
	double time = getTime();
	double condition = camera_params.camPos[primary];
	int steps = 100;
	int fileNumber = 0;
	double lookx, looky, lookz;
	for (i=0;i<7600;i++) {
		init3D(&camera_params, &renderer_params);
		farPixel = renderFractal(camera_params, renderer_params, image, mandelBox_params);
		int j = 0;
		 if (i%10 == 0) {
			lookx = farPixel.hit.x;
			looky = farPixel.hit.y;
			lookz = farPixel.hit.z;
		}
		camera_params.camTarget[0] -= (camera_params.camTarget[0] - lookx) / (double)10;
		camera_params.camTarget[1] -= (camera_params.camTarget[1] - looky) / (double)10;
		camera_params.camTarget[2] -= (camera_params.camTarget[2] - lookz) / (double)10;

		camera_params.camPos[0] -= (camera_params.camPos[0] - farPixel.hit.x) / (double)steps;						//primary axis alwaysmoves
		camera_params.camPos[1] -= (camera_params.camPos[1] - farPixel.hit.y) / (double)steps;						//primary axis alwaysmoves
		camera_params.camPos[2] -= (camera_params.camPos[2] - farPixel.hit.z) / (double)steps;						//primary axis alwaysmoves

		printf("i=%d campos=(%f,%f,%f) lookAt=(%f,%f,%f)\n", i, camera_params.camPos[0],camera_params.camPos[1],camera_params.camPos[2],camera_params.camTarget[0],camera_params.camTarget[1],camera_params.camTarget[2]);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", fileNumber);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printf("Saved image: %d\n",i);
		fileNumber++;
		//printProgress( (double)i/7600, getTime()-time );
	}
	free(image);
}
