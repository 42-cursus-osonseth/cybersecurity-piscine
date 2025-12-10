#include <stdio.h>
#include <string.h>

int main (void){
    char input[100] = {};
    printf("Please enter key: ");
    scanf("%s", input);
    if (strcmp(input, "__stack_check"))
        printf("Nope.\n");
    else
        printf("Good job.\n");

    return 0;
}