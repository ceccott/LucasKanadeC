#ifndef ALG_CONF
#define ALG_CONF

#define IMG_H           388
#define IMG_W           584

#define IMG_H_L         (IMG_H>>level)
#define IMG_W_L         (IMG_W>>level)

#define SCALES          4
#define DERIV_FILT_L    3   //lenght of the derivative filter
/*
#define IMG_BITS
#define DERIV_BITS
#define XPROD_DER_BITS
#define INT_IMG_BITS
*/

/* - - - Filter Params - - - */
#define PDWN_fsize      11

#define SIGMA           (1 + (S1 - 7)*0.2)
static float gauss_filt[11] = {0.0047,0.0188,0.0554,0.1198,0.1903,0.221,0.1903,0.1198,0.554,0.0188,0.0047}; //norm to 1.22
static float gauss_ker[3][3]={{0.0625,0.125,0.0625},{0.125,0.250,0.125},{0.0625,0.125,0.0625}};
static float sobel_ker_y[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};
static float sobel_ker_x[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};

/* - - - Settings - - - */

#define NGBD_SIZE       8   //neighbourood size

#define SYST_THSH       3   //threshold for system determinant
#define WARP_RATIO      1

// 0.00    17.71   38.27   84.41   125.78  130.38  230.78
// 0.00    103.51  105.64  218.12  380.25  385.20  616.24
// 0.00    292.95  303.77  610.32  1232.10 1251.59 1616.73
// 0.00    497.50  554.12  1026.63 2509.03 2742.59 3155.72

//  - - - methods - - -
#define NGBRD_method    0
#define WRP_method      0
#define PDWN_method     0

#define ImgDeriv(img1,Di,dest,prefilt)   ImgDeriv1(img1,Di,dest,prefilt)

#define  VERBOSE


#endif
