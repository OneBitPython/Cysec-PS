#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define USER_MAX 32
#define MAXT 50
#define INT_MAX 2147483647
#define MAX_LOANS 2
#define INTEREST_PER 3
#define INTEREST_FD 2.5
#define INTEREST_CREDIT_CARD 2
#define CREDIT_CARD_MONTH 5

int curr;
char ad[5] = "ADMIN";

void delay_random() {
  struct timespec timestruct;
  float del = ((float)rand() / (float)RAND_MAX);
  timestruct.tv_sec = 2;
  timestruct.tv_nsec = del * 1000000000;
  nanosleep(&timestruct, &timestruct);
}

void clear_stdin() {
  // printf("clear_stdin\n");
  int c;
  while ((c = getchar()) != '\n')
    ;
}

void clear_stdin_str(char *str) {
  // printf("clear_stdin_str\n");
  if (str[strlen(str) - 1] != '\n') {
    // printf("Clearing stdin\n");
    clear_stdin();
  } else {
    str[strlen(str) - 1] = '\0';
  }
}
// withdraw, deposit
struct transact {
  // stores withdraw, deposit information
  int type, amount;
  char time[32];
  char person[USER_MAX + 1];
};

struct loan_data {
  int loan_amt;
  float loan_interest_per;
  char time[32];
};

struct account {
  char username[USER_MAX + 1];
  char pan[33];
  char cvv[33];
  char pin[33];
  int credit_card_balance;
  int credit_card_days;
  int balance;
  int active;
  int loan_amt_total;
  int num_loans;
  int fd;
  int credit_card_amounts[CREDIT_CARD_MONTH];
  float interest_loan;
  char time_fd[32];
  char credit_card_start_time[32];
  struct loan_data loan_info[MAX_LOANS];
  struct transact info[MAXT];
};

void give_time(char buf[]) {
  time_t now = time(NULL);

  struct tm *tm = localtime(&now);
  strftime(buf, 32, "%d/%m/%Y %H:%M:%S", tm);
}

float power(float x, int y) {
  float out = 1;
  for (int i = 0; i < y; i++) {
    out *= x;
  }
  return out;
}

void print_menu() {
  printf("---------------------------------------------------------------------"
         "-----------------\n");
  printf("Welcome to the Interface. Please Enter the number of the operation "
         "you want to perform\n");
  printf("1) Create a New Account\n");
  printf("2) Change PIN of an Existing Account\n");
  printf("3) Get Balance of an Account\n");
  printf("4) Deposit money into account\n");
  printf("5) Withdraw Money from an Account\n");
  printf("6) Transaction History\n");
  printf("7) Make transaction.\n");
  printf("8) Get a Loan from the Admin\n");
  printf("9) Create an FD\n");
  printf("10) Liquidate an FD\n");
  printf("11) Log in as admin\n");
  printf("12) Withdraw from Credit Card\n");
  printf("13) Exit\n");
  printf("Query: ");
}

// AES functions - start

int s_box[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B,
    0xFE, 0xD7, 0xAB, 0x76, 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0, 0xB7, 0xFD, 0x93, 0x26,
    0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2,
    0xEB, 0x27, 0xB2, 0x75, 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84, 0x53, 0xD1, 0x00, 0xED,
    0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F,
    0x50, 0x3C, 0x9F, 0xA8, 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2, 0xCD, 0x0C, 0x13, 0xEC,
    0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14,
    0xDE, 0x5E, 0x0B, 0xDB, 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79, 0xE7, 0xC8, 0x37, 0x6D,
    0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F,
    0x4B, 0xBD, 0x8B, 0x8A, 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E, 0xE1, 0xF8, 0x98, 0x11,
    0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F,
    0xB0, 0x54, 0xBB, 0x16};

int h(char c) {
  if (c == '0') {
    return 0;
  } else if (c == '1') {
    return 1;
  } else if (c == '2') {
    return 2;
  } else if (c == '3') {
    return 3;
  } else if (c == '4') {
    return 4;
  } else if (c == '5') {
    return 5;
  } else if (c == '6') {
    return 6;
  } else if (c == '7') {
    return 7;
  } else if (c == '8') {
    return 8;
  } else if (c == '9') {
    return 9;
  } else if (c == 'a') {
    return 10;
  } else if (c == 'b') {
    return 11;
  } else if (c == 'c') {
    return 12;
  } else if (c == 'd') {
    return 13;
  } else if (c == 'e') {
    return 14;
  } else {
    return 15;
  }
}

int hex_to_str(int n) {
  if (n == 0) {
    return '0';
  } else if (n == 1) {
    return '1';
  } else if (n == 2) {
    return '2';
  } else if (n == 3) {
    return '3';
  } else if (n == 4) {
    return '4';
  } else if (n == 5) {
    return '5';
  } else if (n == 6) {
    return '6';
  } else if (n == 7) {
    return '7';
  } else if (n == 8) {
    return '8';
  } else if (n == 9) {
    return '9';
  } else if (n == 10) {
    return 'a';
  } else if (n == 11) {
    return 'b';
  } else if (n == 12) {
    return 'c';
  } else if (n == 13) {
    return 'd';
  } else if (n == 14) {
    return 'e';
  } else if (n == 15) {
    return 'f';
  }
}

int asc(char c) {
  // printf("asc\n");
  int t = c;
  return t;
}

void make_state(int state[4][4], char *str) {
  // printf("Make state\n%s\n", str);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      // printf("%d%d ", i, j);
      if ((4 * i + j) < strlen(str)) {
        state[j][i] = asc(str[4 * i + j]);
      } else {
        state[j][i] = 16 - strlen(str);
      }
    }
  }
  // for (int i = 0 ; i < 4 ; i++)
  // {
  //   for (int j = 0 ; j < 4 ; j++)
  //   {
  //     printf("%d ", state[i][j]);
  //   }
  // }
}

