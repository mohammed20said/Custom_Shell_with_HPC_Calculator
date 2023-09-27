#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25

int main(int argc, char *argv[]) {
    int fd[2];
    pid_t pid;

    if (argc != 2) {
        fprintf(stderr, "utilisation: lire <nom du fichier>\n");
        return 1;
    }

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child process
        close(fd[1]); // Close the write end of the pipe

        // Redirect the stdin of the child process to the read end of the pipe
        if (dup2(fd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            return 1;
        }

        // Execute the cat command to read from the pipe and write to stdout
        if (execlp("cat", "cat", NULL) == -1) {
            perror("execlp");
            return 1;
        }
    } else {
        // Parent process
        close(fd[0]); // Close the read end of the pipe

        // Open the file for reading
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("fopen");
            return 1;
        }

        // Read from the file and write to the write end of the pipe
        char buffer[BUFFER_SIZE];
        int bytes_read;
        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
            if (write(fd[1], buffer, bytes_read) == -1) {
                perror("write");
                return 1;
            }
        }

        // Close the write end of the pipe to signal EOF to the child process
        close(fd[1]);

        // Wait for the child process to terminate
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Child process terminated with an error\n");
            return 1;
        }
    }

    return 0;
}

