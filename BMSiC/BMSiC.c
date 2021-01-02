#include <curses.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

struct Account {
  int account_id;
  char first_name[64];
  char last_name[64];
  time_t date_created;
  char accout_type;
};

int DisplayHorizontalMenu(char** choices, unsigned int size, int y_max,
                          int x_max) {
  int number_of_elements = size / sizeof(choices[0]);
  int total_text_size = 0;
  int cursor_location = 0;
  int start_location = 0;
  int choice;
  int highlight = 0;
  div_t padding;

  for (int i = 0; i < number_of_elements; i++) {
    total_text_size += strlen(choices[i]);
  }
  padding = div((x_max - total_text_size), (number_of_elements + 1));
  if (padding.rem > 0) {
    start_location = padding.rem / 2;
  };

  clear();
  while (1) {
    cursor_location = start_location;
    for (int i = 0; i < number_of_elements; i++) {
      if (i == highlight) wattron(stdscr, A_REVERSE);
      mvprintw(y_max / 2, cursor_location += padding.quot, "%s", choices[i]);
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

int DisplayVerticalMenu(char** choices, unsigned int size, int y_max,
                        int x_max) {
  int number_of_elements = size / sizeof(choices[0]);
  int padding = (y_max - number_of_elements * 2 - 1) / 2 + 1;
  int cursor_location;
  int choice;
  int highlight = 0;
  clear();
  while (1) {
    cursor_location = padding;
    for (int i = 0; i < number_of_elements; i++) {
      if (i == highlight) wattron(stdscr, A_REVERSE);
      mvprintw(cursor_location, 5, "%s", choices[i]);
      wattroff(stdscr, A_REVERSE);
      cursor_location += 2;
    }
    choice = wgetch(stdscr);
    switch (choice) {
      case KEY_UP:
        highlight--;
        if (highlight < 0) highlight = 0;
        break;
      case KEY_DOWN:
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
    refresh();
  }
  return 0;
}

int InitializeFiles(char* file_name) {
  FILE* file;
  fopen_s(&file, file_name, "w");
  if (file != NULL) {
    fprintf(file, "BMSiC");
    fclose(file);
  }
  return 0;
}

int CreateAccountMenu() {
  int current_input = 0;
  int choice = 0;
  char* fields[] = {"First Name", "Last Name"};
  int number_of_elements = sizeof(fields) / sizeof(fields[0]);
  while (55 != number_of_elements) {
    clear();
    for (int i = 0; i < number_of_elements; i++) {
      if (current_input == i) {
        mvprintw(i, 0, "> ");
      } else {
        mvprintw(i, 0, "  ");
      }
      printw("%s: ", fields[i]);
    }
    if (current_input < number_of_elements) {
      mvprintw(current_input, strlen(fields[current_input]) + 4, "");
      curs_set(1);
    } else {
      curs_set(0);
    }
    choice = wgetch(stdscr);
    if (choice >= 97 && choice <= 122 || choice >= 65 && choice <= 90 ||
        choice == 32) {
      mvprintw(current_input, strlen(fields[current_input]) + 4, "");
    }
    if ((choice == 9 || choice == 258) && current_input < number_of_elements) {
      current_input++;
    }
    if ((choice == 351 || choice == 259) && current_input > 0) {
      current_input--;
    }
    refresh();
  }
  return 0;
}

int main() {
  char* text[] = {"LOG IN", "SIGN IN"};
  int y_max, x_max;
  int choice;
  FILE* file;
  char* file_name = "bmsic_db.txt";
  errno_t err;

  setlocale(LC_CTYPE, "Polish");

  err = fopen_s(&file, file_name, "r");
  if (err != 0) {
    printf_s("Detected first run, creating files...\n");
    if (InitializeFiles(file_name) == 0) {
      printf_s("Successfully created files!\n");
    } else {
      printf_s("Unable to create file. \nProgram will close.");
      return 1;
    }
    system("PAUSE");
  }

  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, 1);

  getmaxyx(stdscr, y_max, x_max);
  y_max = 21;
  x_max = y_max * 4;
  // wresize(stdscr, yMax, xMax);
  resize_term(y_max, x_max);
  // resize_window(stdscr, yMax, xMax);
  printw("Hello BMSiC!\n");
  printw("y_max: %d\nx_max: %d\n", y_max, x_max);
  clear();
  refresh();
  CreateAccountMenu();
  // getch();
  // choice = DisplayHorizontalMenu(&text, sizeof(text), y_max, x_max);
  // printw("You chose: %d (%s)\n", choice, text[choice]);
  // getch();
  endwin();
  system("pause");
  return 0;
}