void key_expansion(char *key, int expanded[44][4]) {
  //   printf("Key Expansion\n");
  int k[4][4];
  int pos = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      k[i][j] = 16 * h(key[pos]) + h(key[pos + 1]);
      pos += 2;
    }
  }

  // for (int i = 0 ; i < 4 ; i++)
  // {
  //   for (int j = 0 ; j < 4 ; j++)
  //   {
  //     printf("%02x", k[i][j]);
  //   }
  // }
  // printf("\n");

  int rc[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};
  int temp[44][4];

  for (int i = 0; i < 44; i++) {
    if (i < 4) {
      for (int j = 0; j < 4; j++) {
        temp[i][j] = k[i][j];
      }
    }

    else if (i % 4 == 0 && i > 0) {
      int rcon[4] = {rc[(i / 4) - 1], 0, 0, 0};
      for (int j = 0; j < 4; j++) {
        temp[i][j] = temp[i - 4][j] ^ rcon[j] ^ s_box[temp[i - 1][(j + 1) % 4]];
      }
    }

    else {
      for (int j = 0; j < 4; j++) {
        temp[i][j] = temp[i - 4][j] ^ temp[i - 1][j];
      }
    }
  }

  // for (int i = 0 ; i < 44 ; i++)
  // {
  //   if (i % 4 == 0)
  //   {
  //     printf("\n");
  //   }
  //   for (int j = 0 ; j < 4 ; j++)
  //   {
  //     printf("%02x", temp[i][j]);
  //   }
  // }
  // printf("\n");

  int round = 0;
  while (round <= 10) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        expanded[4 * round + j][i] = temp[4 * round + i][j];
      }
    }
    round++;
  }
}

void addRoundKey(int state[4][4], int expanded[44][4], int round) {
  // printf("Add round key\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = state[i][j] ^ expanded[4 * round + i][j];
    }
  }
}

void subBytes(int state[4][4]) {
  // printf("Sub Bytes\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = s_box[state[i][j]];
    }
  }
  // printf("Thefo\n");
}

void shiftRows(int state[4][4]) {
  // printf("Shift Rows\n");
  for (int i = 0; i < 4; i++) {
    int arr[4];
    for (int j = 0; j < 4; j++) {
      arr[j] = state[i][(j + i) % 4];
    }
    for (int j = 0; j < 4; j++) {
      state[i][j] = arr[j];
    }
  }
}

void mixColumns(int state[4][4]) {
  // printf("Mix Columns\n");
  for (int i = 0; i < 4; i++) {
    int arr[4];
    arr[0] =
        2 * state[0][i] + 3 * state[1][i] + 1 * state[2][i] + 1 * state[3][i];
    arr[1] =
        1 * state[0][i] + 2 * state[1][i] + 3 * state[2][i] + 1 * state[3][i];
    arr[2] =
        1 * state[0][i] + 1 * state[1][i] + 2 * state[2][i] + 3 * state[3][i];
    arr[3] =
        3 * state[0][i] + 1 * state[1][i] + 1 * state[2][i] + 2 * state[3][i];

    state[0][i] = arr[0] % 256;
    state[1][i] = arr[1] % 256;
    state[2][i] = arr[2] % 256;
    state[3][i] = arr[3] % 256;
  }
}

void encrypt(char *str, char *enc) {
  char *key = (char *)malloc(sizeof(char) * 33);
  key = "2b7e151628aed2a6abf7158809cf4f3c";

  int expanded[44][4];
  key_expansion(key, expanded);

  int state[4][4];
  make_state(state, str);

  for (int round = 0; round <= 10; round++) {
    if (round == 0) {
      // AddRoundKey
      addRoundKey(state, expanded, round);
    } else if (round == 10) {
      // SubBytes
      subBytes(state);

      // ShiftRows
      shiftRows(state);

      // AddRoundKey
      addRoundKey(state, expanded, round);
    } else {
      // SubBytes
      subBytes(state);

      // ShiftRows
      shiftRows(state);

      // MixColumns
      mixColumns(state);

      // AddRoundKey
      addRoundKey(state, expanded, round);
    }
  }

  int pos = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      enc[pos] = hex_to_str(state[j][i] / 16);
      enc[pos + 1] = hex_to_str(state[j][i] % 16);
      pos += 2;
    }
  }
}

// AES functions - end
void SAVE(struct account data[]) {
  FILE *fp2 = fopen("accounts.dat", "wb");
  fwrite(data, sizeof(struct account), MAX, fp2);
  fclose(fp2);
}

int input_num(int len, int mode) {
  // mode = 0 means it has to be exactly 'len' length, otherwise it can be
  // anything less than equal to 'len'
  char toinput[len + 1];
  toinput[len] = '\0';
  int flag = 0;
  int to_return = 0;
  for (int i = 0; i < len; i++) {
    fgets(toinput + i, 2, stdin);

    if (toinput[i] >= '0' && toinput[i] <= '9') {
      to_return *= 10;
      to_return += toinput[i] - '0';
    } else {
      if (mode == 0) {
        clear_stdin_str(toinput);
        return -1;
      } else {
        if (toinput[i] == '\n') {
          clear_stdin_str(toinput);
          return to_return;
        }
        flag = 1;
      }
    }
  }
  if (flag == 1) {
    clear_stdin_str(toinput);
    return -1;
  }
  clear_stdin();
  return to_return;
}

void printstream() {
  while (1) {
    int c = getchar();
    putchar(c);
    if (c == '\n')
      break;
  }
}

void input(char *str, int len, int mode, int allowhex) {
  char toinput[len + 1];
  for (int i = 0; i < len; i++) {
    fgets(toinput + i, 2, stdin);

    if ((toinput[i] >= '0' && toinput[i] <= '9') ||
        (allowhex && (toinput[i] >= 'a' && toinput[i] <= 'f'))) {
      str[i] = toinput[i];
    } else {
      if (mode == 0) {
        if (toinput[i] == '\n') {
          str[i] = '\n';
          str[i + 1] = '\0';
        }
        str[0] = 'z';
        return;
      } else {
        if (toinput[i] == '\n') {
          str[i] = '\n';
          str[i + 1] = '\0';
          return;
        } else {
          str[0] = 'z';
          return;
        }
      }
    }
  }
  str[len] = '\0';
}

