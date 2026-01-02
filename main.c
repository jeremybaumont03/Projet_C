#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusions des modules
#include "bigbinary.h"
#include "multiplication.h"
#include "pgcd.h"
#include "modexp.h"
#include "RSA.h"

// Couleurs
#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_CYAN    "\033[36m"
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_RED     "\033[31m"
#define C_GREY    "\033[90m"

// --- Outils ---

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pause_prog() {
    printf(C_GREY "\n  [Appuyez sur Entree pour continuer...]" C_RESET);
    vider_buffer();
    getchar();
}

// Fonction d'affichage propre
void afficher_resultat(const char* titre, BigBinary nb) {
    printf(C_CYAN "  :: %s ::\n" C_RESET, titre);
    printf("  = ");
    
    if (nb.Signe == -1) printf("-");
    if (nb.Signe == 0) printf("0");
    else {
        for (int i = 0; i < nb.Taille; i++) {
            printf("%d", nb.Tdigits[i]);
            // Espace tous les 4 bits
            if ((nb.Taille - 1 - i) % 4 == 0 && i != nb.Taille - 1) {
                printf(" ");
            }
        }
    }
    printf("\n\n");
}

BigBinary lire_input(const char* nom) {
    char buffer[4096];
    printf("  > %s (dec/bin) : ", nom);
    scanf("%s", buffer);

    BigBinary res;
    // Détection décimale
    int estDecimal = 0;
    for(int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] >= '2' && buffer[i] <= '9') {
            estDecimal = 1;
            break;
        }
    }

    if (estDecimal) {
        unsigned long long n = strtoull(buffer, NULL, 10);
        res = creerBigBinaryDepuisEntier(n);
    } else {
        res = creerBigBinaryDepuisChaine(buffer);
    }

    // --- AJOUT DEMANDE : Afficher la conversion tout de suite ---
    char titre[100];
    sprintf(titre, "Conversion Binaire de %s", nom);
    afficher_resultat(titre, res);
    // ------------------------------------------------------------

    return res;
}

// --- MAIN ---

int main() {
    int choix = -1;
    BigBinary A, B, R, N;
    unsigned int e;

    while (choix != 0) {
        clear_screen();
        
        printf("\n");
        printf(C_BOLD "  === PROJET BIG DIGITS (ESIEA) ===\n" C_RESET);
        printf("\n");
        printf("  1. Addition\n");
        printf("  2. Soustraction\n");
        printf("  3. Multiplication\n");
        printf("  4. PGCD\n");
        printf("  5. Modulo\n");
        printf("  6. Exponentiation\n");
        printf("\n");
        printf(C_YELLOW "  --- Cryptographie RSA ---\n" C_RESET);
        printf("  7. Chiffrement RSA\n");
        printf("  8. Dechiffrement RSA\n");
        printf("  9. Bonus (Exp Grand Entier)\n");
        printf("\n");
        printf(C_RED "  0. Quitter\n" C_RESET);
        printf("\n  > Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }
        
        printf("\n");

        switch (choix) {
            case 1:
                printf(C_BOLD "  [ ADDITION ]\n" C_RESET);
                A = lire_input("A");
                B = lire_input("B");
                R = Addition(A, B);
                afficher_resultat("RESULTAT FINAL (A + B)", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 2:
                printf(C_BOLD "  [ SOUSTRACTION ]\n" C_RESET);
                A = lire_input("A");
                B = lire_input("B");
                if (Inferieur(A, B)) printf(C_RED "  Erreur : A doit etre >= B\n" C_RESET);
                else {
                    R = Soustraction(A, B);
                    afficher_resultat("RESULTAT FINAL (A - B)", R);
                    libereBigBinary(&R);
                }
                libereBigBinary(&A); libereBigBinary(&B);
                pause_prog(); break;

            case 3:
                printf(C_BOLD "  [ MULTIPLICATION ]\n" C_RESET);
                A = lire_input("A");
                B = lire_input("B");
                R = Multiplication_Egyptienne(A, B);
                afficher_resultat("RESULTAT FINAL (A * B)", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 4:
                printf(C_BOLD "  [ PGCD ]\n" C_RESET);
                A = lire_input("A");
                B = lire_input("B");
                R = BigBinary_PGCD(A, B);
                afficher_resultat("PGCD CALCULE", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 5:
                printf(C_BOLD "  [ MODULO ]\n" C_RESET);
                A = lire_input("A");
                B = lire_input("Modulo N");
                R = Modulo(A, B);
                afficher_resultat("RESTE (A % N)", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 6:
                printf(C_BOLD "  [ EXPONENTIATION ]\n" C_RESET);
                A = lire_input("Base A");
                printf("  > Exposant e (entier) : ");
                scanf("%u", &e);
                B = lire_input("Modulo N");
                R = ExponentiationRapideModulaire(A, e, B);
                afficher_resultat("RESULTAT EXP", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 7:
            case 8:
                printf(C_BOLD "  [ MODULE RSA ]\n" C_RESET);
                A = lire_input(choix==7 ? "Message Clair" : "Message Chiffre");
                B = lire_input("Modulus N");
                printf("  > Cle (e/d) : ");
                scanf("%u", &e);
                R = ExponentiationRapideModulaire(A, e, B);
                afficher_resultat(choix==7 ? "MESSAGE CHIFFRE" : "MESSAGE DECHIFFRE", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 9:
                printf(C_BOLD "  [ EXP MAISON ]\n" C_RESET);
                A = lire_input("Base A");
                B = lire_input("Exposant E");
                N = lire_input("Modulo N");
                R = ExponentiationRapideModulaire_Maison(A, B, N);
                afficher_resultat("RESULTAT FINAL", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&N); libereBigBinary(&R);
                pause_prog(); break;

            case 0:
                printf(C_GREEN "\n  Au revoir !\n\n" C_RESET);
                break;

            default:
                printf(C_RED "  Choix invalide.\n" C_RESET);
                pause_prog();
        }
    }
    return 0;
}