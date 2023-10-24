#include <stdio.h>
#include <stdlib.h>
#define MAZE_X_SIZE 	20
#define MAZE_Y_SIZE 	20

static char maze[MAZE_X_SIZE][MAZE_Y_SIZE];
static char itch_direction;
static char itch_distance;
static coordinate position;
static coordinate door;
static int points;
static int dist_traveled;
static FILE *output;
void set_output(FILE *file);
coordinate get_position(void);
int get_points(void);
void reset_points(void);
void reset_distance(void);
void initialize_maze(FILE *file);
void print_maze(void);
int out_bounds_point(coordinate point);
int check_bounds(void);
static void move(int num, int *s_position);
void move_f(int num);
void move_b(int num);
void move_l(int num);
void move_r(int num);
void mark(void);
static int cw(char direction);
int cwl(void);
int cwr(void);
int cwf(void);
int cwb(void);
static int JPI(int distance);
int BJPI(void);
int CJPI(void);
void backtrack(void);
