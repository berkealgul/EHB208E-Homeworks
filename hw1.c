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
Matrix* matrix_from_csv(const char*);
void print_matrix_to_txt(const char*, const Matrix*);
void print_diagonals_to_txt(const char*, const Matrix*);
void print_columns_to_txt(const char*, const Matrix*);

//helper functions
Matrix* insert_data_to_matrix(Matrix*, int, bool);
Matrix_Node* generate_matrix_node(int);

int main()
{
    Matrix* matrix = matrix_from_csv("input.csv"); 
    print_matrix_to_txt("output_print.txt", matrix);
    print_diagonals_to_txt("output.txt", matrix);
    print_columns_to_txt("output.txt", matrix);
    return 0;
}

void print_matrix_to_txt(const char* pszOutputFilePath, const Matrix* pM)
{
    FILE *pFile = fopen(pszOutputFilePath, "w");
    Matrix_Node* pHead = pM->pHead_node;
    Matrix_Node* pRow_head = pHead;

    while(pHead != NULL) // empty matrix will be discarded
    {
        fprintf(pFile, "%d", pHead->nData); // print data
        if(pHead->pNext_column != NULL) // traverse row
        {
            pHead = pHead->pNext_column;
            fprintf(pFile, ","); // print coma if we traverse the row
        }
        else if(pRow_head->pNext_row != NULL) // if row is done, go to next row
        {
            pRow_head = pRow_head->pNext_row;
            pHead = pRow_head;
            fprintf(pFile, "\n"); // print next line for next row
        }
        else { break; } // break if we 
    }

    fclose(pFile);
}

// this function assumes, matrix is square
void print_diagonals_to_txt(const char* pszOutputFilePath, const Matrix* pM)
{
    FILE *pFile = fopen(pszOutputFilePath, "w");
    Matrix_Node* pDiagonal_node = pM->pHead_node;

    while(pDiagonal_node != NULL)
    {
        fprintf(pFile, "%d", pDiagonal_node->nData);

        if(pDiagonal_node->pNext_column != NULL)
        {
            pDiagonal_node = pDiagonal_node->pNext_column->pNext_row;
            fprintf(pFile, ","); 
        }
        else { break; }
    }

    fclose(pFile);
}

void print_columns_to_txt(const char* pszOutputFilePath, const Matrix* pM)
{
    FILE *pFile = fopen(pszOutputFilePath, "a"); // open in 'a' mode to prevent from deleting previous content
    Matrix_Node* pHead = pM->pHead_node;
    Matrix_Node* pColumn_head = pHead;

    // similar algorithm to print_matrix_to_txt() but we traverse columns instead
    fprintf(pFile, "\n");
    while(pHead != NULL) 
    {
        fprintf(pFile, "%d", pHead->nData); 
        if(pHead->pNext_row != NULL) 
        {
            pHead = pHead->pNext_row;
            fprintf(pFile, ","); 
        }
        else if(pColumn_head->pNext_column != NULL) 
        {
            pColumn_head = pColumn_head->pNext_column;
            pHead = pColumn_head;
            fprintf(pFile, ","); 
        }
        else { break; } 
    }

    fclose(pFile);
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
    pM->pHead_node = NULL;

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

    fclose(pFile);
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

    if(pM->pHead_node == NULL) // If head node is empty, then generate it and return
    {
        pM->pHead_node = pNode;
        return pM;
    }

    //Iterate to last row
    Matrix_Node* pHead = pM->pHead_node;
    Matrix_Node* pPrev_head;
    while(pHead->pNext_row != NULL)
    {
        pPrev_head = pHead;
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
        // Iterate head and prev_head node to last column and insert new column node 
        while(pHead->pNext_column != NULL)
        {
            pHead = pHead->pNext_column;

            if(pPrev_head != NULL) 
            {
                pPrev_head = pPrev_head->pNext_column;
            }
        }

        pHead->pNext_column = pNode;

        if(pPrev_head != NULL) 
        {
            pPrev_head->pNext_column->pNext_row = pNode; 
        }
    }

    return pM;
}

Matrix_Node* generate_matrix_node(int nData)
{
    Matrix_Node *pHead_node = malloc(sizeof(Matrix_Node));

    if(pHead_node == NULL)
    {
        printf("ERROR: Memory allocation during node generation has failed!!!");
        exit(1);
    }

    pHead_node->nData = nData;
    pHead_node->pNext_row = NULL;
    pHead_node->pNext_column = NULL;

    return pHead_node;
}