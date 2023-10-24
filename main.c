#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "antstack.h"
#include "antmovement.h"
#include "my_random.h"
//FOR HIGHER STEP VALUES THE BOT VALUE MUST BE HIGHER
#define NUM_STEPS 	200
#define NUM_BOTS	6000
//Mutation chance is 1/(value), so 2 would be a value of 50%
//MAX_MUTS_BOT defines the amount of mutations each mutated bot will recieve at most
//MAX_LINES_MUT defines the maximum amount of lines a mutation could replace
#define MUTATION_CHANCE	5
#define MAX_MUTS_BOT	1
#define MAX_LINES_MUT	4
typedef struct {
	int data[NUM_STEPS];
	int score;
	int points;
} bot;

bot bots[NUM_BOTS];

FILE *itfl;
FILE *maze_f;
FILE *output_f;
int itfl_steps[NUM_STEPS];

void initialize_bots(void){
	
	int i, j;
	
	for (i = 0; i < NUM_BOTS; i++){
		for(j = 0; j < NUM_STEPS; j++){
			bots[i].data[j] = my_rand(15);
		}
	}
	
}

void score_bots(void){
	int i,m;
	
	for(i = 0; i < NUM_BOTS; i++){
		bots[i].score = 0;
		initialize_maze(maze_f);
		for(m = 0; m < NUM_STEPS; m++){ ant_function(bots[i].data[m]); }
		bots[i].score = get_points() * 2 + get_distance();
		bots[i].points = get_points();
	}
	
}

bot mutate(bot mutater){
	
	int i, j;
	int runs;
	
	bot output = mutater;
	
	
	int variations = my_rand(MAX_MUTS_BOT - 1) + 1;
	int len, insert;
	
	for(i = 0; i < variations; i++){
		len = my_rand(MAX_LINES_MUT - 1) + 1;
		insert = my_rand(NUM_STEPS - len);
		
		
		if(my_rand(3) > 0){
			//SIMPLE REPLACE METHOD
			for(j = 0; j < len; j++){
				output.data[insert + j] = my_rand(15);
			}
		} else {
			
			runs = NUM_STEPS - insert - len;
			//REPLACE AND ADD TO BOTTOM
			for(j = 0; j < runs; j++){
				//printf("%d\n", j);
				output.data[insert + j] = mutater.data[insert + len + j];
			}
			
			for(j = 0; j < len; j++){
				output.data[NUM_STEPS - len + j] = my_rand(15);
			}
		}
	}
	
	return output;
	
}

bot mate(bot base, bot cross){
	
	bot output = base;
	int i, j;
	
	int crosses = my_rand(3), len_cross, start_point_b;
	for(i = 0; i < crosses; i++){
		len_cross = my_rand(4) + 1;
		start_point_b = my_rand(NUM_STEPS - len_cross);
		for(j = 0; j < len_cross; j++){
			output.data[start_point_b] = cross.data[start_point_b];
			start_point_b++;
		}
	}
	
	if(my_rand(MUTATION_CHANCE) == MUTATION_CHANCE){
		output = mutate(output);
	}
	
	return output;
	
}

void mate_group(int min, int max, int start, int stop){
	//min is where the start index of the "surviving mating" group and max is the end
	//start is the start of the "dying" to be replaced bots and stop is the end
	int i, b1, b2;

	for(i = start; i < stop + 1; i++){
		b1 = my_rand(max - min) + min;
		b2 = my_rand(max - min) + min;
		bots[i] = mate(bots[b1], bots[b2]);
	}
	
}

void update_itfl_steps(void){
	int i = 0, set_step;
	
	char step[15];
	
	while(i < NUM_STEPS && (fscanf(itfl, "%s", step) != EOF)){
		if     (!strcmp(step, "MARK"))		{ set_step = 0; }
		else if(!strcmp(step, "MOVE_F"))		{ set_step = 1; }
		else if(!strcmp(step, "MOVE_B"))		{ set_step = 2; }
		else if(!strcmp(step, "MOVE_L"))		{ set_step = 3; }
		else if(!strcmp(step, "MOVE_R"))		{ set_step = 4; }
		else if(!strcmp(step, "CWL"))		{ set_step = 5; }
		else if(!strcmp(step, "CWR"))		{ set_step = 6; }
		else if(!strcmp(step, "CWF"))		{ set_step = 7; }
		else if(!strcmp(step, "CWB"))		{ set_step = 8; }
		else if(!strcmp(step, "PUSH"))		{ set_step = 9; }
		else if(!strcmp(step, "POP"))		{ set_step = 10; }
		else if(!strcmp(step, "PEEK"))		{ set_step = 11; }
		else if(!strcmp(step, "CLEAR"))		{ set_step = 12; }
		else if(!strcmp(step, "BJPI"))		{ set_step = 13; }
		else if(!strcmp(step, "CJPI"))		{ set_step = 14; }
		else if(!strcmp(step, "BACKTRACK"))	{ set_step = 15; }
		else 								{ printf("There's a problem with the intelligence file"); exit(1); }
		itfl_steps[i] = set_step;
		i++;
	}
	
}

