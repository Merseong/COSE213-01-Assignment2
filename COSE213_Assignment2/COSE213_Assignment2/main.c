#include <stdio.h>
#include <stdlib.h>
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
	struct _matrixNode* down; // if type is header, down is pointing first top-left term.
	union {
		struct _matrixNode* entry;
		struct _topNode* top;
	} right;
} matrixNode;

typedef struct _topNode {
	int number;
	struct _topNode* next;
	matrixNode* down;
	matrixNode* right;
} topNode;

matrixNode* matrices[MAX_MATRICES]; // array of headers of all matrix
int nextEmpty = 0;
int matCount = 0;

// functions which can use on consol UI
matrixNode* mread();
void mwrite(matrixNode* mat);
void merase(matrixNode* mat);
matrixNode* madd(matrixNode* left, matrixNode* right);
matrixNode* mmult(matrixNode* left, matrixNode* right);
matrixNode* mtranspose(matrixNode* mat);

// functions which uses for other function
void ClearBuf();
matrixNode* minit(int _row, int _col);
int MakeEntry(matrixNode* mat, int _row, int _col, int _value);
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
		printf("弛 help: Browse commands\n");
		printf("弛 quit: Turn off this program\n");
		printf("弛 cls: Clear screen and show main menu\n");
		printf("弛 allmat: See all matrices with list\n");
		printf("弛 mread: Read in sparse matrix and make it\n");
		printf("弛 mwrite: Write out a sparse matrix\n");
		printf("弛 madd: Create the sparse matrix d = a + b\n");
		printf("弛\n戌式式式式式  Command Help END\n");
	}
	return;
}

// scan users input and conduct that command
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
			printf("[ERROR] there are no matrix.\n");
			return 0;
		}
		for (int i = 0; i < nextEmpty; ++i)
		{
			if (matrices[i] != NULL)
			{
				printf(" index [%d], %d x %d, %d elements\n", i, matrices[i]->row, matrices[i]->col, matrices[i]->value);

			}
		}
	}
	else if (!strcmp(_input, "mread")) mread();
	else if (!strcmp(_input, "mwrite"))
	{
		int matIndex;
		printf(" Enter the index of matrix you want to view.\n\t>>> ");
		scanf("%d", &matIndex);
		ClearBuf();
		if (matrices[matIndex] != NULL) mwrite(matrices[matIndex]);
		else printf("[ERROR] there are no matrix in that index.\n");
	}
	else if (!strcmp(_input, "madd"))
	{
		int _left, _right;
		printf(" type two indexes to add.\n\t>>> ");
		scanf("%d %d", &_left, &_right);
		if (matrices[_left] != NULL && matrices[_right] != NULL)
		{
			madd(matrices[_left], matrices[_right]);
		}
		else printf("[ERROR] invalid indexes.\n");
	}
	else printf("[ERROR] wrong input, try again.\n");

	return 0;
}

// make new empty matrix which have _rowSize and _colSize
matrixNode* minit(int _rowSize, int _colSize)
{
	if (_rowSize < 1 || _colSize < 1) // error when invalid size
	{
		printf("[ERROR] row and column must be bigger than 0.\n");
		return NULL;
	}

	matrixNode* out = (matrixNode*)malloc(sizeof(matrixNode));

	out->type = header;
	out->row = _rowSize;
	out->col = _colSize;
	out->value = 0;
	out->down = NULL;

	int topCount = max(_rowSize, _colSize);
	topNode* top = (topNode*)malloc(sizeof(topNode)); // make tops and link them
	out->right.top = top;
	top->down = NULL;
	top->right = NULL;
	top->number = 0;
	for (int i = 0; i < topCount - 1; ++i)
	{
		topNode* newtop = (topNode*)malloc(sizeof(topNode));
		newtop->down = NULL;
		newtop->right = NULL;
		newtop->number = i + 1;

		top->next = newtop;
		top = newtop;
	}
	top->next = NULL; // last top's next will be NULL

	matrices[nextEmpty++] = out;
	matCount++;

	return out;
}

// read in sparse matrix and make it
matrixNode* mread()
{
	int _row, _col, _ele;
	printf(" Enter the size of rows, columns and the number of elements(nonzero terms)\n\t>>> ");
	scanf("%d %d %d", &_row, &_col, &_ele);
	ClearBuf();
	if (_ele > _row * _col)
	{
		printf("[ERROR] number of element is bigger than number of row * column.\n");
		return NULL;
	}

	matrixNode* out = minit(_row, _col);
	if (out == NULL) return NULL;
	out->value = _ele;

	printf(" Enter the term's row, column and value.(row and column is zero-based)\n");
	for (int i = 0; i < _ele; ++i)
	{
		int t_row, t_col, t_val; // term's row, col, value
		printf("\t>>> ");
		scanf("%d %d %d", &t_row, &t_col, &t_val);
		ClearBuf();
		if (MakeEntry(out, t_row, t_col, t_val))
		{
			--i; continue;
		}
	}

	mwrite(out);

	return out;
}

// print matrix
void mwrite(matrixNode* mat)
{
	matrixNode* currentNode = mat->down;
	topNode* currentTop = mat->right.top;

	int index = -1;
	for (int i = 0; i < nextEmpty; i++)
	{
		if (matrices[i] == mat)
		{
			index = i;
			break;
		}
	}
	printf("\n index [%d], %d x %d, %d elements\n\n", index, mat->row, mat->col, mat->value);

	for (int rowCnt = 0; rowCnt < mat->row; ++rowCnt)
	{
		printf(" | ");
		for (int colCnt = 0; colCnt < mat->col; ++colCnt)
		{
			if (currentNode != NULL && currentNode->row == rowCnt && currentNode->col == colCnt)
			{
				printf("%2d ", currentNode->value);
				currentNode = currentNode->right.entry;
			}
			else printf(" 0 ");
		}
		printf(" |\n");
		if (currentTop->next != NULL) currentTop = currentTop->next;
		if (currentNode == NULL) currentNode = currentTop->right;
	}
}

