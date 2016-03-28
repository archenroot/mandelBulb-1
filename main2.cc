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

#define PI 6.2831853

int main(int argc, char** argv)
{
	int i;

	CameraParams    camera_params;
	RenderParams    renderer_params;
	MandelBoxParams mandelBox_params;
	getParameters(argv[1], &camera_params, &renderer_params, &mandelBox_params);

	double thetaOffset = atan(1);
	double startX = camera_params.camPos[0];
	double startY = camera_params.camPos[1];
	double startZ = camera_params.camPos[2];

	double total_pix_normal[3];
	int image_size = renderer_params.width * renderer_params.height;
	unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
	double time = getTime();
	
	int spinning = 900;	
	double radPerFrame = (PI)/spinning; 	//30 seconds of spinning, Total:30
	printf("\n\nRendering X-Z spinning, 900 Frames\n");
	for (i=0; i<spinning; i++) {
		camera_params.camPos[0] = cos((radPerFrame * i) + thetaOffset);
		camera_params.camPos[2] = sin((radPerFrame * i) + thetaOffset);
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/spinning, getTime()-time );
	}
	printProgress( (double)i/spinning, getTime()-time );
	
	int zoomOut = 900; //30 Seconds to Zoom Out to coordinates of 10 all x, y, z, Total: 60
	printf("\nRendering Small Zoom Out, 900 Frames\n"); 
	for (i=0; i<zoomOut; i++) {
		//Zoom out to 5 5 5, we're at 1, 1, 1, this is somewhat of a nice effect
		camera_params.camPos[0] = 0.707106781 + (((5-0.707106781)*i)/zoomOut);
		camera_params.camPos[1] = 1 + ((4*i)/zoomOut);
		camera_params.camPos[2] = 0.707106781 + (((5-0.707106781)*i)/zoomOut);
		
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/zoomOut, getTime()-time );
	}
	printProgress( (double)i/zoomOut, getTime()-time );
	
	
	//New Target: 0.622650 0.622650 0.622650
	int reAngle = 150; // 5 Seconds to Re-Angle, Total: 65
	printf("\nRendering Directed Re - Angle, 150 Frames\n"); 
	for (i=0; i<reAngle; i++) {
	  //target 0.622650, 0.622650, 0.622650, Minor change, almost like a pause before diving
		camera_params.camTarget[0] = (0.622650/reAngle)*i;
		camera_params.camTarget[1] = (0.622650/reAngle)*i; 
		camera_params.camTarget[2] = (0.622650/reAngle)*i; 
		
		//New Cam Up -0.408248 0.816496 -0.408248, this is an upvector
		camera_params.camUp[0] = (-0.408248/reAngle)*i;
		camera_params.camUp[1] = 1-((1-0.816496/reAngle)*i); 
		camera_params.camUp[2] = (-0.408248/reAngle)*i; 
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut); //Adjust 
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/reAngle, getTime()-time );
	}
	printProgress( (double)i/reAngle, getTime()-time );
			
	int Zoom = 900; // 30 seconds to zoom in again, Total: 95
	printf("\nRendering Zoom, 900 Frames\n"); 
	for (i=0; i<Zoom; i++) {
		//Move to 0.65 0.76 0.66, we are at 5 5 5, This is a dive
		camera_params.camPos[0] = 5-(((5-0.65)/Zoom)*i);
		camera_params.camPos[1] = 5-(((5-0.76)/Zoom)*i);
		camera_params.camPos[2] = 5-(((5-0.66)/Zoom)*i);
		
		//Focus on 0.4, 0.4, 0.4, readjust focus, we're at 0.622650, 0.622650, 0.622650
		camera_params.camTarget[0] =0.622650- (((0.622650-0.4)/Zoom)*i);
		camera_params.camTarget[1] =0.622650- (((0.622650-0.4)/Zoom)*i);
		camera_params.camTarget[2] =0.622650- (((0.622650-0.4)/Zoom)*i); 

		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/Zoom, getTime()-time );
	}
	printProgress( (double)i/Zoom, getTime()-time );
			
	
	int ReFocus = 150; // 5 Seconds to refocus, Total: 100
	printf("\nRendering ReFocus, 150 Frames\n"); 
	for (i=0; i<ReFocus; i++) {
		//Move to 0.60 0.76 0.66, we are at 0.65 0.76 0.66
		camera_params.camPos[0] = 0.65- ((0.65-0.6)/ReFocus)*i;
		
		//Focus on 0.54, 0.4, 0.4
		camera_params.camTarget[0] = 0.4 + ((0.54-0.4)/ReFocus)*i;

		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/ReFocus, getTime()-time );
	}
	printProgress( (double)i/ReFocus, getTime()-time );
	
	int moveIn = 300; // 10 seconds to movein more, Total: 110
	printf("\nRendering moveIn, 300 Frames\n"); 
	for (i=0; i<moveIn; i++) {
		//Move to 0.6 0.6 0.6, we are at 0.65 0.76 0.66, slightly bigger move
		camera_params.camPos[1] = 0.76 - ((0.76-0.6)/moveIn)*i;
		camera_params.camPos[2] = 0.66 - ((0.66-0.6)/moveIn)*i;
		
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/moveIn, getTime()-time );
	}
	printProgress( (double)i/moveIn, getTime()-time );
	
	int LookZero = 150; // 5 seconds to look at 0,0,0, Total: 115
	printf("\nRendering look at zero, 150 Frames\n"); 
	for (i=0; i<LookZero; i++) {
		//look at 0 0 0 , we are at 0.54 0.54 0.54, slightly bigger move
		camera_params.camTarget[0] =0.54- (((0.54)/LookZero)*i);
		camera_params.camTarget[1] =0.4- (((0.4)/LookZero)*i);
		camera_params.camTarget[2] =0.4- (((0.4)/LookZero)*i); 
		
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus + moveIn);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/LookZero, getTime()-time );
	}
	printProgress( (double)i/LookZero, getTime()-time );
	
	int MoveTowards = 900; // 30 Seconds to move closer in, Total: 145
	printf("\nRendering MoveTowards, 900 Frames\n"); 
	for (i=0; i<MoveTowards; i++) {
		//Move to 0.56 0.4 0.57, we are at 0.6 0.6 0.6
		camera_params.camPos[0] = 0.6-(((0.6 - 0.56)/Zoom)*i);
		camera_params.camPos[1] = 0.6-(((0.6 - 0.4)/Zoom)*i);
		camera_params.camPos[2] = 0.6-(((0.6 - 0.57)/Zoom)*i);

		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus + moveIn + LookZero);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/MoveTowards, getTime()-time );
	}
	printProgress( (double)i/MoveTowards, getTime()-time );	
	
	int LookOne = 150; // 5 seconds to look at 1,1,0, Total: 150
	printf("\nRendering look at one, 150 Frames\n"); 
	for (i=0; i<LookOne; i++) {
		//look at 1 1 0 , we are at 0 0 0, 
		camera_params.camTarget[0] =(((1)/LookOne)*i);
		camera_params.camTarget[1] =(((1)/LookOne)*i);
		//camera_params.camTarget[2] =(((0.4)/LookOne)*i); 
		
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus + moveIn + LookZero + MoveTowards );
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/LookOne, getTime()-time );
	}
	printProgress( (double)i/LookOne, getTime()-time );
	
	int Scale = 150; // 5 seconds to scale to 0, 4, 10 Total: 155
	printf("\nRendering Scaling, 90 Frames\n"); 
	for (i=0; i<Scale; i++) { //New Scale = 0, Rmin = 4, rFixed = 10;
		mandelBox_params.scale = 2.0 - (2.0/Scale)*i;
		mandelBox_params.rMin = 8.0 - ((8.0-4.0)/Scale)*i;
		mandelBox_params.rFixed = 9 + ((10.0-9.0)/Scale)*i;
		
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus + moveIn + LookZero + MoveTowards + LookOne );
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/Scale, getTime()-time );
	}
	printProgress( (double)i/Scale, getTime()-time );
	
	
	//Jump to 0.31 -.8 0.3 
	int JumpTo = 150; // 5 seconds to jump, Total: 160
	printf("\nRendering Jump, 150 Frames\n"); 
	for (i=0; i<JumpTo; i++) {
		//We are at 0.56 0.4 0.57, Jump to 0.31 -.8 0.3, will clip
		camera_params.camPos[0] = (0.56/JumpTo) * i;
		camera_params.camPos[1] = (-0.8/JumpTo) * i;
		camera_params.camPos[2] = (0.3/JumpTo) * i;

		// 1 1 0 on new Cam Target
		camera_params.camTarget[0] =1 - (1/JumpTo) * i;
		camera_params.camTarget[1] =1- (1/JumpTo) * i;
		//camera_params.camTarget[2] = (0.3/JumpTo) * i;
		//CamUp back to 0 1 0
		camera_params.camUp[0] = 0;
		camera_params.camUp[1] = 1; 
		camera_params.camUp[2] = 0; 
		
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus + moveIn + LookZero + MoveTowards + LookOne + Scale);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/JumpTo, getTime()-time );
	}
	printProgress( (double)i/JumpTo, getTime()-time );	
	
	
	int First_Move = 750; // 25 seconds, moving to new Pos Total: 185
	printf("\nRendering First_Move, 750 Frames\n"); 
	for (i=0; i<First_Move; i++) {
		//Jump from 0.31 -.8 0.3, to 0.26 -.77 0.26 
		camera_params.camPos[0] = 0.31 - ((0.31-0.26)/First_Move) * i;
		camera_params.camPos[1] = -0.8 + ((0.8-0.77)/First_Move) * i;
		camera_params.camPos[2] = 0.3 - ((0.3-0.26)/First_Move) * i;
	
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus + moveIn + LookZero + MoveTowards + LookOne + Scale + JumpTo);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/First_Move, getTime()-time );
	}
	printProgress( (double)i/First_Move, getTime()-time );	
	
	int Second_Move = 750; // 25 seconds, moving to new Pos Total: 210
	printf("\nRendering Second_Move, 750 Frames\n"); 
	for (i=0; i<Second_Move; i++) {
		//Jump from 0.26 -.77 0.26 to 0.24 -.81 0.24 
		camera_params.camPos[0] = 0.26 - ((0.26-0.24)/Second_Move) * i;
		camera_params.camPos[1] = -0.77 - ((0.81-0.77)/Second_Move) * i;
		camera_params.camPos[2] = 0.26 - ((0.26-0.24)/Second_Move) * i;
	
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus + moveIn + LookZero + MoveTowards + LookOne + Scale + JumpTo + First_Move);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/Second_Move, getTime()-time );
	}
	printProgress( (double)i/Second_Move, getTime()-time );
	
	int Third_Move = 750; // 25 seconds, moving to new Pos Total: 235
	printf("\nRendering Third_Move, 750 Frames\n"); 
	for (i=0; i<Third_Move; i++) {
		//New Cam Target 0 0 -1
		camera_params.camTarget[2] = ((-1)/Third_Move) * i;
	
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus + moveIn + LookZero + MoveTowards + LookOne + Scale + JumpTo + First_Move + Second_Move);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/Third_Move, getTime()-time );
	}
	printProgress( (double)i/Third_Move, getTime()-time );		
	
	
	int Fourth_Move = 750; // 25 seconds, moving to new Pos Total: 260
	printf("\nRendering Fourth_Move, 750 Frames\n"); 
	for (i=0; i<Third_Move; i++) {
		//From 0.24 -.81 0.24 to 0.22 -0.77 0.22
		camera_params.camPos[0] = 0.24 - ((0.24-0.22)/Fourth_Move) * i;
		camera_params.camPos[1] = -0.81 + ((0.81-0.77)/Fourth_Move) * i;
		camera_params.camPos[2] = 0.24 - ((0.24-0.22)/Fourth_Move) * i;
		
		//New Cam Target 0 0 0
		camera_params.camTarget[2] = -1 + ((1)/Third_Move) * i;
	
		init3D(&camera_params, &renderer_params);
		renderFractal(camera_params, renderer_params, image, mandelBox_params, total_pix_normal);
		char fileName[80];
		sprintf(fileName, "./output/output_%05d.bmp", i + spinning + zoomOut + reAngle + Zoom + ReFocus + moveIn + LookZero + MoveTowards + LookOne + Scale + JumpTo + First_Move + Second_Move + Third_Move);
		saveBMP(fileName, image, renderer_params.width, renderer_params.height);
		printProgress( (double)i/Fourth_Move, getTime()-time );
	}
	printProgress( (double)i/Fourth_Move, getTime()-time );		
	printf("\n\nAll frames rendered\n");
	
	//printf("\nReduced.x: %f, Reduced.y: %f, Reduced.z: %f\n", total_pix_normal[0], total_pix_normal[1], total_pix_normal[2]);
	free(image);

	return 0;
}