void print_step(int num){
	
	char step[15];
	
	switch(num){
		case 0: strcpy(step, "MARK"); break;
		case 1: strcpy(step, "MOVE_F"); break;
		case 2: strcpy(step, "MOVE_B"); break;
		case 3: strcpy(step, "MOVE_L"); break;
		case 4: strcpy(step, "MOVE_R"); break;
		case 5: strcpy(step, "CWL"); break;
		case 6: strcpy(step, "CWR"); break;
		case 7: strcpy(step, "CWF"); break;
		case 8: strcpy(step, "CWB"); break;
		case 9: strcpy(step, "PUSH"); break;
		case 10: strcpy(step, "POP"); break;
		case 11: strcpy(step, "PEEK"); break;
		case 12: strcpy(step, "CLEAR"); break;
		case 13: strcpy(step, "BJPI"); break;
		case 14: strcpy(step, "CJPI"); break;
		case 15: strcpy(step, "BACKTRACK"); break;
	}
	
	printf("%s", step);
}

void fprint_step(int num, FILE *file){
	
	char step[15];
	
	switch(num){
		case 0: fprintf(file, "%s\n", "MARK"); break;
		case 1: fprintf(file, "%s\n", "MOVE_F"); break;
		case 2: fprintf(file, "%s\n", "MOVE_B"); break;
		case 3: fprintf(file, "%s\n", "MOVE_L"); break;
		case 4: fprintf(file, "%s\n", "MOVE_R"); break;
		case 5: fprintf(file, "%s\n", "CWL"); break;
		case 6: fprintf(file, "%s\n", "CWR"); break;
		case 7: fprintf(file, "%s\n", "CWF"); break;
		case 8: fprintf(file, "%s\n", "CWB"); break;
		case 9: fprintf(file, "%s\n", "PUSH"); break;
		case 10: fprintf(file, "%s\n", "POP"); break;
		case 11: fprintf(file, "%s\n", "PEEK"); break;
		case 12: fprintf(file, "%s\n", "CLEAR"); break;
		case 13: fprintf(file, "%s\n", "BJPI"); break;
		case 14: fprintf(file, "%s\n", "CJPI"); break;
		case 15: fprintf(file, "%s\n", "BACKTRACK"); break;
	}
}

void print_itfl_steps(void){
	
	int i;
	char step[15];
	
	for(i = 0; i < NUM_STEPS; i++){ printf("Step %d: ", i+1); print_step(itfl_steps[i]); printf("\n"); }
	
}

void ant_function(int num){
	
	switch(num){
			case 0: mark(); break;
			case 1: move_f(1); break;
			case 2: move_b(1); break;
			case 3: move_l(1); break;
			case 4: move_r(1); break;
			case 5: cwl(); break;
			case 6: cwr(); break;
			case 7: cwf(); break;
			case 8: cwb(); break;
			case 9: stack_push(get_position()); break;
			case 10: stack_pop(); break;
			case 11: stack_peek(); break;
			case 12: stack_clear(); break;
			case 13: BJPI(); break;
			case 14: CJPI(); break;
			case 15: backtrack(); break;
		}
}

void order_bots(void){
	
	int i, swapped = 1;
	bot placeholder;
	
	while(swapped){
		swapped = 0;
		
		for(i = 0; i < NUM_BOTS - 1; i++){
			if(bots[i].score < bots[i+1].score){
				placeholder = bots[i+1];
				bots[i+1] = bots[i];
				bots[i] = placeholder;
				swapped = 1;
			}
		}
		
	}
	
}


int main (void){
	
	char choice = 0;
	int steps = 0, cycles, i;
	
	if((output_f = fopen("output.txt", "w")) == NULL){
		printf("The output file could not be opened.");
		exit(1);
	}
	
	if((maze_f = fopen("maze0.txt", "r")) == NULL){
		printf("The maze file could not be opened");
		exit(1);
	}
	
	initialize_bots();
	initialize_rand(); //Treasure generation won't work without this
	initialize_maze(maze_f); //Nothing will work without this
	set_output(output_f); //Output is handeled by antmovement file
	
	while(1){
		while(choice < 1 || choice > 3){
			printf("Choices\n========================================\n");
			printf("1) Evolve Ants\n");
			printf("2) Watch current ants\n");
			printf("Enter an option: ");
			scanf("%d", &choice);
			getchar();
		
			if(choice < 1 || choice > 2){ printf("That was not a valid option: Try again!\n\n"); }
		}
	
	
		switch(choice){
			case 1 :
				printf("Enter number of cycles: ");
				scanf("%d", &cycles);
				
				for(i = 1; i <= cycles; i++){
					initialize_maze(maze_f);
					mate_group(0, NUM_BOTS/4, NUM_BOTS/5, NUM_BOTS - 1);
					score_bots();
					order_bots();
					printf("Cycle %d out of %d has been completed. High score is: %d\n", i, cycles, bots[0].points, bots[NUM_BOTS - 2].score);
				}
				
				if( (itfl = fopen("itfl.txt", "w")) == NULL ){ printf("Intelligence file could not be opened\n"); exit(1); }
				
				for(i = 0; i < NUM_STEPS; i++){
					fprint_step(bots[0].data[i], itfl);
				}
				fclose(itfl);
				
				printf("Intelligence file updated\n");
				
				break;
			case 2:
				if( (itfl = fopen("itfl.txt", "r")) == NULL ){ printf("Intelligence file could not be opened\n"); exit(1); }
				update_itfl_steps();
				fclose(itfl);
				initialize_maze(maze_f);
				
				while(steps < NUM_STEPS){
					system("cls");
					ant_function(itfl_steps[steps]);
					print_maze();
					printf("Points: %d\n", get_points());
					printf("Last step: ");
					print_step(itfl_steps[steps]);
					printf("\n");	
					delay(1);
					steps++;
				}
				initialize_maze(maze_f);
				steps = 0;
				break;
		}
		
		choice = 0;	
	}
	
	
	
}
