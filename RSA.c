#include "RSA.h"

BigBinary Chiffrement_RSA(BigBinary M, unsigned int e, BigBinary N) {
    return ExponentiationRapideModulaire(M, e, N);
}

BigBinary Dechiffrement_RSA(BigBinary C, unsigned int d, BigBinary N) {
    return ExponentiationRapideModulaire(C, d, N);
}

BigBinary ExponentiationRapideModulaire_Maison(BigBinary M, BigBinary E, BigBinary N) {
    BigBinary res = initBigBinary(1, 1);
    res.Tdigits[0] = 1;
    
    BigBinary base = Modulo(M, N);
    BigBinary exp = copieBigBinary(E);

    while (exp.Signe != 0) {
        if (isOdd(exp)) {
            BigBinary temp = Multiplication_Egyptienne(res, base);
            libereBigBinary(&res);
            res = Modulo(temp, N);
            libereBigBinary(&temp);
        }
        
        BigBinary sq = Multiplication_Egyptienne(base, base);
        libereBigBinary(&base);
        base = Modulo(sq, N);
        libereBigBinary(&sq);
        
        divisePar2(&exp);
    }
    libereBigBinary(&base);
    libereBigBinary(&exp);
    return res;
}