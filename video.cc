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
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, MandelBoxParams &mandelBox_params, double *returnTotalNorm);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);
extern double getTime();
extern void   printProgress( double perc, double time );
void autoMove(CameraParams &camera_params, RenderParams &renderer_params, MandelBoxParams &mandelBox_params, int primary, int secondary, double velocity, int start);

int main(int argc, char** argv){
	CameraParams    camera_params;
	RenderParams    renderer_params;
	MandelBoxParams mandelBox_params;
	getParameters("parm.dat", &camera_params, &renderer_params, &mandelBox_params);


	double Velocity = 0.0001; // 0.0001 Cartesian Movement Per Frame
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
	double total_pix_normal[3]; //Check Wall Hit
	int image_size = renderer_params.width * renderer_params.height;
	unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
	double time = getTime();
	double condition = camera_params.camPos[primary];
	
	while (condition >0.0){
		camera_params.camPos[primary] -= velocity;						//primary axis alwaysmoves
		camera_params.camPos[secondary] -= velocity*release;		//Release is either 1 or 0, secondsdary axis may not move for 100 cycles
		condition = camera_params.camPos[primary];			//Update Stop Condition
		
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);	
		char fileName[80];
		sprintf(fileName, "./output_%05d.bmp", i);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		i+=1;
		releaseCounter += 1;
		if (releaseCounter > 30){
			release = 1;
		}
		if ( total_pix_normal[0] < 0.1 ||  total_pix_normal[1] < 0.1 ||  total_pix_normal[2] < 0.1 ){		//We actually hit a wall
			i-=1; //delete last frame
			for (t=0; t<10; t++){				//Backtrack 5 Steps
				camera_params.camPos[primary] -= (-1)*velocity;
				camera_params.camPos[secondary] -= (-1)*velocity;
				init3D(&camera_params, &renderer_params);
				renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);	
				char fileName[80];
				sprintf(fileName, "./output_%05d.bmp", i);
				saveBMP(fileName, image, renderer_params.width, renderer_params.height);
				releaseCounter = 0;
				release = 0;
				i+=1;
			}	
		}
		printProgress( (double)((1-camera_params.camPos[primary])/1.0), getTime()-time );	
	}
	free(image);
}
