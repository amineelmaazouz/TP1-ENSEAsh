#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define SIZE 100

// Fonction pour récupérer la commande de l'utilisateur depuis l'entrée standard
char *getCommand() {
    const char *MSGINPUT = "enseash%% ";

    char *cmd = (char *)malloc(SIZE * sizeof(char));
    if (cmd == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Affichage de l'invite de commande
    if (write(STDOUT_FILENO, MSGINPUT, strlen(MSGINPUT)) == -1) {
        perror("Error writing to the terminal");
        free(cmd);
        exit(EXIT_FAILURE);
    }

    // Lecture de la commande depuis l'entrée standard (le terminal)
    ssize_t bytesRead = read(STDIN_FILENO, cmd, SIZE);
    if (bytesRead == -1) {
        perror("Error reading from the terminal");
        free(cmd);
        exit(EXIT_FAILURE);
    }

    // Suppression du caractère de nouvelle ligne à la fin de l'entrée utilisateur
    size_t len = strlen(cmd);
    if (len > 0 && cmd[len - 1] == '\n') {
        cmd[len - 1] = '\0';
    }

    return cmd;
}

int main() {
    const char *ERRORMSG = "Erreur: You can't execute this command\n";
    char *command;
    clock_t t_init_child; // Pour enregistrer le moment où l'enfant est initialisé
    clock_t t_finish_child; // Pour enregistrer le moment où l'enfant a fini
    
    while (1) {
        command = getCommand(); // Récupération de la commande entrée par l'utilisateur
        pid_t child_pid = fork(); // Création d'un processus enfant

        if (child_pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
       
        t_init_child = clock(); // Enregistrement du temps de départ pour l'enfant
        
        // Code exécuté par le processus enfant
        if (child_pid == 0) {
            sleep(2); // Simulation d'une tâche en cours d'exécution
            printf("pid_fils=%d\n", getpid());

            // Vérification si la commande est "exit" ou vide pour quitter le programme
            if (strcmp(command, "exit") == 0 || strlen(command) == 0) {
                write(STDOUT_FILENO, "Bye bye...\n", strlen("Bye bye...\n"));
                free(command);
                break;
            }

            // Exécution de la commande entrée par l'utilisateur
            if (execlp(command, command, NULL) == -1) {
                perror(ERRORMSG);
                free(command);
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        } else {
            int status;
            wait(&status); // Attente de la fin du processus enfant
            t_finish_child = clock(); // Enregistrement du temps de fin pour l'enfant

            double finish = (t_finish_child - t_init_child) / (CLOCKS_PER_SEC / 1000.0); // Calcul du temps écoulé en millisecondes

            char msgState[100] = "";

            // Vérification si le processus s'est terminé correctement ou s'il y a eu une interruption, puis affichage des résultats
            if (WIFEXITED(status)) {
                sprintf(msgState, "enseash %% [exit : %d | %.0fms]\n", WEXITSTATUS(status), finish);
                write(STDOUT_FILENO, msgState, strlen(msgState));
            } else {
                sprintf(msgState, "enseash %% [sign : %d | %.0fms]\n", status, finish);
                write(STDOUT_FILENO, msgState, strlen(msgState));
            }
            free(command); // Libération de la mémoire allouée pour la commande
        }
    }
    return 0;
}
