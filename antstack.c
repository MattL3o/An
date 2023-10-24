#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 50
 
typedef struct {
 	int x;
 	int y;
} coordinate;
 
static coordinate stack_data[MAX_SIZE];
 
static int count = 0;
 
int stack_empty(void){
 	if (count == 0){ return 1; }
	else { return 0; }
}

int stack_full(void){
	if (count == MAX_SIZE){ return 1; }
	else { return 0; }
}
 
coordinate stack_peek(void){
	coordinate to_return;
 	if(!stack_empty()){
 		to_return = stack_data[count - 1];
 		return to_return;
	}
	//else {printf("The Stack was Empty."); exit(1);}
}
 
coordinate stack_pop(void){
	if(!stack_empty()){
		coordinate to_return;
		to_return = stack_peek();
 		count--;
 		return to_return;
	}
 	
}

coordinate stack_push(coordinate in){
	if(!stack_full()){
		stack_data[count] = in;
		count++;
		return in;
	}
}

void stack_clear(void){
	
	count = 0;
	
}
