#include <stdio.h>

void clear_stdin()
{
  // printf("clear_stdin\n");
  int c;
  while((c = getchar()) != '\n');
}

int hex(char c)
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

void key_expansion(char *key, int expanded[44][4])
{
  int k[4][4];
  int pos = 0;
  for (int i = 0 ; i < 4 ; i++)
  {
    k[i][j] = 16*hex(key[pos]) + hex(key[pos+1]);
    pos += 2;
  }
}

int main()
{
  char *str = (char *)malloc(sizeof(char) * 17);
  char *key = (char *)malloc(sizeof(char) * 33);

  printf("Enter the key: ");
  fgets(key, 33, stdin);
  clear_stdin();

  printf("Enter the string: ");
  fgets(str, 17, stdin);
  clear_stdin();

  int expanded[44][4];

  int round;
  for (round = 0 ; round <= 10 ; round++)
  {
    if (round == 0)
    {
      //AddRoundKey
    }
    else if (round == 10)
    {
      //SubBytes

      //ShiftRows

      //AddRoundKey
    }
    else
    {
      //SubBytes

      //ShiftRows

      //MixColumns

      //AddRoundKey
    }
  }
}