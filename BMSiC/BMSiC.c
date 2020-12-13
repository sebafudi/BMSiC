#include <stdio.h>
#include <curses.h>

int main() {
	initscr();
	printw("Hello BMSiC!");
	refresh();
	getch();
	endwin();
	return 0;
}