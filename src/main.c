#include "main.h"


//#define DEBUG
#define OUTPUT_ENABLED

int i,j,k,l=0;
char filename[20];
void DebugMatrices(int row, int scales,int n_el);
void PrintSubMatrix_int(int** matrix,int row,int row_off,int col, int col_off);
void PrintSubMatrix(float** matrix,int row,int row_off,int col, int col_off);
void SaveToText(float** matrix,char* fname,int row,int col, FILE* fp);

FILE* fp = NULL;

int main(){

    printf("Start\n" );

    init_LK();

    //readPGM("frame10.pgm", src);
    //readPGM("frame11.pgm", src2);

    src =(PGMData_int*)malloc(sizeof(PGMData_int));
    src2 =(PGMData_int*)malloc(sizeof(PGMData_int));

    src->max_gray = 255;
    src->col = IMG_W;
    src->row = IMG_H;
    src->matrix = allocate_dynamic_matrix_int(src->row,src->col);

    src2->max_gray = 255;
    src2->col = IMG_W;
    src2->row = IMG_H;
    src2->matrix = allocate_dynamic_matrix_int(src2->row,src2->col);

    for(i=0;i<IMG_H;i++)
        for(j=0;j<IMG_W;j++){
            src->matrix[i][j] = rand() % 255;
            src2->matrix[i][j] = rand() % 255;
        }


    //feeding pyramid vectors
    for (i = 0; i < src->row; i++)
        for (j = 0; j < src->col; j++){
            // img1_scales->matrix[i][j] = src->matrix[i][j];
            // img2_scales->matrix[i][j] = src2->matrix[i][j];
            img1_scales->matrix[i][j] = frame10[i][j];
            img2_scales->matrix[i][j] = frame11[i][j];
        }

#ifdef DEBUG
    printf("buffer filled %d %.2f\n",src->matrix[10][10],img1_scales->matrix[10][10]);
#endif

    //calculating pyramid scales
    for (i = 0; i < SCALES-1; i++){
        PyrDown(&img1_scales[i],&img1_scales[i+1],PDWN_method);
        PyrDown(&img2_scales[i],&img2_scales[i+1],PDWN_method);
    }

    //ImgSum(&img1_scales[0],&img2_scales[0],&img_buffer[0]);

    img_warp[3] = img2_scales[3];

    //compute flow for different scales
    for (i = SCALES-1; i > -1; i--) {
        ImgSum(&img1_scales[i],&img_warp[i],&img_sum[i]);
        ImgDiff(&img_warp[i],&img1_scales[i],&img_diff[i]);
        ImgDiff(&img1_scales[i],&img_warp[i],&img_diff[i]);
        //derivatives
        ImgDeriv(&img_sum[i],DX,&img_dx[i],1);
        ImgDeriv(&img_sum[i],DY,&img_dy[i],1);
        ImgDeriv(&img_diff[i],DT,&img_dt[i],0);
        //X-products
        ImgProd(&img_dx[i],&img_dx[i],&img_xx[i],0); // 0 means not normalized to max_gray
        ImgProd(&img_dy[i],&img_dy[i],&img_yy[i],0);
        ImgProd(&img_dx[i],&img_dy[i],&img_xy[i],0);
        ImgProd(&img_dy[i],&img_dt[i],&img_ty[i],0);
        ImgProd(&img_dx[i],&img_dt[i],&img_xt[i],0);

        //integrals
        ImgInteg(&img_xx[i],&img_ixx[i]);
        ImgInteg(&img_yy[i],&img_iyy[i]);
        ImgInteg(&img_xy[i],&img_ixy[i]);
        ImgInteg(&img_xt[i],&img_ixt[i]);
        ImgInteg(&img_ty[i],&img_ity[i]);

        //neighbourhoods
        ImgNgbrd(&img_ixx[i],&img_Gxx[i],NGBD_SIZE,NGBRD_method);
        ImgNgbrd(&img_iyy[i],&img_Gyy[i],NGBD_SIZE,NGBRD_method);
        ImgNgbrd(&img_ixy[i],&img_Gxy[i],NGBD_SIZE,NGBRD_method);
        ImgNgbrd(&img_ixt[i],&img_Gxt[i],NGBD_SIZE,NGBRD_method);
        ImgNgbrd(&img_ity[i],&img_Gty[i],NGBD_SIZE,NGBRD_method);

        SolveSystem(&img_Gxx[i],&img_Gxy[i],&img_Gyy[i],&img_Gxt[i],&img_Gty[i],&img_u[i],&img_v[i],SYST_THSH,i);

        //flow update
        for(k=0;k<img_u[i].row;k++)
            for (l = 0; l < img_u[i].col; l++) {
                img_u[i].matrix[k][l] += img_u_up[i].matrix[k][l];
                img_v[i].matrix[k][l] += img_v_up[i].matrix[k][l];
            }

        //upscaling
        if(i > 0){
            PyrUp(&img_u[i],&img_u_up[i-1],1);
            PyrUp(&img_v[i],&img_v_up[i-1],1);

            ImgWarp(&img2_scales[i-1],&img_u_up[i-1],&img_v_up[i-1],&img_warp[i-1],WRP_method);

            //  GaussFilt(&img_buffer[i-1], &img_warp[i-1],1);

        }
    }


#ifdef OUTPUT_ENABLED

#ifdef DEBUG
    PrintSubMatrix(img_u[2].matrix,10,7,10,7);
    float max = findmax(&img_u[2]);
    printf("max in matrix: %f\n",max);
    DebugMatrices(0,1,10);
    PrintSubMatrix(img_xx->matrix,3,7,3,7);
    PrintSubMatrix(img_ixx->matrix,2,8,2,8);
    printf("%.2f\n",img_buffer->matrix[6][6]);
#endif

    dest->col = img_warp->col;
    dest->row = img_warp->row;
    dest->max_gray = 255;

    dest->matrix = allocate_dynamic_matrix_int(dest->row,dest->col);

#ifdef DEBUG
    PrintSubMatrix(img_xx[3].matrix,0,10,0,7);
    PrintSubMatrix(img_ixx[3].matrix,25,10,0,7);
    PrintSubMatrix(img_Gxx[3].matrix,25,10,0,7);
#endif

    int k = 0;

    BuildGrad(&img_u[k],&img_v[k],&img_buffer[k]);

    for (i = 0; i < img_warp[k].row; i++)
        for (j = 0; j < img_warp[k].col; j++)
            dest->matrix[i][j] =(img_buffer[k].matrix[i][j])*100;


    SaveToText(img_u->matrix,"out_flow_u.txt",img_u->row, img_u->col, fp);
    SaveToText(img_v->matrix,"out_flow_v.txt",img_v->row, img_u->col, fp);
#ifdef DEBUG
    SaveToText(img1_scales[k].matrix,"out_debug",img_diff[k].row,img_diff[k].col,fp);
#endif

    //output debug
    for(i=0;i<SCALES;i++){
        sprintf(filename,"img_buf_%d",i);
        //writePGM(filename,&img_u[i]);
    }
    writePGM("out_img.pgm",&dest[0]);
    //writePGM("img_buf",&img_buffer[0]);
#endif
    dealloc_all();
    printf("Stop\n" );
    printf("Optic Flow result in out_img.pgm\n");

    return 0;
}

