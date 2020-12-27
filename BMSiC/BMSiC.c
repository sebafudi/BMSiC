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
  int start_location = 0;
  div_t padding;

  for (int i = 0; i < number_of_elements; i++) {
    total_text_size += strlen(choices[i]);
  }
  padding = div((xMax - total_text_size), (number_of_elements + 1));
  if (padding.rem > 0) {
    start_location = padding.rem / 2;
  };

  clear();
  int choice;
  int highlight = 0;
  keypad(stdscr, 1);
  while (1) {
    cursor_location = start_location;
    for (int i = 0; i < number_of_elements; i++) {
      if (i == highlight) wattron(stdscr, A_REVERSE);
      mvprintw(yMax / 2, cursor_location += padding.quot, "%s", choices[i]);
      wattroff(stdscr, A_REVERSE);
      cursor_location += strlen(choices[i]);
    }
    choice = wgetch(stdscr);
    switch (choice) {
      case KEY_LEFT:
        highlight--;
        if (highlight < 0) highlight = 0;
        break;
      case KEY_RIGHT:
        highlight++;
        if (highlight > number_of_elements - 1)
          highlight = number_of_elements - 1;
        break;
      default:
        break;
    }
    if (choice == 10) {
      clear(stdscr);
      refresh();
      return highlight;
    }
  }
  return 0;
}

int main() {
  char* lang[] = {"ENGLISH", "POLSKI"};
  char* text[] = {"LOG IN", "SIGN IN"};
  int y_max, x_max;
  int choice;

  setlocale(LC_CTYPE, "Polish");

  initscr();
  noecho();
  cbreak();
  curs_set(0);

  getmaxyx(stdscr, y_max, x_max);
  y_max = 15;
  x_max = y_max * 4;
  // wresize(stdscr, yMax, xMax);
  resize_term(y_max, x_max);
  // resize_window(stdscr, yMax, xMax);
  printw("Hello BMSiC!\n");
  printw("y_max: %d\nx_max: %d\n", y_max, x_max);
  refresh();
  // getch();
  choice = display_horizontal_menu(&text, sizeof(text), y_max, x_max);
  printw("You chose: %d (%s)\n", choice, text[choice]);
  refresh();
  getch();
  endwin();
  system("pause");
  return 0;
}