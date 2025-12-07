#include <stdio.h>
#include <string.h>


int main (void){
    char input[16] = {};
    printf("Please enter key: ");
    scanf("%15s", input);
    if (strcmp(input, "__stack_check"))
        printf("Nope.\n");
    else
        printf("Good job.\n");

    return 0;
}