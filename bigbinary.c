#include "bigbinary.h"
#include <string.h>
#include <stdlib.h>

// --- FONCTIONS INTERNES ---

// Supprime les zéros inutiles au début du nombre (ex: 0011 -> 11)
// Essentiel pour que les comparaisons et opérations restent justes.
void normaliser(BigBinary *nb) {
    // Si le signe est nul, on vide tout (c'est le nombre 0)
    if (nb->Signe == 0) {
        if (nb->Tdigits) free(nb->Tdigits);
        nb->Tdigits = NULL;
        nb->Taille = 0;
        return;
    }

    // Compte le nombre de zéros en tête
    int zeros = 0;
    while (zeros < nb->Taille && nb->Tdigits[zeros] == 0) zeros++;
    
    // Si tout est zéro, on réinitialise à 0
    if (zeros == nb->Taille) {
        libereBigBinary(nb);
        *nb = initBigBinary(0, 0);
    } 
    // Sinon, on réduit la taille du tableau en enlevant les zéros
    else if (zeros > 0) {
        int newTaille = nb->Taille - zeros;
        int *newTab = malloc(sizeof(int) * newTaille);
        // Recopie des bits significatifs
        for(int i=0; i<newTaille; i++) newTab[i] = nb->Tdigits[i + zeros];
        free(nb->Tdigits);
        nb->Tdigits = newTab;
        nb->Taille = newTaille;
    }
}

// --- GESTION MÉMOIRE ---

// Alloue la mémoire pour un nouveau Grand Entier (tableau de bits vide)
BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;
    nb.Signe = signe;
    if (taille > 0) {
        // calloc initialise la mémoire à 0
        nb.Tdigits = calloc(taille, sizeof(int));
    } else {
        nb.Tdigits = NULL;
        nb.Signe = 0;
    }
    return nb;
}

// Libère la mémoire allouée pour éviter les fuites 
void libereBigBinary(BigBinary *nb) {
    if (nb->Tdigits != NULL) free(nb->Tdigits);
    nb->Tdigits = NULL;
    nb->Taille = 0;
    nb->Signe = 0;
}

// Crée une copie indépendante d'un nombre pour ne pas modifier l'original
BigBinary copieBigBinary(BigBinary A) {
    BigBinary copie = initBigBinary(A.Taille, A.Signe);
    if (A.Taille > 0) memcpy(copie.Tdigits, A.Tdigits, sizeof(int) * A.Taille);
    return copie;
}

// Convertit un entier décimal (ex: 14) en sa représentation binaire (ex: 1110)
BigBinary creerBigBinaryDepuisEntier(unsigned long long n) {
    if (n == 0) return initBigBinary(0, 0);

    // Stockage temporaire des bits (max 64 bits pour un long long)
    int tempBits[64];
    int count = 0;

    // Algorithme classique : divisions successives par 2
    while (n > 0) {
        tempBits[count++] = n % 2;
        n /= 2;
    }

    // Création du BigBinary et inversion pour avoir le MSB (Poids fort) au début
    BigBinary res = initBigBinary(count, 1);
    for (int i = 0; i < count; i++) {
        res.Tdigits[i] = tempBits[count - 1 - i];
    }
    return res;
}

// Parse une chaîne de caractères ("1011") pour créer un BigBinary
BigBinary creerBigBinaryDepuisChaine(const char *chaine) {
    int len = strlen(chaine);
    int start = 0;
    
    // Ignore les caractères non '1' au début (ex: les 0 initiaux ou espaces)
    while (start < len && chaine[start] != '1') start++;

    if (start == len) return initBigBinary(0, 0); // C'est 0

    BigBinary nb = initBigBinary(len - start, 1);
    for (int i = 0; i < nb.Taille; i++) {
        // Copie seulement si c'est '1', sinon met 0
        char c = chaine[start + i];
        if (c == '1') nb.Tdigits[i] = 1;
        else nb.Tdigits[i] = 0;
    }
    return nb;
}

// Affiche le nombre binaire bit par bit dans la console
void afficheBigBinary(BigBinary nb) {
    if (nb.Signe == 0) { printf("0"); return; }
    if (nb.Signe == -1) printf("-");
    for (int i = 0; i < nb.Taille; i++) printf("%d", nb.Tdigits[i]);
}

