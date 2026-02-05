#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
char ad[5] = "ADMIN";
struct account
{
  char *username;
  char *pan;
  char *cvv;
  long long pin;
  long long int balance;
  long long int loan_amt;
  long long int fd;
  long long int interest_fd;
};

void print_menu(){
  printf("Welcome to the Interface. Please Enter the number of the operation you want to perform\n");
  printf("1) Create a New Account\n");
  printf("2) Change PIN of an Existing Account\n");
  printf("3) Get Balance of an Account\n");
  printf("4) Withdraw Money from an Account\n");
  printf("5) Get a Loan from the Admin\n");
  printf("6) Create an FD\n");
  printf("7) Liquidate an FD\n");
}

long long int input_num(int len)
{
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
      return -1;
    }
  }
  return to_return;
}

void new_acc(struct account data[], int curr)
{
  data[curr].username = (char *)malloc(sizeof(char)*21);
  printf("Enter your username: ");
  fgets(data[curr].username, 20, stdin);
  char buf;
  fgets(&buf, 1, stdin);
  if (strncmp(data[curr].username, ad, 5) == 0)
  {
    printf("Invalid Username\n");
    return;
  }

  printf("Enter the pin you want to set(8 digits): ");
  long long int temp_pin = input_num(8);
  if (temp_pin == -1)
  {
    printf("Enter valid pin\n");
    new_acc(data, curr);
  }
  else
  {
    data[curr].pin = temp_pin;
  }
}

int main()
{
  struct account data[MAX];
  int curr = 0;
  
  long long int query = -1;
  while (true)
  {
    print_menu(); 
    query = input_num(1);
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
      // withdraw(data);
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
      printf("Enter a valid option\n");
      print_menu();
    }
  }
}