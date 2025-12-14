#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


int main(void){

  int n;  
  int atoi_ret;
  size_t len;
  bool end;
  char buff [4];
  char input [31];
  char str_to_cmp [9];
  ulong local_20; int res;
  int local_14;
  int local_10;
 
  
  printf("Please enter key: ");
  n = scanf("%s", input);
  if (n != 1)
    printf("Nope.\n");
  if (input[1] != '2')
    printf("Nope.\n");
  if (input[0] != '4')
    printf("Nope.\n");
  memset(str_to_cmp,0,9);
  str_to_cmp[0] = '*';
  buff[3] = '\0';
  int  j = 2;
  int i = 1;
  while(1) {
    len = strlen(str_to_cmp);
    end = false;
    if (len < 8) {
      len = strlen(input);
      end = j < len;
    }
    if (!end) break;
    buff[0] = input[j];
    buff[1] = input [j + 1];
    buff[2] = input [j + 2];
    atoi_ret = atoi(buff);
    str_to_cmp[i] = (char)atoi_ret;
    j += 3;
    i++;
  }
  str_to_cmp[i] = '\0';
  res = strcmp(str_to_cmp,"********");
  if (res == -2)
    printf("Nope.\n");
  else if (res == -1)
    printf("Nope.\n");
  else if (res == 0)
    printf("Good job.\n");
  else if (res == 1)
    printf("Nope.\n");
  else if (res == 2)
    printf("Nope.\n");
  else if (res == 3)
    printf("Nope.\n");
  else if (res == 4)
    printf("Nope.\n");
  else if (res == 5)
    printf("Nope.\n");
  else if (res == 0x73)
    printf("Nope.\n");
  else
    printf("Nope.\n");
  return 0;
}