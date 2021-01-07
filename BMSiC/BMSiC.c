#include <assert.h>
#include <curses.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Account {
  int account_id;
  char first_name[64];
  char last_name[64];
  char login[64];
  char password[64];
  __time64_t date_created;
  char accout_type;
  char account_number[27];
  double balance;
};

int DisplayHorizontalMenu(char** choices, int size, int y_max, int x_max) {
  int number_of_elements = size / sizeof(choices[0]);
  int total_text_size = 0;
  int cursor_location = 0;
  int start_location = 0;
  int choice;
  int highlight = 0;
  div_t padding;
  curs_set(0);
  for (size_t i = 0; i < number_of_elements; i++) {
    total_text_size += (int)strlen(choices[i]);
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
      cursor_location += (int)strlen(choices[i]);
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
      refresh();
      return highlight;
    }
    if (choice == 27) {
      return 27;
    }
  }
  return 0;
}

int DisplayVerticalMenu(char** choices, int size, int y_max, int x_max) {
  int number_of_elements = size / sizeof(choices[0]);
  int padding = (y_max - number_of_elements * 2 - 1) / 2 + 1;
  int cursor_location;
  int choice;
  int highlight = 0;
  curs_set(0);
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
      return highlight;
    }
    if (choice == 27) {
      return 27;
    }
    refresh();
  }
  return 0;
}

int InitializeFiles(char* file_name) {
  FILE* file;
  fopen_s(&file, file_name, "w");
  if (file != NULL) {
    fprintf(file, "BMSiC\n");
    fclose(file);
  }
  return 0;
}

int InputMenu(char** fields, int size, char** fields_text, int y_max, int x_max,
              bool already_set) {
  int current_input = 0;
  int key_pressed = 0;
  int number_of_elements = size / sizeof(fields[0]);
  bool exit = 0;
  curs_set(0);
  if (!already_set) {
    for (int i = 0; i < number_of_elements; i++) {
      fields_text[i] = calloc(64, sizeof(char));
      assert(fields_text[i]);
    }
  }
  while (exit == 0) {
    clear();
    for (int i = 0; i < number_of_elements; i++) {
      if (current_input == i) {
        mvprintw(i, 0, "> ");
      } else {
        mvprintw(i, 0, "  ");
      }
      printw("%s: ", fields[i]);
      if (fields[i] == "Password") {
        for (int j = 0; j < strlen(fields_text[i]); j++) {
          printw("*");
        }
      } else {
        printw("%s", fields_text[i]);
      }
    }
    if (current_input < number_of_elements) {
      if (fields[current_input]) {
        mvprintw(current_input,
                 (int)strlen(fields[current_input]) + 4 +
                     (int)strlen(fields_text[current_input]),
                 "");
        curs_set(1);
      }
    } else {
      curs_set(0);
    }
    key_pressed = wgetch(stdscr);
    if (current_input < number_of_elements) {
      if (key_pressed >= 32 && key_pressed <= 126) {
        int len = (int)strlen(fields_text[current_input]);
        if (len < 63) {
          mvprintw(current_input, len + 4, "");
          fields_text[current_input][len] = key_pressed;
          fields_text[current_input][len + 1] = '\0';
        }
      }
      if (key_pressed == 8) {
        fields_text[current_input][strlen(fields_text[current_input]) - 1] =
            '\0';
      }
    }
    if (key_pressed == 9 || key_pressed == 258 || key_pressed == 10) {
      current_input++;
    }
    if ((key_pressed == 351 || key_pressed == 259) && current_input > 0) {
      current_input--;
    }
    if (current_input == number_of_elements + 1 && key_pressed == 10) {
      return 10;
    }
    if (key_pressed == 27) {
      return 27;
    }
    refresh();
  }
  return 0;
}

void SafelyClose() { exit(0); }

void GenerateAccountNumber(char* random_number, size_t size) {
  for (int i = 0; i < size - 1; i++) {
    random_number[i] = rand() % 10 + '0';
    random_number[i + 1] = '\0';
  }
}

int CreateUser(struct Account* current_account, char** fields_text, int count,
               int last_id) {
  char random_number[27] = {'\0'};
  GenerateAccountNumber(random_number, sizeof(random_number));
  current_account->account_id = last_id + 1;
  strcpy_s(current_account->first_name, sizeof(current_account->first_name),
           fields_text[0]);
  strcpy_s(current_account->last_name, sizeof(current_account->last_name),
           fields_text[1]);
  strcpy_s(current_account->login, sizeof(current_account->login),
           fields_text[2]);
  strcpy_s(current_account->password, sizeof(current_account->password),
           fields_text[3]);
  _time64(&current_account->date_created);
  current_account->accout_type = 0;
  strcpy_s(current_account->account_number,
           sizeof(current_account->account_number), random_number);
  current_account->balance = 0;
  return 0;
}

