#include "modexp.h"

// --- CALCUL DU MODULO (A % N) ---
// Méthode : Division Euclidienne Binaire (similaire à la division posée à la main).
// Complexité : Rapide, évite les soustractions naïves infinies.
BigBinary Modulo(BigBinary A, BigBinary N) {
    // Cas de base
    if (N.Signe == 0) return initBigBinary(0, 0); // Erreur div par 0
    if (Inferieur(A, N)) return copieBigBinary(A); // A est déjà le reste

    BigBinary R = copieBigBinary(A); // R = Reste courant
    BigBinary D = copieBigBinary(N); // D = Diviseur qu'on va décaler

    // 1. ALIGNEMENT : On décale N vers la gauche (x2) pour qu'il "colle" à A
    // (On cherche le plus grand multiple de N * 2^k <= A)
    while (Inferieur(D, R) || Egal(D, R)) {
        BigBinary temp = multipliePar2(D);
        if (Superieur(temp, R)) {
            libereBigBinary(&temp);
            break; // On a dépassé A, on s'arrête
        }
        libereBigBinary(&D);
        D = temp;
    }

    // 2. SOUSTRACTION : On redescend bit par bit
    // On divise D par 2 à chaque tour et on soustrait si possible.
    while (Egal(D, N) || Superieur(D, N)) {
        if (!Inferieur(R, D)) { // Si R >= D
            BigBinary temp = Soustraction(R, D);
            libereBigBinary(&R); // On met à jour le reste
            R = temp;
        }
        divisePar2(&D); // Décalage à droite (bit suivant)
    }
    
    libereBigBinary(&D);
    return R;
}



// --- EXPONENTIATION RAPIDE (A^e % N) ---
// Méthode : "Square and Multiply" (Mise au carré et Multiplication).
// Intérêt : Permet de calculer des puissances énormes en un temps record (logarithmique).
BigBinary ExponentiationRapideModulaire(BigBinary A, unsigned int e, BigBinary N) {
    
    // Résultat initialisé à 1
    BigBinary res = initBigBinary(1, 1);
    res.Tdigits[0] = 1; 
    
    // On travaille modulo N dès le départ
    BigBinary base = Modulo(A, N);

    // Boucle sur chaque bit de l'exposant 'e'
    while (e > 0) {
        
        // MULTIPLY : Si le bit est 1 (impair), on accumule dans le résultat
        if (e % 2 == 1) {
            BigBinary temp = Multiplication_Egyptienne(res, base);
            libereBigBinary(&res);
            res = Modulo(temp, N); // Toujours modulo N !
            libereBigBinary(&temp);
        }
        
        // SQUARE : On met la base au carré pour le bit suivant (x -> x^2 -> x^4...)
        BigBinary sq = Multiplication_Egyptienne(base, base);
        libereBigBinary(&base);
        base = Modulo(sq, N); // Toujours modulo N !
        libereBigBinary(&sq);
        
        // Bit suivant (division par 2 de l'exposant)
        e /= 2;
    }

    libereBigBinary(&base);
    return res;
}