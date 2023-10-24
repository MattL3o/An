#include <stdio.h>
#include <windows.h>
#include <time.h>

void initialize_rand(void){
	srand(clock());   /* seconds since program start */
	srand(time(NULL));   /* seconds since 1 Jan 1970 */
}

int my_rand(int max){
	int i;
	return (rand() % (max + 1));
}

void SetColor(int ForgC) //Curtosy of @LearningCODE on //https://stackoverflow.com/questions/29574849/how-to-change-text-color-and-console-color-in-codeblocks
{
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

void delay(int seconds){
	
	time_t start = time(NULL);
	time_t now = time(NULL);
	while(now - start < seconds){
		now = time(NULL);
	}	
}
