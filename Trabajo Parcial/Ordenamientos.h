#pragma once
#include <iostream>
#include "Lista.h"
#include "producto.h"

using namespace std;

//QUICKSORT

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

//MERGESORT

void Merge(int* A1, int* A2, int* A, int n) {
    int i = 0, j = 0, k = 0;
    int mitad = n / 2;
    while (i < mitad && j < n - mitad) {
        if (A1[i] < A2[j]) {
            A[k] = A1[i];
            i++; k++;
        }
        else {
            A[k] = A2[j];
            j++; k++;
        }
    }
    while (i < mitad) {
        A[k] = A1[i];
        i++; k++;
    }
    while (j < n - mitad) {
        A[k] = A2[j];
        j++; k++;
    }
}

void mergeSort(int* A, int n) {
    if (n > 1) {
        int mitad = n / 2;
        int* A1 = new int[mitad];
        int* A2 = new int[n - mitad];

        for (int i = 0; i < mitad; i++) {
            A1[i] = A[i];
        }
        for (int i = mitad; i < n; i++) {
            A2[i - mitad] = A[i];
        }
        mergeSort(A1, mitad);
        mergeSort(A2, n - mitad);
        Merge(A1, A2, A, n);
    }
}

//QUICKSELECT

int particion(int* A, int p, int r) {
    int x = A[r]; //el pivote
    int i = p - 1; //indice de los menores
    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[r]);
    return i + 1;
}

int quickselect(int* A, int p, int r, int k) {
    if (p == r) return A[p];
    //indice del pivote con A ordenado Izq(Menores) Der(Mayores) al pivote
    int q = particion(A, p, r);
    int l = q - p + 1; //nro elementos del sub arreglo donde se encuentra el kesimo elemento
    if (k == l)
        return A[q];
    else if (k < l) {
        return quickselect(A, p, q - 1, k);
    }
    else {
        return quickselect(A, q + 1, r, k - l);
    }


}