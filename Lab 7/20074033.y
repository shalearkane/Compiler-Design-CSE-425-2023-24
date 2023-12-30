%{
#include <stdio.h>
#include <stdlib.h>

// Assuming you have a data structure to store the size of the arrays
int array_sizes[100] = {0};

%}

%union {
   char *str;
   int int_val;
   int array_dim;
   int array_indices;
}

%type <array_dim> array_dimensions
%type <array_indices> array_indices

%token ASSIGN_OP
%token <str> IDENTIFIER
%token INT_KEYWORD
%token <int_val> INT
%token OPEN_BRACKET CLOSE_BRACKET COMMA SEMICOLON

%%

program:
   | program array_declaration
   | program array_access
   | program array_assign
   ;

array_declaration:
   INT_KEYWORD IDENTIFIER array_dimensions SEMICOLON
   {
       // Assuming you have a function to add the array size to the data structure
       add_array_size($1, $2);
   }
   ;

array_dimensions:
 OPEN_BRACKET INT CLOSE_BRACKET
 {
   // Assign the value of the integer to $$
   $$ = $2;
 }
 | array_dimensions OPEN_BRACKET INT CLOSE_BRACKET
 {
   // Combine the values of the dimensions
   $$ = $1 * $3;
 }
 ;

array_assign:
   IDENTIFIER array_indices ASSIGN_OP INT SEMICOLON
   {
       int array_index = atoi($2);
       if (array_index < 0 || array_index >= array_sizes[atoi($1)]) {
           printf("Error: Index out of bounds\n");
       }
   }
   ;

array_access:
   IDENTIFIER ASSIGN_OP IDENTIFIER array_indices SEMICOLON
   {
       int array_index = atoi($4);
       if (array_index < 0 || array_index >= array_sizes[atoi($1)]) {
           printf("Error: Index out of bounds\n");
       }
   }
   ;

array_indices:
   OPEN_BRACKET INT CLOSE_BRACKET
   | array_indices OPEN_BRACKET INT CLOSE_BRACKET
   ;

%%

void add_array_size(char *array_name, int size) {
   // Map the array name to an index in the array_sizes array
   int index = array_name[0] - 'a';
    printf("size %d\n", size);
   // Add the size of the array to the array_sizes array
   array_sizes[index] = size;
}

int main() {
  yyparse();
  return 0;
}

int yyerror(char *s) {
  fprintf(stderr, "Error: %s\n", s);
  return 0;
}
