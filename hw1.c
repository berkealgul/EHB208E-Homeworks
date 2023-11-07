// Berke Algül
// 040230762
// 7.11.2023

#include "stdio.h"
#include "string.h"

typedef struct Matrix_Node {
    struct Matrix_Node* pNext_column;
    struct Matrix_Node* pNext_row;
    int nData;
} Matrix_Node;

typedef struct Matrix {
    Matrix_Node *pNode;
} Matrix;

Matrix* matrix_from_csv(const char*);
Matrix* print_matrix_to_txt(const char *);


int main()
{
    Matrix* matrix = matrix_from_csv("input.csv"); 

    if(!matrix) { return 1; } // terminate the program if matrix is null pointer

    printf("cyka");

    return 0;
}

Matrix* matrix_from_csv(const char *pszInputFilePath)
{
    FILE *pFile = fopen(pszInputFilePath, "r");

    if(!pFile)
    {
        printf("Couldn't read input file. Terminating!");
        return NULL;
    }
}