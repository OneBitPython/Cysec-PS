#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
#define USER_MAX 32
#define MAXT 50

int curr;
char ad[5] = "ADMIN";

void clear_stdin()
{
  // printf("clear_stdin\n");
  int c;
  while((c = getchar()) != '\n');
}

void clear_stdin_str(char *str)
{
  // printf("clear_stdin_str\n");
    if (str[strlen(str)-1] != '\n')
    {
        // printf("Clearing stdin\n");
        clear_stdin();
    }
    else
    {
        str[strlen(str)-1] = '\0';
    }
}
// withdraw, deposit
struct transact{
  // stores withdraw, deposit information
  int type, amount;
  char time[32];
  char person[USER_MAX+1];
};
struct account
{
  char username[USER_MAX+1];
  char pan[17];
  char cvv[4];
  char pin[33];
  int balance;
  int loan_amt;
  int fd;
  int interest_fd;
  struct transact info[MAXT];
  int active;
};

void give_time(char buf[]){
  time_t now = time(NULL);

  struct tm *tm = localtime(&now);
  strftime(buf, 32, "%d/%m/%Y %H:%M:%S", tm);
}

void print_menu(){
  printf("--------------------------------------------------------------------------------------\n");
  printf("Welcome to the Interface. Please Enter the number of the operation you want to perform\n");
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
  printf("12) Exit\n");
  printf("Query: ");
}

//AES functions - start

int s_box[256] = {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};

int h(char c)
{
    if (c == '0')
    {
        return 0;
    }
    else if (c == '1')
    {
        return 1;
    }
    else if (c == '2')
    {
        return 2;
    }
    else if (c == '3')
    {
        return 3;
    }
    else if (c == '4')
    {
        return 4;
    }
    else if (c == '5')
    {
        return 5;
    }
    else if (c == '6')
    {
        return 6;
    }
    else if (c == '7')
    {
        return 7;
    }
    else if (c == '8')
    {
        return 8;
    }
    else if (c == '9')
    {
        return 9;
    }
    else if (c == 'a')
    {
        return 10;
    }
    else if (c == 'b')
    {
        return 11;
    }
    else if (c == 'c')
    {
        return 12;
    }
    else if (c == 'd')
    {
        return 13;
    }
    else if (c == 'e')
    {
        return 14;
    }
    else
    {
        return 15;
    }
}

int hex_to_str(int n)
{
    if (n == 0)
    {
        return '0';
    }
    else if (n == 1)
    {
        return '1';
    }
    else if (n == 2)
    {
        return '2';
    }
    else if (n == 3)
    {
        return '3';
    }
    else if (n == 4)
    {
        return '4';
    }
    else if (n == 5)
    {
        return '5';
    }
    else if (n == 6)
    {
        return '6';
    }
    else if (n == 7)
    {
        return '7';
    }
    else if (n == 8)
    {
        return '8';
    }
    else if (n == 9)
    {
        return '9';
    }
    else if (n == 10)
    {
        return 'a';
    }
    else if (n == 11)
    {
        return 'b';
    }
    else if (n == 12)
    {
        return 'c';
    }
    else if (n == 13)
    {
        return 'd';
    }
    else if (n == 14)
    {
        return 'e';
    }
    else if (n == 15)
    {
        return 'f';
    }
}

int asc(char c)
{
  // printf("asc\n");
  int t = c;
  return t;
}

