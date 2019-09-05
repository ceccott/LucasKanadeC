#ifndef DEFS
#define DEFS

#define HI(num) (((num) & 0x0000FF00) >> 8)   //endianess with max pixel value > 255
#define LO(num) ((num) & 0x000000FF)

typedef struct _PGMData {
    int row;
    int col;
    int max_gray;
    float **matrix;
} PGMData;

typedef struct _PGMData_int {
    int row;
    int col;
    int max_gray;
    int **matrix;
} PGMData_int;

typedef struct _PGMData_long {
    int row;
    int col;
    int max_gray;
    long **matrix;
} PGMData_long;


#endif
