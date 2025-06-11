#pragma once
#include <iostream>
#include "Lista.h"
#include "producto.h"

using namespace std;

int particion(Lista<producto*>* A, int p, int r) {

    double x = A->obtenerPos(r)->getprecio();

    int i = p - 1;

    for (int j = p; j < r; j++) {
        if (A->obtenerPos(j)->getprecio() >= x) {
            i++;
            A->intercambiar(i, j);
        }
    }
    A->intercambiar(i + 1, r);
    return i + 1;
}

void quicksort(Lista<producto*>* A, int p, int r) {
    int q; 

    if (p < r) {
        q = particion(A, p, r); 
        quicksort(A, p, q - 1); 
        quicksort(A, q + 1, r);
    }
}