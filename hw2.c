// Berke Algül
// 040230762
// 6.12.2023

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// nodes of student linked list
typedef struct StudentNode {
    struct StudentNode* next;
    char data;
} StudentNode;

/*
Linked list to contains a single student
what pointers point to:
              name_start    year_start
                  |             |
head -> 0 4 ... 2 ; a l i ... n ; 2 0 0 0

aim is to get easy access to name and year of the student
*/ 
typedef struct Student {
    struct StudentNode* head; // first node
    struct StudentNode* name_start; // start of name of the student (or first ;)
    struct StudentNode* year_start; // start of birth year of the student (or second ;)
} Student;

// stack node
typedef struct StackNode {
    struct StackNode* next;
    struct Student* student;
} StackNode;

// stack to store students
typedef struct Stack {
    struct StackNode* top;
} Stack;

// stack funtions
void push(Stack*, Student*);
bool empty(Stack*);
Student* pop(Stack*);
Student* top(Stack*);
Stack* init_stack();

// main functions
Stack* read_students_from_txt(const char*);
Stack* sort_stack_by_names(Stack*);

// helper functions
Student* create_student(const char*);
StudentNode* new_student_node(char);
StackNode* new_stack_node(Student*);

// student compare functions
// returns if a < b
bool cmp_std_year(const Student* a, const Student* b); // compares by birth year
bool cmp_std_code(const Student* a, const Student* b); // compares by faculty code

int main()
{
    Stack* student_stack = read_students_from_txt("input.txt");
    student_stack = sort_stack(student_stack, &cmp_std_year); // sort stack by year
    //student_stack = sort_stack(student_stack, &cmp_std_code); // sort stack by faculty code
    return 0;
}

// delete top element of stack
Student* pop(Stack* stack)
{
    if(empty(stack)) { return NULL;} // if stack is empty return nullptr
    StackNode* popped = stack->top; // get popped node
    Student* popped_student = popped->student; // get popped student from stack node
    stack->top = stack->top->next; // change top node to its next
    free(popped); // free memory of popped stack node
    return popped_student;
}

// return top student of the stack
Student* top(Stack* stack)
{
    if(empty(stack)) { return NULL; } // if stack is empty return null pointer
    return stack->top->student;
}

Stack* sort_stack(Stack* stack, bool (*cmp_func)(const Student*, const Student*))
{
    Stack* sorted_stack = init_stack();

    while(!empty(stack))
    {
        Student* tmp = pop(stack);
        
        while(!empty(sorted_stack) && cmp_func(top(sorted_stack), tmp))
        {
            push(stack, pop(sorted_stack));
        }
        
        push(sorted_stack, tmp);
    }

    return sorted_stack;
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

bool empty(Stack* s)
{
    return s->top == NULL;
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
    char line[100000]; // char array for storing a single line

    // read each line of the input file
    while (!feof(pFile))
    {
        fgets(line, 100000, pFile); // read line
        push(stack, create_student(line)); // push stack a new student
    }

    fclose(pFile);
    return stack;
}

Student* create_student(const char* student_line)
{
    // initialize new student and assing first node for head
    Student* student = malloc(sizeof(Student));
    student->head = new_student_node(student_line[0]);
    student->name_start = NULL;
    student->year_start = NULL;

    // prepare for line iteration
    StudentNode* current = student->head;
    int i = 1;
    char c = student_line[1];

    // start iterate line
    while(c != '\n')
    {
        current->next = new_student_node(c);
        current = current->next;

        // if we detect split character ; we set checkpoint pointers for name and birth year of student
        if(c == ';')
        {
            // first checkpoint will be for student name 
            if(student->name_start == NULL)
            {
                student->name_start = current;
            }
            // second is year
            {
                student->year_start = current;
            }
        }

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