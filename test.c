#include <stdio.h>
#include <dirent.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(void) {
    DIR *d = opendir("./test");
    struct dirent *dir;
    for (int i = 0; i < 5; i++) {
        char output_file_path[100];
        sprintf(output_file_path, "record_%d.txt", i);
        FILE *output_fp = fopen(output_file_path, "w");
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".") == 0) {
                    continue;
                }
                printf("%s\n", dir->d_name);

                char file_path[300];
                sprintf(file_path, "%s/%s", "./test", dir->d_name);
                int fd[2];

                if (pipe(fd) < 0) {
                    fprintf(stderr, "error: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                pid_t pid;
                if ((pid = fork()) < 0) {
                    fprintf(stderr, "error: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    if (dup2(fd[1], 1) < 0) {
                        fprintf(stderr, "error: %s\n", strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                    close(fd[0]);
                    int ret = execlp(file_path, file_path, NULL);
                    printf("%s\n", file_path);
                    if (ret < 0) {
                        fprintf(stderr, "error: %s\n", strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                } else {
                    wait(NULL);
                }
                FILE *fp = fdopen(fd[0], "r");
                double time;
                fscanf(fp, "%lf", &time);
                printf("%s %lf\n", dir->d_name, time);
                fprintf(output_fp, "%s %lf\n", dir->d_name, time);
            }
            closedir(d);
        }
    }
    
  return(0);
}