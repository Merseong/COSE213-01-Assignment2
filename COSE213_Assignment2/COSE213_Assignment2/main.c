#include <stdio.h>

/*
Sparse Matrices

mread: Read in a sparse matrix
mwrite: Write out a sparse matrix
merase: Erase a sparse matrix
madd: Create the sparse matrix d = a + b
mmult: Create the sparse matrix d = a * b
mtranspose: Create the sparse matrix b = a^T
*/

typedef struct _matrix matrix;

void mread(matrix mat);
void mwrite();
void merase(matrix mat);
matrix madd(matrix left, matrix right);
matrix mmult(matrix left, matrix right);
matrix mtranspose(matrix mat);

int UIreader();

int main()
{
	printf("hello world!");
	return 0;
}