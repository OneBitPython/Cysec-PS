#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>

int stdin_is_empty(void) {
    fd_set set;
    struct timeval timeout = {0, 0}; // non-blocking

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    return select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) <= 0;
}

void clear_stdin() {
    int c;
    if(stdin_is_empty()){
        printf("empty\n");
        return;
    }
    while (1) {
        c= getchar();
        if(c=='\n')break;
    }
}
void printstream(){
      int c;
    while (1) {
        c= getchar();
        putchar(c);
        if(c=='\n')break;
    }

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
        if(toinput[i] == '\n')break;
        return -1;
      }
    }
  }
  return to_return;
}


int main(){
    long long int query = input_num(1,0);

    clear_stdin();
    printf("%d\n", query);

      printf("Enter your username: ");

  char* user = (char*)malloc(sizeof(char) * 21);

  fgets(user, 21, stdin);
//   clear_stdin();

}