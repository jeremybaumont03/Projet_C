#ifndef PGCD_H
#define PGCD_H

#include "bigbinary.h"

/* PGCD binaire (Stein) sur BigBinary : retourne nouveau BigBinary (à libérer) */
BigBinary BigBinary_PGCD(BigBinary A, BigBinary B);

/* Utilitaire d'affichage et test (pratique) */
void print_pgcd_example(unsigned long a_dec, unsigned long b_dec);

#endif // PGCD_H
