#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
// A structure to represent a stack allows push and pop string 
struct Stack
{
    int top;
    unsigned capacity;
    char **array;
};

// A structure to save variables and their values
typedef struct 
{
	int size;
	char *var;
	double *value;
	double notCreated;// This value is different from all variables values. This is return when not created variables value.
}Variable;

struct Stack *createStack(unsigned);// Function to create a stack of given capacity. It initializes size of stack as 0

int isFull(struct Stack *);// Stack is full when top is equal to the last index

int isEmpty(struct Stack *);// Stack is empty when top is equal to -1

void push(struct Stack *, char *);// Function to add an item to stack. It increases top by 1

char *pop(struct Stack *);// Function to remove an item from stack. It decreases top by 1

char *getWholeStack(struct Stack *);// Function to return all stack a string

Variable *initVars(unsigned);// Funtion to initialize and return Variable struct with given capacity

char **initStringArray(unsigned, unsigned);// Function to create a string array. First number is what is the lenght of string, second is how many string will be stored

double getVar(Variable *, char);// Function to search and return value of variables. If variable is not created before it's return notCreated value

void addVar(Variable *, char, double);// Function to add a new variable to Variable struct

int getPriority(char);// Function to return arithmatic operations priority. If operation is not defined return defined operations count + 1

char *createPostfix(struct Stack *, char *, Variable *, char *);// Function to generate postfix form of arithmetic operation

double operation(double, double, char);// Function to do the operation -which char in parameters- between two double

void toString(char *, int, double);// Function to convert double to string

double solvePostfix(struct Stack *, char *, Variable *);// Function to solve the postfix form of arithmatic function

void printWholeVars(Variable *);// Function to write whole variables with their values

FILE *openFile(char *, char *);// Function to opening file. It's take filename, file open mode and return a FILE pointer

// Main function of the program
int main(int argc, char *argv[]) 
{
	int postfixCount = 0;// This is store how many postfix created
	int i = 0;// This is for loops
	int j;// This is for loops
	struct Stack *stack = createStack(100);// Stack initialization
	Variable *vars = initVars(100);// Variable initialization
	char **strArray = initStringArray(100, 100);// This is for store created postfix forms
	FILE *input = openFile("input.txt", "r");// Opening file
	while(!feof(input))// While file is over
	{
		char *function = (char *) calloc(1000, sizeof(char));// Create a function string which store 1000 character
		char variable;// This is store our variable character
		fgets(function, 1000, input);// Read a line from file
		if(function[strlen(function) - 1] == '\n')// Some times read new line char, but sometimes not read this is for this
			function[strlen(function) - 1] = '\0';// Changing \n character to null character '\0'
		printf("----------\nFunction : %s \n\nCreation of Postfix:\n", function);
		strArray[postfixCount] = createPostfix(stack, function, vars, &variable);// The function create postfix form and return a char pointer
		addVar(vars, variable, solvePostfix(stack, strArray[postfixCount++], vars));// addVar funtion add or update a variable. solvePostfix function return the value of function
		printf("\nVariables : ");
		printWholeVars(vars);// Here print all variables which created
		printf("\n");
	}
	for(i = 0; i < postfixCount; i++)// When all postfixes are created and file is ended. Write down all postfix expressions
	{
		printf("%d. Postfix : ", i + 1);
		for(j = 0; j < 100; j++)
			printf("%c", strArray[i][j]);
		printf("\n");
	}
	return 0;
}

struct Stack *createStack(unsigned capacity)
{
    struct Stack *stack = (struct Stack *) malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (char **) calloc(stack->capacity, sizeof(char *));
    return stack;
}

int isFull(struct Stack *stack)
{   return stack->top == stack->capacity - 1; }

int isEmpty(struct Stack *stack)
{   return stack->top == -1;  }

void push(struct Stack *stack, char *item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = (char *) calloc(strlen(item), sizeof(char));
    strcpy(stack->array[stack->top], item) ;
}

char *pop(struct Stack *stack)
{
    if (isEmpty(stack))
        return NULL;
    return stack->array[stack->top--];
}

char *getWholeStack(struct Stack *stack)
{
	char *ret = (char *) calloc(100, sizeof(char));
	int i;
	strcat(ret, "Stack: ");
	for(i = 0; i <= stack->top; i++)
	{
		strcat(ret, stack->array[i]);
		strcat(ret, " ");
	}
	if(stack->top == -1)
		strcat(ret, "Empty");
	return ret;
}

Variable *initVars(unsigned capacity)
{
	Variable *vars = (Variable *) malloc(sizeof(Variable));
	vars->value = (double *) calloc(capacity, sizeof(double));
	vars->var = (char *) calloc(capacity, sizeof(char));
	vars->notCreated = -100000;
	vars->size = 0;
	return vars;
}

char **initStringArray(unsigned strlong, unsigned strnumb)
{
	char **strarr = (char **) calloc(strnumb, sizeof(char *));
	int i;
	for(i = 0; i < strnumb; i++)
		strarr[i] = (char *) calloc(strlong, sizeof(char));
	return strarr;
}

double getVar(Variable *vars, char variable)
{
	int i = 0;
	while(i < vars->size && vars->var[i] != variable)// Looking for variable is created before
		i++;
	if(i == vars->size)// If variable cannot found return notCreated value
		return vars->notCreated;
	return vars->value[i];
}