void make_state(int state[4][4], char *str)
{
  // printf("Make state\n%s\n", str);
  for (int i = 0 ; i < 4 ; i++)
  {
    for (int j = 0 ; j < 4 ; j++)
    {
      // printf("%d%d ", i, j);
      if ((4*i + j) < strlen(str))
      {
        state[j][i] = asc(str[4*i + j]);
      }
      else
      {
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

void key_expansion(char *key, int expanded[44][4])
{
//   printf("Key Expansion\n");
  int k[4][4];
  int pos = 0;
  for (int i = 0 ; i < 4 ; i++)
  {
    for (int j = 0 ; j < 4 ; j++)
    {
      k[i][j] = 16*h(key[pos]) + h(key[pos+1]);
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

  for (int i = 0 ; i < 44 ; i++)
  {
    if (i < 4)
    {
      for (int j = 0 ; j < 4 ; j++)
      {
        temp[i][j] = k[i][j];
      }
    }

    else if (i % 4 == 0 && i > 0)
    {
      int rcon[4] = {rc[(i/4)-1], 0, 0, 0};
      for (int j = 0 ; j < 4 ; j++)
      {
        temp[i][j] = temp[i-4][j] ^ rcon[j] ^ s_box[temp[i-1][(j+1)%4]];
      }
    }

    else
    {
      for (int j = 0 ; j < 4 ; j++)
      {
        temp[i][j] = temp[i-4][j] ^ temp[i-1][j];
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
  while (round <= 10)
  {
    for (int i = 0 ; i < 4 ; i++)
    {
      for (int j = 0 ; j < 4 ; j++)
      {
        expanded[4*round + j][i] = temp[4*round + i][j];
      }
    }
    round++;
  }
}

void addRoundKey(int state[4][4], int expanded[44][4], int round)
{
  // printf("Add round key\n");
  for (int i = 0 ; i < 4 ; i++)
  {
    for (int j = 0 ; j < 4 ; j++)
    {
      state[i][j] = state[i][j] ^ expanded[4*round + i][j];
    }
  }
}

void subBytes(int state[4][4])
{
  // printf("Sub Bytes\n");
  for (int i = 0 ; i < 4 ; i++)
  {
    for (int j = 0 ; j < 4 ; j++)
    {
      state[i][j] = s_box[state[i][j]];
    }
  }
  // printf("Thefo\n");
}

void shiftRows(int state[4][4])
{
  // printf("Shift Rows\n");
  for (int i = 0 ; i < 4 ; i++)
  {
    int arr[4];
    for (int j = 0 ; j < 4 ; j++)
    {
      arr[j] = state[i][(j+i)%4];
    }
  }
}

void mixColumns(int state[4][4])
{
  // printf("Mix Columns\n");
  for (int i = 0 ; i < 4 ; i++)
  {
    int arr[4];
    arr[0] = 2*state[0][i] + 3*state[1][i] + 1*state[2][i] + 1*state[3][i];
    arr[1] = 1*state[0][i] + 2*state[1][i] + 3*state[2][i] + 1*state[3][i];
    arr[2] = 1*state[0][i] + 1*state[1][i] + 2*state[2][i] + 3*state[3][i];
    arr[3] = 3*state[0][i] + 1*state[1][i] + 1*state[2][i] + 2*state[3][i];

    state[0][i] = arr[0] % 256;
    state[1][i] = arr[1] % 256;
    state[2][i] = arr[2] % 256;
    state[3][i] = arr[3] % 256;
  }
}

void encrypt(char *str, char *enc)
{
    char *key = (char *)malloc(sizeof(char) * 33);
    key = "2b7e151628aed2a6abf7158809cf4f3c";
    
    int expanded[44][4];
    key_expansion(key, expanded);

    int state[4][4];
    make_state(state, str);

    for (int round = 0 ; round <= 10 ; round++)
    {
        if (round == 0)
        {
            //AddRoundKey
            addRoundKey(state, expanded, round);
        }
        else if (round == 10)
        {
            //SubBytes
            subBytes(state);

            //ShiftRows
            shiftRows(state);

            //AddRoundKey
            addRoundKey(state, expanded, round);
        }
        else
        {
            //SubBytes
            subBytes(state);

            //ShiftRows
            shiftRows(state);

            //MixColumns
            mixColumns(state);

            //AddRoundKey
            addRoundKey(state, expanded, round);
        }
    }

    int pos = 0;
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int j = 0 ; j < 4 ; j++)
        {
            enc[pos] = hex_to_str(state[j][i] / 16);
            enc[pos+1] = hex_to_str(state[j][i] % 16);
            pos += 2;
        }
    }
}

//AES functions - end
void SAVE(struct account data[]){
  FILE* fp2 = fopen("accounts.dat", "wb");
  fwrite(data, sizeof(struct account), MAX, fp2);
  fclose(fp2);
}

int input_num(int len, int mode)
{
  //mode = 0 means it has to be exactly 'len' length, otherwise it can be anything less than equal to 'len'
  char toinput[len+1];
  toinput[len] = '\0';
  int to_return = 0;
  for (int i = 0 ; i < len ; i++)
  {
    fgets(toinput+i, 2, stdin);

    if (toinput[i] >= '0' && toinput[i] <= '9')
    {
      to_return *= 10;
      to_return += toinput[i] - '0';
    }
    else
    {
      if(mode == 0)
      {
        clear_stdin_str(toinput);
        return -1;
      }
      else{
        if(toinput[i] == '\n'){
          clear_stdin_str(toinput);
          return to_return;
        }
        return -1;
      }
    }
  }
  clear_stdin();
  return to_return;
}

void printstream(){
  while(1){
    int c = getchar();
    putchar(c);
    if(c=='\n')break;
  }
}

void input(char *str, int len, int mode, int allowhex)
{
  char toinput[len+1];
  for (int i = 0 ; i < len ; i++)
  {
    fgets(toinput+i, 2, stdin);

    if ((toinput[i] >= '0' && toinput[i] <= '9') || (allowhex && (toinput[i] >= 'a' && toinput[i] <= 'f')))
    {
      str[i] = toinput[i];
    }
    else
    {
      if (mode == 0)
      {
        if (toinput[i] == '\n')
        {
          str[i] = '\n';
          str[i+1] = '\0';
        }
        str[0] = 'z';
        return;
      }
      else
      {
        if (toinput[i] == '\n')
        {
          str[i] = '\n';
          str[i+1] = '\0';
          return;
        }
        else
        {
          str[0] = 'z';
          return;
        }
      }
    }
  }
  str[len] = '\0';
}

void new_acc(struct account data[], int *cur)
{
  int curr = *cur;
  char c;
  char temp_username[USER_MAX+2];
  printf("Enter your username: ");
  fgets(temp_username, (USER_MAX+1), stdin);
  clear_stdin_str(temp_username);

  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(temp_username, data[i].username, strlen(temp_username)) == 0 && strlen(temp_username) == strlen(data[i].username))
    {
        printf("User already exists\n\n\n");
        return;
    }
  }
  
  strncpy(data[curr].username, temp_username, strlen(temp_username));
  if (strncmp(data[curr].username, ad, 5) == 0)
  {
    printf("Invalid Username\n");
    return;
  }
  // printf("Username: %s", data[curr].username);

  while(1){
    printf("Enter the pin you want to set(First 6 digits will be taken): ");
    char *temp_pin;
    temp_pin = (char *)malloc(sizeof(char)*8);
    data[curr].active = 0;
    input(temp_pin, 6, 0,1);
    clear_stdin_str(temp_pin);

    if ((temp_pin)[0] == 'z')
    {
      printf("Enter valid pin\n\n");
    }
    else
    {
      printf("Successfully set PIN to %s\n\n\n", temp_pin);
      (*cur)++;
      encrypt(temp_pin, data[curr].pin);
      SAVE(data);
      break;
    }
  }
//   printf("Exiting new_acc\n");
}


void slide(struct transact info[]){
  for(int i = 0;i<MAXT;++i)info[i] = info[i+1];
}

void withdraw(struct account data[]){
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }
  int times = 0;
  while(times <= 3)
  {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0,1);
    clear_stdin_str(temp_pin);
    char *enc = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc);
    if (temp_pin[0] == 'z')
    {
        if (times != 3)
        {
            printf("Invalid PIN, ");
        }
        else
        {
            printf("3 attempts exhausted\n\n\n");
        }
    }
    else if (strncmp(data[pos].pin, enc, 32) != 0)
    {
        if (times != 3)
        {
            printf("PIN doesn't match, ");
        }
        else
        {
            printf("3 attempts exhausted\n\n\n");
        }
    }
    else
    {
      printf("Successfully logged in to account\n\n");
      printf("Current balance is %lld. Enter the amount you want to withdraw: ", data[pos].balance);
      char withdraw[11];

      input(withdraw, 9, 1,0);
      clear_stdin_str(withdraw);

      if (withdraw[0] == 'z')
      {
        printf("Invalid number entered\n\n");
      }
      else
      {
        int temp = 0;
        for (int i = 0 ; i < strlen(withdraw) ; i++)
        {
          temp *= 10;
          temp += withdraw[i] - '0';
        }
        if(data[pos].balance < temp){
          printf("Insufficient funds.\n\n\n");
          break;
        }
        data[pos].balance -= temp;
        printf("Your balance now is %lld\n\n\n", data[pos].balance);

        struct transact new_trans;
        new_trans.type = 0;
        new_trans.amount = temp;
        give_time(new_trans.time);

        if(data[pos].active==MAXT)slide(data[pos].info), data[pos].active--;

        data[pos].info[data[pos].active] = new_trans;
        if(data[pos].active < MAXT)data[pos].active++;

        SAVE(data);

        break;
      }
    }
  }
}
void liquidate_fd(struct account data[]){
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }
  int times = 0;
  while(times < 3)
  {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0,1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z')
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
      printf("Invalid PIN, ");
    }
    else if (strncmp(data[pos].pin, enc_pin, 32) != 0)
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
        printf("PIN doesn't match, ");
    }
    else
    {
      printf("Successfully logged in to account\n\n");
      printf("Current balance is %lld. ", data[pos].balance);

      data[pos].balance += data[pos].fd;
      data[pos].fd = 0;
      printf("Your balance now is %lld\n\n\n", data[pos].balance);
      SAVE(data);
      break;
    }
  }
}

