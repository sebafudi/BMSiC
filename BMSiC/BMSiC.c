#include <curses.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define BMSiC_VERTICAL 0
#define BMSiC_HORIZONTAL 1

int display_horizontal_menu(char** choices, unsigned int size, int yMax,
                            int xMax) {
  int number_of_elements = size / sizeof(choices[0]);
  int total_text_size = 0;
  int cursor_location = 0;
  div_t padding;

  for (int i = 0; i < number_of_elements; i++) {
    total_text_size += strlen(choices[i]);
  }
  padding = div((xMax - total_text_size), (number_of_elements + 1));
  if (padding.rem > 0) {
    cursor_location += padding.rem / 2;
  };

  clear();
  for (int i = 0; i < number_of_elements; i++) {
    mvprintw(yMax / 2, cursor_location += padding.quot, "%s", choices[i]);
    cursor_location += strlen(choices[i]);
  }
  refresh();

  return 0;
}

int main() {
  char* text[] = {"LOG IN", "SIGN IN"};
  int y_max, x_max;
  int choice;
  int highlight = 0;

  setlocale(LC_CTYPE, "Polish");

  initscr();
  noecho();
  cbreak();

  getmaxyx(stdscr, y_max, x_max);
  y_max = 15;
  x_max = y_max * 4;
  // wresize(stdscr, yMax, xMax);
  resize_term(y_max, x_max);
  // resize_window(stdscr, yMax, xMax);
  printw("Hello BMSiC!\n");
  printw(text[0]);
  printw("\n%d\n%d\n", y_max, x_max);
  refresh();
  // getch();
  display_horizontal_menu(&text, sizeof(text), y_max, x_max);
  getch();
  endwin();
  system("pause");
  return 0;
}