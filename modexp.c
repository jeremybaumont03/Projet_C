#include "pgcd.h"
#include <stdio.h>

// Helper: compare to zero
static bool bb_eq_zero(const BigBinary *a) {
    return bb_is_zero(a);
}

// multiply by 2^k (fast shift left k times)
static BigBinary bb_shifleft_k(const BigBinary *a, size_t k) {
    BigBinary r = bb_copy(a);
    for (size_t i = 0; i < k; ++i) {
        BigBinary tmp = bb_shiftl1(&r);
        bb_free(&r);
        r = tmp;
    }
    return r;
}

// Stein's algorithm (binary gcd) using BigBinary operations
BigBinary bb_gcd(BigBinary a, BigBinary b) {
    // handle zero cases
    if (bb_is_zero(&a)) {
        BigBinary r = bb_copy(&b);
        return r;
    }
    if (bb_is_zero(&b)) {
        BigBinary r = bb_copy(&a);
        return r;
    }
    // remove common factors of 2
    size_t shift = 0;
    while (bb_is_even(&a) && bb_is_even(&b)) {
        BigBinary ta = bb_shiftr1(&a);
        BigBinary tb = bb_shiftr1(&b);
        bb_free(&a); a = ta;
        bb_free(&b); b = tb;
        shift++;
    }
    // make a odd
    while (bb_is_even(&a)) {
        BigBinary ta = bb_shiftr1(&a);
        bb_free(&a); a = ta;
    }
    while (!bb_is_zero(&b)) {
        while (bb_is_even(&b)) {
            BigBinary tb = bb_shiftr1(&b);
            bb_free(&b); b = tb;
        }
        // now both odd
        int cmp = bb_cmp(&a, &b);
        if (cmp > 0) {
            // a > b
            BigBinary tmp = bb_sub(&a, &b);
            bb_free(&a);
            a = tmp;
            // make a odd
            while (bb_is_even(&a)) {
                BigBinary ta = bb_shiftr1(&a);
                bb_free(&a); a = ta;
            }
        } else if (cmp < 0) {
            BigBinary tmp = bb_sub(&b, &a);
            bb_free(&b);
            b = tmp;
            while (bb_is_even(&b)) {
                BigBinary tb = bb_shiftr1(&b);
                bb_free(&b); b = tb;
            }
        } else {
            // equal
            break;
        }
    }
    // result = a << shift
    BigBinary res = a;
    if (shift > 0) {
        BigBinary x = bb_shifleft_k(&res, shift);
        bb_free(&res);
        res = x;
    }
    bb_free(&b);
    return res;
}

void afficher_pgcd_binaire_int(int a, int b) {
    BigBinary A = bb_from_uint64((uint64_t)a);
    BigBinary B = bb_from_uint64((uint64_t)b);
    BigBinary G = bb_gcd(A, B);
    printf("PGCD(%d, %d) = ", a, b);
    // convert G to decimal if it fits in u64
    int ok;
    uint64_t val = bb_to_u64(&G, &ok);
    if (ok) printf("%llu", (unsigned long long)val);
    else printf("(grand entier)");
    printf("\nPGCD en binaire: ");
    bb_print_binary(&G);
    printf("\n");
    bb_free(&A);
    bb_free(&B);
    bb_free(&G);
}
