#ifndef PGMGUNC
#define PGMFUNC

// PGM images R/W functions

#include "stdio.h"
#include "stdlib.h"
#include "defines.h"


int **allocate_dynamic_matrix_int(int row, int col);
float **allocate_dynamic_matrix(int row, int col);
void deallocate_dynamic_matrix(float **matrix, int row);

PGMData_int* readPGM(const char *file_name, PGMData_int *data);
void writePGM(const char *filename, const PGMData_int *data);
void SkipComments(FILE *fp);



#endif
