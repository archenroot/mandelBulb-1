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
void autoMove(CameraParams &camera_params, RenderParams &renderer_params, MandelBoxParams &mandelBox_params, int start, int total);
void RenderSpin(CameraParams &camera_params, RenderParams &renderer_params, MandelBoxParams &mandelBox_params, int spinFrames);

#define PI 3.14159265

int main(int argc, char** argv){
	int spin, total;
	/*Input Parameters for Video
	Video will be rendered at 30FPS, by default, it will first spin for 10 seconds, then automatically navigate
	You may choose to override these defaults with Arguments 1 = Total frames, Argument2 = Spin frames
	Note: Total AutoMove Frames = Total Frames - Spin Frames*/
	if(argc == 2){
		total = atoi(argv[1]);
		printf("\nTotal %d Auto Move Frames will be Rendered\n", total);
		spin = 300;
		printf("Default Total %d Spin Frames will be Rendered\n", spin);
	} else if (argc == 3) {
		total = atoi(argv[1]);
		spin = atoi(argv[2]);
		printf("\nTotal %d Auto Move Frames will be Rendered\n", total);
		printf("\nTotal %d Spin Frames will be Rendered\n", total);
	}else{
		total = 7200;
		spin = 300;
		printf("Default Total %d Spin Frames will be Rendered\n", spin);
		printf("Default Total %d Auto Move Frames will be Rendered\n", total);
	}

	/*Init*/
	CameraParams    camera_params;
	RenderParams    renderer_params;
	MandelBoxParams mandelBox_params;
	getParameters("parm.dat", &camera_params, &renderer_params, &mandelBox_params);
	
	double thetaOffset = atan(camera_params.camPos[2]/camera_params.camPos[0]);
	double startX, startZ;
	startX = camera_params.camPos[0];
	startZ = camera_params.camPos[2];
	/*Render MandelBulb Spin for X seconds*/
	RenderSpin(camera_params, renderer_params, mandelBox_params, spin);
	
	/*Update local camera_params*/
	camera_params.camPos[0] = startX * cos(2*PI + thetaOffset);
	camera_params.camPos[2] = startZ * sin(2*PI + thetaOffset);
		
	/*Render MandelBulb Moving from X until TotalFrames*/
	autoMove(camera_params, renderer_params, mandelBox_params, spin, total);
	
	printf("\n\nAll frames rendered\n");
	/*Call Script to make Video*/
	printf("\n\nMaking Video\n");
	
	/*Video Making Done*/
	printf("\n\nCompleted Video\n");
	return 0;
}


void autoMove(CameraParams &camera_params, RenderParams &renderer_params, MandelBoxParams &mandelBox_params, int start, int total){
	int i;
	pixelData farPixel; //Check Wall Hit
	int image_size = renderer_params.width * renderer_params.height;
	unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
	double time = getTime();
	double eps = 0.0000001;	//Stuck Bailout, Precausionary 
	int steps = 400;				//Steps for camPos
	int targetSteps = 40;		//Steps for camTarget
	int fileNumber = start;
	double lookX, lookY, lookZ;
	
	for (i=0; i<total; i++) {
		init3D(&camera_params, &renderer_params);
		farPixel = renderFractal(camera_params, renderer_params, image, mandelBox_params);
		 if (i%10 == 0) {
		 	lookX =  farPixel.hit.x;
		 	lookY =  farPixel.hit.y;
		 	lookZ =  farPixel.hit.z;	
		 }

		camera_params.camTarget[0] -= (camera_params.camTarget[0] - lookX) / (double)targetSteps;
		camera_params.camTarget[1] -= (camera_params.camTarget[1] - lookY) / (double)targetSteps;
		camera_params.camTarget[2] -= (camera_params.camTarget[2] - lookZ) / (double)targetSteps;

		camera_params.camPos[0] -= (camera_params.camPos[0] - farPixel.hit.x) / (double)steps;						//Chasing farthest pixel
		camera_params.camPos[1] -= (camera_params.camPos[1] - farPixel.hit.y) / (double)steps;						
		camera_params.camPos[2] -= (camera_params.camPos[2] - farPixel.hit.z) / (double)steps;						

		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", fileNumber);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		fileNumber++;
		if (abs(camera_params.camPos[0]) < eps && abs(camera_params.camPos[1]) < eps && abs(camera_params.camPos[2]) < eps){	//Camera Kickedout if we hit 0,0,0
			camera_params.camPos[0] = -1;
			camera_params.camPos[1] = -1;					
			camera_params.camPos[2] = -1;
			camera_params.camTarget[0] = 0;
			camera_params.camTarget[1] = 0;
			camera_params.camTarget[2] = 0;
		}
		printProgress( (double)i/7600, getTime()-time );
	}
	free(image);
}
void RenderSpin(CameraParams &camera_params, RenderParams &renderer_params, MandelBoxParams &mandelBox_params, int spinFrames){
	int i;
	double startX, startZ;
	double thetaOffset = atan(camera_params.camPos[2]/camera_params.camPos[0]);
	int image_size = renderer_params.width * renderer_params.height;
	unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
	startX = camera_params.camPos[0];
	startZ = camera_params.camPos[2];
	double radPerFrame = (2*PI)/spinFrames;//Make a full Circle
	double time = getTime();
	printf("\n\nRendering %d frames of spinning\n", spinFrames);
	for (i=0; i<spinFrames; i++) {
		//Circle of Spin
		camera_params.camPos[0] = startX * cos(radPerFrame * i + thetaOffset);
		camera_params.camPos[2] = startZ * sin(radPerFrame * i + thetaOffset);
		init3D(&camera_params, &renderer_params);

		renderFractal(camera_params, renderer_params, image, mandelBox_params);

		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/spinFrames, getTime()-time );
	}
	printf("\n\nAll Spin Frames Rendered\n");
}