// --- UTILITAIRES LOGIQUES ---

// Vérifie si le nombre est impair 
bool isOdd(BigBinary nb) {
    return (nb.Taille > 0 && nb.Tdigits[nb.Taille - 1] == 1);
}

// Division entière par 2 = Décalage vers la droite (supprime le dernier bit)
void divisePar2(BigBinary *nb) {
    if (nb->Taille > 0) {
        nb->Taille--; // On réduit simplement la taille de 1
        if (nb->Taille == 0) {
            free(nb->Tdigits);
            nb->Tdigits = NULL;
            nb->Signe = 0;
        }
    }
}

// Multiplication par 2 = Décalage vers la gauche (ajoute un 0 à la fin)
BigBinary multipliePar2(BigBinary A) {
    if (A.Signe == 0) return initBigBinary(0, 0);
    BigBinary res = initBigBinary(A.Taille + 1, A.Signe);
    // Copie les bits et met le dernier à 0
    memcpy(res.Tdigits, A.Tdigits, sizeof(int) * A.Taille);
    res.Tdigits[A.Taille] = 0;
    return res;
}

// --- COMPARAISONS ---

// Vérifie l'égalité stricte (Signe, Taille et contenu des bits)
bool Egal(BigBinary A, BigBinary B) {
    if (A.Signe != B.Signe) return false;
    if (A.Taille != B.Taille) return false;
    for (int i = 0; i < A.Taille; i++) {
        if (A.Tdigits[i] != B.Tdigits[i]) return false;
    }
    return true;
}

// Vérifie si A < B
bool Inferieur(BigBinary A, BigBinary B) {
    // Gestion simple des signes et du zéro
    if (A.Signe < B.Signe) return true;
    if (A.Signe > B.Signe) return false;
    if (A.Signe == 0) return false;

    // Comparaison par taille (plus de bits = plus grand)
    if (A.Taille < B.Taille) return true;
    if (A.Taille > B.Taille) return false;

    // Si tailles égales, on compare bit à bit depuis le début (MSB)
    for (int i = 0; i < A.Taille; i++) {
        if (A.Tdigits[i] < B.Tdigits[i]) return true;
        if (A.Tdigits[i] > B.Tdigits[i]) return false;
    }
    return false;
}

bool Superieur(BigBinary A, BigBinary B) {
    return !Inferieur(A, B) && !Egal(A, B);
}

// --- OPÉRATIONS ARITHMÉTIQUES ---

// Additionne deux nombres positifs 
BigBinary Addition(BigBinary A, BigBinary B) {
    int maxT = (A.Taille > B.Taille) ? A.Taille : B.Taille;
    BigBinary res = initBigBinary(maxT + 1, 1); // +1 pour la retenue finale possible
    int retenue = 0;

    // On parcourt de la fin (LSB) vers le début (MSB)
    for (int i = 0; i < maxT; i++) {
        // Récupère le bit s'il existe, sinon 0
        int bitA = (i < A.Taille) ? A.Tdigits[A.Taille - 1 - i] : 0;
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;
        
        int somme = bitA + bitB + retenue;
        res.Tdigits[maxT - i] = somme % 2; // Le bit résultat
        retenue = somme / 2;               // La retenue
    }
    res.Tdigits[0] = retenue; // Dernière retenue
    normaliser(&res); // Enlève les zéros en trop
    return res;
}

// Soustrait B de A (Suppose A >= B) (Méthode avec emprunt/retenue négative)
BigBinary Soustraction(BigBinary A, BigBinary B) {
    BigBinary res = initBigBinary(A.Taille, 1);
    int emprunt = 0;

    for (int i = 0; i < A.Taille; i++) {
        int bitA = A.Tdigits[A.Taille - 1 - i];
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;
        
        int diff = bitA - bitB - emprunt;
        if (diff < 0) {
            diff += 2; // On "emprunte" au bit suivant (base 2)
            emprunt = 1;
        } else {
            emprunt = 0;
        }
        res.Tdigits[A.Taille - 1 - i] = diff;
    }
    normaliser(&res);
    return res;
}