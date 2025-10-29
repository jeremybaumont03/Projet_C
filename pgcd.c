#include "pgcd.h"
#include <stdio.h>
#include <stdlib.h>

/* helper: make BigBinary from unsigned long (simple conversion) */
BigBinary from_ulong(unsigned long v) {
    if (v == 0) return creerBigBinaryDepuisChaine("0");
    char buf[256];
    int pos = 0;
    // build binary string MSB..LSB
    unsigned long tmp = v;
    char rev[256];
    int idx = 0;
    while (tmp > 0) {
        rev[idx++] = (tmp & 1UL) ? '1' : '0';
        tmp >>= 1;
    }
    for (int i = 0; i < idx; ++i) buf[i] = rev[idx - 1 - i];
    buf[idx] = '\0';
    return creerBigBinaryDepuisChaine(buf);
}

/* Stein's gcd for BigBinary (using operations defined) */
BigBinary BigBinary_PGCD(BigBinary A, BigBinary B) {
    // handle zeros
    if (BigBinary_isZero(&A)) return copyBigBinary(&B);
    if (BigBinary_isZero(&B)) return copyBigBinary(&A);
    // remove common factors of 2
    int shift = 0;
    while (BigBinary_isEven(&A) && BigBinary_isEven(&B)) {
        BigBinary tmpA = divisePar2(&A);
        libereBigBinary(&A);
        A = tmpA;
        BigBinary tmpB = divisePar2(&B);
        libereBigBinary(&B);
        B = tmpB;
        shift++;
    }
    while (BigBinary_isEven(&A)) {
        BigBinary tmpA = divisePar2(&A);
        libereBigBinary(&A);
        A = tmpA;
    }
    while (!BigBinary_isZero(&B)) {
        while (BigBinary_isEven(&B)) {
            BigBinary tmpB = divisePar2(&B);
            libereBigBinary(&B);
            B = tmpB;
        }
        // now both odd: ensure A <= B
        if (Inferieur(&B, &A)) {
            BigBinary t = copyBigBinary(&A);
            libereBigBinary(&A);
            A = copyBigBinary(&B);
            libereBigBinary(&B);
            B = t;
        }
        // B = B - A
        BigBinary nb = Soustraction(&B, &A);
        libereBigBinary(&B);
        B = nb;
    }
    // result = A * 2^shift -> multiply by repeated multPar2
    BigBinary result = copyBigBinary(&A);
    for (int i = 0; i < shift; ++i) {
        BigBinary tmp = multPar2(&result);
        libereBigBinary(&result);
        result = tmp;
    }
    libereBigBinary(&A);
    return result;
}

/* helper to test with small decimals --> prints binary result (uses from_ulong) */
void print_pgcd_example(unsigned long a_dec, unsigned long b_dec) {
    BigBinary A = from_ulong(a_dec);
    BigBinary B = from_ulong(b_dec);
    BigBinary G = BigBinary_PGCD(A, B);
    printf("PGCD(%lu, %lu) = ", a_dec, b_dec);
    afficheBigBinary(&G);
    printf("\n");
    libereBigBinary(&G);
    libereBigBinary(&A);
    libereBigBinary(&B);
}
