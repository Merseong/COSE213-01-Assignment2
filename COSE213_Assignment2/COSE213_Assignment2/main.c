#include <stdio.h>

/*
Sparse Matrices

mread: Read in a sparse matrix (scanf)
mwrite: Write out a sparse matrix (printf)
merase: Erase a sparse matrix
madd: Create the sparse matrix d = a + b
mmult: Create the sparse matrix d = a * b
mtranspose: Create the sparse matrix b = a^T
*/

typedef enum {header, entry} tag;
typedef struct _matrixNode {
	int row;
	int col;
	int value;
	tag type;
	struct _matrixNode* down;
	struct _matrixNode* right;
} matrixNode;

typedef struct _topNode {
	struct _topNode* next;
	matrixNode* down;
	matrixNode* right;
} topNode;

matrixNode* matrices[50]; // array of all matrices's header node
int nextEmpty = 0;

// functions which can use on consol UI
matrixNode* mread(matrixNode* mat);
void mwrite(matrixNode mat);
void merase(matrixNode* mat);
matrixNode madd(matrixNode left, matrixNode right);
matrixNode mmult(matrixNode left, matrixNode right);
matrixNode mtranspose(matrixNode mat);

// functions which uses for other function
matrixNode* minit(int row, int col);
void makeNode(matrixNode* mat, int row, int col, int value);
int UIreader();
void UImenu(int mode);

int main()
{
	UImenu(0);
	UImenu(1);
	while (1)
	{
		if (UIreader()) break;
	}
	return 0;
}