void addVar(Variable *vars, char variable, double value)
{
	int i = 0;
	while(i < vars->size && vars->var[i] != variable)// Look for this variable created before
		i++;
	if(i == vars->size)// If this variable is not created before, create a new variable
	{
		vars->var[vars->size] = variable;
		vars->value[vars->size++] = value;
	}
	else// If this variable created before, only update this value
		vars->value[i] = value;
	if(vars->notCreated == value)
	vars->notCreated++;
}

int getPriority(char operation)
{
	int i = 0;
	char *priority = "*/-+";// The priortiy of arithmatic operations. When added a new operation it's must be added to operation function too
	while(priority[i++] != operation);
	return i;
}

char *createPostfix(struct Stack *stack, char *function, Variable *vars, char *var)
{
	char *postfix = (char *) calloc(1000, sizeof(char));// Init a char array
	char *token = strtok(function, " ");// Parse function string by ' ' character
	*var = token[0];// The first part of function is our variable
	token = strtok(NULL, " ");// This is for jump to '=' character
	token = strtok(NULL, " ");// This is for jump to '=' character
    while (token != NULL && token[0] != ';')// While a token can be created and token is not ';' character
    {
        if(token[0] <= 57 && token[0] >= 48)// If token is a number
		{
			strcat(postfix, token);// Numbers added to postfix directly
			strcat(postfix, " ");
		}
		else if(getVar(vars, token[0]) != vars->notCreated)// If token is a variable
		{
			strcat(postfix, token);// Variables added to postfix directly
			strcat(postfix, " ");
		}
		else if(getPriority(token[0]) <= 4)// If token is a arithmatic operation
		{
			char *operationInStack = pop(stack);
			while(operationInStack != NULL && getPriority(operationInStack[0]) < getPriority(token[0]))// While there is higher priority operations in the stack
			{
				strcat(postfix, operationInStack);// Operations -which are higher priority- popped stack and added to postfix
				strcat(postfix, " ");
				operationInStack = pop(stack);
			}
			if(operationInStack != NULL)// If last poped operation is not null its must be push again
				push(stack, operationInStack);
			push(stack, token);// New arithmatic operation is pushed
		}
		else if(token[0] == '(')// If token is '(' it's pushed directly
			push(stack, token);
		else if(token[0] == ')')// If token is ')' while poped operation is '(' all operations poped and added to postfix expression
		{
			char *operationInStack = pop(stack);
			while(operationInStack[0] != '(')
			{
				strcat(postfix, operationInStack);
				strcat(postfix, " ");
				operationInStack = pop(stack);
			}
		}
    	token = strtok(NULL, " ");
    	printf("Postfix: %-50s %s\n", postfix, getWholeStack(stack));
    }
	char *operationInStack = pop(stack);
	while(operationInStack != NULL)// Operations left at stack must be added to postfix too
	{
		strcat(postfix, operationInStack);
		strcat(postfix, " ");
		printf("Postfix: %-50s %s\n", postfix, getWholeStack(stack));
		operationInStack = pop(stack);
	}
	printf("\nPostfix version of function: %-50s \n", postfix);
    return postfix;
}

double operation(double number1, double number2, char operation)
{
	if(operation == '+')
		return number1 + number2;
	if(operation == '-')
		return number1 - number2;
	if(operation == '/')
		return number1 / number2;
	if(operation == '*')
		return number1 * number2;
}

void toString(char *outStr, int length, double val)
{
	_snprintf(outStr, length, "%f", val);
}

double solvePostfix(struct Stack *stack, char *postfix, Variable *vars)
{
	printf("\nSolving postfix: \n");
	char *token = strtok(postfix, " ");// Parse function string by ' ' character
	while(token != NULL)
	{
		if(token[0] <= 57 && token[0] >= 48)// If token is number, it's push to stack
		{
			push(stack, token);
		}
		else if(getVar(vars, token[0]) != vars->notCreated)// If token is a variable, it's value push to stack
		{
			char *string = (char *) calloc(100, sizeof(char));
			toString(string, 10, getVar(vars, token[0]));// Convert value(double) to string
			push(stack, string);
		}
		else if(getPriority(token[0]) <= 4)// If token is a arithmatic operation, pop two values from stack and do the operation and return value pushed to stack
		{
			char *string = (char *) calloc(100, sizeof(char));
			toString(string, 10, operation(atof(pop(stack)), atof(pop(stack)), token[0]));
			push(stack, string);
		}
		printf("%s\n", getWholeStack(stack));
		token = strtok(NULL, " ");
	}
	return atof(pop(stack));// When all postfix solved. One value remained in stack. It's the result of our function
}

void printWholeVars(Variable *vars)
{
	int i = 0;
	for(i = 0; i < vars->size; i++)
		printf("%c=%f ", vars->var[i], vars->value[i]);
	printf("\n");
}

FILE *openFile(char *fName, char *mode)
{
	FILE *tmp;
	tmp = fopen(fName, mode);
	if(!tmp)// If file cannot be opened, program will be ended.
	{
		printf("File cannot be open!");
		exit(-1);
	}
	return tmp;
}
