
#include "utils.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdint.h>
#include <openssl/hmac.h>

void hex_to_bytes(const char *hex, unsigned char *bytes, int len) {
    for (int i = 0; i < len/2; i++)
        sscanf(hex + i*2, "%2hhx", &bytes[i]);
}




void encrypt_key(char **buffer){

    for(int i = 0; i < 64; i++)
        (*buffer)[i] ^= i;
}
void create_the_file_with_the_encrypted_key(char ** buffer){
    int fd = open("ft_otp.key", O_CREAT | O_WRONLY, 0644);
    if (fd == -1){
    fprintf(stderr, "Error: creating file ft_opt.key failed.\n");
    free(*buffer);
    exit(EXIT_FAILURE);
    }
    if (write(fd, *buffer, 64) != 64) {
        fprintf(stderr, "Error writing to file\n");
        close(fd);
        free(*buffer);
        exit(EXIT_FAILURE);
    }
    printf("Key was successfully saved in ft_otp.key\n");
    close(fd);

}


void open_and_read_input_file(char * input_file, char ** buffer){

    int fd; 
    if ((fd = open(input_file, O_RDONLY)) < 0) {
    fprintf(stderr, "Error: Opening file failed.\n");
    free(*buffer);
    exit(EXIT_FAILURE);
    }
    int bytes_readed = read(fd, *buffer, 65);
    if (bytes_readed != 64){
        fprintf(stderr, "Error: key must be 64 hexadecimal characters.\n");
        free(*buffer);
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
    (*buffer)[64] = '\0';

}

void generate_ftoptkey_file(char *input_file)
{
    char * buffer = malloc(65 * sizeof(char));
    if (!buffer){
        fprintf(stderr, "Error: malloc buffer failed.\n");
        exit(EXIT_FAILURE);
    }
    open_and_read_input_file(input_file, &buffer);
    check_is_hex_key(&buffer);
    encrypt_key(&buffer);
    create_the_file_with_the_encrypted_key(&buffer);
    free(buffer);
 
}

void generate_otp(char * input_file){
    char * buffer = malloc(65 * sizeof(char));
    if (!buffer){
        fprintf(stderr, "Error: malloc buffer failed.\n");
        exit(EXIT_FAILURE);
    }
    open_and_read_input_file(input_file, &buffer);
    encrypt_key(&buffer);

    time_t now = time(NULL);
    uint64_t timestep = now / 30;
    unsigned char counter[8];

    for (int i = 7; i >= 0; i--) {
    counter[i] = timestep & 0xFF;
    timestep >>= 8;
    }
    unsigned char hmac[20];
    unsigned char key_bytes[32];
    hex_to_bytes(buffer, key_bytes, 64);
    HMAC(EVP_sha1(), key_bytes, 32, counter, 8, hmac, NULL);
 
    int offset = hmac[19] & 0x0F;
    uint32_t sbytes = (hmac[offset] & 0x7F) << 24 |
                        (hmac[offset+1] & 0xFF) << 16 |
                        (hmac[offset+2] & 0xFF) << 8  |
                        (hmac[offset+3] & 0xFF);
    uint32_t otp = sbytes % 1000000;
    printf("OPT = %d\n", otp);
}

int main(int argc, char **argv)
{

    if (argc == 1)
        return (fprintf(stderr, "The program must have arguments (-k or -g with a file).\n"), 1);
    char opt_flag[2] = {};
    char *input_file = parse_args(argc, argv, opt_flag);
    if (opt_flag[0] == 'k')
        generate_otp(input_file);
    else if (opt_flag[0] == 'g')
        generate_ftoptkey_file(input_file);

    return 0;
}

