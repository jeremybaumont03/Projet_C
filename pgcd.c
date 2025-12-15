#include "pgcd.h"

BigBinary BigBinary_PGCD(BigBinary A, BigBinary B) {
    // Algorithme binaire d'Euclide (version soustractive simplifiée)
    BigBinary a = copieBigBinary(A);
    BigBinary b = copieBigBinary(B);

    if (a.Signe == 0) { libereBigBinary(&a); return b; }
    if (b.Signe == 0) { libereBigBinary(&b); return a; }

    // Tant que a != b et qu'ils sont positifs
    while (!Egal(a, b) && a.Signe != 0 && b.Signe != 0) {
        if (Inferieur(a, b)) {
            // b = b - a
            BigBinary diff = Soustraction(b, a);
            libereBigBinary(&b);
            b = diff;
        } else {
            // a = a - b
            BigBinary diff = Soustraction(a, b);
            libereBigBinary(&a);
            a = diff;
        }
    }
    
    // Le PGCD est a (ou b, ils sont égaux)
    libereBigBinary(&b);
    return a;
}