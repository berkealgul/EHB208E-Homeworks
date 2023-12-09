// Berke Algül
// 040230762
// 6.12.2023

#include <stdio.h>
#include <stdlib.h>

typedef struct StudentNode {
    struct StudentNode* next;
    char data;
} StudentNode;

typedef struct Student {
    struct StudentNode* head; // first node
    struct StudentNode* name_start; // start of name of student (or first ;)
    struct StudentNode* year_start; // start of birth year of student (or second ;)
} Student;

typedef struct StackNode {
    struct StackNode* next;
    struct Student* student;
} StackNode;

typedef struct Stack {
    struct StackNode* top;
} Stack;

// stack funtions
void push(Stack*, Student*);
Stack* init_stack();
// main functions
Stack* read_students_from_txt(const char*);
void sort_stack_by_names(Stack*);
// helper functions
Student* create_student(const char*);
StudentNode* new_student_node(char);
StackNode* new_stack_node(Student*);

int main()
{
    Stack* student_stack = read_students_from_txt("input.txt");
    return 0;
}

void sort_stack_by_names(Stack* stack)
{

}

void push(Stack* stack, Student* student)
{
    // if stack is empty we fill the top node
    StackNode* newNode = new_stack_node(student);
    if(stack->top == NULL)
    {
        stack->top = newNode;
        return;
    }
    // if not update top node
    newNode->next = stack->top; // put new on on top and assing old top to next
    stack->top = newNode;
}

Stack* init_stack()
{
    Stack* s = malloc(sizeof(Stack)); // allocate memory for stack
    s->top = NULL; // make top node null
    return s;
}

// creates stack of students from file
Stack* read_students_from_txt(const char* inputFilePath)
{
    // try to open file in read mode. If read fails than terminate the program
    FILE *pFile = fopen(inputFilePath, "r");
    if(!pFile)
    {
        printf("Couldn't read input file. Terminating!");
        exit(1);
    }

    Stack* stack = init_stack();
    char row[100000]; //char array for storeing row

    // read each row 
    while (!feof(pFile))
    {
        fgets(row, 100000, pFile);
        push(stack, create_student(row));
    }

    fclose(pFile);
    return stack;
}

Student* create_student(const char* student_line)
{
    Student* student = malloc(sizeof(Student));
    student->head = new_student_node(student_line[0]);
    StudentNode* current = student->head;
    int i = 1;
    char c = student_line[1];

    while(c != '\n')
    {
        current->next = new_student_node(c);
        current = current->next;
        c = student_line[++i];
    }

    return student;
}

// Create new student node with char c
StudentNode* new_student_node(char c)
{
    StudentNode* node = malloc(sizeof(StudentNode)); // allocate memory for new node
    // assing default values and return the node
    node->next = NULL;
    node->data = c;
    return node;
}

// Create new stack node
StackNode* new_stack_node(Student* student)
{
    StackNode* node = malloc(sizeof(StackNode)); // allocate memory for new node
    // assing default values and return the node
    node->next = NULL;
    node->student = student;
    return node;
}