void create_fd(struct account data[]){
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }
  int times = 0;
  while(times < 3)
  {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0,1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z')
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
      printf("Invalid PIN, ");
    }
    else if (strncmp(data[pos].pin, enc_pin, 32) != 0)
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
        printf("PIN doesn't match, ");
    }
    else
    {
      printf("Successfully logged in to account\n\n");
      printf("Current balance is %lld. Enter the amount you want to add to FD : ", data[pos].balance);
      char withdraw[11];

      input(withdraw, 9, 1,0);
      clear_stdin_str(withdraw);

      if (withdraw[0] == 'z')
      {
        printf("Invalid number entered\n\n");
      }
      else
      {
        int temp = 0;
        for (int i = 0 ; i < strlen(withdraw) ; i++)
        {
          temp *= 10;
          temp += withdraw[i] - '0';
        }
        if(data[pos].balance < temp){
          printf("Insufficient funds.\n\n\n");
          break;
        }
        data[pos].balance -= temp;
        printf("Your balance now is %lld\n\n\n", data[pos].balance);

        data[pos].fd += temp;
        SAVE(data);

        break;
      }
    }
  }
}
void change_pin(struct account data[])
{
  char temp_user[USER_MAX+2];
  printf("Enter the username: ");
  fgets(temp_user, USER_MAX+1, stdin);
  clear_stdin_str(temp_user);
  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, temp_user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("User doesn't exist\n\n\n");
    return;
  }

  printf("Enter the existing PIN: ");
  char temp_pin[8];
  input(temp_pin, 6, 0,1);
  clear_stdin_str(temp_pin);
  char *enc_pin = (char *)malloc(sizeof(char) * 33);
  encrypt(temp_pin, enc_pin);
  if (temp_pin[0] == 'z')
  {
    printf("Invalid PIN entered. Exiting process\n\n\n");
    return;
  }
  else if (strncmp(data[pos].pin, enc_pin, 32) != 0)
  {
    printf("PIN doesn't match. Exiting process\n\n\n");
    return;
  }

  printf("Enter new PIN: ");
  char new_pin1[8];
  input(new_pin1, 6, 0,1);
  clear_stdin_str(new_pin1);
  if (new_pin1[0] == 'z')
  {
    printf("Invalid PIN entered. Exiting process\n\n\n");
    return;
  }
  else if (strncmp(new_pin1, temp_pin, 6) == 0)
  {
    printf("Can't change PIN to existing PIN. Exiting process \n\n\n");
    return;
  }
  printf("Enter the PIN again: ");
  char new_pin2[8];
  input(new_pin2, 6, 0,1);
  clear_stdin_str(new_pin2);
  if (new_pin2[0] == 'z')
  {
    printf("Invalid PIN entered. Exiting process\n\n\n");
    return;
  }
  else if (strncmp(new_pin2, new_pin1, 6) != 0)
  {
    printf("PINs don't match. Exiting process\n\n\n");
  }
  else
  {
    char *enc_pin_new =(char *)malloc(sizeof(char) * 33);
    encrypt(new_pin1, enc_pin_new);
    strncpy(data[pos].pin, enc_pin_new, 32);
    printf("PIN successfully changed to %s\n\n\n", new_pin1);
    SAVE(data);

    return;
  }
}

