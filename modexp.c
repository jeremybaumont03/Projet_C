#include "modexp.h"

BigBinary Modulo(BigBinary A, BigBinary N) {
    if (N.Signe == 0) return initBigBinary(0, 0);
    if (Inferieur(A, N)) return copieBigBinary(A);

    BigBinary R = copieBigBinary(A);
    BigBinary D = copieBigBinary(N);

    // Alignement (recherche de k)
    while (Inferieur(D, R) || Egal(D, R)) {
        BigBinary temp = multipliePar2(D);
        if (Superieur(temp, R)) {
            libereBigBinary(&temp);
            break;
        }
        libereBigBinary(&D);
        D = temp;
    }

    // Soustractions successives
    while (Egal(D, N) || Superieur(D, N)) {
        if (!Inferieur(R, D)) {
            BigBinary temp = Soustraction(R, D);
            libereBigBinary(&R);
            R = temp;
        }
        divisePar2(&D);
    }
    
    libereBigBinary(&D);
    return R;
}

BigBinary ExponentiationRapideModulaire(BigBinary A, unsigned int e, BigBinary N) {
    BigBinary res = initBigBinary(1, 1);
    res.Tdigits[0] = 1; // On force à 1
    
    BigBinary base = Modulo(A, N);

    while (e > 0) {
        if (e % 2 == 1) {
            BigBinary temp = Multiplication_Egyptienne(res, base);
            libereBigBinary(&res);
            res = Modulo(temp, N);
            libereBigBinary(&temp);
        }
        
        BigBinary sq = Multiplication_Egyptienne(base, base);
        libereBigBinary(&base);
        base = Modulo(sq, N);
        libereBigBinary(&sq);
        
        e /= 2;
    }
    libereBigBinary(&base);
    return res;
}