#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 100

// Fonction pour récupérer la commande de l'utilisateur
char *executeCommand()
{
    const char *MSGINPUT = "enseash%% ";
    char *cmd = (char *)malloc(SIZE * sizeof(char)); // Alloue de la mémoire pour stocker la commande
    if (cmd == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Affiche le prompt pour demander à l'utilisateur d'entrer une commande
    if (write(STDOUT_FILENO, MSGINPUT, strlen(MSGINPUT)) == -1)
    {
        perror("Error writing to the terminal");
        free(cmd);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead = read(STDIN_FILENO, cmd, SIZE); // Lit la commande entrée par l'utilisateur
    if (bytesRead == -1)
    {
        perror("Error reading from the terminal");
        free(cmd);
        exit(EXIT_FAILURE);
    }

    // Supprime le caractère de nouvelle ligne à la fin de l'entrée utilisateur, s'il y en a un
    size_t len = strlen(cmd);
    if (len > 0 && cmd[len - 1] == '\n')
    {
        cmd[len - 1] = '\0';
    }

    return cmd;
}

int main()
{
    const char *ERRORMSG = "Erreur: You can't execute this command\n";

    while (1)
    {
        char *command = executeCommand(); // Récupère la commande entrée par l'utilisateur
        pid_t child_pid = fork(); // Crée un nouveau processus

        if (child_pid == -1)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0)
        {
            // Processus enfant
            if (execlp(command, command, NULL) == -1) // Exécute la commande entrée par l'utilisateur
            {
                perror(ERRORMSG);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // Processus parent
            int status;
            waitpid(child_pid, &status, 0); // Attend que le processus enfant se termine
            free(command); // Libère la mémoire allouée pour stocker la commande
        }
    }

    return 0;
}