void make_transaction(struct account data[]){
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  int times = 0;
  while(times < 3)
  {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0,1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z')
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
      printf("Invalid PIN, ");
    }
    else if (strncmp(data[pos].pin, enc_pin, 32) != 0)
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
      printf("PIN doesn't match, ");
    }
    else
    {
      printf("Successfully logged in to account\n\n");

      int t2 = 0;
      while(t2 < 3){
        t2++;
        printf("Enter username you want to send money to : ");
        char user2[USER_MAX+2];
        fgets(user2, (USER_MAX+1), stdin);
        clear_stdin_str(user2);

        int pos2 = -1;
        for (int i = 0 ; i < curr ; i++)
        {
          if (strncmp(data[i].username, user2, USER_MAX) == 0)
          {
            pos2 = i;
            break;
          }
        }

        if(pos2==-1){
          printf("Invalid user.\n");
          continue;
        }
        if(pos == pos2){
          printf("Can't send money to yourself. \n");
          continue;
        }
        printf("Current balance is %lld. Enter the amount you want to send: ", data[pos].balance);
        char withdraw[11];

        input(withdraw, 9, 1,0);
        clear_stdin_str(withdraw);

        if (withdraw[0] == 'z')
        {
          printf("Invalid number entered\n\n");
        }
        else
        {
          int temp = 0;
          for (int i = 0 ; i < strlen(withdraw) ; i++)
          {
            temp *= 10;
            temp += withdraw[i] - '0';
          }
          if(data[pos].balance < temp){
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

          if(data[pos].active==MAXT)slide(data[pos].info), data[pos].active--;

          data[pos].info[data[pos].active] = new_trans;
          if(data[pos].active < MAXT)data[pos].active++;


          struct transact new_trans2;
          new_trans2.type = 3;
          new_trans2.amount = temp;
          give_time(new_trans2.time);
          
          strncpy(new_trans2.person, user, strlen(user));

          if(data[pos2].active==MAXT)slide(data[pos2].info), data[pos2].active--;

          data[pos2].info[data[pos2].active] = new_trans2;
          if(data[pos2].active < MAXT)data[pos2].active++;
          SAVE(data);

          break;
        }
        break;
      }
      break;
    }
  }
}

