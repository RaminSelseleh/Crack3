#include <stdio.h>
#include <string.h>
#include "md5.h"

int main()
{
    char pass[100];
    while (fgets(pass, 100, stdin) != NULL)
    {
        pass[strlen(pass)-1] = '\0';
        char *p = md5(pass, strlen(pass));
        printf("%s\n", p);
    }
}