void new_acc(struct account data[], int *cur) {
  int curr = *cur;
  char c;
  char temp_username[USER_MAX + 2];
  printf("Enter your username: ");
  fgets(temp_username, (USER_MAX + 1), stdin);
  clear_stdin_str(temp_username);

  if (strlen(temp_username) == 0) {
    printf("Can't enter an empty username\n\n\n");
    return;
  }
  for (int i = 0; i < curr; i++) {
    if (strncmp(temp_username, data[i].username, strlen(temp_username)) == 0 &&
        strlen(temp_username) == strlen(data[i].username)) {
      printf("User already exists\n\n\n");
      return;
    }
  }

  strncpy(data[curr].username, temp_username, strlen(temp_username));
  if (strncmp(data[curr].username, ad, 5) == 0) {
    printf("Invalid Username\n");
    return;
  }
  // printf("Username: %s", data[curr].username);

  while (1) {
    printf("Enter the pin you want to set(First 6 digits will be taken): ");
    char *temp_pin;
    temp_pin = (char *)malloc(sizeof(char) * 8);
    data[curr].active = 0;
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);

    if ((temp_pin)[0] == 'z') {
      printf("Enter valid pin\n\n");
      free(temp_pin);
    } else {
      printf("Successfully set PIN to %s\n", temp_pin);
      (*cur)++;
      encrypt(temp_pin, data[curr].pin);
      free(temp_pin);
      char pan_temp[13];
      pan_temp[12] = '\0';
      int count = 0;
      while (count < 12) {
        int r = (int)(9999 * ((float)rand() / (float)RAND_MAX));
        for (int i = 0 ; i < 4 ; i++) {
          pan_temp[count] = (char) ((r%10) + '0');
          r /= 10;
          count++;
        }
      }
      char cvv_temp[4];
      cvv_temp[3] = '\0';
      int r = (int)(999 * ((float)rand() / (float)RAND_MAX));
      for (int i = 0 ; i < 3 ; i++) {
        cvv_temp[i] = (char) ((r%10) + '0');
        r /= 10;
      }
      encrypt(pan_temp, data[curr].pan);
      encrypt(cvv_temp, data[curr].cvv);
      printf("Your PAN number is %s\n", pan_temp);
      printf("Your CVV number is %s\n", cvv_temp);
      printf("These numbers will not be shown again. CVV and PAN can't be reset\n\n\n");
      SAVE(data);
      break;
    }
  }
  //   printf("Exiting new_acc\n");
}

void slide(struct transact info[]) {
  for (int i = 0; i < MAXT; ++i)
    info[i] = info[i + 1];
}

int num_days_per_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void update_loan_interest_amount(struct account data[]) {
  int temp_num = 0;
  time_t now = time(NULL);
  struct tm *time_now = localtime(&now);
  int day_now =
      time_now->tm_yday + 1; // yday!=yesterday yday = num from 0 to 365
  int temp_day, temp_month, temp1, temp2, temp3, temp4, day;
  float acc_interest;
  day = 0;
  while (data[temp_num].username[0] != '\0') {
    acc_interest = 0;
    for (int i = 0; i < data[temp_num].num_loans; i++) {
      day = 0;
      sscanf(data[temp_num].loan_info[i].time, "%d/%d/%d %d:%d:%d", &temp_day,
             &temp_month, &temp1, &temp2, &temp3, &temp4);
      for (int j = 0; j < (temp_month - 1); j++) {
        day += num_days_per_month[j];
      }
      day += temp_day;

      if (temp_day != day_now) {
        float temp_interest = 0;
        temp_interest = (float)data[temp_num].loan_info[i].loan_amt *
                        power((1 + (float)INTEREST_PER / 100), (day_now - day));
        // printf("%f", power((1 + (float)INTEREST_PER / 100), (day_now -
        // day)));
        temp_interest -= (float)data[temp_num].loan_info[i].loan_amt;
        data[temp_num].loan_info[i].loan_interest_per = temp_interest;
        acc_interest += temp_interest;
      }
    }
    data[temp_num].interest_loan = acc_interest;
    temp_num++;
  }
}

int check_time_limit_loan_repayment(struct account data[], int pos,
                                    int loan_num) {
  time_t now = time(NULL);
  struct tm *time_now = localtime(&now);
  int day_now = time_now->tm_yday + 1;
  int final_time_now =
      time_now->tm_hour * 3600 + time_now->tm_min * 60 + time_now->tm_sec;
  int temp_day, temp_month, temp_year, temp_hour, temp_min, temp_sec, day,
      final_time;
  day = 0;
  sscanf(data[pos].loan_info[loan_num].time, "%d/%d/%d %d:%d:%d", &temp_day,
         &temp_month, &temp_year, &temp_hour, &temp_min, &temp_sec);
  final_time = 0;
  final_time = temp_hour * 3600 + temp_min * 60 + temp_sec;
  for (int i = 0; i < (temp_month - 1); i++) {
    day += num_days_per_month[i];
  }
  day += temp_day;
  int time_diff = (day_now - day) * 86400 - final_time + final_time_now;
  if (time_diff >= 86400) {
    return 1;
  } else {
    return 0;
  }
}


int check_time_fd_liquidation(struct account data[], int pos) {
  time_t now = time(NULL);
  struct tm *time_now = localtime(&now);
  int day_now = time_now->tm_yday + 1;
  int final_time_now =
      time_now->tm_hour * 3600 + time_now->tm_min * 60 + time_now->tm_sec;
  int temp_day, temp_month, temp_year, temp_hour, temp_min, temp_sec, day,
      final_time;
  day = 0;
  sscanf(data[pos].time_fd, "%d/%d/%d %d:%d:%d", &temp_day,
         &temp_month, &temp_year, &temp_hour, &temp_min, &temp_sec);
  final_time = 0;
  final_time = temp_hour * 3600 + temp_min * 60 + temp_sec;
  for (int i = 0; i < (temp_month - 1); i++) {
    day += num_days_per_month[i];
  }
  day += temp_day;
  int time_diff = (day_now - day) * 86400 - final_time + final_time_now;
  if (time_diff >= 86400) {
    return time_diff;
  } else {
    return 0;
  }
}


int check_time_credit_card(struct account data[], int pos) {
  time_t now = time(NULL);
  struct tm *time_now = localtime(&now);
  int day_now = time_now->tm_yday + 1;
  int final_time_now =
      time_now->tm_hour * 3600 + time_now->tm_min * 60 + time_now->tm_sec;
  int temp_day, temp_month, temp_year, temp_hour, temp_min, temp_sec, day,
      final_time;
  day = 0;
  sscanf(data[pos].credit_card_start_time, "%d/%d/%d %d:%d:%d", &temp_day,
         &temp_month, &temp_year, &temp_hour, &temp_min, &temp_sec);
  for (int i = 0; i < (temp_month - 1); i++) {
    day += num_days_per_month[i];
  }
  day += temp_day;
  if(day_now!=day){
    return (day_now-day);
  }
  else{
    return 0;
  }
}


