#include <stdio.h>
#include <stdlib.h>
#define MAZE_X_SIZE 	20
#define MAZE_Y_SIZE 	20
#define MARK 			88
#define WALL 			42
#define DOOR 			68
#define OPEN 			32
#define TREASURE		49
#define NONE 			0
#define LEFT 			1
#define RIGHT 			2
#define FORWARD 		3
#define BACKWARD 		4
#define LIGHTMAGENTA	13
#define WHITE			15
#define BROWN			6
#define GREEN			2
#include "antstack.h"
#include "my_random.h"


static char maze[MAZE_X_SIZE][MAZE_Y_SIZE];

static char itch_direction = NONE;
static char itch_distance = 0;

static coordinate position;
static coordinate door;

static int points = 0;
static int dist_traveled = 0;
static char spaces[MAZE_X_SIZE][MAZE_Y_SIZE];

static FILE *output;

void set_output(FILE *file){
	output = file;
	fprintf(output, "The starting position is (%d,%d)\n", position.x, position.y);
}

coordinate get_position(void){
	return position;
}

int get_points(void){
	return points;
}

int get_distance(void){
	//Counts unique spaces the ant steps on
	return dist_traveled;
}

void reset_distance(void){
	int i, j;
	dist_traveled = 0;
	
	for(i = 0; i < MAZE_Y_SIZE; i++){
		for(j = 0; j < MAZE_X_SIZE; j++){
			spaces[j][i] = 0;
		}
	}
	
	spaces[door.x][door.y] = 1;
	
	
	
}

void reset_points(void){
	points = 0;
}


void initialize_maze(FILE *file){
	
	fseek(file, 0, SEEK_SET);
	
	//READS FILE
	int i, j;
	//int space_count = 0;
	for(i = 0; i < MAZE_Y_SIZE; i++){
		for(j = 0; j < MAZE_X_SIZE; j++){
			maze[j][i] = fgetc(file);							//MUST REMAIN AT TOP OF FOR LOOP
			if(maze[j][i] == '\n'){ maze[j][i] = fgetc(file); } //MUST REMAIN AT TOP OF FOR LOOP
			if(maze[j][i] == 'D'){ door.x = j; door.y = i; position = door; }
		}
	}
	
	stack_clear();
	reset_points();
	reset_distance();
	dist_traveled = 0;
	position = door;
	
}

void print_maze(void){
	//FOR DEBUG
	int i, j;
	char c;
	
	c = maze[position.x][position.y];
	maze[position.x][position.y] = 'A';
	
	SetColor(WHITE);
	for(i = 0; i < MAZE_Y_SIZE; i++){
		for(j = 0; j < MAZE_X_SIZE; j++){
			
			if(maze[j][i] >= TREASURE && maze[j][i] < TREASURE + 5){ SetColor(LIGHTMAGENTA); }
			if(maze[j][i] == DOOR){ SetColor(BROWN); }
			if(maze[j][i] == WALL) { SetColor(GREEN); }
			printf("%c", maze[j][i]);
			SetColor(WHITE);
		}
		
		printf("\n");
	}
	
	maze[position.x][position.y] = c;
	
}

int out_bounds_point(coordinate point){
	//Returns 1 if "ANT" is in bounds and exits program if not.
	if(point.x < 0 || point.y < 0 || point.x > MAZE_X_SIZE-1 || point.y > MAZE_Y_SIZE-1){
		return 1;
		}
	else { return 0; }
}

int check_bounds(void){
	//Returns 1 if "ANT" is in bounds and exits program if not.
	if(position.x < 0 || position.y < 0 || position.x > MAZE_X_SIZE-1 || position.y > MAZE_Y_SIZE-1){
		printf("Ant out of bounds at (%d,%d)", position.x, position.y);
		exit(1);
		}
	else { return 1; }
}

static void move(int num, int *s_position){

	*s_position += num;
	if(maze[position.x][position.y] == WALL || out_bounds_point(position) ){ *s_position -= num; }
	else {
		if(!spaces[position.x][position.y]){
			dist_traveled++;
			spaces[position.x][position.y] = 1; 
		}
		
	}
	check_bounds();
	
	if(maze[position.x][position.y] >= TREASURE && maze[position.x][position.y] < TREASURE + 5){
		points += maze[position.x][position.y] - '0';
		maze[position.x][position.y] = OPEN;
	}
	
	
	itch_direction = NONE;
	itch_distance = 0;
}

void move_f(int num){ move(num, &position.y); }
void move_b(int num){ move(-num, &position.y); }
void move_l(int num){ move(-num, &position.x); }
void move_r(int num){ move(num, &position.x); }

void mark(void) {
	if(maze[position.x][position.y] == OPEN){
		maze[position.x][position.y] = MARK;	
	}
}

static int cw(char direction){
	int num = 0, x, y;
	
	//TO AVOID 4 SEPERATE FUNCTIONS (EOA)
	switch(direction){
		case LEFT:
			x = position.x - 1;
			while(maze[x][position.y] == OPEN || (maze[x][position.y] >= TREASURE && maze[x][position.y] < TREASURE + 5)){
				num++;
				x--;
			}
			break;
		case RIGHT:
			x = position.x + 1;
			while(maze[x][position.y] == OPEN || (maze[x][position.y] >= TREASURE && maze[x][position.y] < TREASURE + 5)){
				num++;
				x++;
			}
			break;
		case FORWARD:
			y = position.y + 1;
			while(maze[position.x][y] == OPEN || (maze[position.x][y] >= TREASURE && maze[position.x][y] < TREASURE + 5)){
				num++;
				y++;
			}
			break;
		case BACKWARD:
			y = position.y - 1;
			while(maze[position.x][y] == OPEN || (maze[position.x][y] >= TREASURE && maze[position.x][y] < TREASURE + 5)){
				num++;
				y--;
			}
			break;
		default: printf("INVALID OPTION FOR CW FUNCTION.\n"); exit(1); 		
	}
	
	if(num > 0){
		itch_direction = direction;
		itch_distance = num;
		return 1;
	} else { return 0; }	
}
int cwl(void){ return cw(LEFT); }
int cwr(void){ return cw(RIGHT); }
int cwf(void){ return cw(FORWARD); }
int cwb(void){ return cw(BACKWARD); }

static int JPI(int distance){
	switch(itch_direction){
		case NONE: return 0;
		case LEFT: move_l(distance); break;
		case RIGHT: move_r(distance); break;
		case FORWARD: move_f(distance); break;
		case BACKWARD: move_b(distance); break;
	}
	return 1; 
}
int BJPI(void){ return JPI(itch_distance); }
int CJPI(void){ return JPI(1); }

void backtrack(void){
	if(!stack_empty()){
		position = stack_pop();
	}
	
	itch_direction = NONE;
	itch_distance = 0;
}


