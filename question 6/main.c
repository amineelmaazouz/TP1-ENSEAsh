#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include "fonction.h" // Fichier contenant les déclarations de fonctions personnalisées
#include <sys/wait.h>

#define MAX_ARGUMENTS 10
#define MAX_INPUT_SIZE 1024

// Fonction pour lire une commande entrée par l'utilisateur
char *executeCommand() {
    const char *MSGINPUT = "enseash%% ";
    
    char *cmd = (char *)malloc(MAX_INPUT_SIZE * sizeof(char));
    if (cmd == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Affichage du prompt
    if (write(STDOUT_FILENO, MSGINPUT, strlen(MSGINPUT)) == -1) {
        perror("Error writing to the terminal");
        free(cmd);
        exit(EXIT_FAILURE);
    }
    
    // Lecture de la commande entrée par l'utilisateur
    ssize_t bytesRead = read(STDIN_FILENO, cmd, MAX_INPUT_SIZE);
    if (bytesRead == -1) {
        perror("Error reading from the terminal");
        free(cmd);
        exit(EXIT_FAILURE);
    }

    // Suppression du caractère de nouvelle ligne à la fin de l'entrée
    size_t len = strlen(cmd);
    if (len > 0 && cmd[len - 1] == '\n') {
        cmd[len - 1] = '\0';
    }

    return cmd;
}

int main() {
    const char *ERRORMSG = "Erreur: You can't execute this command\n";

    while (1) {
        char *command = executeCommand();
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        
        if (child_pid == 0) {
            char *args[MAX_ARGUMENTS];
            parseArguments(command, args); // Fonction pour séparer les arguments de la commande
            
            // Exécution de la commande avec les arguments correspondants
            if (execvp(args[0], args) == -1) {
                perror(ERRORMSG);
                exit(EXIT_FAILURE);
            }
		} else {
            // Processus parent
            int status;
            waitpid(child_pid, &status, 0); // Attente de la fin du processus enfant
            // Libération de la mémoire allouée pour la commande
            free(command);
        }
    }

    return 0;
}

// Fonction pour séparer les arguments de la commande
void parseArguments(char *input, char *args[]) {
    char commandCopy[MAX_INPUT_SIZE];
    strcpy(commandCopy, input);

    char *token = strtok(commandCopy, " ");
    int i = 0;

    while (token != NULL && i < MAX_ARGUMENTS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }

    args[i] = NULL; // Assurer que le dernier élément est NULL pour execvp
}