// make new Entry and put this in to matrix.
int MakeEntry(matrixNode* mat, int _row, int _col, int _value)
{
	// error when row and col is out of range
	if (mat->row <= _row || mat->col <= _col)
	{
		printf("[ERROR] row and col must be smaller than matrix's rowSize and colSize. try again.\n");
		return -1;
	}
	// error when value is zero
	if (_value == 0)
	{
		printf("[ERROR] your input is zero term. type non-zero term again.\n");
		return -1;
	}

	matrixNode* newEntry = (matrixNode*)malloc(sizeof(matrixNode));
	
	newEntry->type = entry;
	newEntry->row = _row;
	newEntry->col = _col;
	newEntry->value = _value;

	topNode* rowTop = mat->right.top;
	topNode* colTop = mat->right.top;
	while (rowTop->next != NULL && rowTop->number != _row) rowTop = rowTop->next;
	while (colTop->next != NULL && colTop->number != _col) colTop = colTop->next;
	
	// when this is first term
	if (mat->down == NULL)
	{
		mat->down = newEntry;
		newEntry->down = NULL;
		newEntry->right.entry = NULL;
		rowTop->right = newEntry;
		colTop->down = newEntry;
	}
	else
	{
		newEntry->down = NULL;
		newEntry->right.entry = NULL;

		matrixNode* rowBefore = NULL;
		matrixNode* rowNext = rowTop->right;
		matrixNode* colBefore = NULL;
		matrixNode* colNext = colTop->down;

		while (newEntry->right.entry == NULL) // sort row
		{
			if (rowNext != NULL)
			{
				if (rowNext->col > _col)
				{
					newEntry->right.entry = rowNext;
					if (rowBefore != NULL) rowBefore->right.entry = newEntry;
					else rowTop->right = newEntry; // when this term is first right term of that top node
				}
				else if (rowNext->col == _col) // same position
				{
					printf("[ERROR] there are already other term. try another.\n");
					free(newEntry);
					return -1;
				}
				else // rowNext->col < _col
				{
					rowBefore = rowNext;
					rowNext = rowNext->right.entry;
				}
			}
			else // first term of that empty top node or last term
			{
				if (rowBefore != NULL) rowBefore->right.entry = newEntry; // last term of top node
				else rowTop->right = newEntry; // first term of that empty top node
				break;
			}
		}

		while (newEntry->down == NULL)
		{
			if (colNext != NULL)
			{
				if (colNext->row > _row)
				{
					newEntry->down = colNext;
					if (colBefore != NULL) colBefore->down = newEntry;
					else colTop->down = newEntry;
				}
				else // colNext->row < _row
				{
					colBefore = colNext;
					colNext = colNext->down;
				}
			}
			else
			{
				if (colBefore != NULL) colBefore->down = newEntry;
				else colTop->down = newEntry;
				break;
			}
		}

		if (mat->down->row > _row) mat->down = newEntry;
		else if (mat->down->row == _row && mat->down->col > _col) mat->down = newEntry;
	}
	return 0;
}

// add two matrices and make new matrix
matrixNode* madd(matrixNode* left, matrixNode* right)
{
	if ((left->col != right->col) || (left->row != right->row))
	{
		printf("[ERROR] two inputs do not have same size of row and column.\n");
		return NULL;
	}

	matrixNode* out = minit(left->row, left->col);

	topNode* currentTopL = left->right.top;
	topNode* currentTopR = right->right.top;
	matrixNode* currentNodeL = currentTopL->right;
	matrixNode* currentNodeR = currentTopR->right;
	while (currentTopL != NULL)
	{
		if (currentNodeL != NULL && currentNodeR != NULL)
		{
			if (currentNodeL->col < currentNodeR->col)
			{
				MakeEntry(out, currentNodeL->row, currentNodeL->col, currentNodeL->value);
				currentNodeL = currentNodeL->right.entry;
				out->value++;
			}
			else if (currentNodeL->col == currentNodeR->col)
			{
				int _val = currentNodeL->value + currentNodeR->value;
				if (_val != 0)
				{
					MakeEntry(out, currentNodeL->row, currentNodeL->col, _val);
					currentNodeL = currentNodeL->right.entry;
					currentNodeR = currentNodeR->right.entry;
					out->value++;
				}
			}
			else // NodeL->col > NodeR->col
			{
				MakeEntry(out, currentNodeR->row, currentNodeR->col, currentNodeR->value);
				currentNodeR = currentNodeR->right.entry;
				out->value++;
			}
		}
		else if (currentNodeL != NULL)
		{
			MakeEntry(out, currentNodeL->row, currentNodeL->col, currentNodeL->value);
			currentNodeL = currentNodeL->right.entry;
			out->value++;
		}
		else if (currentNodeR != NULL)
		{
			MakeEntry(out, currentNodeR->row, currentNodeR->col, currentNodeR->value);
			currentNodeR = currentNodeR->right.entry;
			out->value++;
		}
		else // both node is NULL
		{
			currentTopL = currentTopL->next;
			currentTopR = currentTopR->next;
			if (currentTopL != NULL) currentNodeL = currentTopL->right;
			if (currentTopR != NULL) currentNodeR = currentTopR->right;
		}
	}

	mwrite(out);
	return out;
}