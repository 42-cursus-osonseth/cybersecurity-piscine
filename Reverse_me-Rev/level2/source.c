#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main(void)
{
    size_t len;
    int atoi_ret;
    bool end_loop;
    char input[24];
    int n;
    char str_to_cmp[9];
    char buff[4];

    printf("Please enter key: ");
    n = scanf("%s", input);
    if (n != 1)
        return(printf("Nope.\n"), 1);
    if (input[1] != '0')
        return(printf("Nope.\n"), 1);
    if (input[0] != '0')
        return(printf("Nope.\n"), 1);
    memset(str_to_cmp, 0, 9);
    str_to_cmp[0] = 'd';
    buff[3] = '\0';

    int i = 1;
    int j = 2;

    while (1){
        len = strlen(str_to_cmp);
        end_loop = false;

        if (len < 8){
            len = strlen(input);
            end_loop = j < len;
        }
        if (end_loop == false)
            break;
        buff[0] = input[j];
        buff[1] = input[j + 1];
        buff[2] = input[j + 2];
        atoi_ret = atoi(buff);
        str_to_cmp[i] = (char)atoi_ret;
        j+=3;
        i++;
    }

    str_to_cmp[i] = '\0';
    if (strcmp(str_to_cmp, "delabere") == 0)
        return(printf("Good job.\n"), 0);
    else
        return(printf("Nope.\n"), 1);

    return 0;
}