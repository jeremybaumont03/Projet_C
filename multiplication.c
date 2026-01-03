#include "multiplication.h"

// Implémente la multiplication de deux Grands Entiers par la méthode Égyptienne
// Principe : A * B = (2*A) * (B/2) si B est pair
//            A * B = (2*A) * (B/2) + A si B est impair
// Cela permet de ne faire que des additions et des décalages (x2, /2)
BigBinary Multiplication_Egyptienne(BigBinary A, BigBinary B) {
    
    // 1. Initialisation du résultat à 0
    BigBinary resultat = initBigBinary(0, 0); 

    // 2. On travaille sur des copies pour ne pas modifier les nombres originaux A et B
    // (L'utilisateur du main.c veut garder ses variables intactes après le calcul)
    BigBinary tempA = copieBigBinary(A);
    BigBinary tempB = copieBigBinary(B);

    // 3. Boucle principale : Tant que le multiplicateur (B) n'est pas nul
    while (tempB.Signe != 0) { 
        
        // Si B est IMPAIR (le dernier bit est 1)
        // Alors on ajoute la valeur actuelle de A au résultat
        if (isOdd(tempB)) { 
            BigBinary newRes = Addition(resultat, tempA);
            libereBigBinary(&resultat); // On libère l'ancienne mémoire du résultat
            resultat = newRes;          // On met à jour avec la nouvelle somme
        }
        
        // 4. Étape de progression (Algorithme Égyptien)
        
        // On DOUBLE A (Décalage gauche : A = A * 2)
        BigBinary doubleA = multipliePar2(tempA);
        libereBigBinary(&tempA); // Nettoyage ancienne valeur
        tempA = doubleA;

        // On DIVISE B par 2 (Décalage droite : B = B / 2)
        // C'est ce qui nous permet d'avancer vers 0 et finir la boucle
        divisePar2(&tempB);
    }

    // 5. Nettoyage final : on libère les copies temporaires
    libereBigBinary(&tempA);
    libereBigBinary(&tempB);

    return resultat; // On renvoie le produit calculé
}