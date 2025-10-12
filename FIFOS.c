// File: process1.c (Sender/Receiver)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define MAX 1024

int main() {
    char buffer[MAX];
    int fd1, fd2;

    // Create FIFOs if not already created
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    while (1) {
        printf("Enter sentence (or 'exit' to quit): ");
        fgets(buffer, MAX, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // remove newline

        if (strcmp(buffer, "exit") == 0)
            break;

        // Write sentence to FIFO1
        fd1 = open(FIFO1, O_WRONLY);
        write(fd1, buffer, strlen(buffer) + 1);
        close(fd1);

        // Read processed result from FIFO2
        fd2 = open(FIFO2, O_RDONLY);
        read(fd2, buffer, MAX);
        printf("Processed Output:\n%s\n", buffer);
        close(fd2);
    }

    unlink(FIFO1);
    unlink(FIFO2);
    return 0;
}




// File: process2.c (Processor)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define MAX 1024
#define FILE_NAME "output.txt"

int main() {
    char buffer[MAX];
    int fd1, fd2;

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    while (1) {
        // Read from FIFO1
        fd1 = open(FIFO1, O_RDONLY);
        read(fd1, buffer, MAX);
        close(fd1);

        if (strcmp(buffer, "exit") == 0)
            break;

        // Count characters, words, lines
        int chars = 0, words = 0, lines = 1; // at least 1 line
        for (int i = 0; buffer[i]; i++) {
            chars++;
            if (buffer[i] == ' ' || buffer[i] == '\t')
                words++;
            if (buffer[i] == '\n')
                lines++;
        }
        words++; // count last word

        // Write summary to file
        FILE *fp = fopen(FILE_NAME, "w");
        if (!fp) {
            perror("File open error");
            exit(1);
        }
        fprintf(fp, "Number of characters: %d\n", chars);
        fprintf(fp, "Number of words: %d\n", words);
        fprintf(fp, "Number of lines: %d\n", lines);
        fclose(fp);

        // Read file content and write to FIFO2
        fp = fopen(FILE_NAME, "r");
        if (!fp) {
            perror("File read error");
            exit(1);
        }
        char fileContent[MAX];
        fread(fileContent, 1, MAX, fp);
        fclose(fp);

        fd2 = open(FIFO2, O_WRONLY);
        write(fd2, fileContent, strlen(fileContent) + 1);
        close(fd2);
    }

    unlink(FIFO1);
    unlink(FIFO2);
    return 0;
}




/* Output:-

$ gcc process1.c -o out (Run at 1st terminal)
$ gcc process2.c -o out (Run at 2nd terminal)
$ ./out

*/