void withdraw_from_credit_card(struct account data[]){
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }
  int times = 0;
  while (times < 3) {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);
    char *enc = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc);
    if (temp_pin[0] == 'z') {
      if (times != 3) {
        delay_random();
        printf("Invalid PIN, ");
        free(enc);
      } else {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        free(enc);
      }
    } else if (strncmp(data[pos].pin, enc, 32) != 0) {
      if (times != 3) {
        delay_random();
        printf("PIN doesn't match, ");
        free(enc);
      } else {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        free(enc);
      }
    } 
    else {
      printf("Successfully logged in to account\n\n");
      printf("Please enter the amount you want to withdraw(from your credit card):\n");
      char withdraw[11];
      input(withdraw,9,1,0);
      clear_stdin_str(withdraw);
      if(withdraw[0]=='z'){
        printf("Invalid number enetered \n\n");
      }
      else{
        int temp=0;
        for(int i=0;i<strlen(withdraw);i++){
          temp*=10;
          temp+=withdraw[i]-'0';
        }
        if(temp == 0){
          printf("Please enter a non-zero amount");
          return;
        }
        if(data[pos].credit_card_start_time[0] == '\0'){
          give_time(data[pos].credit_card_start_time);
          data[pos].credit_card_balance=0;
          data[pos].credit_card_days=0;
          for(int i =0;i<CREDIT_CARD_MONTH;i++){
            data[pos].credit_card_amounts[i] = 0;
          }
          SAVE(data);
        }
        if(check_time_credit_card(data,pos)==0){
          data[pos].credit_card_amounts[data[pos].credit_card_days] +=temp;
          data[pos].credit_card_balance+=temp;
          if(data[pos].credit_card_balance > 500){
            printf("Entered amount causes credit card balance to exceed 500 points . NOT Processing Withdrawal \n\n");
            data[pos].credit_card_balance-=temp;
            data[pos].credit_card_amounts[data[pos].credit_card_days]-=temp;
            SAVE(data);
            return;
          }
          data[pos].balance+=temp;
          SAVE(data);
          return;
        }
        if(check_time_credit_card(data, pos) != 0){
          data[pos].credit_card_days=check_time_credit_card(data, pos);
          if(data[pos].credit_card_days < 5){
            data[pos].credit_card_amounts[data[pos].credit_card_days] +=temp;
            data[pos].credit_card_balance+=temp;
            if(data[pos].credit_card_balance > 500){
              printf("Entered amount causes credit card balance to exceed 500 points . NOT Processing Withdrawal \n\n");
              data[pos].credit_card_balance-=temp;
              data[pos].credit_card_amounts[data[pos].credit_card_days]-=temp;
              SAVE(data);
              return;
            }
            data[pos].balance+=temp;
            SAVE(data);
            return;
          }
          else{
            printf("The Credit Card month has ended . Forcing Repayment of Credit card amount ");
            printf("Credit Card Amount was : %d\n",data[pos].credit_card_balance);
            float interest = 0;
            float temp_val = 0;
            for(int i =0;i<CREDIT_CARD_MONTH;i++){
              temp_val+=data[pos].credit_card_amounts[i];
              interest += (float)(temp_val+interest)*(float)INTEREST_CREDIT_CARD/100;
            }
            printf("Credit Card Interest was : %d\n",(int)interest);
            if(data[pos].balance<=(data[pos].credit_card_balance+interest)){
              printf("Sufficient Balance was found , Automatically deducting the required amount from account balance\n");
              data[pos].balance-=data[pos].credit_card_balance;
              data[pos].balance-=(int)interest;
            }
            else{
              printf("Insufficient balance was found , Automatially removing all the balance left in the account\n");
              data[pos].balance = 0;
            }
            data[pos].credit_card_start_time[0]='\0';
            SAVE(data);
            return;
          }
        }
        
        // printf("%s",data[pos].credit_card_start_time);
        // give_time(data[pos].credit_card_start_time);
        // if(data[pos].credit_card_start_time[0] == '\0'){
          // printf("Not started ;-;\n");
        // }
        // printf("%d",data[pos].credit_card_days);
        // printf("%d",data[pos].credit_card_balance);
        // printf("%d",data[pos].credit_card_amounts[0]);
        break;
      }
    }
  }
}



void loan(struct account data[]) {
  printf("Enter your username:");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);
  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }
  if (pos == -1) {
    printf("No such user exists\n\n\n");
    return;
  }
  int times = 0;
  while (times <= 3) {
    times++;
    printf("Enter the PIN:");
    char temp_pin[8];
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);
    char *enc = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc);
    if (temp_pin[0] == 'z') {
      if (times != 3) {
        printf("Invalid PIN ,");
      } else {
        printf("3 attempts exhausted\n\n\n");
      }
    } else if (strncmp(data[pos].pin, enc, 32) != 0) {
      if (times != 3) {
        printf("PIN doent match, ");
      } else {
        printf("3 attempts exhausted\n\n\n");
      }
    } else {
      printf("Successfully logged into account \n\n");
      if (data[pos].loan_amt_total > 0) {
        printf("Do you want to repay existing loans? (Enter y for yes)[case "
               "sensitive]\n(Enter anything else if you want to take a loan "
               "from the admin) "
               ": ");
        char repay_loan_option[3];
        fgets(repay_loan_option, 2, stdin);
        clear_stdin_str(repay_loan_option);
        if (strncmp(repay_loan_option, "y", 2) == 0) {
          int possible_repayable_loan_indexes[MAX_LOANS] = {0};
          for (int temp_var = 0; temp_var < data[pos].num_loans; temp_var++) {
            if (check_time_limit_loan_repayment(data, pos, temp_var)) {
              possible_repayable_loan_indexes[temp_var] = 1;
            }
          }
          update_loan_interest_amount(data);
          int found_atleast_one_repayable_loan = 0;
          for (int i = 0; i < data[pos].num_loans; i++) {
            if (possible_repayable_loan_indexes[i]) {
              found_atleast_one_repayable_loan = 1;
              break;
            }
          }
          if (!found_atleast_one_repayable_loan) {
            printf("No Repayable loan found \n\n\n");
            return;
          }
          printf("These are the possible loans you can repay : \n");
          printf("Index | Loan Principal | Loan Interest\n");
          for (int i = 0; i < data[pos].num_loans; i++) {
            if (possible_repayable_loan_indexes[i]) {
              printf("  %-4d|      %-10d|      %-8d\n", i,
                     data[pos].loan_info[i].loan_amt,
                     (int)data[pos].loan_info[i].loan_interest_per);
            }
          }
          printf("Please enter the index of the loan you want to repay : ");
          int repay_loan_num;
          repay_loan_num = input_num(2, 1);
          if (possible_repayable_loan_indexes[repay_loan_num] == 1) {
            if (data[pos].balance >=
                (data[pos].loan_info[repay_loan_num].loan_amt +
                 (int)data[pos].loan_info[repay_loan_num].loan_interest_per)) {
              data[pos].balance -=
                  (data[pos].loan_info[repay_loan_num].loan_amt +
                   (int)data[pos].loan_info[repay_loan_num].loan_interest_per);
              data[pos].loan_amt_total -=
                  (int)data[pos].loan_info[repay_loan_num].loan_amt;
              data[pos].interest_loan -=
                  data[pos].loan_info[repay_loan_num].loan_interest_per;
              for (int i = repay_loan_num; i < (data[pos].num_loans - 1); i++) {
                data[pos].loan_info[i] = data[pos].loan_info[i + 1];
              }
              data[pos].num_loans -= 1;
              printf("Successfully Repaid Loan\n\n");
              return;
            } else {
              printf("Insufficient Balance to repay back the loan.\n");
              printf("Current Balance : %d\n", data[pos].balance);
              printf("Amount required to repay loan : %d",
                     data[pos].loan_info[repay_loan_num].loan_amt +
                         (int)data[pos]
                             .loan_info[repay_loan_num]
                             .loan_interest_per);
              printf("\n\n\n");
              return;
            }
          } else {
            printf("Invalid index entered\n\n\n");
            return;
          }
        }
      }
      printf("Enter the amount you want to loan from the admin\n");
      char loan_amount[11];
      input(loan_amount, 9, 1, 0);
      clear_stdin_str(loan_amount);
      if (loan_amount[0] == 'z') {
        printf("Invalid number entered \n\n");
      } else {
        int temp = 0;
        for (int i = 0; i < strlen(loan_amount); i++) {
          temp *= 10;
          temp += loan_amount[i] - '0';
        }
        if(temp == 0){
          printf("Please enter a non zero amount\n\n\n");
          return;
        }
        if ((data[pos].loan_amt_total + temp) >
            500) { // im assuming each point corresponds to 1 currency
                   // cuz the
                   // pdf mentions max of 500 points ;-;
          printf("Max Loan amount is 500 points , Please enter amount within "
                 "the limit of 500 points\n\n");
          break;
        }
        if (data[pos].num_loans >= MAX_LOANS) {
          printf("Maximum number of loans per account reached\n\n");
          break;
        }
        data[pos].balance += temp;
        data[pos].loan_amt_total += temp;
        struct loan_data new_loan;
        give_time(new_loan.time);
        new_loan.loan_amt = temp;
        data[pos].loan_info[data[pos].num_loans] = new_loan;
        data[pos].num_loans++;
        SAVE(data);
        free(enc);
        break;
      }
    }
  }
}

