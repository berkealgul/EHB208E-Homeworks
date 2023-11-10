// Berke Algül
// 040230762
// 7.11.2023

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Matrix_Node {
    struct Matrix_Node* pNext_column;
    struct Matrix_Node* pNext_row;
    int nData;
} Matrix_Node;

typedef struct Matrix {
    Matrix_Node *pNode;
} Matrix;

// main functions
Matrix* matrix_from_csv(const char*);
Matrix* print_matrix_to_txt(const char *);

//helper functions
Matrix* insert_data_to_matrix(Matrix*, int, bool);
Matrix_Node* generate_matrix_node(int);

int main()
{
    Matrix* matrix = matrix_from_csv("input.csv"); 

    //if(!matrix) { return 1; } // terminate the program if matrix is null pointer



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

    char row[100000]; //row with predefined maximum char size for row of csv file
    char *cToken;
    
    // read each row 
    while (!feof(pFile))
    {
        fgets(row, 100000, pFile);

        cToken = strtok(row, ",");

        // extract tokens from colums
        while(cToken != NULL)
        {
            int nData = strtol(cToken, NULL, 10);



            cToken = strtok(NULL, ",");
        }
    }
}

Matrix* insert_data_to_matrix(Matrix* pM, int nData, bool bNewRow)
{
    // 
    if(!pM->pNode)
    {
        pM->pNode = generate_matrix_node(nData);
        return pM;
    }

    if(bNewRow)
    {

    }
    

    return pM;
}

Matrix_Node* generate_matrix_node(int nData)
{
    Matrix_Node *pNode = malloc(sizeof(Matrix_Node));

    if(!pNode)
    {
        printf("ERROR: Memory allocation during node generation has failed!!!");
        return NULL;
    }

    pNode->nData = nData;

    return pNode;
}