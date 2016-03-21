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

#ifndef _3d_H
#define _3d_H


#define NEAR 1
#define FAR 100
#include "vec3d.h"
#include "color.h"
#include "camera.h"
#include "renderer.h"
#include "mandelbox.h"

void   LoadIdentity   (double *matrix);
void   Perspective    (double fov, double aspect, double zNear, double zFar, double *projMatrix);
void   Frustum        (double left, double right, double bottom, double top, double znear, double zfar, double *matrix);
void   LookAt         (double *eye, double *target, double *up, double *modelMatrix);
double LengthVector   (double *vector);
void   NormalizeVector(double *vector);
void   ComputeNormalOfPlane(double *normal, double *v1, double *v2);
void   MultiplyMatrices(double *result, const double *matrix1, const double *matrix2);
void   MultiplyMatrixByVector(double *resultvector, double *matrix, double *pvector);
int    InvertMatrix(double *m, double *out);
void   Translate(double *result, double x, double y, double z);
int UnProject(double winX, double winY, double view[4], double matrix[16], double *obj);
void rayMarch(const RenderParams &render_params, const vec3 &from, const vec3  &direction, double eps, pixelData& pix_data, MandelBoxParams mandelBox_params);
vec3 getColour(const pixelData &pixData, const RenderParams &render_params, const vec3 &from, const vec3  &direction);
/*int    UnProject(double winX, double winY, double winZ, const double *model, const double *proj, const int *view, double *obj);
void rayMarch(const RenderParams &render_params, const vec3 &from, const vec3  &direction, double eps, pixelData& pix_data, MandelBoxParams mandelBox_params);
vec3 getColour(const pixelData &pixData, const RenderParams &render_params, const vec3 &from, const vec3  &direction);*/
double clamp(double d, double min, double max); 
void lighting(const vec3 &n, const vec3 &color, const vec3 &pos, const vec3 &direction,  vec3 &outV);
double DE(const vec3 &p, MandelBoxParams mandelBox_params);
void normal(const vec3 & p, vec3 & normal, MandelBoxParams mandelBox_params);
double MandelBoxDE(const vec3 &pos, const MandelBoxParams &mPar, double c1, double c2);
double MandelBulbDistanceEstimator(const vec3 &p0, const MandelBoxParams params);
#endif