void withdraw(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }
  int times = 0;
  while (times < 3) {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);
    char *enc = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc);
    if (temp_pin[0] == 'z') {
      if (times != 3) {
        delay_random();
        printf("Invalid PIN, ");
        free(enc);
      } else {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        free(enc);
      }
    } else if (strncmp(data[pos].pin, enc, 32) != 0) {
      if (times != 3) {
        delay_random();
        printf("PIN doesn't match, ");
        free(enc);
      } else {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        free(enc);
      }
    } else {
      printf("Successfully logged in to account\n\n");
      printf("Current balance is %lld. Enter the amount you want to withdraw: ",
             data[pos].balance);
      char withdraw[11];

      input(withdraw, 9, 1, 0);
      clear_stdin_str(withdraw);

      if (withdraw[0] == 'z') {
        printf("Invalid number entered\n\n");
      } else {
        int temp = 0;
        for (int i = 0; i < strlen(withdraw); i++) {
          temp *= 10;
          temp += withdraw[i] - '0';
        }
        if (data[pos].balance < temp) {
          printf("Insufficient funds.\n\n\n");
          break;
        }
        data[pos].balance -= temp;
        printf("Your balance now is %lld\n\n\n", data[pos].balance);

        struct transact new_trans;
        new_trans.type = 0;
        new_trans.amount = temp;
        give_time(new_trans.time);

        if (data[pos].active == MAXT)
          slide(data[pos].info), data[pos].active--;

        data[pos].info[data[pos].active] = new_trans;
        if (data[pos].active < MAXT)
          data[pos].active++;

        SAVE(data);
        free(enc);

        break;
      }
    }
  }
}
void liquidate_fd(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }
  int times = 0;
  while (times < 3) {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z') {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        free(enc_pin);
        return;
      }
      delay_random();
      printf("Invalid PIN, ");
    } else if (strncmp(data[pos].pin, enc_pin, 32) != 0) {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        free(enc_pin);
        return;
      }
      delay_random();
      printf("PIN doesn't match, ");
    } else {
      printf("Successfully logged in to account\n\n");
      printf("Current balance is %lld. ", data[pos].balance);
      if(data[pos].fd == 0){
        printf("There is no existing fd . Please Create an fd first\n\n\n");
        return;
      }
      if(check_time_fd_liquidation(data, pos)==0){
        printf("Please wait 1 day before trying to liquidate fd\n\n\n");
        return;
      }
      int time_diff =  check_time_fd_liquidation(data, pos);
      int day_diff = time_diff/86400;
      int interest_fd_temp = (int)data[pos].fd*power((1+(float)INTEREST_FD/100),day_diff)-data[pos].fd;
      printf("Successfully Liquidated FD amount of %d and interest of %d\n",data[pos].fd,interest_fd_temp);
      data[pos].balance+=data[pos].fd+interest_fd_temp;
      data[pos].fd = 0;
      printf("Your balance now is %lld\n\n\n", data[pos].balance);
      SAVE(data);
      free(enc_pin);
      break;
    }
  }
}

