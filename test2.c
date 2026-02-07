#include <stdio.h>
#include <string.h>

void clear_stdin()
{
    printf("clear_stdin\n");
  // printf("clear_stdin\n");
  int c;
  while((c = getchar()) != '\n');
}

void clear_stdin_str(char *str)
{
    printf("clear_stdin_str\n");
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

void print_stream()
{
    int c;
    while(1)
    {
        if ((c = getchar()) == '\n')
        {
            printf("New line\n");
            return;
        }
        putchar(c);
    }
}

int main()
{
    int query;
    query = input_num(2, 1);
    printf("%d\n", query);
    // char s[2];
    // s[1] = '\0';
    // printf("Enter string: ");
    // fgets(s, 2,stdin);
    // printf("%s\n", s);
    // print_stream();
}