void print_history(struct account data[]){

  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  int times = 0;
  while(times <= 3)
  {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0,1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z')
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
      printf("Invalid PIN, ");
    }
    else if (strncmp(data[pos].pin, enc_pin, 32) != 0)
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
        printf("PIN doesn't match, ");
    }
    else
    {
      printf("Successfully logged in to account\n\n");
      if(data[pos].active==0){
        printf("No history\n\n");
        return;
      }
      for(int i = 0;i<data[pos].active;++i){
        printf("%d. ", i+1);
        if(data[pos].info[i].type == 1)printf("Deposit of %lld made on %s\n", data[pos].info[i].amount,data[pos].info[i].time);
        else if(data[pos].info[i].type == 0) printf("Withdrawal of %lld made on %s\n", data[pos].info[i].amount,data[pos].info[i].time);
        else if(data[pos].info[i].type==2) printf("Transaction of %lld made to %s on %s\n", data[pos].info[i].amount,data[pos].info[i].person, data[pos].info[i].time);
        else if(data[pos].info[i].type==3) printf("Transaction of %lld made from %s on %s\n", data[pos].info[i].amount,data[pos].info[i].person, data[pos].info[i].time);
      }
      break;
    }
  }
}
void deposit(struct account data[])
{
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  int times = 0;
  while(times <= 3)
  {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0,1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z')
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
        printf("Invalid PIN, ");
    }
    else if (strncmp(data[pos].pin, enc_pin, 32) != 0)
    {
        if (times == 3)
        {
            printf("3 attempts exhausted\n\n\n");
            return;
        }
        printf("PIN doesn't match, ");
    }
    else
    {
      printf("Successfully logged in to account\n\n");
      printf("Enter the amount you want to deposit: ");
      char deposit[11];
      input(deposit, 9, 1,0);
      clear_stdin_str(deposit);

      if (deposit[0] == 'z')
      {
        printf("Invalid number entered\n\n");
      }
      else
      {
        int temp = 0;

        for (int i = 0 ; i < strlen(deposit) ; i++)
        {
          if(deposit[i]=='\n')break;
          temp *= 10;
          temp += deposit[i] - '0';
        }
        data[pos].balance += temp;
        printf("Your balance now is %lld\n\n\n", data[pos].balance);

        struct transact new_trans;
        new_trans.type = 1;
        new_trans.amount = temp;
        give_time(new_trans.time);

        if(data[pos].active==MAXT)slide(data[pos].info),data[pos].active--;

        data[pos].info[data[pos].active] = new_trans;
        if(data[pos].active < MAXT)data[pos].active++;
        SAVE(data);

        break;
      }
    }
  }
}

