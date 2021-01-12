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
  char account_number[27];
  char login[64];
  char password[64];
  char first_name[64];
  char last_name[64];
  __time64_t date_created;
  char accout_type;
  long long int balance;
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
      return -1;
    }
  }
  return -1;
}

int DisplayVerticalMenu(char** choices, unsigned int size, int y_max,
                        int x_max) {
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
      return -1;
    }
    refresh();
  }
  return -1;
}

int InitializeFiles(char* file_name) {
  FILE* file;
  fopen_s(&file, file_name, "w");
  if (file != NULL) {
    fprintf(file, "BMSiC\n");
    fclose(file);
    return 0;
  }
  return -1;
}

int TextInputMenu(char** fields, int size, char** fields_text, int y_max,
                  int x_max, bool already_set) {
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
    if (current_input == number_of_elements) wattron(stdscr, A_REVERSE);
    mvprintw(number_of_elements, 2, "CONFIRM");
    wattroff(stdscr, A_REVERSE);
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
      return -1;
    }
    refresh();
  }
  return -1;
}

long long int FloatInputMenu(char* text, int y_max, int x_max) {
  int key_pressed = 0;
  long long int number = 0;
  char input[16] = {'\0'};
  size_t input_length = 0;
  clear();
  curs_set(1);
  while (key_pressed != 10) {
    if (key_pressed >= 48 && key_pressed <= 57 || key_pressed == 46) {
      if (strlen(input) < 15) {
        if (!(key_pressed == 46 && (input[strlen(input) - 2] == '.' ||
                                    input[strlen(input) - 1] == '.'))) {
          if (input[strlen(input) - 3] != '.') {
            sprintf_s(input, sizeof(input), "%s%c", input, key_pressed);
          }
        }
      }
    } else if (key_pressed == 8 && strlen(input) > 0) {
      input[strlen(input) - 1] = '\0';
    } else if (key_pressed == 27) {
      return -1;
    }
    clear();
    mvprintw(y_max / 2, 5, "%s", "To confirm press ENTER, to cancel press ESC");
    mvprintw(y_max / 2 - 1, 5, "%s: %s", text, input);
    key_pressed = wgetch(stdscr);
    if (key_pressed == 10 && (strlen(input) == 0 || !strcmp(input, "."))) {
      key_pressed = 0;
    }
  }
  curs_set(0);
  input_length = strlen(input);
  if (input[input_length - 3] == '.') {
    input[input_length - 3] = input[input_length - 2];
    input[input_length - 2] = input[input_length - 1];
    input[input_length - 1] = '\0';
    number = atoll(input);
  } else if (input[input_length - 2] == '.') {
    input[input_length - 2] = input[input_length - 1];
    input[input_length - 1] = '\0';
    number = atoll(input) * 10;
  } else {
    number = atoll(input) * 100;
  }
  return number;
}

void SafelyClose() {
  clear();
  endwin();
  printf("Poprawnie zamknięto program\n");
  system("pause");
  exit(0);
}

void GenerateAccountNumber(char* random_number, size_t size) {
  for (int i = 0; i < size - 1; i++) {
    random_number[i] = rand() % 10 + '0';
    random_number[i + 1] = '\0';
  }
}

int CreateUser(struct Account* current_account, char** fields_text,
               size_t count, int last_id) {
  char random_number[27] = {'\0'};
  GenerateAccountNumber(random_number, sizeof(random_number));
  current_account->account_id = last_id + 1;
  strcpy_s(current_account->login, sizeof(current_account->login),
           fields_text[0]);
  strcpy_s(current_account->password, sizeof(current_account->password),
           fields_text[1]);
  strcpy_s(current_account->first_name, sizeof(current_account->first_name),
           fields_text[2]);
  strcpy_s(current_account->last_name, sizeof(current_account->last_name),
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
    fprintf(file, "%s", current_account->account_number);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%s", current_account->login);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%s", current_account->password);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%s", current_account->first_name);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%s", current_account->last_name);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%d", (unsigned int)current_account->date_created);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%d", current_account->accout_type);
    fprintf(file, "%c", data_separator);
    fprintf(file, "%lld", current_account->balance);
    fprintf(file, "\n");
    fclose(file);
    return 0;
  }
  return -1;
}