void create_fd(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }
  int times = 0;
  while (times < 3) {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z') {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        free(enc_pin);
        return;
      }
      delay_random();
      printf("Invalid PIN, ");
    } else if (strncmp(data[pos].pin, enc_pin, 32) != 0) {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        free(enc_pin);
        return;
      }
      delay_random();
      printf("PIN doesn't match, ");
    } else {
      printf("Successfully logged in to account\n\n");
      if(data[pos].fd != 0){
        printf("Account already has an FD . Please liquidate the existing fd before creating a new one \n\n\n");
        return;
      }
      printf(
          "Current balance is %lld. Enter the amount you want to add to FD : ",
          data[pos].balance);
      char withdraw[11];

      input(withdraw, 9, 1, 0);
      clear_stdin_str(withdraw);

      if (withdraw[0] == 'z') {
        printf("Invalid number entered\n\n");
      } else {
        int temp = 0;
        for (int i = 0; i < strlen(withdraw); i++) {
          temp *= 10;
          temp += withdraw[i] - '0';
        }
        if(temp == 0){
          printf("Please enter a non-zero amount\n\n\n");
          return;
        }
        if (data[pos].balance < temp) {
          printf("Insufficient funds.\n\n\n");
          break;
        }
        data[pos].balance -= temp;
        data[pos].fd= temp;
        give_time(data[pos].time_fd);
        printf("Your balance now is %lld\n\n\n", data[pos].balance);
        SAVE(data);
        free(enc_pin);

        break;
      }
    }
  }
}
void change_pin(struct account data[]) {
  char temp_user[USER_MAX + 2];
  printf("Enter the username: ");
  fgets(temp_user, USER_MAX + 1, stdin);
  clear_stdin_str(temp_user);
  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, temp_user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("User doesn't exist\n\n\n");
    return;
  }

  printf("Enter the existing PIN: ");
  char temp_pin[8];
  input(temp_pin, 6, 0, 1);
  clear_stdin_str(temp_pin);
  char *enc_pin = (char *)malloc(sizeof(char) * 33);
  encrypt(temp_pin, enc_pin);
  if (temp_pin[0] == 'z') {
    delay_random();
    printf("Invalid PIN entered. Exiting process\n\n\n");
    free(enc_pin);
    return;
  } else if (strncmp(data[pos].pin, enc_pin, 32) != 0) {
    delay_random();
    printf("PIN doesn't match. Exiting process\n\n\n");
    free(enc_pin);
    return;
  }

  printf("Enter new PIN: ");
  char new_pin1[8];
  input(new_pin1, 6, 0, 1);
  clear_stdin_str(new_pin1);
  if (new_pin1[0] == 'z') {
    printf("Invalid PIN entered. Exiting process\n\n\n");
    return;
  } else if (strncmp(new_pin1, temp_pin, 6) == 0) {
    printf("Can't change PIN to existing PIN. Exiting process \n\n\n");
    return;
  }
  printf("Enter the PIN again: ");
  char new_pin2[8];
  input(new_pin2, 6, 0, 1);
  clear_stdin_str(new_pin2);
  if (new_pin2[0] == 'z') {
    printf("Invalid PIN entered. Exiting process\n\n\n");
    return;
  } else if (strncmp(new_pin2, new_pin1, 6) != 0) {
    printf("PINs don't match. Exiting process\n\n\n");
  } else {
    char *enc_pin_new = (char *)malloc(sizeof(char) * 33);
    encrypt(new_pin1, enc_pin_new);
    strncpy(data[pos].pin, enc_pin_new, 32);
    printf("PIN successfully changed to %s\n\n\n", new_pin1);
    free(enc_pin_new);
    SAVE(data);

    return;
  }
}

void make_transaction(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  int times = 0;
  while (times < 3) {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z') {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        return;
      }
      delay_random();
      printf("Invalid PIN, ");
    } else if (strncmp(data[pos].pin, enc_pin, 32) != 0) {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        return;
      }
      delay_random();
      printf("PIN doesn't match, ");
    } else {
      printf("Successfully logged in to account\n\n");

      int t2 = 0;
      while (t2 < 3) {
        t2++;
        printf("Enter username you want to send money to : ");
        char user2[USER_MAX + 2];
        fgets(user2, (USER_MAX + 1), stdin);
        clear_stdin_str(user2);

        int pos2 = -1;
        for (int i = 0; i < curr; i++) {
          if (strncmp(data[i].username, user2, USER_MAX) == 0) {
            pos2 = i;
            break;
          }
        }

        if (pos2 == -1) {
          printf("Invalid user.\n");
          continue;
        }
        if (pos == pos2) {
          printf("Can't send money to yourself. \n");
          continue;
        }
        printf("Current balance is %lld. Enter the amount you want to send: ",
               data[pos].balance);
        char withdraw[11];

        input(withdraw, 9, 1, 0);
        clear_stdin_str(withdraw);

        if (withdraw[0] == 'z') {
          printf("Invalid number entered\n\n");
        } else {
          int temp = 0;
          for (int i = 0; i < strlen(withdraw); i++) {
            temp *= 10;
            temp += withdraw[i] - '0';
          }
          if (data[pos].balance < temp) {
            printf("Insufficient funds.\n\n\n");
            break;
          }
          data[pos].balance -= temp;
          printf("Your balance now is %lld\n\n\n", data[pos].balance);
          data[pos2].balance += temp;

          struct transact new_trans;
          new_trans.type = 2;
          new_trans.amount = temp;
          give_time(new_trans.time);

          strncpy(new_trans.person, user2, strlen(user2));

          if (data[pos].active == MAXT)
            slide(data[pos].info), data[pos].active--;

          data[pos].info[data[pos].active] = new_trans;
          if (data[pos].active < MAXT)
            data[pos].active++;

          struct transact new_trans2;
          new_trans2.type = 3;
          new_trans2.amount = temp;
          give_time(new_trans2.time);

          strncpy(new_trans2.person, user, strlen(user));

          if (data[pos2].active == MAXT)
            slide(data[pos2].info), data[pos2].active--;

          data[pos2].info[data[pos2].active] = new_trans2;
          if (data[pos2].active < MAXT)
            data[pos2].active++;
          SAVE(data);

          break;
        }
        break;
      }
      break;
    }
  }
}

