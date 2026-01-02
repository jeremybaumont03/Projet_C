#include "bigbinary.h"
#include <string.h>
#include <stdlib.h>

// --- FONCTIONS INTERNES ---
void normaliser(BigBinary *nb) {
    if (nb->Signe == 0) {
        if (nb->Tdigits) free(nb->Tdigits);
        nb->Tdigits = NULL;
        nb->Taille = 0;
        return;
    }
    int zeros = 0;
    while (zeros < nb->Taille && nb->Tdigits[zeros] == 0) zeros++;
    
    if (zeros == nb->Taille) {
        libereBigBinary(nb);
        *nb = initBigBinary(0, 0);
    } else if (zeros > 0) {
        int newTaille = nb->Taille - zeros;
        int *newTab = malloc(sizeof(int) * newTaille);
        for(int i=0; i<newTaille; i++) newTab[i] = nb->Tdigits[i + zeros];
        free(nb->Tdigits);
        nb->Tdigits = newTab;
        nb->Taille = newTaille;
    }
}

// --- GESTION MEMOIRE ---

BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;
    nb.Signe = signe;
    if (taille > 0) {
        nb.Tdigits = calloc(taille, sizeof(int));
    } else {
        nb.Tdigits = NULL;
        nb.Signe = 0;
    }
    return nb;
}

void libereBigBinary(BigBinary *nb) {
    if (nb->Tdigits != NULL) free(nb->Tdigits);
    nb->Tdigits = NULL;
    nb->Taille = 0;
    nb->Signe = 0;
}

BigBinary copieBigBinary(BigBinary A) {
    BigBinary copie = initBigBinary(A.Taille, A.Signe);
    if (A.Taille > 0) memcpy(copie.Tdigits, A.Tdigits, sizeof(int) * A.Taille);
    return copie;
}

// NOUVEAU : Conversion Decimal -> Binaire pour le projet
BigBinary creerBigBinaryDepuisEntier(unsigned long long n) {
    if (n == 0) return initBigBinary(0, 0);

    // Un unsigned long long fait max 64 bits
    int tempBits[64];
    int count = 0;

    // Conversion en binaire (on obtient les bits à l'envers : LSB en premier)
    while (n > 0) {
        tempBits[count++] = n % 2;
        n /= 2;
    }

    // On crée le BigBinary (MSB en premier)
    BigBinary res = initBigBinary(count, 1);
    for (int i = 0; i < count; i++) {
        res.Tdigits[i] = tempBits[count - 1 - i]; // On inverse
    }
    return res;
}

BigBinary creerBigBinaryDepuisChaine(const char *chaine) {
    int len = strlen(chaine);
    int start = 0;
    while (start < len && chaine[start] != '1') start++;

    if (start == len) return initBigBinary(0, 0);

    BigBinary nb = initBigBinary(len - start, 1);
    for (int i = 0; i < nb.Taille; i++) {
        char c = chaine[start + i];
        if (c == '1') nb.Tdigits[i] = 1;
        else nb.Tdigits[i] = 0;
    }
    return nb;
}

void afficheBigBinary(BigBinary nb) {
    if (nb.Signe == 0) { printf("0"); return; }
    if (nb.Signe == -1) printf("-");
    for (int i = 0; i < nb.Taille; i++) printf("%d", nb.Tdigits[i]);
}

// --- HELPERS ---

bool isOdd(BigBinary nb) {
    return (nb.Taille > 0 && nb.Tdigits[nb.Taille - 1] == 1);
}

void divisePar2(BigBinary *nb) {
    if (nb->Taille > 0) {
        nb->Taille--;
        if (nb->Taille == 0) {
            free(nb->Tdigits);
            nb->Tdigits = NULL;
            nb->Signe = 0;
        }
    }
}

BigBinary multipliePar2(BigBinary A) {
    if (A.Signe == 0) return initBigBinary(0, 0);
    BigBinary res = initBigBinary(A.Taille + 1, A.Signe);
    memcpy(res.Tdigits, A.Tdigits, sizeof(int) * A.Taille);
    res.Tdigits[A.Taille] = 0;
    return res;
}

// --- OPÉRATIONS ---

bool Egal(BigBinary A, BigBinary B) {
    if (A.Signe != B.Signe) return false;
    if (A.Taille != B.Taille) return false;
    for (int i = 0; i < A.Taille; i++) {
        if (A.Tdigits[i] != B.Tdigits[i]) return false;
    }
    return true;
}

bool Inferieur(BigBinary A, BigBinary B) {
    if (A.Signe < B.Signe) return true;
    if (A.Signe > B.Signe) return false;
    if (A.Signe == 0) return false;

    if (A.Taille < B.Taille) return true;
    if (A.Taille > B.Taille) return false;

    for (int i = 0; i < A.Taille; i++) {
        if (A.Tdigits[i] < B.Tdigits[i]) return true;
        if (A.Tdigits[i] > B.Tdigits[i]) return false;
    }
    return false;
}

bool Superieur(BigBinary A, BigBinary B) {
    return !Inferieur(A, B) && !Egal(A, B);
}

BigBinary Addition(BigBinary A, BigBinary B) {
    int maxT = (A.Taille > B.Taille) ? A.Taille : B.Taille;
    BigBinary res = initBigBinary(maxT + 1, 1);
    int retenue = 0;

    for (int i = 0; i < maxT; i++) {
        int bitA = (i < A.Taille) ? A.Tdigits[A.Taille - 1 - i] : 0;
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;
        int somme = bitA + bitB + retenue;
        res.Tdigits[maxT - i] = somme % 2;
        retenue = somme / 2;
    }
    res.Tdigits[0] = retenue;
    normaliser(&res);
    return res;
}

BigBinary Soustraction(BigBinary A, BigBinary B) {
    BigBinary res = initBigBinary(A.Taille, 1);
    int emprunt = 0;

    for (int i = 0; i < A.Taille; i++) {
        int bitA = A.Tdigits[A.Taille - 1 - i];
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;
        
        int diff = bitA - bitB - emprunt;
        if (diff < 0) {
            diff += 2;
            emprunt = 1;
        } else {
            emprunt = 0;
        }
        res.Tdigits[A.Taille - 1 - i] = diff;
    }
    normaliser(&res);
    return res;
}