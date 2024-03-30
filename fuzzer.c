#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main() {

    char buffer[1000];
    srand(time(NULL));

    int length = 900;
    int i, counter, counter1, counter2, randPos ;
    int totalBugs = 0;

    FILE *file;
    FILE *fout;

    for (i = 0; i < 1000; ++i) {
        file = fopen("cross.jpg", "rb");
        counter = 0;

        while (1) {
            int byte = fgetc(file);
            if (byte == EOF) {
                break;
            }
            buffer[counter] = byte;
            counter++;
        }
        fclose(file);

        if (i < 100) {
            randPos = rand() % length;
            buffer[randPos] = rand() % 256;
        } else if (i < 500) {
            randPos = rand() % (length - 5);
            for (int j = 0; j < 5; ++j) {
                buffer[randPos + j] = rand() % 256;
            }
        } else {
            for (int j = 0; j < 6; ++j) {
                randPos = rand() % length;
                buffer[randPos] = rand() % 256;
            }
        }

        fout = fopen("cross_muted.jpg", "wb");
        counter2 = 0;

        while (counter2 < counter) {
            fputc(buffer[counter2], fout);
            counter2++;
        }

        fclose(fout);

        char commandBuf[100];
        sprintf(commandBuf, "./jpeg2bmp cross_muted.jpg cross.bmp");

        int execute = system(commandBuf);
        int retCode = WEXITSTATUS(execute);

        if (retCode == 134 || retCode == 139) {

            FILE *pipe = popen("./jpeg2bmp cross_muted.jpg cross.bmp 2>&1", "r");
            char out[100];
            fgets(out, sizeof(out), pipe);
            pclose(pipe);

            int n;
            char *bug_nStr = strstr(out, "#");
            if (bug_nStr != NULL) {
                sscanf(bug_nStr, "#%d", &n);
                
                char filename[15];
                sprintf(filename, "test-%d.jpg", n);

                fout = fopen(filename, "wb");
                counter1 = 0;

                while (counter1 < counter) {
                    fputc(commandBuf[counter1], fout);
                    counter1++;
                }

                fclose(fout);
                printf("retCode: %d, Image: %s \n",retCode,filename);
                fflush(stdout);
                
            }
        }
    }

    return 0;
}
