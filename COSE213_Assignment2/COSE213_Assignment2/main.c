#include <stdio.h>
#include <string.h>
#include <Windows.h>

#define LONGEST_COMMAND 6
#define MAX_MATRICES 50

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

matrixNode* matrices[MAX_MATRICES]; // headers of all matrix
int nextEmpty = 0;
int matCount = 0;

// functions which can use on consol UI
matrixNode* mread(matrixNode* mat);
void mwrite(matrixNode mat);
void merase(matrixNode* mat);
matrixNode madd(matrixNode left, matrixNode right);
matrixNode mmult(matrixNode left, matrixNode right);
matrixNode mtranspose(matrixNode mat);

// functions which uses for other function
void ClearBuf();
matrixNode* minit(int row, int col);
void MakeNode(matrixNode* mat, int row, int col, int value);
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

// clear input buffer
void ClearBuf()
{
	while (getchar() != '\n');
}

// print main menu(0) or command help(1)
void UImenu(int mode)
{
	if (!mode) // main menu
	{
		printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖\n");
		printf("弛     Sparse Matrix Calculator     弛\n");
		printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");
	}
	else if (mode) // command help
	{
		printf("忙式式式式式  Command Help\n弛\n");
		printf("弛 help: browse commands\n");
		printf("弛 quit: turn off this program\n");
		printf("弛 cls: clear screen and show main menu\n");
		printf("弛 allmat: see all matrices with list\n");
		printf("弛\n戌式式式式式  Command Help END\n");
	}
	return;
}

int UIreader()
{
	char _input[LONGEST_COMMAND + 1]; // size is come from longest command char
	printf("\n>> ");
	scanf_s("%s", _input, LONGEST_COMMAND + 1);
	ClearBuf();

	if		(!strcmp(_input, "help")) UImenu(1);
	else if (!strcmp(_input, "cls"))
	{
		system("cls");
		UImenu(0);
	}
	else if (!strcmp(_input, "quit")) return 1;
	else if (!strcmp(_input, "allmat"))
	{
		if (nextEmpty == 0)
		{
			printf("[WARNING] there are no matrix.");
			return 0;
		}
		for (int i = 0; i < nextEmpty; i++)
		{
			if (matrices[i] != NULL)
			{
				printf(" index %d, %d x %d, %d elements\n", i, matrices[i]->row, matrices[i]->col, matrices[i]->value);

			}
		}
	}
	else printf("[ERROR] wrong input, try again.");

	return 0;
}