int ParseUserFromLine(char* data, struct Account* current_account,
                      struct Account* temp_account) {
  const int LINE_SIZE = 512;
  const int MAX_FIELDS = 16;
  char output[16][512] = {'\0'};
  int i;
  int output_field_count;
  int output_char_idx;
  char data_separator = 149;

  output_field_count = 0;
  output_char_idx = 0;

  for (i = 0; i < LINE_SIZE; i++) {
    if ((data[i] != data_separator) && (output_field_count < MAX_FIELDS) &&
        ((output_char_idx + 1) < LINE_SIZE)) {
      output[output_field_count][output_char_idx] = data[i];
      output[output_field_count][output_char_idx + 1] = '\0';
      output_char_idx++;
    } else if (data[i] == data_separator) {
      output_field_count++;
      output_char_idx = 0;
    }
  }

  temp_account->account_id = atoi(output[0]);
  strcpy_s(temp_account->account_number, sizeof(temp_account->account_number),
           output[1]);
  strcpy_s(temp_account->login, sizeof(temp_account->login), output[2]);
  strcpy_s(temp_account->password, sizeof(temp_account->password), output[3]);
  strcpy_s(temp_account->first_name, sizeof(temp_account->first_name),
           output[4]);
  strcpy_s(temp_account->last_name, sizeof(temp_account->last_name), output[5]);
  temp_account->date_created = atoi(output[6]);
  temp_account->accout_type = atoi(output[7]);
  temp_account->balance = atoll(output[8]);
  return 0;
}

int FindLineContainingText(char* file_name, char* text, char* current_line,
                           unsigned int current_line_size,
                           struct Account* current_account,
                           struct Account* temp_account) {
  FILE* file;
  int line_num = 0;
  int find_result = 0;
  fopen_s(&file, file_name, "r");
  if (file != NULL) {
    while (fgets(current_line, current_line_size, file) != NULL) {
      if ((strstr(current_line, text)) != NULL) {
        ParseUserFromLine(current_line, current_account, temp_account);
        if (!strcmp(text, temp_account->login)) {
          find_result++;
          break;
        }
      }
      line_num++;
    }
    if (find_result == 0) {
      fclose(file);
      return -1;
    }
    fclose(file);
    return line_num;
  }
  return -1;
}

