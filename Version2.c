```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
long long int curr;
char ad[5] = "ADMIN";

void clear_stdin()
{
    int c;
    while((c = getchar()) != '\n');
}

void clear_stdin_str(char *str)
{
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
  long long pin;
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
  printf("Username: %s", data[curr].username);

  while(1){
    printf("Enter the pin you want to set(8 digits): ");
    long long int temp_pin = input_num(8,0);
    if (temp_pin == -1)
    {
      printf("Enter valid pin\n");
      continue;
    }
    else
    {
      printf("Successfully set PIN to %lld\n", temp_pin);
      clear_stdin();
      data[curr].pin = temp_pin;
      data[curr].balance = 0;
      break;
    }
    clear_stdin();
  }
}

void deposit(struct account data[]){
  printf("Enter your username: ");

  char* user = (char*)malloc(sizeof(char) * 21);

  fgets(user, 21, stdin);
  clear_stdin_str(user);

  int pos = -1;
  printf("came here\n");
  for(int i = 0;i<curr;++i){
    if(strncmp(data[i].username, user, 21) == 0){
      pos = i;
      break;
    }
  }
  printf("%d\n", pos);
  if(pos == -1){
    printf("No such user\n");
    return;
  }
  long long int pin;
  while(1){
    printf("Enter pin: ");
    pin = input_num(8,0);
    clear_stdin();
    if(pin != -1)break;
  }

  if(data[pos].pin == pin){
    printf("Successfull logged in to account\n");
    printf("Enter the amount you want to deposit: ");
    long long int deposit;
    deposit = input_num(10,1);
    printf("came here\n");
    clear_stdin();
    printf("%lld\n", deposit);

    if(deposit==-1){
      printf("Invalid number entered\n");
    }else{
      data[pos].balance += deposit;
      printf("Your remaining ballance is %lld\n", data[pos].balance);
    }

  }else{
    printf("Invalid PIN\n");
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
}```