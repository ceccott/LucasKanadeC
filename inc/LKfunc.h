#ifndef LKFUNC_H
#define LKFUNC_H

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "defines.h"
#include "pgmfunc.h"
#include "algo_config.h"
//include <gsl/gsl_math.h>
#include <math.h>

typedef enum {DX, DY, DT} Deriv;



void Img_Allocate(PGMData* img, int row, int col, int max_gray,uint8_t id);
void Img_Allocate_int(PGMData_int* img, int row, int col, int max_gray,uint8_t id);
void init_LK();

#define isInside(x,y)     ((x>=0) && (x<in_row) && (y>=0) && (y<in_col))
uint8_t ImgSum(const PGMData* img1, const PGMData* img2, PGMData* dest);
uint8_t ImgDiff(const PGMData* img1, const PGMData* img2, PGMData* dest);
uint8_t ImgDeriv1(PGMData* img1, Deriv Di, PGMData* dest,uint8_t prefilt);
uint8_t ImgDeriv2(PGMData* img1, Deriv Di, PGMData* dest,uint8_t prefilt);
uint8_t ImgInteg(PGMData* img1, PGMData* dest);
uint8_t ImgNgbrd(PGMData* img_int, PGMData* dest,uint8_t neighb_size,uint8_t method);
uint8_t ImgProd(const PGMData* img1, const PGMData* img2, PGMData* dest, uint8_t normalize);
uint8_t BuildGrad(const PGMData* img_dx, const PGMData* img_dy, PGMData* dest);
uint8_t GaussFilt(const PGMData* src, PGMData* dest, uint8_t Xbright);
uint8_t PyrDown(const PGMData* src, PGMData* dest, uint8_t method);
uint8_t PyrUp(const PGMData* src, PGMData* dest, uint8_t method);
uint8_t ImgWarp(const PGMData* img,const PGMData* vx,const PGMData* vy, PGMData* dest, uint8_t method);

uint8_t SolveSystem(const PGMData* Gxx,const PGMData* Gxy,const PGMData* Gyy,const PGMData* Gxt,const PGMData* Gyt,PGMData* u,PGMData* v,int threshold,uint8_t level);

void PrintSubMatrix_dbg(float** matrix,int row,int row_off,int col, int col_off);

float sq_root(float x);
float absval(float x);
float findmax(PGMData* img);

void dealloc_all();



#endif
