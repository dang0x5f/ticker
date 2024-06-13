#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* #define TEXTBOX_SIZE 25 */
/* #define TEXTBOX_MINUS1 (TEXTBOX_SIZE - 1) */
#define BUFSIZE 100

void usage(char **);
void reverse(char*);
void itoa(int, char*);
void read_from_save_file(int, char*);
int split_strings(char*, const char*, char**);
void printscroll(char**, int, int, int);
void printscroll_withpad(char**, int, int, int);
void write_to_save_file(char*, const char*, char**, char*);

int main(int argc, char** argv){
    int songtitle_len;
    int fd, index = 0;         
    char buffer[BUFSIZE];
    char* fields_arr[3];
    const char* DELIM = "~";

    int textbox_size, textbox_minus1;
                                                
    if (argc < 2){
        /* fprintf(stderr, "usage: %s str\n", argv[0]); // err chk: arg count */
        usage(argv);
        exit(EXIT_FAILURE);
    }
    if((fd = open(argv[2], O_RDONLY)) == -1){        // err chk: open file
        fprintf(stderr, "failed to open\n");
        exit(EXIT_FAILURE);
    }
    textbox_size = strtol(argv[3], NULL, 10);
    textbox_minus1 = textbox_size - 1;

    read_from_save_file(fd,buffer);             

    songtitle_len = split_strings(buffer, DELIM, fields_arr);

    // expression: Is the same song still playing? 
    if (strcmp(fields_arr[1], argv[1]) == 0){

        // Fits within TextBox constraints - No Scroll
        if(songtitle_len <= textbox_size){            
            printscroll_withpad(fields_arr, index, songtitle_len, textbox_size);
            /* printf("%s",fields_arr[1]); // :) */
            exit(EXIT_SUCCESS);
        }

        // Assign Index 1st field_arr value(last index) 
        // Increment index count from previous execution
        index = atoi(fields_arr[0]) + 1;

        printscroll(fields_arr, index, textbox_size, textbox_minus1); 

        // Reset to beginning of scroll
        if((strlen(fields_arr[1]) - index) == textbox_minus1)
            index = -1;

        char cindex[10];
        itoa(index, cindex);

        write_to_save_file(cindex, DELIM, fields_arr, argv[2]);
    } 
    else{                                       
        if(songtitle_len <= textbox_size)
            printscroll_withpad(fields_arr, index, songtitle_len, textbox_size);
        else
            printscroll(fields_arr, index, textbox_size, textbox_minus1);

        write_to_save_file("0", DELIM, argv, argv[2]);
    }

    return(EXIT_SUCCESS);
} 
// **** END  MAIN ****


void write_to_save_file(char* ind, const char* delim, char** title, char* file){
    int write_fd;

    if((write_fd = open(file, O_WRONLY|O_TRUNC)) == -1){
        fprintf(stderr, "failed to open");
        exit(EXIT_FAILURE);
    }
    write(write_fd, ind, sizeof(char) * strlen(ind));
    write(write_fd, delim, sizeof(char));
    write(write_fd, title[1], sizeof(char) * strlen(title[1]));

    close(write_fd);
}

void printscroll(char** fields_arr, int index, int tb_size, int tb_min1){
    int i, x;
    char final[100];
    char final2[100];
    char* finale = malloc(tb_size * sizeof(char));

    strncpy(final,fields_arr[1],strlen(fields_arr[1]));
    for(i = 0, x = index; i < tb_min1; i++, x++){
        final2[i] = final[x];
    }
    final2[i] = '\0';

    strncpy(finale, final2, tb_size);

    printf("%s\n", finale);

    free(finale);
}

void printscroll_withpad(char** fields_arr, int index, int len, int tb_size){
    int i, x;
    char final[100];
    char final2[100];
    char* finale = malloc(tb_size * sizeof(char));

    int padding = (tb_size % len) - 1; // leave space for '\0'

    strncpy(final,fields_arr[1],strlen(fields_arr[1]));
    for(i = 0, x = index; i < len; i++, x++){
        final2[i] = final[x];
    }
    for(x = 0; x < padding; i++, x++){
        final2[i] = ' ';
    }
    final2[i] = '\0';

    strncpy(finale, final2, tb_size);

    printf("%s\n", finale);

    free(finale);
}

// separate string from save file
int split_strings(char* buffer, const char* delim, char** fields_arr){
    int x = 0;
    char* field, * record;

    // !!! ->   char [] != char *   <- !!! THEY BUILT DIFFERENT
    record = buffer;   
    while( (field = strsep(&record, delim)) != NULL ){
        /* printf("%s\n",field); // debug */
        fields_arr[x++] = field;
    }
    return strlen(fields_arr[1]);
}

// read index and title from save file
void read_from_save_file(int fd, char* buffer){
    int bytes_read;                                    

    bytes_read = read(fd, buffer, BUFSIZE);   // read into buffer sizeof bufsize
    buffer[bytes_read] = '\0';                 // null terminate

    close(fd);                                 // close RO file
}

void reverse(char *b){
    int i, j, c;

    for(i = 0, j = strlen(b) - 1; i < j; i++, j--){
        c = b[j];
        b[j] = b[i];
        b[i] = c;
    }
}

void itoa(int nbr, char *arr){
    int i, sign;

    if((sign = nbr) < 0)
    nbr = -nbr;

    i = 0;
    do{
        arr[i++] = nbr % 10 + '0';
    }while((nbr/=10) > 0);

    if(sign < 0)
        arr[i++] = '-';
    arr[i] = '\0';
    reverse(arr);
}

void usage(char ** av){
    fprintf(stderr, "usage:\n  %s string file txtsize buffsize\n", av[0]);
}
