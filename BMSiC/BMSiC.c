#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <locale.h>
#include <math.h>

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>

#define BMSiC_VERTICAL 0
#define BMSiC_HORIZONTAL 1

int displayMenu(char **choices, bool orientation, int yMax, int xMax, unsigned int size) {
	int noOfElements = size / sizeof(choices[0]);
	int totalTextSize = 0;
	int cursorLocation = 0;
	int padding;
	for (int i = 0; i < noOfElements; i++) {
		totalTextSize += strlen(choices[i]);
	}
	padding = (xMax - totalTextSize) / (noOfElements + 1);
	clear();
	for (int i = 0; i < noOfElements; i++) {
		mvprintw(yMax / 2, cursorLocation += padding, "%s", choices[i]);
		cursorLocation += strlen(choices[i]);
	}
	refresh();
	return 0;
}

int main() {
	char *text[] = { "LOG IN", "SIGN IN"};
	int yMax, xMax;
	int choice;
	int highlight = 0;

	setlocale(LC_CTYPE, "Polish");

	initscr();
	noecho();
	cbreak();

	getmaxyx(stdscr, yMax, xMax);
	yMax = 15;
	xMax = yMax*4;
	//wresize(stdscr, yMax, xMax);
	resize_term(yMax, xMax);
	//resize_window(stdscr, yMax, xMax);
	printw("Hello BMSiC!\n");
	printw(text[0]);
	printw("\n%d\n%d\n", yMax, xMax);
	refresh();
	getch();
	displayMenu(&text, BMSiC_HORIZONTAL, yMax, xMax, sizeof(text));
	getch();
	endwin();
	system("pause");
	return 0;
}