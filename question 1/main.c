#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 100

int main() {
    char input[MAX_LENGTH];
    char prompt[] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash % ";

    write(STDOUT_FILENO, prompt, strlen(prompt)); // Utilisation de write pour afficher le prompt

    while (1) {
        read(STDIN_FILENO, input, MAX_LENGTH); // Lecture de l'entrée utilisateur

        input[strcspn(input, "\n")] = '\0'; // Supprimer le saut de ligne à la fin de l'entrée utilisateur

        if (strcmp(input, "exit") == 0) {
            write(STDOUT_FILENO, "Au revoir !\n", strlen("Au revoir !\n"));
            break;
        }

        // Ajoutez la logique pour exécuter les commandes ici (par exemple, en utilisant la fonction system).

        write(STDOUT_FILENO, "Commande non prise en charge : ", strlen("Commande non prise en charge : "));
        write(STDOUT_FILENO, input, strlen(input));
        write(STDOUT_FILENO, "\nenseash % ", strlen("\nenseash % ")); // Afficher le prompt pour la prochaine commande
    }

    return 0;
}
