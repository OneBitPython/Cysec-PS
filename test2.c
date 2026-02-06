#include <stdio.h>

void fun(int state[2][2])
{
    state[0][0] = 1;
}

int main()
{
    int state[2][2];
    fun(state);
    printf("%d", state[0][0]);
}