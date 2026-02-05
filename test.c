#include <stdio.h>
#include <string.h>

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

long long int input_num(int len)
{
  char toinput[len+1];
  toinput[len] = '\0';
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

int main()
{
    long long int q;
    q = input_num(1);
    printf("%d\n", q);
    clear_stdin();
    char str[21];
    fgets(str, 21, stdin);
    clear_stdin_str(str);
    printf("%s\n", str);
}