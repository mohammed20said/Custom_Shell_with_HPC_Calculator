#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* Child process */
        close(pipefd[0]); /* Close unused read end */

        /* Duplicate stdout on write end of pipe */
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        /* Execute ls with arguments */
        char *ls_args[] = {"ls", NULL};
        execvp(ls_args[0], ls_args);

        /* If execvp returns, it must have failed */
        perror("execvp");
        exit(EXIT_FAILURE);

    } else {
        /* Parent process */
        close(pipefd[1]); /* Close unused write end */

        /* Duplicate read end of pipe on stdin */
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        /* Execute wc command */
        execlp("cat", "cat", NULL);

        /* If execlp returns, it must have failed */
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    return 0;
}

