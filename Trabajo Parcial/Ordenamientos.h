#pragma once
#include <iostream>
#include "Lista.h"
#include "producto.h"

using namespace std;

int particion(Lista<producto*>* A, int p, int r) {
    // El pivote es el precio del producto en la posición 'r' de la lista.
    double x = A->obtenerPos(r)->getprecio();

    // 'i' es el índice de los elementos que son MAYORES o IGUALES al pivote.
    // Estos elementos se moverán al lado izquierdo de la partición.
    int i = p - 1;

    for (int j = p; j < r; j++) {
        // Para ordenar de MAYOR A MENOR, movemos los elementos
        // cuyo precio es MAYOR o IGUAL al pivote hacia la izquierda.
        if (A->obtenerPos(j)->getprecio() >= x) { // ¡Cambiado a >= para orden descendente!
            i++;
            // Intercambiamos los punteros a los productos dentro de la lista.
            // ASUMIMOS que tu clase Lista tiene un método 'intercambiar' o similar.
            A->intercambiar(i, j);
        }
    }
    // Colocamos el pivote en su posición final correcta.
    A->intercambiar(i + 1, r);
    return i + 1;
}

void quicksort(Lista<producto*>* A, int p, int r) {
    int q; // Para almacenar el indice del pivote

    if (p < r) {
        q = particion(A, p, r); // Devuelve el indice del pivote
        quicksort(A, p, q - 1); // Ordena la sub-lista izquierda
        quicksort(A, q + 1, r); // Ordena la sub-lista derecha
    }
}