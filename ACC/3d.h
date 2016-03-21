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

#include "camera.h"
#include "renderer.h"


inline void   LoadIdentity   (double *matrix);
inline void   Perspective    (double fov, double aspect, double zNear, double zFar, double *projMatrix);
inline void   Frustum        (double left, double right, double bottom, double top, double znear, double zfar, double *matrix);
inline void   LookAt         (double *eye, double *target, double *up, double *modelMatrix);
inline double LengthVector   (double *vector);
inline void   NormalizeVector(double *vector);
inline void   ComputeNormalOfPlane(double *normal, double *v1, double *v2);
inline void   MultiplyMatrices(double *result, const double *matrix1, const double *matrix2);
inline void   MultiplyMatrixByVector(double *resultvector, double *matrix, double *pvector);
inline int    InvertMatrix(double *m, double *out);
inline void   Translate(double *result, double x, double y, double z);



#endif