int SaveUser(char* file_name, char data_separator,
             struct Account* current_account) {
  FILE* file;
  fopen_s(&file, file_name, "a");
  if (file != NULL) {
    fprintf(file, "%d", current_account->account_id);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%s", current_account->first_name);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%s", current_account->last_name);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%s", current_account->login);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%s", current_account->password);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%d", (int)current_account->date_created);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%d", current_account->accout_type);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%s", current_account->account_number);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%f", current_account->balance);
    fprintf(file, "\n");
    fclose(file);
    return 0;
  }
  return 1;
}

int ReadUser() { return 0; }

int GetLastId(char* file_name, char data_separator) {
  FILE* file;
  fopen_s(&file, file_name, "r");
  if (file != NULL) {
    long file_size;
    char current_character;
    int count = 0;
    int end;
    int start;
    char last_id[16] = "";
    int int_last_id = -1;
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    for (int i = 0; i < file_size; i++) {
      fseek(file, -1 * i - 1, SEEK_END);
      current_character = fgetc(file);
      if (current_character == '\n') {
        count++;
      }
      if (current_character == '\n' && count <= 2) {
        end = i;
      }
      if (current_character == '\n' && count > 2) {
        start = i;
        fseek(file, -start, SEEK_END);
        for (int i = 0; i < start - end; i++) {
          current_character = fgetc(file);
          if (current_character == data_separator) {
            int_last_id = atoi(last_id);
            break;
          }
          sprintf_s(last_id, sizeof(last_id), "%s%c", last_id,
                    current_character);
        }
        break;
      }
    }
    fclose(file);
    return int_last_id;
  }
  return -1;
}

int main() {
  char* text[] = {"LOG IN", "SIGN IN", "EXIT"};
  char* sign_in_text[] = {"First Name", "Last Name", "Login", "Password"};
  char* log_in_text[] = {"Login", "Password"};
  char** fields_text = calloc(10, sizeof(char*));
  char data_separator = 149;
  struct Account current_account;
  int y_max, x_max;
  int choice = -1;
  FILE* file;
  char* file_name = "bmsic_db.txt";
  errno_t err;
  assert(fields_text);

  setlocale(LC_CTYPE, "Polish");
  srand((unsigned int)time(0));

  err = fopen_s(&file, file_name, "r");
  if (err != 0) {
    printf_s("Detected first run, creating files...\n");
    if (InitializeFiles(file_name) == 0) {
      printf_s("Successfully created files!\n");
    } else {
      printf_s("Unable to create file.\nProgram will close.\n");
      system("PAUSE");
      return 1;
    }
    system("PAUSE");
  } else if (file != NULL) {
    fclose(file);
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
  // getch();
  int stage = 0;
  do {
    if (stage == 0) {
      choice = DisplayHorizontalMenu(text, sizeof(text), y_max, x_max);
      if (choice == 2) {
        SafelyClose();
      }
      if (choice == 27) {
        SafelyClose();
      }
      stage = 1;
    }
    if (stage == 1) {
      if (choice == 0) {
        choice = InputMenu(log_in_text, sizeof(log_in_text), fields_text, y_max,
                           x_max, 0);
      } else if (choice == 1) {
        bool flag = 0;
        char error_text[50];
        choice = 0;
        while (choice != 27) {
          choice = InputMenu(sign_in_text, sizeof(sign_in_text), fields_text,
                             y_max, x_max, flag);
          flag = 1;
          if (choice == 27) {
            stage = 0;
            break;
          } else if (strlen(fields_text[0]) == 0) {
            strcpy_s(error_text, sizeof(error_text),
                     "First Name cannot be empty!");
          } else if (strlen(fields_text[1]) == 0) {
            strcpy_s(error_text, sizeof(error_text),
                     "Last Name cannot be empty!");
          } else if (strlen(fields_text[2]) == 0) {
            strcpy_s(error_text, sizeof(error_text), "Login cannot be empty!");
          } else if (strlen(fields_text[3]) < 8) {
            strcpy_s(error_text, sizeof(error_text),
                     "Password must have 8 or more characters!");
          } else if (choice != 27) {
            break;
          }
          clear();
          printw("%s", error_text);
          getch();
        }
      }
      if (choice == 27) {
        stage = 0;
      }
    }
  } while (choice != 10);
  if (choice == 10) {
    CreateUser(&current_account, fields_text, sizeof(fields_text),
               GetLastId(file_name, data_separator));
    SaveUser(file_name, data_separator, &current_account);
  }
  clear();
  printw("You chose: %d\n%s\n%s", choice, fields_text[0], fields_text[1]);
  getch();
  endwin();
  system("pause");
  return 0;
}