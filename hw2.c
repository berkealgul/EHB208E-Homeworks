// Berke Algül
// 040230762
// 6.12.2023

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// nodes of student linked list
typedef struct StudentNode {
    struct StudentNode* next;
    char data;
} StudentNode;

/*
Linked list that contains a single student
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
Stack* sort_stack(Stack*, bool(*)(const Student*, const Student*));

// helper functions
Student* create_student(const char*);
StudentNode* new_student_node(char);
StackNode* new_stack_node(Student*);

// student compare functions
// returns if a < b
bool cmp_std_year(const Student* a, const Student* b); // compares by student birth year
bool cmp_std_code(const Student* a, const Student* b); // compares by student faculty code

// debug functions
void print_stack(Stack*);
void print_student(Student*);

int main()
{
    // STAGE 1 - read txt and create stack
    Stack* student_stack = read_students_from_txt("input.txt");
    FILE *pFile = fopen("output.txt", "w"); // open output file

    // STAGE 2 - sort stack by birth year and print it to output.txt
    student_stack = sort_stack(student_stack, &cmp_std_year); // sort stack by year
    //print_stack(student_stack); // for debugging

    // print stack sorted by birth year
    fprintf(pFile, "Student names in ascending order by birthday:\n");
    StackNode* stack_node = student_stack->top;
    while(stack_node != NULL)
    {
        // print student 
        StudentNode* student_node = stack_node->student->name_start->next; // get students first char of name
        while(student_node != NULL) // iterate until the end
        {
            fprintf(pFile, "%c", student_node->data);
            student_node = student_node->next;
        }
        fprintf(pFile, "\n");
        stack_node = stack_node->next; // move on to next stack node
    }

    // STAGE 3 - sort stack by faculty code and print it to output.txt
    student_stack = sort_stack(student_stack, &cmp_std_code); // sort stack by faculty code
    //print_stack(student_stack); // for debugging

    fprintf(pFile, "School numbers by the faculty codes in ascending order:\n");
    stack_node = student_stack->top;
    while(stack_node != NULL)
    {
        // print student 
        StudentNode* student_node = stack_node->student->head; 
        // since we print only school numbers, we only need to iterate until we hit name_start
        while(student_node != stack_node->student->name_start) 
        {
            fprintf(pFile, "%c", student_node->data);
            student_node = student_node->next;
        }
        fprintf(pFile, "\n");
        stack_node = stack_node->next; // move on to next stack node
    }

    fclose(pFile);
    return 0;
}

bool cmp_std_year(const Student* a, const Student* b)
{
    StudentNode* a_n = a->year_start->next;
    StudentNode* b_n = b->year_start->next;

    while(a_n != NULL && b_n != NULL)
    {
        if(a_n->data < b_n->data) // if a < b then return true
            return true;
        else if(a_n->data == b_n->data) // if a == b then iterate for next node
        {
            a_n = a_n->next;
            b_n = b_n->next;
        }
        else // if a > b return false
            return false;
    }

    return false; // a == b or at least one node is null, return false
}

bool cmp_std_code(const Student* a, const Student* b)
{
    StudentNode* a_n = a->head;
    StudentNode* b_n = b->head;
    // faculty code is only first 3 digit of school number
    // therefor we only have to check first 3 digits
    const int code_len = 3; 
    int n = 0;

    while(a_n != NULL && b_n != NULL && n < code_len)
    {
        if(a_n->data < b_n->data) // if a < b then return true
            return true;
        else if(a_n->data == b_n->data) // if a == b then iterate for next node
        {
            a_n = a_n->next;
            b_n = b_n->next;
            n++;
        }
        else // if a > b return false
            return false;
    }

    return false; // a == b or at least one node is null, return false
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
        Student* tmp = pop(stack);  // get top from input stack

        // since we want smallest on top
        // pop from sorted stack until tmp is not bigger than top(sorted_stack)
        while(!empty(sorted_stack) && cmp_func(top(sorted_stack), tmp))
        {
            push(stack, pop(sorted_stack)); // repush popped elements to input stack
        }

        push(sorted_stack, tmp); // when sorted stack top is bigger than tmp we push it on top
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
    // read each line of the input file
    while (!feof(pFile))
    {
        char line[100000]; // char array for storing a single line
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

    // start iterate line until we hit null or new line
    while(c != '\n' && c != '\0')
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
            else // second is year
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

void print_stack(Stack* stack)
{
    printf("Stack ----\n");
    StackNode* node = stack->top;
    while(node != NULL)
    {
        print_student(node->student);
        node = node->next;
    }
    printf("----------\n");
}

void print_student(Student* student)
{
    printf("S -> ");
    StudentNode* node = student->head;
    while(node != NULL)
    {
        printf("%c", node->data);
        node = node->next;
    }
    printf("\n");
}