#ifndef MODEXP_H
#define MODEXP_H

#include "bigbinary.h"

// modexp: calcule (base^exp) mod mod
// exp is an unsigned long long (fits the course constraint)
BigBinary bb_modexp(BigBinary base, unsigned long long exp, BigBinary mod);

#endif