/* ---------------------- EXTRA FUNCTIONS -------------------------------------*/

void DebugMatrices(int row, int scales,int n_el){
    int k=0;
    int j=0;

    for(k=scales-1;k>-1;k--){
        printf("- - - img_deriv - - -\n");
        printf("i=%d :  ",k);
        for(j=0;j<n_el;j++){
            printf("dx=%f\t",img_dx[k].matrix[row][j]);}
        printf("\n");
        printf("- - - img_prod - - -\n");
        printf("i=%d :  ",k);
        for(j=0;j<n_el;j++){
            printf("xx= %f\t",img_xx[k].matrix[row][j]);}
        printf("\n");
        printf("- - - img_integ - - -\n");
        printf("i=%d :  ",k);
        for(j=0;j<n_el;j++){
            printf("ixx=%f\t",img_ixx[k].matrix[row][j]);}
        printf("\n\n\n");
    }
}

void PrintSubMatrix_int(int** matrix,int row,int row_off,int col, int col_off){
    int i=0;
    int j=0;
    printf("- - - SubMatrix - - -\n");
    for(i=row;i<row+row_off;i++){
        for(j=col;j<col+col_off;j++)
            printf("%d\t",matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void PrintSubMatrix(float** matrix,int row,int row_off,int col, int col_off){
    int i=0;
    int j=0;
    printf("- - - SubMatrix - - -\n");
    for(i=row;i<row+row_off;i++){
        for(j=col;j<col+col_off;j++)
            printf("%.2f\t",matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void SaveToText(float** matrix,char* fname,int row,int col, FILE* fp){

    int i=0;
    int j=0;

    fp = fopen(fname,"w");
    if ( fp == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; i++){
        for (j = 0; j < col; j++)
            fprintf(fp, "%.4f ",matrix[i][j]);

        fprintf(fp, "\n");
    }
    fclose(fp);
}
