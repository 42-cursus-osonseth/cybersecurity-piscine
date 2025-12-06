#include "errors.h"
#include "utils.h"
#include <time.h>
#include <stdint.h>
#include <openssl/hmac.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>




void ascii_hex_to_bytes(const char *hex, unsigned char *bytes, int len) {
    for (int i = 0; i < len/2; i++)
        sscanf(hex + i*2, "%2hhx", &bytes[i]);
}

void create_the_file_with_the_encrypted_key(char ** buffer){
    int fd = open("ft_otp.key", O_CREAT | O_WRONLY, 0644);
    if (fd == -1)
        error_exit("Error: creating file ft_opt.key failed", *buffer, USE_ERRNO);
    if (write(fd, *buffer, 64) != 64){
        close(fd);
        error_exit("Error: writing to file ft_opt.key failed", *buffer, USE_ERRNO);
    }
    printf("Key was successfully saved in ft_otp.key\n");
    close(fd);
}

void open_and_read_input_file(char * input_file, char ** buffer){

    int fd; 
    if ((fd = open(input_file, O_RDONLY)) == -1)
        error_exit("Error: Opening file failed", *buffer, USE_ERRNO);
    int bytes_readed = read(fd, *buffer, 65);
    if (bytes_readed != 64){
         close(fd);
         error_exit("Error: key must be 64 hexadecimal characters", *buffer, USE_ERRNO);
    }
    close(fd);
    (*buffer)[64] = '\0';
}

void generate_ftoptkey_file(char *input_file){
    char * buffer = allocate_the_buffer();
    open_and_read_input_file(input_file, &buffer);
    check_is_hex_key(&buffer);
    crypt_decrypt_key(&buffer);
    create_the_file_with_the_encrypted_key(&buffer);
    free(buffer);
}

void generate_otp(char * input_file){
    char * buffer = allocate_the_buffer();
    open_and_read_input_file(input_file, &buffer);
    crypt_decrypt_key(&buffer);

    time_t now = time(NULL);
    uint64_t timestep = now / 30;
    unsigned char counter[8];
    create_counter_array(counter, timestep);

    unsigned char hmac[20];
    unsigned char key_bytes[32];
    ascii_hex_to_bytes(buffer, key_bytes, 64);
    HMAC(EVP_sha1(), key_bytes, 32, counter, 8, hmac, NULL);
    uint32_t sbytes = create_sbytes(hmac);
    uint32_t otp = sbytes % 1000000;
    printf("%d\n", otp);
    free(buffer);
}

int main(int argc, char **argv){
    if (argc == 1)
        error_exit("The program must have arguments (-k or -g with a file)", NULL, NO_ERRNO);

    char opt_flag[2] = {};
    char *input_file = parse_args(argc, argv, opt_flag);

    if (opt_flag[0] == 'k')
        generate_otp(input_file);
    else if (opt_flag[0] == 'g')
        generate_ftoptkey_file(input_file);

    return 0;
}

