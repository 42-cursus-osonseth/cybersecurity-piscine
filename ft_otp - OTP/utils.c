#include "utils.h"

bool is_hex_digit(char c){
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

bool is_hex_key(char * str){

    while (*str){
        if (!is_hex_digit(*str))
            return false;
        str++; 
    }
    return true;
}

void check_is_hex_key(char **buffer){
    if (!is_hex_key(*buffer)){
        free(*buffer);
        exit(EXIT_FAILURE);
    }

}

char * allocate_the_buffer(void){
    char * buffer = malloc(65 * sizeof(char));
    if (!buffer){
        fprintf(stderr, "Error: malloc buffer failed.\n");
        exit(EXIT_FAILURE);
    }
}

char *parse_args(int argc, char **argv, char *opt_flag)
{
    int c;
    int kflg = 0, gflg = 0, errflg = 0;
    char *file = NULL;

    while ((c = getopt(argc, argv, ":g:k:")) != -1)
    {
        switch (c)
        {
        case 'k':
            if ((gflg) || (kflg))
                errflg++;
            else
            {
                file = optarg;
                kflg++;
            }
            break;
        case 'g':
            if ((kflg) || (gflg))
                errflg++;
            else
            {
                file = optarg;
                gflg++;
            }
            break;
        case '?':
            fprintf(stderr,
                    "Unrecognized option: '-%c'\n", optopt);
            exit(1);
        case ':':
            fprintf(stderr, "option -%c requires an argument\n", optopt);
            errflg++;
            break;
        }
    }
    if (errflg)
    {
        fprintf(stderr, "Error: invalid option\n");
        exit(EXIT_FAILURE);
    }
    return  kflg ? (opt_flag[0] = 'k', file)
          : gflg ? (opt_flag[0] = 'g', file)
          : NULL;
}