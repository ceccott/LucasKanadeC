#ifndef MAIN_H
#define MAIN_H

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include <math.h>
#include "LKfunc.h"
#include "algo_config.h"
#include "defines.h"
#include "pgmfunc.h"
//#include <strings.h>
//#include <gsl/gsl_math.h>

#include "frames.h"

extern PGMData_int*  src;
extern PGMData_int*  src2;
extern PGMData_int*  dest;

extern PGMData*  img1_scales;
extern PGMData*  img2_scales;
extern PGMData*  img_buffer;

extern PGMData*  img_sum;
extern PGMData*  img_diff;
extern PGMData*  img_dx;
extern PGMData*  img_dy;
extern PGMData*  img_dt;
extern PGMData*  img_warp;

extern PGMData*  img_xx;
extern PGMData*  img_yy;
extern PGMData*  img_xy;
extern PGMData*  img_ty;
extern PGMData*  img_xt;

extern PGMData*  img_ixx;
extern PGMData*  img_iyy;
extern PGMData*  img_ixy;
extern PGMData*  img_ity;
extern PGMData*  img_ixt;

extern PGMData*  img_Gxx;
extern PGMData*  img_Gyy;
extern PGMData*  img_Gxy;
extern PGMData*  img_Gty;
extern PGMData*  img_Gxt;

extern PGMData*  img_u;
extern PGMData*  img_v;
extern PGMData*  img_u_up;
extern PGMData*  img_v_up;

extern float** matrix;



#endif /* ifndef MAIN_H */