void print_balance(struct account data[])
{
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  int times = 0;
  while(times < 3)
  {
    times++;
    printf("Enter the PIN: ");
    char temp_pin[8];
    input(temp_pin, 6, 0,1);
    clear_stdin_str(temp_pin);
    char *enc_pin = (char *)malloc(sizeof(char) * 33);
    encrypt(temp_pin, enc_pin);
    if (temp_pin[0] == 'z')
    {
      if (times != 3)
      {
        printf("Invalid PIN, ");
      }
      else
      {
        printf("3 attempts exhausted\n\n\n");
      }
    }
    else if (strncmp(data[pos].pin, enc_pin, 32) != 0)
    {
      if (times != 3)
      {
        printf("PIN doesn't match, ");
      }
      else
      {
        printf("3 attempts exhausted\n\n\n");
      }
    }
    else
    {
      printf("Successfully logged in to account\n");
      printf("Your balance now is %lld\n\n\n", data[pos].balance);
      break;
    }
  }
}

int check_admin()
{
    char *pass = (char *)malloc(sizeof(char) * 17);
    char *pass_enc = (char *)malloc(sizeof(char) * 33);
    printf("Enter the admin password(only the first 16 characters will be taken): ");
    fgets(pass, 17, stdin);
    clear_stdin_str(pass);
    encrypt(pass, pass_enc);

    //Password: xA9!Qf7$L2@RkZ#M
    char *admin_pass = (char *)malloc(sizeof(char) * 33);
    admin_pass = "42fc799587b75b4270097744d4b19255";
    if (strncmp(admin_pass, pass_enc, 32) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void print_menu_admin(){
  printf("--------------------------------------------------------------------------------------\n");
  printf("Welcome to the Interface. Please Enter the number of the operation you want to perform\n");
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
  printf("11) Exit admin\n");
  printf("Query: ");
}

void change_pin_admin(struct account data[])
{
  char temp_user[USER_MAX+2];
  printf("Enter the username: ");
  fgets(temp_user, USER_MAX+1, stdin);
  clear_stdin_str(temp_user);
  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, temp_user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("User doesn't exist\n\n\n");
    return;
  }
  
  printf("Enter new PIN: ");
  char new_pin1[8];
  input(new_pin1, 6, 0,1);
  clear_stdin_str(new_pin1);
  char *new_pin_enc = (char *)malloc(sizeof(char) * 33);
  encrypt(new_pin1, new_pin_enc);
  if (new_pin1[0] == 'z')
  {
    printf("Invalid PIN entered. Exiting process\n\n\n");
    return;
  }
  else if (strncmp(new_pin_enc, data[pos].pin, 6) == 0)
  {
    printf("Can't change PIN to existing PIN. Exiting process \n\n\n");
    return;
  }
  printf("Enter the PIN again: ");
  char new_pin2[8];
  input(new_pin2, 6, 0,1);
  clear_stdin_str(new_pin2);
  if (new_pin2[0] == 'z')
  {
    printf("Invalid PIN entered. Exiting process\n\n\n");
    return;
  }
  else if (strncmp(new_pin2, new_pin1, 6) != 0)
  {
    printf("PINs don't match. Exiting process\n\n\n");
  }
  else
  {
    char *enc_pin_new =(char *)malloc(sizeof(char) * 33);
    encrypt(new_pin1, enc_pin_new);
    strncpy(data[pos].pin, enc_pin_new, 32);
    printf("PIN successfully changed to %s\n\n\n", new_pin1);
    SAVE(data);

    return;
  }
}

void print_balance_admin(struct account data[])
{
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  printf("Balance of user is %d\n\n\n", data[pos].balance);
  return;
}

void deposit_admin(struct account data[])
{
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  printf("Enter the amount you want to deposit: ");
  char deposit[11];
  input(deposit, 9, 1,0);
  clear_stdin_str(deposit);
  if (deposit[0] == 'z')
  {
    printf("Invalid number entered\n\n");
  }
  else
  {
    int temp = 0;

    for (int i = 0 ; i < strlen(deposit) ; i++)
    {
      if(deposit[i]=='\n')break;
      temp *= 10;
      temp += deposit[i] - '0';
    }
    data[pos].balance += temp;
    printf("Your balance now is %lld\n\n\n", data[pos].balance);

    struct transact new_trans;
    new_trans.type = 1;
    new_trans.amount = temp;
    give_time(new_trans.time);

    if(data[pos].active==MAXT)slide(data[pos].info),data[pos].active--;
    data[pos].info[data[pos].active] = new_trans;
    if(data[pos].active < MAXT)data[pos].active++;
    SAVE(data);

    return;
  }
}

void withdraw_admin(struct account data[])
{
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  printf("Current balance is %lld. Enter the amount you want to withdraw: ", data[pos].balance);
      char withdraw[11];

      input(withdraw, 9, 1,0);
      clear_stdin_str(withdraw);

      if (withdraw[0] == 'z')
      {
        printf("Invalid number entered\n\n");
      }
      else
      {
        int temp = 0;
        for (int i = 0 ; i < strlen(withdraw) ; i++)
        {
          temp *= 10;
          temp += withdraw[i] - '0';
        }
        if(data[pos].balance < temp){
          printf("Insufficient funds.\n\n\n");
          return;
        }
        data[pos].balance -= temp;
        printf("Your balance now is %lld\n\n\n", data[pos].balance);

        struct transact new_trans;
        new_trans.type = 0;
        new_trans.amount = temp;
        give_time(new_trans.time);

        if(data[pos].active==MAXT)slide(data[pos].info), data[pos].active--;

        data[pos].info[data[pos].active] = new_trans;
        if(data[pos].active < MAXT)data[pos].active++;
        SAVE(data);

        return;
      }
    }

void print_history_admin(struct account data[])
{
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  printf("Successfully logged in to account\n\n");
  if(data[pos].active==0){
    printf("No history\n\n");
    return;
  }
  for(int i = 0;i<data[pos].active;++i){
    printf("%d. ", i+1);
    if(data[pos].info[i].type == 1)printf("Deposit of %lld made on %s\n", data[pos].info[i].amount,data[pos].info[i].time);
    else if(data[pos].info[i].type == 0) printf("Withdrawal of %lld made on %s\n", data[pos].info[i].amount,data[pos].info[i].time);
    else if(data[pos].info[i].type==2) printf("Transaction of %lld made to %s on %s\n", data[pos].info[i].amount,data[pos].info[i].person, data[pos].info[i].time);
    else if(data[pos].info[i].type==3) printf("Transaction of %lld made from %s on %s\n", data[pos].info[i].amount,data[pos].info[i].person, data[pos].info[i].time);
  }
}

void make_transaction_admin(struct account data[])
{
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  int t2 = 0;
  while(t2 < 3){
    t2++;
    printf("Enter username you want to send money to : ");
    char user2[USER_MAX+2];
    fgets(user2, (USER_MAX+1), stdin);
    clear_stdin_str(user2);

    int pos2 = -1;
    for (int i = 0 ; i < curr ; i++)
    {
      if (strncmp(data[i].username, user2, USER_MAX) == 0)
      {
        pos2 = i;
        break;
      }
    }

    if(pos2==-1){
      printf("Invalid user.\n");
      continue;
    }
    if(pos == pos2){
      printf("Can't send money to yourself. \n");
      continue;
    }
    printf("Current balance is %lld. Enter the amount you want to send: ", data[pos].balance);
    char withdraw[11];

    input(withdraw, 9, 1,0);
    clear_stdin_str(withdraw);

    if (withdraw[0] == 'z')
    {
      printf("Invalid number entered\n\n");
    }
    else
    {
      int temp = 0;
      for (int i = 0 ; i < strlen(withdraw) ; i++)
      {
        temp *= 10;
        temp += withdraw[i] - '0';
      }
      if(data[pos].balance < temp){
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
      if(data[pos].active==MAXT)slide(data[pos].info), data[pos].active--;

      data[pos].info[data[pos].active] = new_trans;
      if(data[pos].active < MAXT)data[pos].active++;


      struct transact new_trans2;
      new_trans2.type = 3;
      new_trans2.amount = temp;
      give_time(new_trans2.time);
          
      strncpy(new_trans2.person, user, strlen(user));
      if(data[pos2].active==MAXT)slide(data[pos2].info), data[pos2].active--;

      data[pos2].info[data[pos2].active] = new_trans2;
      if(data[pos2].active < MAXT)data[pos2].active++;
      SAVE(data);

      break;
    }
    break;
}
}

void create_fd_admin(struct account data[])
{
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  printf("Current balance is %lld. Enter the amount you want to add to FD : ", data[pos].balance);
  char withdraw[11];

  input(withdraw, 9, 1,0);
  clear_stdin_str(withdraw);

  if (withdraw[0] == 'z')
  {
    printf("Invalid number entered\n\n");
  }
  else
  {
    int temp = 0;
    for (int i = 0 ; i < strlen(withdraw) ; i++)
    {
      temp *= 10;
      temp += withdraw[i] - '0';
    }
    if(data[pos].balance < temp){
      printf("Insufficient funds.\n\n\n");
      return;
    }
    data[pos].balance -= temp;
    printf("Your balance now is %lld\n\n\n", data[pos].balance);

    data[pos].fd += temp;
    SAVE(data);

    return;
  }
}

void liquidate_fd_admin(struct account data[])
{
  printf("Enter your username: ");
  char user[USER_MAX+2];
  fgets(user, (USER_MAX+1), stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, USER_MAX) == 0)
    {
      pos = i;
      break;
    }
  }

  if (pos == -1)
  {
    printf("No such user\n\n\n");
    return;
  }

  printf("Current balance is %lld. ", data[pos].balance);

  data[pos].balance += data[pos].fd;
  data[pos].fd = 0;
  printf("Your balance now is %lld\n\n\n", data[pos].balance);
}

