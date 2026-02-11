#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  // printf("Key Expansion\n");
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
    for (int j = 0 ; j < 4 ; j++)
    {
      state[i][j] = arr[j];
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

int main()
{
  char *str = (char *)malloc(sizeof(char) * 17);
  char *key = (char *)malloc(sizeof(char) * 33);

  key = "2b7e151628aed2a6abf7158809cf4f3c";

  // printf("Enter the string: ");
  // fgets(str, 17, stdin);
  // clear_stdin();

  int expanded[44][4];

  key_expansion(key, expanded);
  // for (int i = 0 ; i < 44 ; i++)
  // {
  //   if (i % 4 == 0)
  //   {
  //     printf("\n");
  //   }
  //   for (int j = 0 ; j < 4 ; j++)
  //   {
  //     printf("%02x", expanded[i][j]);
  //   }
  // }

  printf("Key: %s\n", key);
  printf("Enter the string: ");
  fgets(str, 17, stdin);
  clear_stdin_str(str);

  int state[4][4];
  make_state(state, str);
  // printf("State is made\n");
  // printf("%d", state[0][0]);

  for (int round = 0 ; round <= 10 ; round++)
  {
    // printf("%d", round);
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

  printf("Ciphertext: ");
  for (int i = 0 ; i < 4 ; i++)
  {
    for (int j = 0 ; j < 4 ; j++)
    {
      printf("%02x", state[j][i]);
    }
  }
  printf("\n");
}