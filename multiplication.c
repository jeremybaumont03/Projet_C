#include "multiplication.h"

BigBinary Multiplication_Egyptienne(BigBinary A, BigBinary B) {
    // Méthode : Doubler A et Diviser B par 2
    BigBinary resultat = initBigBinary(0, 0); // = 0
    BigBinary tempA = copieBigBinary(A);
    BigBinary tempB = copieBigBinary(B);

    while (tempB.Signe != 0) { // Tant que B > 0
        if (isOdd(tempB)) { // Si B est impair
            BigBinary newRes = Addition(resultat, tempA);
            libereBigBinary(&resultat);
            resultat = newRes;
        }
        
        // A = A * 2
        BigBinary doubleA = multipliePar2(tempA);
        libereBigBinary(&tempA);
        tempA = doubleA;

        // B = B / 2
        divisePar2(&tempB);
    }

    libereBigBinary(&tempA);
    libereBigBinary(&tempB);
    return resultat;
}