void print_history(struct account data[]) {

  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  int times = 0;
  while (times <= 3) {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z') {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        return;
      }
      delay_random();
      printf("Invalid PIN, ");
    } else if (strncmp(data[pos].pin, enc_pin, 32) != 0) {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        return;
      }
      delay_random();
      printf("PIN doesn't match, ");
    } else {
      printf("Successfully logged in to account\n\n");
      if (data[pos].active == 0) {
        printf("No history\n\n");
        return;
      }
      for (int i = 0; i < data[pos].active; ++i) {
        printf("%d. ", i + 1);
        if (data[pos].info[i].type == 1)
          printf("Deposit of %lld made on %s\n", data[pos].info[i].amount,
                 data[pos].info[i].time);
        else if (data[pos].info[i].type == 0)
          printf("Withdrawal of %lld made on %s\n", data[pos].info[i].amount,
                 data[pos].info[i].time);
        else if (data[pos].info[i].type == 2)
          printf("Transaction of %lld made to %s on %s\n",
                 data[pos].info[i].amount, data[pos].info[i].person,
                 data[pos].info[i].time);
        else if (data[pos].info[i].type == 3)
          printf("Transaction of %lld made from %s on %s\n",
                 data[pos].info[i].amount, data[pos].info[i].person,
                 data[pos].info[i].time);
        else if (data[pos].info[i].type == 4)
          printf("Deposit of %lld made by admin on %s\n",
                 data[pos].info[i].amount, data[pos].info[i].time);
      }
      break;
    }
  }
}
void deposit(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  int times = 0;
  while (times <= 3) {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z') {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        return;
      }
      delay_random();
      printf("Invalid PIN, ");
    } else if (strncmp(data[pos].pin, enc_pin, 32) != 0) {
      if (times == 3) {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
        return;
      }
      delay_random();
      printf("PIN doesn't match, ");
    } else {
      printf("Successfully logged in to account\n\n");
      printf("Enter the amount you want to deposit: ");
      char deposit[11];
      input(deposit, 9, 1, 0);
      clear_stdin_str(deposit);

      if (deposit[0] == 'z') {
        printf("Invalid number entered\n\n");
      } else {
        int temp = 0;

        for (int i = 0; i < strlen(deposit); i++) {
          if (deposit[i] == '\n')
            break;
          temp *= 10;
          temp += deposit[i] - '0';
        }
        if (INT_MAX - temp < data[pos].balance) {
          printf("Too much to deposit\n\n\n");
          return;
        }
        data[pos].balance += temp;
        printf("Your balance now is %lld\n\n\n", data[pos].balance);

        struct transact new_trans;
        new_trans.type = 1;
        new_trans.amount = temp;
        give_time(new_trans.time);

        if (data[pos].active == MAXT)
          slide(data[pos].info), data[pos].active--;

        data[pos].info[data[pos].active] = new_trans;
        if (data[pos].active < MAXT)
          data[pos].active++;
        SAVE(data);

        break;
      }
    }
  }
}

void print_balance(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  int times = 0;
  while (times < 3) {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0, 1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z') {
      if (times != 3) {
        delay_random();
        printf("Invalid PIN, ");
      } else {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
      }
    } else if (strncmp(data[pos].pin, enc_pin, 32) != 0) {
      if (times != 3) {
        delay_random();
        printf("PIN doesn't match, ");
      } else {
        delay_random();
        printf("3 attempts exhausted\n\n\n");
      }
    } else {
      printf("Successfully logged in to account\n");
      printf("Your balance now is %lld\n\n\n", data[pos].balance);
      break;
    }
  }
}

int check_admin() {
  char *pass = (char *)malloc(sizeof(char) * 17);
  char *pass_enc = (char *)malloc(sizeof(char) * 33);
  printf(
      "Enter the admin password(only the first 16 characters will be taken): ");
  fgets(pass, 17, stdin);
  clear_stdin_str(pass);
  encrypt(pass, pass_enc);
  printf("Checking key\n");
  delay_random();
  // Password: xA9!Qf7$L2@RkZ#M
  char *admin_pass = (char *)malloc(sizeof(char) * 33);
  admin_pass = "42fc799587b75b4270097744d4b19255";
  if (strncmp(admin_pass, pass_enc, 32) == 0) {
    return 1;
  } else {
    return 0;
  }
}

void print_menu_admin() {
  printf("---------------------------------------------------------------------"
         "-----------------\n");
  printf("Welcome to the Interface. Please Enter the number of the operation "
         "you want to perform\n");
  printf("1) Create a New Account\n");
  printf("2) Change PIN of an Existing Account\n");
  printf("3) Get Balance of an Account\n");
  printf("4) Deposit money into an account from your account\n");
  printf("5) Withdraw Money from an Account\n");
  printf("6) Transaction History\n");
  printf("7) Make transaction between 2 accounts\n");
  printf("8) Get a Loan from the Admin\n");
  printf("9) Create an FD\n");
  printf("10) Liquidate an FD\n");
  printf("11) Exit admin\n");
  printf("Query: ");
}

void print_balance_admin(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  printf("Balance of user is %d\n\n\n", data[pos].balance);
  return;
}

void deposit_admin(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  printf("Enter the amount you want to deposit: ");
  char deposit[11];
  input(deposit, 9, 1, 0);
  clear_stdin_str(deposit);
  if (deposit[0] == 'z') {
    printf("Invalid number entered\n\n");
  } else {
    int temp = 0;

    for (int i = 0; i < strlen(deposit); i++) {
      if (deposit[i] == '\n')
        break;
      temp *= 10;
      temp += deposit[i] - '0';
    }
    if (INT_MAX - temp < data[pos].balance) {
      printf("Too much to deposit\n\n\n");
      return;
    }
    data[pos].balance += temp;
    printf("Balance of user now is %lld\n\n\n", data[pos].balance);

    struct transact new_trans;
    new_trans.type = 4;
    new_trans.amount = temp;
    give_time(new_trans.time);

    if (data[pos].active == MAXT)
      slide(data[pos].info), data[pos].active--;
    data[pos].info[data[pos].active] = new_trans;
    if (data[pos].active < MAXT)
      data[pos].active++;
    SAVE(data);

    return;
  }
}

void withdraw_admin(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  printf("Current balance is %lld. Enter the amount you want to withdraw: ",
         data[pos].balance);
  char withdraw[11];

  input(withdraw, 9, 1, 0);
  clear_stdin_str(withdraw);

  if (withdraw[0] == 'z') {
    printf("Invalid number entered\n\n");
  } else {
    int temp = 0;
    for (int i = 0; i < strlen(withdraw); i++) {
      temp *= 10;
      temp += withdraw[i] - '0';
    }
    if (data[pos].balance < temp) {
      printf("Insufficient funds.\n\n\n");
      return;
    }
    data[pos].balance -= temp;
    printf("Balance of user now is %lld\n\n\n", data[pos].balance);

    struct transact new_trans;
    new_trans.type = 0;
    new_trans.amount = temp;
    give_time(new_trans.time);

    if (data[pos].active == MAXT)
      slide(data[pos].info), data[pos].active--;

    data[pos].info[data[pos].active] = new_trans;
    if (data[pos].active < MAXT)
      data[pos].active++;
    SAVE(data);

    return;
  }
}

void print_history_admin(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  printf("Successfully logged in to account\n\n");
  if (data[pos].active == 0) {
    printf("No history\n\n");
    return;
  }
  for (int i = 0; i < data[pos].active; ++i) {
    printf("%d. ", i + 1);
    if (data[pos].info[i].type == 1)
      printf("Deposit of %lld made on %s\n", data[pos].info[i].amount,
             data[pos].info[i].time);
    else if (data[pos].info[i].type == 0)
      printf("Withdrawal of %lld made on %s\n", data[pos].info[i].amount,
             data[pos].info[i].time);
    else if (data[pos].info[i].type == 2)
      printf("Transaction of %lld made to %s on %s\n", data[pos].info[i].amount,
             data[pos].info[i].person, data[pos].info[i].time);
    else if (data[pos].info[i].type == 3)
      printf("Transaction of %lld made from %s on %s\n",
             data[pos].info[i].amount, data[pos].info[i].person,
             data[pos].info[i].time);
    else if (data[pos].info[i].type == 4)
      printf("Deposit of %lld made by admin on %s\n", data[pos].info[i].amount,
             data[pos].info[i].time);
  }
}

