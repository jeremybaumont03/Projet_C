#include "RSA.h"

// --- PHASE 3 : CRYPTOGRAPHIE RSA ---

// Chiffre un message M avec la clé publique (e, N).
// Formule mathématique : C = M^e mod N
// Note : Ici, 'e' est un entier standard (unsigned int) comme demandé dans le sujet de base.
BigBinary Chiffrement_RSA(BigBinary M, unsigned int e, BigBinary N) {
    // On réutilise simplement l'exponentiation rapide modulaire codée en Phase 2
    return ExponentiationRapideModulaire(M, e, N); 
}

// Déchiffre un message chiffré C avec la clé privée (d, N).
// Formule mathématique : M = C^d mod N
// Note : C'est exactement la même opération mathématique que le chiffrement.
BigBinary Dechiffrement_RSA(BigBinary C, unsigned int d, BigBinary N) {
    return ExponentiationRapideModulaire(C, d, N);
}

// --- BONUS : EXPONENTIATION AVANCÉE (CLÉS > 64 BITS) ---

// Version "Maison" de l'exponentiation où l'EXPOSANT est aussi un BigBinary.
// Cela permet d'utiliser des clés RSA gigantesques (centaines de bits),
// dépassant la limite des entiers standards (int/long) du C.
BigBinary ExponentiationRapideModulaire_Maison(BigBinary M, BigBinary E, BigBinary N) {
    
    // 1. Initialisation du résultat à 1 (élément neutre de la multiplication)
    BigBinary res = initBigBinary(1, 1);
    res.Tdigits[0] = 1;
    
    // 2. Initialisation de la base (A mod N) et copie de l'exposant
    BigBinary base = Modulo(M, N);
    BigBinary exp = copieBigBinary(E); // Copie pour ne pas détruire l'exposant original

    // 3. Algorithme "Square and Multiply" (Mise au carré et Multiplication)
    // On parcourt les bits de l'exposant jusqu'à ce qu'il soit nul.
    while (exp.Signe != 0) {
        
        // Si le bit actuel de l'exposant est 1 (nombre impair)
        if (isOdd(exp)) {
            // MULTIPLY : On multiplie le résultat courant par la base
            BigBinary temp = Multiplication_Egyptienne(res, base);
            libereBigBinary(&res);
            res = Modulo(temp, N); // On applique le modulo à chaque étape
            libereBigBinary(&temp);
        }
        
        // SQUARE : On met la base au carré pour le tour suivant
        // (base = base * base mod N)
        BigBinary sq = Multiplication_Egyptienne(base, base);
        libereBigBinary(&base);
        base = Modulo(sq, N);
        libereBigBinary(&sq);
        
        // On passe au bit suivant de l'exposant (Division par 2)
        divisePar2(&exp);
    }

    // 4. Nettoyage mémoire
    libereBigBinary(&base);
    libereBigBinary(&exp);
    
    return res;
}