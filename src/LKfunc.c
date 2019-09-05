#include "LKfunc.h"

PGMData_int*  src;
PGMData_int*  src2;
PGMData_int*  dest;

PGMData*  img1_scales;
PGMData*  img2_scales;
PGMData*  img_buffer;
PGMData*  img_sum;
PGMData*  img_diff;
PGMData*  img_dx;
PGMData*  img_dy;
PGMData*  img_dt;
PGMData*  img_warp;

PGMData*  img_xx;
PGMData*  img_yy;
PGMData*  img_xy;
PGMData*  img_ty;
PGMData*  img_xt;

PGMData*  img_ixx;
PGMData*  img_iyy;
PGMData*  img_ixy;
PGMData*  img_ity;
PGMData*  img_ixt;

PGMData*  img_Gxx;
PGMData*  img_Gyy;
PGMData*  img_Gxy;
PGMData*  img_Gty;
PGMData*  img_Gxt;

PGMData*  img_u;
PGMData*  img_v;

PGMData*  img_u_up;
PGMData*  img_v_up;


float** matrix;

void Img_Allocate_int(PGMData_int* img, int row, int col, int max_gray,uint8_t id){
  img[id].row = row;
  img[id].col = col;
  img[id].max_gray = max_gray;
  (img[id].matrix) = allocate_dynamic_matrix_int(row,col); //level-wise allocation
}

void Img_Allocate(PGMData* img, int row, int col, int max_gray,uint8_t id){
  img[id].row = row;
  img[id].col = col;
  img[id].max_gray = max_gray;
  (img[id].matrix) = allocate_dynamic_matrix(row,col); //level-wise allocation
}


