#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100 // Longueur maximale de la commande

int main() {
    char input[MAX_LENGTH]; // Variable pour stocker la commande de l'utilisateur

    printf("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash %% ");

    while (1) { // Boucle infinie pour lire continuellement les commandes de l'utilisateur
        fgets(input, MAX_LENGTH, stdin); // Lire la commande de l'utilisateur

        // Supprimer le saut de ligne à la fin de l'entrée utilisateur
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) { // Vérifier si l'utilisateur veut quitter
            printf("Au revoir !\n");
            break; // Sortir de la boucle while pour terminer le programme
        }

        // Ici, vous pouvez ajouter la logique pour exécuter les commandes entrées par l'utilisateur.
        // Par exemple, vous pouvez utiliser la fonction system pour exécuter les commandes dans le shell.

        printf("Commande non prise en charge : %s\nenseash %% ", input); // Afficher un message par défaut pour les autres commandes
    }

    return 0;
}
