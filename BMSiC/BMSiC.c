#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <locale.h>

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>

#define BMSiC_VERTICAL 0
#define BMSiC_HORIZONTAL 1

int displayMenu(char *choices[], bool orientation, int yMax, int xMax) {
	clear();
	mvprintw(yMax/2, (xMax-strlen(choices[0]))/2, "%s", choices[0]);
	refresh();
	return 0;
}

int main() {
	char *text[] = { "LOG IN", "SIGN IN" };
	int yMax, xMax;
	int choice;
	int highlight = 0;

	setlocale(LC_CTYPE, "Polish");

	initscr();
	noecho();
	cbreak();

	getmaxyx(stdscr, yMax, xMax);
	//yMax = 30;
	//xMax = 60;
	//wresize(stdscr, yMax, xMax);
	//resize_term(yMax, xMax);
	//resize_window(stdscr, yMax, xMax);
	printw("Hello BMSiC!\n");
	printw(text[0]);
	printw("\n%d\n%d\n", yMax, xMax);
	refresh();
	getch();
	displayMenu(text, BMSiC_HORIZONTAL, yMax, xMax);
	getch();
	endwin();
	system("pause");
	return 0;
}