void init_LK(){

  uint8_t level=0;

  src = (PGMData_int*) malloc(sizeof(PGMData));
  src2 = (PGMData_int*) malloc(sizeof(PGMData));
  dest = (PGMData_int*) malloc(sizeof(PGMData));

  img1_scales= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img2_scales= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_buffer= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_sum= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_diff= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_warp = (PGMData*) malloc(sizeof(PGMData)*SCALES);

//Derivatives
  img_dx= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_dy= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_dt= (PGMData*) malloc(sizeof(PGMData)*SCALES);

//X-crossproducts
  img_xx= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_yy= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_xy= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_ty= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_xt= (PGMData*) malloc(sizeof(PGMData)*SCALES);

  //Integral images
  img_ixx= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_iyy= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_ixy= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_ity= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_ixt= (PGMData*) malloc(sizeof(PGMData)*SCALES);

//Neighbouroods images
  img_Gxx= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_Gyy= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_Gxy= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_Gty= (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_Gxt= (PGMData*) malloc(sizeof(PGMData)*SCALES);

//Flow vectors
  img_u     = (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_v     = (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_u_up  = (PGMData*) malloc(sizeof(PGMData)*SCALES);
  img_v_up  = (PGMData*) malloc(sizeof(PGMData)*SCALES);

  //memory allocation for input images scales
  for (level = 0; level < SCALES; level++) {

      Img_Allocate(img1_scales,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img2_scales,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_buffer,IMG_H_L,IMG_W_L,255,level);

      //Flow Buffers
      Img_Allocate(img_sum,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_diff,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_dx,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_dy,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_dt,IMG_H_L,IMG_W_L,255,level);

      Img_Allocate(img_xx,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_yy,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_xy,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_ty,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_xt,IMG_H_L,IMG_W_L,255,level);

      Img_Allocate(img_ixx,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_iyy,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_ixy,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_ity,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_ixt,IMG_H_L,IMG_W_L,255,level);

      Img_Allocate(img_Gxx,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_Gyy,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_Gxy,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_Gty,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_Gxt,IMG_H_L,IMG_W_L,255,level);

      Img_Allocate(img_u,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_v,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_u_up,IMG_H_L,IMG_W_L,255,level);
      Img_Allocate(img_v_up,IMG_H_L,IMG_W_L,255,level);

      Img_Allocate(img_warp,IMG_H_L,IMG_W_L,255,level);

    //printf("W=%d H=%d  MGV=%d\n",img1_scales[level].col,img1_scales[level].row,img1_scales[level].max_gray);
  }
      matrix = allocate_dynamic_matrix(IMG_H,IMG_W);
}

uint8_t ImgWarp(const PGMData* img,const PGMData* vx,const PGMData* vy, PGMData* dest, uint8_t method){

  int i=0;
  int j=0;
  int dx=0;
  int dy=0;
  float fracx=0;
  float fracy=0;

  dest->row = img->row;
  dest->col = img->col;
  dest->max_gray = img->max_gray;

  #ifdef DEBUG
  printf("Input Img1: W=%d H=%d  MGV=%d\n",img->col,img->row,img->max_gray);
  printf("Input Img1: W=%d H=%d  MGV=%d\n",vx->col,vx->row,vx->max_gray);
  printf("Input Img1: W=%d H=%d  MGV=%d\n",vy->col,vy->row,vy->max_gray);
  #endif

  if(method == 0){

  for (i = 0; i < img->row; i++)
    for(j=0; j< img->col;j++){
      fracy = vy->matrix[i][j]+i;
      fracx = vx->matrix[i][j]+j;
      dx = (int) fracx;
      dy = (int) fracy;

      fracx = fracx - dx;
      fracy = fracy - dy;

      //handling borders
      dy = (dy<img->row-1) ? dy: (img->row-2);
      dx = (dx<img->col-1) ? dx: (img->col-2);
      dy = (dy<0) ? 0:dy;
      dx = (dx<0) ? 0:dx;

      //dest->matrix[i][j] = img->matrix[i][j];
      // dest->matrix[i][j] = (img->matrix[dx][dy+1]-img->matrix[dx+1][dy+1])*fracx +
      // (img->matrix[dx+1][dy]-img->matrix[dx+1][dy+1])*fracy +
      // (img->matrix[dx+1][dy+1] - img->matrix[dx][dy+1] - img->matrix[dx+1][dy] + img->matrix[dx][dy])*fracx*fracy;

      dest->matrix[i][j] = img->matrix[dy][dx];
      dest->matrix[i][j] += (img->matrix[dy][dx+1]-img->matrix[dy][dx])*fracx +
                            (img->matrix[dy+1][dx]-img->matrix[dy][dx])*fracy +
                            (img->matrix[dy+1][dx+1]+img->matrix[dy][dx] -
                            img->matrix[dy][dx+1]-img->matrix[dy+1][dx])*fracx*fracy;
    }
  }

  if(method == 1){

  for (i = 0; i < img->row-1; i++)
    for(j=0; j< img->col-1;j++){
      fracx = i+vx->matrix[i][j]; //truncate to int
      fracy = j+vy->matrix[i][j];
      dx =(int) fracx;
      dy =(int) fracy;

      fracx = fracx - dx;
      fracy = fracy - dy;

      // fracx = absval(fracx);
      // fracx = absval(fracy);

      //handling borders
      dx = (dx<img->row-1) ? dx: (img->row-2);
      dy = (dy<img->col-1) ? dy: (img->col-2);
      dx = (dx<0) ? 0:dx;
      dy = (dy<0) ? 0:dy;

      dest->matrix[dx][dy] = img->matrix[i][j];
      dest->matrix[dx+1][dy] += (img->matrix[i+1][j]-img->matrix[i][j])*fracx;
      dest->matrix[dx][dy+1] += (img->matrix[i][j+1]-img->matrix[i][j])*fracy;
      dest->matrix[dx+1][dy+1] += (-img->matrix[i][j]+img->matrix[i+1][j+1]+
                                  img->matrix[i+1][j]+img->matrix[i][j+1])*(fracx*fracy);

      }
      }
  return 0;
}

uint8_t SolveSystem(const PGMData* Gxx,const PGMData* Gxy,const PGMData* Gyy,const PGMData* Gxt,const PGMData* Gyt,PGMData* u,PGMData* v,int threshold,uint8_t level){
  int i=0;
  int j=0;

  u->row = Gxx->row;
  u->col = Gxx->col;
  u->max_gray = Gxx->max_gray;

  v->row = Gxx->row;
  v->col = Gxx->col;
  v->max_gray = Gxx->max_gray;

  float det= 0;

  for(i=0;i<Gxx->row;i++)
    for(j=0;j<Gxx->col;j++){
      det = Gxx->matrix[i][j]*Gyy->matrix[i][j] - Gxy->matrix[i][j]*Gxy->matrix[i][j];
      if(absval(det) < SYST_THSH){
        u->matrix[i][j] = 0;
        v->matrix[i][j] = 0;
      }
      else{
        u->matrix[i][j] =-(Gxt->matrix[i][j]*Gyy->matrix[i][j] - Gxy->matrix[i][j]*Gyt->matrix[i][j])/det;
        v->matrix[i][j] =-(Gxx->matrix[i][j]*Gyt->matrix[i][j] - Gxt->matrix[i][j]*Gxy->matrix[i][j])/det;
      }
  }

  return 0;
}

uint8_t ImgNgbrd(PGMData* img_int, PGMData* dest,uint8_t neighb_size, uint8_t method){
    int i=0;
    int j=0;
    int in_row = img_int->row;
    int in_col = img_int->col;

    uint8_t off_f = floor(neighb_size/2);
    uint8_t off_c = off_f + 1;

    PGMData*  tmp;

    dest->row = img_int->row;
    dest->col = img_int->col;
    dest->max_gray = img_int->max_gray;

    if(method ==0){ //iint extension by mirroring

    tmp = (PGMData*) malloc(sizeof(PGMData));
    tmp->row = img_int->row+2*off_c;
    tmp->col = img_int->col+2*off_c;
    tmp->max_gray = img_int->max_gray;
    tmp->matrix = allocate_dynamic_matrix(img_int->row+2*off_c,img_int->col+2*off_c);

    //matrix padding (mirroring)
    for (i = 0; i < off_c; i++)     //top and bottom
      for (j = 0; j < img_int->col;j++){
        tmp->matrix[i][off_c+j] = img_int->matrix[off_c-1-i][j];
        tmp->matrix[tmp->row-1-i][off_c+j] = img_int->matrix[img_int->row-off_c+i][j];
      }

    for (j = 0; j < off_c; j++)     //left and right
      for (i = 0; i <img_int->row;i++){
        tmp->matrix[off_c+i][j] = img_int->matrix[i][off_c-1-j];
        tmp->matrix[off_c+i][tmp->col-1-j] = img_int->matrix[i][img_int->col-off_c+j];
      }

    for (i = 0; i < img_int->row; i++)     //area
      for (j = 0; j < img_int->col;j++)
        tmp->matrix[i+off_c][j+off_c] = img_int->matrix[i][j];

      //TODO handle corners

      for (i = off_f; i >= 0; i--)     //corners
        for (j = off_c; j>= 0;j--){
          tmp->matrix[i][j] = (tmp->matrix[i+1][j+1])/2;
        }

      //   for (i = off_f; i >= 0; i--)     //corners
      //     for (j = tmp->col; j>= off_f;j--){
      //       tmp->matrix[i][j] = (tmp->matrix[i+1][j] + tmp->matrix[i][j-1])/2;
      //     }


    //extract neighbourhoods
    for (i = 0; i < img_int->row; i++)
      for (j = 0; j < img_int->col;j++) {
        dest->matrix[i][j] =  tmp->matrix[i+off_c+off_f][j+off_c+off_f] -
                              tmp->matrix[i+off_c-off_c][j+off_c+off_f] -
                              tmp->matrix[i+off_c+off_f][j+off_c-off_c] +
                              tmp->matrix[i+off_c-off_c][j+off_c-off_c];
    }
  }
    if(method ==1){ //inner neighbourhoods and after padding

    for (i = off_c; i < img_int->row-off_c; i++)  //extract neighbourhoods
      for (j = off_c; j < img_int->col-off_c;j++) {
        dest->matrix[i][j] =  img_int->matrix[i+off_f][j+off_f] -
                              img_int->matrix[i-off_c][j+off_f] -
                              img_int->matrix[i+off_f][j-off_c] +
                              img_int->matrix[i-off_c][j-off_c];
    }

    for (i = off_c; i < dest->row-off_c; i++)  //padding cols
      for (j = 0; j < off_c;j++){
        dest->matrix[i][j] = dest->matrix[i][off_c];
        dest->matrix[i][dest->col-1-j]=dest->matrix[i][dest->col-off_c];
      }

    for (i = 0; i < off_c; i++)  //padding rows
      for (j = 0; j < dest->col;j++){
        dest->matrix[i][j] = dest->matrix[off_c][j];
        dest->matrix[dest->row-1-i][j] = dest->matrix[dest->row-off_c][j];
      }
   }
    if(method ==2){

       for(i=0;i<img_int->row;i++)
        for(j=0;j<img_int->col;j++){
          dest->matrix[i][j] =  (isInside(i+off_f, j+off_f) ? img_int->matrix[i+off_f][j+off_f] : 0) -
                                (isInside(i-off_c,j+off_f) ? img_int->matrix[i-off_c][j+off_f] : 0) -
                                (isInside(i+off_f,j-off_c) ? img_int->matrix[i+off_f][j-off_c] : 0) +
                                (isInside(i-off_c,j-off_c) ? img_int->matrix[i-off_c][j-off_c] : 0);
        }

        //printf("%.2f = %.2f - %.2f - %.2f + %.2f\n",dest->matrix[i][j],(isInside(i+off_f,j+off_f) ? img_int->matrix[i+off_f][j+off_f] : 0),(isInside(i+off_f,j-off_c) ? img_int->matrix[i+off_f][j-off_c] : 0),(isInside(i-off_c,j-off_c) ? img_int->matrix[i-off_c][j-off_c] : 0));
        //scanf("debug%d\n",&tmp);
       }
    // PrintSubMatrix_dbg(dest->matrix,0,7,0,7);
    // scanf("breakpt %c\n");

   return 0;
}

uint8_t ImgInteg(PGMData* img1, PGMData* dest){
  int i=0;
  int j=0;

  dest->col = img1->col;
  dest->row = img1->row;
  dest->max_gray = img1->max_gray;
  for (i = 0; i < img1->row; i++)
    for (j = 0; j < img1->col;j++) {
      dest->matrix[i][j] = img1->matrix[i][j] +
                           (((i-1) < 0) ? 0 : dest->matrix[i-1][j]) +
                           (((j-1) < 0) ? 0 : dest->matrix[i][j-1]) -
                           (( ((i-1) < 0) || ((j-1) < 0)) ? 0 : dest->matrix[i-1][j-1]);
    }

  return 0;
}

uint8_t BuildGrad(const PGMData* img_dx, const PGMData* img_dy, PGMData* dest){

  int i=0;
  int j=0;

  dest->col = img_dx->col;
  dest->row = img_dx->row;
  dest->max_gray = img_dx->max_gray;

  for(i=0;i<dest->row;i++)
    for(j=0;j<dest->col;j++){
      dest->matrix[i][j] = (sq_root(img_dx->matrix[i][j]*img_dx->matrix[i][j]+ //sqrt is slow ..
                           img_dy->matrix[i][j]*img_dy->matrix[i][j]));
  //handling saturation
      // if (dest->matrix[i][j] >= 255)
      //       dest->matrix[i][j] = 254;
      }
  return 0;
}

uint8_t ImgDeriv2(PGMData* img1, Deriv Di, PGMData* dest, uint8_t prefilt){

  return 0;
}

uint8_t ImgDeriv1(PGMData* img1, Deriv Di, PGMData* dest, uint8_t prefilt){
  int i=0;
  int j=0;

  PGMData*  tmp;

  if(prefilt > 0){
    tmp = (PGMData*) malloc(sizeof(PGMData));
    tmp->row = img1->row;
    tmp->col = img1->col;
    tmp->max_gray = img1->max_gray;
    tmp->matrix = allocate_dynamic_matrix(img1->row,img1->col);
    GaussFilt(img1,tmp,1);}
  else
    tmp = img1;

  if(Di == DX){
  //handling area
    for(i=1;i<img1->row-1;i++)
      for(j=1;j<img1->col-1;j++){
        dest->matrix[i][j] = tmp->matrix[i-1][j-1]*sobel_ker_x[0][0]+
                            tmp->matrix[i][j-1]*sobel_ker_x[1][0]+
                            tmp->matrix[i+1][j-1]*sobel_ker_x[2][0]+
                            tmp->matrix[i-1][j]*sobel_ker_x[0][1]+
                            tmp->matrix[i][j]*sobel_ker_x[1][1]+
                            tmp->matrix[i+1][j]*sobel_ker_x[2][1]+
                            tmp->matrix[i-1][j+1]*sobel_ker_x[0][2]+
                            tmp->matrix[i][j+1]*sobel_ker_x[1][2]+
                            tmp->matrix[i+1][j+1]*sobel_ker_x[2][2];

      }

  //border handling
    for (i = 0; i < img1->row; i++) {
      dest->matrix[i][0] = dest->matrix[i][1];
      dest->matrix[i][img1->col-1] = dest->matrix[i][img1->col-2];
    }
    for (i = 0; i < img1->col; i++) {
      dest->matrix[0][i] = dest->matrix[1][i];
      dest->matrix[img1->row-1][i] = dest->matrix[img1->row-2][i];
    }


  }else
  if(Di == DY){
    for(i=1;i<img1->row-1;i++)
      for(j=1;j<img1->col-1;j++)
        dest->matrix[i][j]= tmp->matrix[i-1][j-1]*sobel_ker_y[0][0]+
                            tmp->matrix[i][j-1]*sobel_ker_y[1][0]+
                            tmp->matrix[i+1][j-1]*sobel_ker_y[2][0]+
                            tmp->matrix[i-1][j]*sobel_ker_y[0][1]+
                            tmp->matrix[i][j]*sobel_ker_y[1][1]+
                            tmp->matrix[i+1][j]*sobel_ker_y[2][1]+
                            tmp->matrix[i-1][j+1]*sobel_ker_y[0][2]+
                            tmp->matrix[i][j+1]*sobel_ker_y[1][2]+
                            tmp->matrix[i+1][j+1]*sobel_ker_y[2][2];
  //border handling
    for (i = 0; i < img1->row; i++) {
      dest->matrix[i][0] = dest->matrix[i][1];
      dest->matrix[i][img1->col-1] = dest->matrix[i][img1->col-2];
    }
    for (i = 0; i < img1->col; i++) {
      dest->matrix[0][i] = dest->matrix[1][i];
      dest->matrix[img1->row-1][i] = dest->matrix[img1->row-2][i];
    }


  }else
  if(Di == DT){
    //smoothing only
    GaussFilt(tmp,dest,1);

  }else return 1;

  return 0;
}


uint8_t ImgProd(const PGMData* img1, const PGMData* img2, PGMData* dest, uint8_t normalize){
    int i=0;
    int j=0;

  if((img1->col == img2->col) && (img1->row == img2->row)){

  if(normalize>0)
    for(i=0;i<img1->row;i++)
      for(j=0;j<img2->col;j++)
        dest->matrix[i][j] = img1->matrix[i][j]*img2->matrix[i][j]/img1->max_gray;
  else
    for(i=0;i<img1->row;i++)
      for(j=0;j<img2->col;j++)
      dest->matrix[i][j] = img1->matrix[i][j]*img2->matrix[i][j];

    } else{
        perror("E@ImgProd:sizes do not match\n");
        return 1;
      }

    return 0;
}


uint8_t ImgDiff(const PGMData* img1, const PGMData* img2, PGMData* dest){
    int i=0;
    int j=0;

  if((img1->col == img2->col) && (img1->row == img2->row)){
    for(i=0;i<img1->row;i++)
      for(j=0;j<img2->col;j++)
        dest->matrix[i][j] =( img1->matrix[i][j] - img2->matrix[i][j]);
    } else return 1;

    return 0;
}

uint8_t ImgSum(const PGMData* img1, const PGMData* img2, PGMData* dest){
    int i=0;
    int j=0;

  if((img1->col == img2->col) && (img1->row == img2->row)){
    for(i=0;i<img1->row;i++)
      for(j=0;j<img2->col;j++)
        dest->matrix[i][j] = (img1->matrix[i][j] + img2->matrix[i][j])/2;
  } else return 1;
    return 0;
}

uint8_t GaussFilt(const PGMData* src, PGMData* dest, uint8_t Xbright){
  int i=0;
  int j=0;

  dest->row = src->row;
  dest->col = src->col;
  dest->max_gray = src->max_gray;


  for(i=1;i<src->row-1;i++)
    for(j=1;j<src->col-1;j++)
      dest->matrix[i][j] =(src->matrix[i][j]*gauss_ker[1][1] +
                         (src->matrix[i+1][j]+src->matrix[i-1][j]+
                          src->matrix[i][j+1]+src->matrix[i][j-1])*gauss_ker[0][1]+
                         (src->matrix[i+1][j+1]+src->matrix[i-1][j-1]+
                          src->matrix[i-1][j+1]+src->matrix[i+1][j-1])*gauss_ker[0][0])*Xbright;

  //border handling
  for (i = 0; i < src->row; i++) {
    dest->matrix[i][0] = dest->matrix[i][1];
    dest->matrix[i][src->col-1] = dest->matrix[i][src->col-2];
  }

  for (i = 0; i < src->col; i++) {
    dest->matrix[0][i] = dest->matrix[1][i];
    dest->matrix[src->row-1][i] = dest->matrix[src->row-2][i];
  }
  return 0;
}

//upscaling
uint8_t PyrUp(const PGMData* src, PGMData* dest, uint8_t method){
    int i=0;
    int j=0;

    PGMData*  tmp;

    dest->row = 2*src->row;
    dest->col = 2*src->col;
    dest->max_gray = src->max_gray;

  if(method == 0){ //linear interpolation
    for(i=1;i<src->row;i++)
      for(j=1;j<src->col;j++){
        dest->matrix[2*i-1][2*j-1] = (src->matrix[i][j]+src->matrix[i-1][j-1]+src->matrix[i][j-1]+src->matrix[i-1][j])/4;
        dest->matrix[2*i-2][2*j-1] = (src->matrix[i-1][j-1]+src->matrix[i-1][j])/2;
        dest->matrix[2*i-1][2*j-2] = (src->matrix[i-1][j-1]+src->matrix[i][j-1])/2;
        dest->matrix[2*i-1][2*j]   = (src->matrix[i-1][j]+src->matrix[i][j])/2;
        dest->matrix[2*i][2*j-1]   = (src->matrix[i][j-1]+src->matrix[i][j])/2;
        dest->matrix[2*i-2][2*j-2] = src->matrix[i-1][j-1];
        dest->matrix[2*i][2*j]     = src->matrix[i][j];
        dest->matrix[2*i][2*j-2]   = src->matrix[i][j-1];
        dest->matrix[2*i-2][2*j]   = src->matrix[i-1][j];
    }
  }

  if(method == 1){ //zero filling + gaussian
    tmp = (PGMData*) malloc(sizeof(PGMData));
    tmp->row = 2*src->row;
    tmp->col = 2*src->col;
    tmp->max_gray = src->max_gray;
    tmp->matrix = allocate_dynamic_matrix(2*src->row,2*src->col);

    for(i=0;i<src->row;i++)
      for(j=0;j<src->col;j++){
        tmp->matrix[2*i][2*j] = src->matrix[i][j];
        tmp->matrix[2*i+1][2*j+1] = 0;
      }
    GaussFilt(tmp,dest,4);
  }

  return 0;
}


uint8_t PyrDown(const PGMData* src, PGMData* dest, uint8_t method){

    int i=0;
    int j=0;
    int in_row = src->row;
    int in_col = src->col;

    if(method == 1){
    PGMData*  tmp;
    tmp = (PGMData*) malloc(sizeof(PGMData));
    tmp->row = src->row;
    tmp->col = src->row;
    tmp->max_gray = src->max_gray;
    tmp->matrix = allocate_dynamic_matrix(src->row,src->col);

    for(i=1;i<src->row-1;i++)
      for(j=1;j<src->col-1;j++)
        tmp->matrix[i][j] = src->matrix[i][j]*gauss_ker[1][1] +
                           (src->matrix[i+1][j]+src->matrix[i-1][j]+
                            src->matrix[i][j+1]+src->matrix[i][j-1])*gauss_ker[0][1]+
                           (src->matrix[i+1][j+1]+src->matrix[i-1][j-1]+
                            src->matrix[i-1][j+1]+src->matrix[i+1][j-1])*gauss_ker[0][0];

    //border handling
    for (i = 0; i < src->row; i++) {
      tmp->matrix[i][0] = tmp->matrix[i][1];
      tmp->matrix[i][src->col-1] = tmp->matrix[i][src->col-2];
    }

    for (i = 0; i < src->col; i++) {
      tmp->matrix[0][i] = tmp->matrix[1][i];
      tmp->matrix[src->row-1][i] = tmp->matrix[src->row-2][i];
    }


  //downsampling
    for (i = 0;i < src->row/2; i++)
       for (j = 0; j< src->col/2; j++)
         dest->matrix[i][j] = tmp->matrix[2*i][2*j];


    deallocate_dynamic_matrix(tmp->matrix,tmp->row);
    free(tmp);
  }else
  if(method == 0){
    int m,n;
    int off = PDWN_fsize/2;

    for(i=1;i<src->row-1;i+=2)
      for(j=1;j<src->col-1;j+=2){
          for(m=0;m<PDWN_fsize;m++)
            dest->matrix[i/2][j/2] += gauss_filt[m]* (isInside(i, j-off+m) ? src->matrix[i][j-off+m] : 0);
          for(m=0;m<PDWN_fsize;m++)
            dest->matrix[i/2][j/2] += gauss_filt[m]* (isInside(i-off+m, j) ? src->matrix[i-off+m][j] : 0);
      }

  }
    return 0;
}

float absval(float x){
     if (x < 0)
         x = -x;
     return x;
}

 //newton method square root
float sq_root(float x){
      float guess = 1;

    while(absval((guess * guess) / x  - 1.0) >= 0.0001 )
          guess = ((x/guess) + guess) / 2;

      return guess;
}

float findmax(PGMData* img){
  int i=0;
  int j=0;

  float max = img->matrix[0][0];

  for (i = 0;i < img->row; i++)
     for (j = 0; j< img->col; j++)
      if(img->matrix[i][j] > max)
        max = img->matrix[i][j];

  return max;
}

void dealloc_all(){

int i=0;

//deallocate_dynamic_matrix(src->matrix,src->row);
//deallocate_dynamic_matrix(src2->matrix,src2->row);
//deallocate_dynamic_matrix(dest->matrix,dest->row);

for(i=0;i<SCALES;i++){
//deallocate_dynamic_matrix(img1_scales[0].matrix,img1_scales[0].row);
// deallocate_dynamic_matrix(img2_scales->matrix,img2_scales->row);
// deallocate_dynamic_matrix(img_buffer->matrix,img_buffer->row);
// deallocate_dynamic_matrix(img_sum->matrix,img_sum->row);
// deallocate_dynamic_matrix(img_diff->matrix,img_diff->row);
// deallocate_dynamic_matrix(img_warp->matrix,img_warp->row);
// deallocate_dynamic_matrix(img_dx->matrix,img_dx->row);
// deallocate_dynamic_matrix(img_dy->matrix,img_dy->row);
// deallocate_dynamic_matrix(img_dt->matrix,img_dt->row);
// deallocate_dynamic_matrix(img_xx->matrix,img_xx->row);
// deallocate_dynamic_matrix(img_yy->matrix,img_yy->row);
// deallocate_dynamic_matrix(img_xy->matrix,img_xy->row);
// deallocate_dynamic_matrix(img_xt->matrix,img_xt->row);
// deallocate_dynamic_matrix(img_ty->matrix,img_ty->row);
}

free(src);
free(src2);
free(dest);
free(img1_scales);
free(img2_scales);
free(img_buffer);
free(img_sum);
free(img_diff);
free(img_warp);
free(img_dx);
free(img_dy);
free(img_dt);
free(img_xx);
free(img_yy);
free(img_xy);
free(img_ty);
free(img_xt);

}



void PrintSubMatrix_dbg(float** matrix,int row,int row_off,int col, int col_off){
    int i=0;
    int j=0;
  printf("- - - SubMatrix - - -\n");
    for(i=row;i<row+row_off;i++){
      for(j=col;j<col+col_off;j++)
        printf("%f\t",matrix[i][j]);
      printf("\n");
    }
  printf("\n");
}