void admin(struct account data[])
{
    printf("\nLogging in as ADMIN\n\n");
    
    int query = -1;
    // clear_stdin();
    // printf("%d\n", query);
    while(1)
    {
        print_menu_admin();
        query = input_num(2,1);
        switch(query)
        {
        case 1:
        new_acc(data, &curr);
        break;

        case 2:
        change_pin_admin(data);
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

int main()
{
  struct account data[MAX];
  int query = -1;

  FILE *fp = fopen("accounts.dat", "rb");
  if (fp) {
      fread(data, sizeof(struct account), MAX, fp);
      fclose(fp);
  }
  curr = 0;
  for(int i = 0;i<MAX;++i){
    if(data[i].username[0] != '\0'){
      curr = i+1;
    }else{
      break;
    }
  }
  int done = 0;
  while (!done)
  {
    print_menu();
    //scanf("%lld", &query);
    query = input_num(2,1);
    // clear_stdin();
    // printf("%d\n", query);
    switch(query)
    {
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
      break;

      case 9:
      create_fd(data);
      break;

      case 10:
      liquidate_fd(data);
      break;

      case 11:
      int c = check_admin();
      if (c == 0)
      {
        printf("Invalid password!!\n");
        query = -1;
        break;
      }
      admin(data);
      break;
      
      case 12:
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