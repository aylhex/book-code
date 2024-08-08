//
// Created by AYL on 2022/11/24.
//
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
    if( strcmp(argv[1], "test") )
    {
        printf("Incorrect password\n");
    }
    else
    {
        printf("Correct password\n");
    }
    return 0;
}