void make_transaction_admin(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  int t2 = 0;
  while (t2 < 3) {
    t2++;
    printf("Enter username you want to send money to : ");
    char user2[USER_MAX + 2];
    fgets(user2, (USER_MAX + 1), stdin);
    clear_stdin_str(user2);

    int pos2 = -1;
    for (int i = 0; i < curr; i++) {
      if (strncmp(data[i].username, user2, USER_MAX) == 0) {
        pos2 = i;
        break;
      }
    }

    if (pos2 == -1) {
      printf("Invalid user.\n");
      continue;
    }
    if (pos == pos2) {
      printf("Can't send money to yourself. \n");
      continue;
    }
    printf("Current balance is %lld. Enter the amount you want to send: ",
           data[pos].balance);
    char withdraw[11];

    input(withdraw, 9, 1, 0);
    clear_stdin_str(withdraw);

    if (withdraw[0] == 'z') {
      printf("Invalid number entered\n\n");
    } else {
      int temp = 0;
      for (int i = 0; i < strlen(withdraw); i++) {
        temp *= 10;
        temp += withdraw[i] - '0';
      }
      if (data[pos].balance < temp) {
        printf("Insufficient funds.\n\n\n");
        break;
      }
      data[pos].balance -= temp;
      printf("Balance of user now is %lld\n\n\n", data[pos].balance);
      data[pos2].balance += temp;

      struct transact new_trans;
      new_trans.type = 2;
      new_trans.amount = temp;
      give_time(new_trans.time);

      strncpy(new_trans.person, user2, strlen(user2));
      if (data[pos].active == MAXT)
        slide(data[pos].info), data[pos].active--;

      data[pos].info[data[pos].active] = new_trans;
      if (data[pos].active < MAXT)
        data[pos].active++;

      struct transact new_trans2;
      new_trans2.type = 3;
      new_trans2.amount = temp;
      give_time(new_trans2.time);

      strncpy(new_trans2.person, user, strlen(user));
      if (data[pos2].active == MAXT)
        slide(data[pos2].info), data[pos2].active--;

      data[pos2].info[data[pos2].active] = new_trans2;
      if (data[pos2].active < MAXT)
        data[pos2].active++;
      SAVE(data);

      break;
    }
    break;
  }
}

void create_fd_admin(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  printf("Current balance is %lld. Enter the amount you want to add to FD : ",
         data[pos].balance);
  char withdraw[11];

  input(withdraw, 9, 1, 0);
  clear_stdin_str(withdraw);

  if (withdraw[0] == 'z') {
    printf("Invalid number entered\n\n");
  } else {
    int temp = 0;
    for (int i = 0; i < strlen(withdraw); i++) {
      temp *= 10;
      temp += withdraw[i] - '0';
    }
    if (data[pos].balance < temp) {
      printf("Insufficient funds.\n\n\n");
      return;
    }
    data[pos].balance -= temp;
    printf("Balance of user now is %lld\n\n\n", data[pos].balance);

    data[pos].fd += temp;
    SAVE(data);

    return;
  }
}

void liquidate_fd_admin(struct account data[]) {
  printf("Enter your username: ");
  char user[USER_MAX + 2];
  fgets(user, (USER_MAX + 1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0; i < curr; i++) {
    if (strncmp(data[i].username, user, USER_MAX) == 0) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    printf("No such user\n\n\n");
    return;
  }

  printf("Current balance is %lld. ", data[pos].balance);

  data[pos].balance += data[pos].fd;
  data[pos].fd = 0;
  printf("Balance of user now is %lld\n\n\n", data[pos].balance);
}

void admin(struct account data[]) {
  printf("Logged in as ADMIN\n\n");

  int query = -1;
  // clear_stdin();
  // printf("%d\n", query);
  while (1) {
    print_menu_admin();
    query = input_num(2, 1);
    switch (query) {
    case 1:
      new_acc(data, &curr);
      break;

    case 2:
      printf("SIKE! Can't change PIN\n\n\n");
      break;

    case 3:
      print_balance_admin(data);
      break;

    case 4:
      deposit_admin(data);
      break;

    case 5:
      withdraw_admin(data);
      break;

    case 6:
      print_history_admin(data);
      break;

    case 7:
      make_transaction_admin(data);
      break;

    case 8:
      break;

    case 9:
      create_fd_admin(data);
      break;

    case 10:
      liquidate_fd_admin(data);
      break;

    case 11:
      printf("Exiting admin\n\n\n");
      return;

    default:
      printf("Enter a valid option\n\n");
    }
  }
}

int main() {
  srand(time(NULL));
  struct account data[MAX];
  int query = -1;

  FILE *fp = fopen("accounts.dat", "rb");
  if (fp) {
    fread(data, sizeof(struct account), MAX, fp);
    fclose(fp);
  }
  curr = 0;
  for (int i = 0; i < MAX; ++i) {
    if (data[i].username[0] != '\0') {
      curr = i + 1;
    } else {
      break;
    }
  }
  int done = 0;
  while (!done) {
    update_loan_interest_amount(data);
    print_menu();
    // scanf("%lld", &query);
    query = input_num(2, 1);
    // clear_stdin();
    // printf("%d\n", query);
    switch (query) {
    case 1:
      new_acc(data, &curr);
      break;

    case 2:
      change_pin(data);
      break;

    case 3:
      print_balance(data);
      break;

    case 4:
      deposit(data);
      break;

    case 5:
      withdraw(data);
      break;

    case 6:
      print_history(data);
      break;

    case 7:
      make_transaction(data);
      break;

    case 8:
      loan(data);
      break;

    case 9:
      create_fd(data);
      break;

    case 10:
      liquidate_fd(data);
      break;

    case 11:
      int c = check_admin();
      if (c == 0) {
        printf("Invalid password!!\n");
        query = -1;
        break;
      }
      admin(data);
      break;
    case 12:
      withdraw_from_credit_card(data);
      break;
    case 13:
      done = 1;
      break;

    default:
      printf("Enter a valid option\n\n");
      break;
    }
    query = -1;
  }
  SAVE(data);
}
