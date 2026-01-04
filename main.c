#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Inclusions des modules du projet
#include "bigbinary.h"
#include "multiplication.h"
#include "pgcd.h"
#include "modexp.h"
#include "RSA.h"

// Codes Couleurs ANSI
#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_CYAN    "\033[36m"
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_RED     "\033[31m"
#define C_GREY    "\033[90m"

// --- OUTILS D'INTERFACE ---

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

void afficher_resultat(const char* titre, BigBinary nb) {
    printf(C_CYAN "  :: %s ::\n" C_RESET, titre);
    printf("  = ");
    
    if (nb.Signe == -1) printf("-");
    if (nb.Signe == 0 && nb.Taille == 0) printf("0");
    else {
        for (int i = 0; i < nb.Taille; i++) {
            printf("%d", nb.Tdigits[i]);
            if ((nb.Taille - 1 - i) % 4 == 0 && i != nb.Taille - 1) {
                printf(" ");
            }
        }
    }
    printf("\n\n");
}

BigBinary lire_input(const char* nom) {
    char buffer[256];
    BigBinary res;
    int mode = 0;
    int estValide = 0;

    while (!estValide) {
        // 1. Demander le type de saisie
        printf("\n    --- Saisie de %s ---\n", nom);
        printf("    1. Decimal (ex: 45)\n");
        printf("    2. Binaire (ex: 101101)\n");
        printf("    > Votre choix (1 ou 2) : ");
        
        if (scanf("%d", &mode) != 1) {
            printf(C_RED "    [!] Erreur de saisie. Entrez 1 ou 2.\n" C_RESET);
            vider_buffer();
            continue;
        }
        vider_buffer(); // Nettoie le '\n' restant pour le prochain scanf/fgets

        // 2. Demander la valeur
        printf("    > Entrez la valeur : ");
        scanf("%s", buffer);

        // 3. Validation et Conversion
        if (mode == 1) { // --- MODE DÉCIMAL ---
            int erreur = 0;
            if (strlen(buffer) > 19) {
                printf(C_RED "    [!] Erreur : 19 chiffres maximum en decimal.\n" C_RESET);
                erreur = 1;
            } else {
                for (int i = 0; buffer[i] != '\0'; i++) {
                    if (!isdigit(buffer[i])) {
                        printf(C_RED "    [!] Erreur : '%c' n'est pas un chiffre.\n" C_RESET, buffer[i]);
                        erreur = 1; break;
                    }
                }
            }
            if (!erreur) {
                unsigned long long n = strtoull(buffer, NULL, 10);
                res = creerBigBinaryDepuisEntier(n);
                estValide = 1;
            }
        } 
        else if (mode == 2) { // --- MODE BINAIRE ---
            int erreur = 0;
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (buffer[i] != '0' && buffer[i] != '1') {
                    printf(C_RED "    [!] Erreur : '%c' interdit en binaire.\n" C_RESET, buffer[i]);
                    erreur = 1; break;
                }
            }
            if (!erreur) {
                res = creerBigBinaryDepuisChaine(buffer);
                estValide = 1;
            }
        } 
        else {
            printf(C_RED "    [!] Choix invalide. Tapez 1 ou 2.\n" C_RESET);
        }
    }

    return res;
}

// --- PROGRAMME PRINCIPAL ---

int main() {
    int choix = -1;
    BigBinary A, B, R, N;
    unsigned int e;

    while (choix != 0) {
        clear_screen();
        printf("\n" C_BOLD "  === PROJET BIG DIGITS (ESIEA) ===\n" C_RESET);
        printf("\n  1. Addition\n  2. Soustraction\n  3. Multiplication\n  4. PGCD\n  5. Modulo\n  6. Exponentiation\n");
        printf(C_YELLOW "\n  --- Cryptographie RSA ---\n" C_RESET);
        printf("  7. Chiffrement RSA\n  8. Dechiffrement RSA\n  9. Bonus (Exp Grand Entier)\n");
        printf(C_RED "\n  0. Quitter\n" C_RESET);
        printf("\n  > Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }
        
        switch (choix) {
            case 1:
                A = lire_input("A"); B = lire_input("B");
                R = Addition(A, B);
                afficher_resultat("RESULTAT (A + B)", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 2:
                A = lire_input("A"); B = lire_input("B");
                if (Inferieur(A, B)) printf(C_RED "  Erreur : A < B\n" C_RESET);
                else {
                    R = Soustraction(A, B);
                    afficher_resultat("RESULTAT (A - B)", R);
                    libereBigBinary(&R);
                }
                libereBigBinary(&A); libereBigBinary(&B);
                pause_prog(); break;

            case 3:
                A = lire_input("A"); B = lire_input("B");
                R = Multiplication_Egyptienne(A, B);
                afficher_resultat("RESULTAT (A * B)", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 4:
                A = lire_input("A"); B = lire_input("B");
                R = BigBinary_PGCD(A, B);
                afficher_resultat("PGCD", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 5:
                A = lire_input("A"); B = lire_input("Modulo N");
                R = Modulo(A, B);
                afficher_resultat("RESTE (A % N)", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 6:
            case 7:
            case 8:
                A = lire_input("Base/Message");
                printf("  > Cle/Exposant (e) : "); scanf("%u", &e);
                B = lire_input("Modulo N");
                R = ExponentiationRapideModulaire(A, e, B);
                afficher_resultat("RESULTAT", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&R);
                pause_prog(); break;

            case 9:
                A = lire_input("Base A"); B = lire_input("Exposant E"); N = lire_input("Modulo N");
                R = ExponentiationRapideModulaire_Maison(A, B, N);
                afficher_resultat("A^E mod N", R);
                libereBigBinary(&A); libereBigBinary(&B); libereBigBinary(&N); libereBigBinary(&R);
                pause_prog(); break;

            case 0:
                printf(C_GREEN "  Fin du programme.\n" C_RESET); break;
        }
    }
    return 0;
}