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
    Matrix_Node *pHead_node;
} Matrix;

// main functions
void print_matrix_to_txt(const char*, const Matrix*);
Matrix* matrix_from_csv(const char*);

//helper functions
Matrix* insert_data_to_matrix(Matrix*, int, bool);
Matrix_Node* generate_matrix_node(int);

int main()
{
    Matrix* matrix = matrix_from_csv("input.csv"); 

    if(!matrix) { return 1; } // terminate the program if matrix is null pointer

    print_matrix_to_txt("output_print.txt", matrix);

    return 0;
}

void print_matrix_to_txt(const char* pszOutputFilePath, const Matrix* pM)
{
    Matrix_Node* pHead = pM->pHead_node;

    while(pHead)
    {
        printf("%d ", pHead->nData);

        if(pHead->pNext_column)
        {
        
        }
    }
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
    
    Matrix* pM = malloc(sizeof(Matrix));

    // read each row 
    while (!feof(pFile))
    {
        fgets(row, 100000, pFile);
        cToken = strtok(row, ",");
        bool bNewRow = true;

        // extract data from colums
        while(cToken != NULL)
        {
            int nData = strtol(cToken, NULL, 10);
            insert_data_to_matrix(pM, nData, bNewRow);
            cToken = strtok(NULL, ",");
            bNewRow = false; 
        }
    }

    return pM;
}


/* 
    Data insertion starts from top left and values are inserted in row major order

    1, 2, 3            
    4, 5, 6
    7, 8, 9
*/
Matrix* insert_data_to_matrix(Matrix* pM, int nData, bool bNewRow)
{
    Matrix_Node* pNode = generate_matrix_node(nData);

    if(!pM->pHead_node) // If head node is empty, then generate it and return
    {
        pM->pHead_node = pNode;
        return pM;
    }

    //Iterate to last row
    Matrix_Node* pHead = pM->pHead_node;
    while(pHead->pNext_row)
    {
        pHead = pHead->pNext_row;
    }

    // If we create new row, then insert new row node
    if(bNewRow) 
    {
        pHead->pNext_row = pNode;
    }
    // else insert data to end of last row
    else
    {
        // Iterate head node to last column and insert new column node 
        while(pHead->pNext_column)
        {
            pHead = pHead->pNext_column;
        }
        pHead->pNext_column = pNode;
    }

    return pM;
}

Matrix_Node* generate_matrix_node(int nData)
{
    Matrix_Node *pHead_node = malloc(sizeof(Matrix_Node));

    if(!pHead_node)
    {
        printf("ERROR: Memory allocation during node generation has failed!!!");
        return NULL;
    }

    pHead_node->nData = nData;

    return pHead_node;
}