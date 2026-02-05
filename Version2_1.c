#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
long long int curr;
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
struct account
{
  char *username;
  char *pan;
  char *cvv;
  char *pin;
  long long int balance;
  long long int loan_amt;
  long long int fd;
  long long int interest_fd;
};

void print_menu(){
  printf("--------------------------------------------------------------------------------------\n");
  printf("Welcome to the Interface. Please Enter the number of the operation you want to perform\n");
  printf("1) Create a New Account\n");
  printf("2) Change PIN of an Existing Account\n");
  printf("3) Get Balance of an Account\n");
  printf("4) Deposit money into account\n");
  printf("5) Withdraw Money from an Account\n");
  printf("6) Get a Loan from the Admin\n");
  printf("7) Create an FD\n");
  printf("8) Liquidate an FD\n");
}

long long int input_num(int len, int mode)
{
  //mode = 0 means it has to be exactly 'len' length, otherwise it can be anything less than 'len'
  char toinput[len+1];
  long long int to_return = 0;
  for (int i = 0 ; i < len ; i++)
  {
    fgets(toinput+i, 2, stdin);

    if (toinput[i] >= '0' && toinput[i] <= '9')
    {
      to_return *= 10;
      to_return += (long long)(toinput[i] - '0');
    }
    else
    {
      if(mode == 0)return -1;
      else{
        if(toinput[i] == '\n'){
          break;
        }
        return -1;
      }
    }
  }
  return to_return;
}

void input_hex(char *str, int len, int mode)
{
  char toinput[len+1];
  for (int i = 0 ; i < len ; i++)
  {
    fgets(toinput+i, 2, stdin);

    if ((toinput[i] >= '0' && toinput[i] <= '9') || (toinput[i] >= 'a' && toinput[i] <= 'f'))
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
          return;
        }
        str[0] = 'z';
        return;
      }
      else
      {
        if (toinput[i] == '\n')
        {
          str[i] = '\n';
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
}

void new_acc(struct account data[], int curr)
{
  char c;
  data[curr].username = (char *)malloc(sizeof(char)*21);
  printf("Enter your username: ");
  fgets(data[curr].username, 21, stdin);
  clear_stdin_str(data[curr].username);

  if (strncmp(data[curr].username, ad, 5) == 0)
  {
    printf("Invalid Username\n");
    return;
  }
  // printf("Username: %s", data[curr].username);

  while(1){
    printf("Enter the pin you want to set(First 6 digits will be taken): ");
    data[curr].pin = (char *)malloc(sizeof(char)*7);
    input_hex(data[curr].pin, 6, 0);
    clear_stdin_str(data[curr].pin);
    if ((data[curr].pin)[0] == 'z')
    {
      printf("Enter valid pin\n\n");
    }
    else
    {
      printf("Successfully set PIN to %s\n\n\n", data[curr].pin);
      break;
    }
  }
}

void deposit(struct account data[])
{
  printf("Enter your username: ");
  char *user = (char *)malloc(sizeof(char) * 21);
  fgets(user, 21, stdin);
  clear_stdin_str(user);

  int pos = -1;
  for (int i = 0 ; i < curr ; i++)
  {
    if (strncmp(data[i].username, user, 21) == 0)
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

  while(1)
  {
    printf("Enter the PIN: ");
    char *temp_pin;
    temp_pin = (char *)malloc(sizeof(char) * 7);
    input_hex(temp_pin, 6, 0);
    clear_stdin_str(temp_pin);
    if (temp_pin[0] == 'z' || strncmp(data[pos].pin, temp_pin, 6) != 0)
    {
      printf("PIN doesn't match, ");
    }
    else
    {
      printf("Successfully logged in to account\n\n");
      printf("Enter the amount you want to deposit: ");
      char *deposit;
      deposit = (char *)malloc(sizeof(char) * 11);
      input_hex(deposit, 10, 1);
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
          temp *= 10;
          temp += deposit[i] - '0';
        }
        data[pos].balance += temp;
        printf("Your balance now is %lld\n\n\n", data[pos].balance);
        break;
      }
    }
  }
}

int main()
{
  struct account data[MAX];
  curr = 0;

  long long int query = -1;
  while (1)
  {
    print_menu();
    //scanf("%lld", &query);
    query = input_num(1,0);
    clear_stdin();
    // printf("%d\n", query);
    switch(query)
    {
      case 1:
      new_acc(data, curr);
      curr++;
      break;

      case 2:
      // change_pin(data);
      break;

      case 3:
      // print_balance(data);
      break;

      case 4:
      deposit(data);
      break;

      case 5:
      // get_loan(data);
      break;

      case 6:
      // create_fd(data);
      break;

      case 7:
      // liquidate_fd(data);
      break;

      default:
      printf("Enter a valid option\n\n");
    }
    query = -1;
  }
}