int GetLastId(char* file_name, char data_separator) {
  FILE* file;
  long file_size;
  char current_character;
  int count = 0;
  int end;
  int start;
  char last_id[16] = "";
  int int_last_id = -1;
  fopen_s(&file, file_name, "r");
  if (file != NULL) {
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

int ModifyUserInFile(char* file_name, char data_separator,
                     struct Account* account_to_modify,
                     struct Account* temp_account) {
  FILE* file;
  FILE* file_temp;
  char found_line[512] = {'\0'};
  int lno = FindLineContainingText(file_name, account_to_modify->login,
                                   found_line, sizeof(found_line),
                                   account_to_modify, temp_account);
  int linectr = 0;
  char str[512];
  char* file_tmp_name = "bmsic_tmp_db.txt";

  if (lno == -1) {
    return -1;
  }
  fopen_s(&file, file_name, "r");
  fopen_s(&file_temp, file_tmp_name, "w");
  if (file != NULL && file_temp != NULL) {
    while (!feof(file)) {
      strcpy_s(str, sizeof(str), "\0");
      fgets(str, sizeof(str), file);
      if (!feof(file)) {
        if (linectr != lno) {
          fprintf(file_temp, "%s", str);
        } else {
          fprintf(file_temp, "%d", account_to_modify->account_id);
          fprintf(file_temp, "%c", data_separator);
          fprintf(file_temp, "%s", account_to_modify->account_number);
          fprintf(file_temp, "%c", data_separator);
          fprintf(file_temp, "%s", account_to_modify->login);
          fprintf(file_temp, "%c", data_separator);
          fprintf(file_temp, "%s", account_to_modify->password);
          fprintf(file_temp, "%c", data_separator);
          fprintf(file_temp, "%s", account_to_modify->first_name);
          fprintf(file_temp, "%c", data_separator);
          fprintf(file_temp, "%s", account_to_modify->last_name);
          fprintf(file_temp, "%c", data_separator);
          fprintf(file_temp, "%d",
                  (unsigned int)account_to_modify->date_created);
          fprintf(file_temp, "%c", data_separator);
          fprintf(file_temp, "%d", account_to_modify->accout_type);
          fprintf(file_temp, "%c", data_separator);
          fprintf(file_temp, "%lld", account_to_modify->balance);
          fprintf(file_temp, "\n");
        }
        linectr++;
      }
    }
    fclose(file);
    fclose(file_temp);
    remove(file_name);
    if (!rename(file_tmp_name, file_name)) {
      return -1;
    }
    return 0;
  }
  return -1;
}

long long int DisplayDepositMoney(int y_max, int x_max) {
  char* text = "Enter amount of money to deposit";
  long long int sum = FloatInputMenu(text, y_max, x_max);
  return sum;
}

long long int DisplayWithdrawMoney(int y_max, int x_max) {
  char* text = "Enter amount of money to withdraw";
  long long int sum = FloatInputMenu(text, y_max, x_max);
  return sum;
}
int FindByLogin(char* file_name, char* login, struct Account* current_account,
                struct Account* temp_account) {
  char found_line[512] = {'\0'};
  int out =
      FindLineContainingText(file_name, login, found_line, sizeof(found_line),
                             current_account, temp_account);
  if (out >= 0) {
    return 0;
  }
  return -1;
}

int DisplayUserBalance(struct Account* current_account, int y_max, int x_max) {
  char first_line[128] = {'\0'};
  char second_line[128] = {'\0'};
  clear();
  sprintf_s(first_line, sizeof(first_line), "Welcome %s!",
            current_account->first_name);
  mvprintw((y_max - 2) / 2, (x_max - (int)strlen(first_line)) / 2, "%s\n",
           first_line);
  if (current_account->balance >= 0) {
    sprintf_s(second_line, sizeof(second_line),
              "Your account balance: %lld.%02lld",
              current_account->balance / 100, current_account->balance % 100);
  } else {
    sprintf_s(second_line, sizeof(second_line),
              "Your account balance: -%lld.%02lld",
              llabs(current_account->balance) / 100,
              llabs(current_account->balance) % 100);
  }
  mvprintw((y_max - 2) / 2 + 1, (x_max - (int)strlen(second_line)) / 2, "%s\n",
           second_line);
  refresh();
  getch();
  return 0;
}

int DisplayTransferMoney() { return 0; }

int DisplayMyAccount(struct Account* current_account, char** my_account_text,
                     unsigned int size, int y_max, int x_max, char* file_name,
                     char data_separator, struct Account* temp_account) {
  int choice;
  long long int sum;
  choice = DisplayVerticalMenu(my_account_text, size, y_max, x_max);
  while (choice != -1) {
    switch (choice) {
      case 4:
        return -1;
      case 0:
        DisplayUserBalance(current_account, y_max, x_max);
        break;
      case 1:
        DisplayTransferMoney();
        break;
      case 2:
        sum = DisplayDepositMoney(y_max, x_max);
        if (sum > 0) {
          current_account->balance += sum;
          ModifyUserInFile(file_name, data_separator, current_account,
                           temp_account);
        }
        break;
      case 3:
        sum = DisplayWithdrawMoney(y_max, x_max);
        if (sum > 0) {
          current_account->balance -= sum;
          ModifyUserInFile(file_name, data_separator, current_account,
                           temp_account);
        }
        break;
      default:
        break;
    }
    choice = DisplayVerticalMenu(my_account_text, size, y_max, x_max);
  }
  return -1;
}

int main() {
  char* text[] = {"LOG IN", "SIGN IN", "EXIT"};
  char* sign_in_text[] = {"Login", "Password", "First Name", "Last Name"};
  char* log_in_text[] = {"Login", "Password"};
  char* my_account_text[] = {"BALANCE", "TRANSACTIONS", "DEPOSIT", "WITHDRAW",
                             "LOG OUT"};
  char** fields_text = NULL;
  char data_separator = 149;
  struct Account current_account;
  struct Account temp_account;
  int y_max, x_max;
  int stage = 0;
  int choice = -1;
  char error_text[50];
  FILE* file;
  char* file_name = "bmsic_db.txt";
  errno_t err;
  bool flag = 0;

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
      return -1;
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
  resize_term(y_max, x_max);
  do {
    if (stage == 0) {
      choice = DisplayHorizontalMenu(text, sizeof(text), y_max, x_max);
      if (choice == 2) {
        SafelyClose();
      }
      if (choice == -1) {
        SafelyClose();
      }
      stage = 1;
    }
    if (stage == 1) {
      if (choice == 0) {
        fields_text =
            calloc(sizeof(log_in_text) / sizeof(log_in_text[0]), sizeof(char*));
        assert(fields_text);
        while (choice != -1) {
          choice = TextInputMenu(log_in_text, sizeof(log_in_text), fields_text,
                                 y_max, x_max, 0);
          if (choice == 10) {
            if (!FindByLogin(file_name, fields_text[0], &current_account,
                             &temp_account)) {
              clear();
              if (!strcmp(fields_text[1],
                          temp_account.password) &&
                  strlen(fields_text[0]) > 0 &&
                  strlen(fields_text[1]) > 0) {  // Logged in user
                current_account = temp_account;
                choice = DisplayMyAccount(
                    &current_account, my_account_text, sizeof(my_account_text),
                    y_max, x_max, file_name, data_separator, &temp_account);
              } else {
                printw("Incorrect password!");
                refresh();
                getch();
              }
            } else {
              clear();
              printw("Incorrect password!");
              refresh();
              getch();
            }
          }
        }
        for (int i = 0; i < sizeof(log_in_text) / sizeof(log_in_text[0]); i++) {
          free(fields_text[i]);
        }
        free(fields_text);
      } else if (choice == 1) {
        flag = 0;
        choice = 0;
        fields_text = calloc(sizeof(sign_in_text) / sizeof(sign_in_text[0]),
                             sizeof(char*));
        assert(fields_text);
        assert(fields_text);
        while (choice != -1) {
          choice = TextInputMenu(sign_in_text, sizeof(sign_in_text),
                                 fields_text, y_max, x_max, flag);
          flag = 1;

          if (choice == -1) {
            stage = 0;
            break;
          } else if (strlen(fields_text[0]) == 0) {
            strcpy_s(error_text, sizeof(error_text), "Login cannot be empty!");
          } else if (strlen(fields_text[1]) < 8) {
            strcpy_s(error_text, sizeof(error_text),
                     "Password must have 8 or more characters!");
          } else if (strlen(fields_text[2]) == 0) {
            strcpy_s(error_text, sizeof(error_text),
                     "First Name cannot be empty!");
          } else if (strlen(fields_text[3]) == 0) {
            strcpy_s(error_text, sizeof(error_text),
                     "Last Name cannot be empty!");
          } else if (!FindByLogin(file_name, fields_text[0], &current_account,
                                  &temp_account)) {
            strcpy_s(error_text, sizeof(error_text), "Login already exists");
          } else if (choice != 27) {
            break;
          }
          clear();
          printw("%s", error_text);
          getch();
        }
        if (choice == 10) {  // Registered user
          current_account = temp_account;
          CreateUser(&current_account, fields_text, sizeof(fields_text),
                     GetLastId(file_name, data_separator));
          SaveUser(file_name, data_separator, &current_account);
          choice = DisplayMyAccount(&current_account, my_account_text,
                                    sizeof(my_account_text), y_max, x_max,
                                    file_name, data_separator, &temp_account);
        }
        for (int i = 0; i < sizeof(sign_in_text) / sizeof(sign_in_text[0]);
             i++) {
          free(fields_text[i]);
        }
        free(fields_text);
      }
      if (choice == -1) {
        stage = 0;
      }
    }
  } while (choice != 10);
  SafelyClose();
  return 0;
}