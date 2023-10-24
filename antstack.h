#define MAX_SIZE 50
typedef struct {
 	int x;
 	int y;
} coordinate;
static coordinate stack_data[MAX_SIZE];
static int count;
int stack_empty(void);
int stack_full(void);
coordinate stack_peek(void);
coordinate stack_pop(void);
coordinate stack_push(coordinate in);
void stack_clear(void);
