#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define SIZE 100

char *executeCommand() {
    const char *MSGINPUT = "enseash%% ";

    char *cmd = (char *)malloc(SIZE * sizeof(char));
    if (cmd == NULL) {
        fprintf(stderr, "Memory allocation error \n");
        exit(EXIT_FAILURE);
    }

    if (write(STDOUT_FILENO, MSGINPUT, strlen(MSGINPUT)) == -1) {
        perror("Error writing to the terminal");
        free(cmd);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead = read(STDIN_FILENO, cmd, SIZE);
    if (bytesRead == -1) {
        perror("Error reading from the terminal");
        free(cmd);
        exit(EXIT_FAILURE);
    }

    // Remove the newline character from the end of the input
    size_t len = strlen(cmd);
    if (len > 0 && cmd[len - 1] == '\n') {
        cmd[len - 1] = '\0';
    }

    return cmd;
}

int main() {
    const char *ERRORMSG = "Erreur: You can't execute this command\n";
    char *command;
    
    while (1) {
        command = executeCommand();
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            printf("\npid_fils=%d\n", getpid());
            sleep(60);

            if (strcmp(command, "exit") == 0 || strlen(command) == 0) {
                write(STDOUT_FILENO, "Bye bye...\n", strlen("Bye bye...\n"));
                free(command);
                break;
            }

            if (execlp(command, command, NULL) == -1) {
                perror(ERRORMSG);
                free(command);
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        } else {
            int status;
            wait(&status);
            printf("%d",status);
            char msgState[100] = "";

            if (WIFEXITED(status)) {
                sprintf(msgState, "enseash %% [exit | %d]\n", WEXITSTATUS(status));
                write(STDOUT_FILENO, msgState, strlen(msgState));
            } else {
                sprintf(msgState, "enseash %% [sign :%d]\n", status);
                write(STDOUT_FILENO, msgState, strlen(msgState));
            }
            free(command); // Libérer la mémoire allouée pour la commande
        }
    }
    return 0;
}
