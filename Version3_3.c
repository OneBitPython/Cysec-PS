#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>

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
  char *person;
};
struct account
{
  char *username;
  char *pan;
  char *cvv;
  char *pin;
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
  
  data[curr].username = (char *)malloc(sizeof(char) * (USER_MAX+1));
  strncpy(data[curr].username, temp_username, strlen(temp_username));
  if (strncmp(data[curr].username, ad, 5) == 0)
  {
    printf("Invalid Username\n");
    return;
  }
  // printf("Username: %s", data[curr].username);

  while(1){
    printf("Enter the pin you want to set(First 6 digits will be taken): ");
    data[curr].pin = (char *)malloc(sizeof(char)*8);
    data[curr].active = 0;
    input(data[curr].pin, 6, 0,1);
    clear_stdin_str(data[curr].pin);

    if ((data[curr].pin)[0] == 'z')
    {
      printf("Enter valid pin\n\n");
    }
    else
    {
      printf("Successfully set PIN to %s\n\n\n", data[curr].pin);
      (*cur)++;
      break;
    }
  }
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
    if (temp_pin[0] == 'z' || strncmp(data[pos].pin, temp_pin, 6) != 0)
    {
      printf("PIN doesn't match. ");
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
    if (temp_pin[0] == 'z' || strncmp(data[pos].pin, temp_pin, 6) != 0)
    {
      if (times != 3)
      {
        printf("PIN doesn't match ");
      }
      else
      {
        printf("Exhausted 3 attempts\n\n\n");
        return;
      }
    }
    else
    {
      printf("Successfully logged in to account\n\n");
      printf("Current balance is %lld. ", data[pos].balance);

      data[pos].balance += data[pos].fd;
      data[pos].fd = 0;
      printf("Your balance now is %lld\n\n\n", data[pos].balance);

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
    if (temp_pin[0] == 'z' || strncmp(data[pos].pin, temp_pin, 6) != 0)
    {
      printf("PIN doesn't match. ");
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
  if (temp_pin[0] == 'z' || strncmp(temp_pin, data[pos].pin, 6) != 0)
  {
    printf("Invalid PIN entered. Exiting process\n\n\n");
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
    strncpy(data[pos].pin, new_pin1, 6);
    printf("PIN successfully changed to %s\n\n\n", data[pos].pin);
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
    if (temp_pin[0] == 'z' || strncmp(data[pos].pin, temp_pin, 6) != 0)
    {
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
          
          new_trans.person = (char *)malloc(sizeof(char) * (USER_MAX+1));
          strncpy(new_trans.person, user2, strlen(user2));

          if(data[pos].active==MAXT)slide(data[pos].info), data[pos].active--;

          data[pos].info[data[pos].active] = new_trans;
          if(data[pos].active < MAXT)data[pos].active++;


          struct transact new_trans2;
          new_trans2.type = 3;
          new_trans2.amount = temp;
          give_time(new_trans2.time);
          
          new_trans2.person = (char *)malloc(sizeof(char) * (USER_MAX+1));
          strncpy(new_trans2.person, user, strlen(user));

          if(data[pos2].active==MAXT)slide(data[pos2].info), data[pos2].active--;

          data[pos2].info[data[pos2].active] = new_trans2;
          if(data[pos2].active < MAXT)data[pos2].active++;
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
    if (temp_pin[0] == 'z' || strncmp(data[pos].pin, temp_pin, 6) != 0)
    {
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
    if (temp_pin[0] == 'z' || strncmp(data[pos].pin, temp_pin, 6) != 0)
    {
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
    if (temp_pin[0] == 'z' || strncmp(data[pos].pin, temp_pin, 6) != 0)
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
int main()
{
  struct account data[MAX];
  curr = 0;
  int query = -1;
  while (1)
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
      
      default:
      printf("Enter a valid option\n\n");
    }
    query = -1;
  }
}