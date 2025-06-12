#pragma once
#include <iostream>
#include "Lista.h"
#include "producto.h"

using namespace std;

//QUICKSORT
//Se usa para ordenar por precio

int particion(Lista<producto*>* A, int p, int r,bool menor) {

    double x = A->obtenerPos(r)->getprecio();

    int i = p - 1;

    if (!menor)
    {
        for (int j = p; j < r; j++) {
            if (A->obtenerPos(j)->getprecio() >= x) {
                i++;
                A->intercambiar(i, j);
            }
        }
    }else{
        for (int j = p; j < r; j++) {
            if (A->obtenerPos(j)->getprecio() <= x) {
                i++;
                A->intercambiar(i, j);
            }
        }
    }
    
    A->intercambiar(i + 1, r);
    return i + 1;
}

void quicksort(Lista<producto*>* A, int p, int r,bool menor) {
    int q; 

    if (p < r) {
        q = particion(A, p, r,menor); 
        quicksort(A, p, q - 1,menor); 
        quicksort(A, q + 1, r,menor);
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

int particionSelect(Lista<producto*>* A, int p, int r,bool menor) {
    int x = A->obtenerPos(r)->getprecio(); //el pivote
    int i = p - 1; //indice de los menores
    if (menor) {
        // Quiero los más baratos ? precios menores a la izquierda
        for (int j = p; j < r; j++) {
            if (A->obtenerPos(j)->getprecio() <= x) {
                i++;
                A->intercambiar(i, j);
            }
        }
    }
    else {
        // Quiero los más caros ? precios mayores a la izquierda
        for (int j = p; j < r; j++) {
            if (A->obtenerPos(j)->getprecio() >= x) {
                i++;
                A->intercambiar(i, j);
            }
        }
    }
    A->intercambiar(i+1,r);
    return i + 1;
}

int quickselect(Lista<producto*>* A, int p, int r, int k,bool menor) {
    if (p == r) return A->obtenerPos(p)->getprecio();
    //indice del pivote con A ordenado Izq(Menores) Der(Mayores) al pivote
    int q = particionSelect(A, p, r,menor);
    int l = q - p + 1; //nro elementos del sub arreglo donde se encuentra el kesimo elemento
    if (k == l)
        return A->obtenerPos(q)->getprecio();
    else if (k < l) {
        return quickselect(A, p, q - 1, k,menor);
    }
    else {
        return quickselect(A, q + 1, r, k - l, menor);
    }

}

void ObtenerMasCoB(Lista<producto*>* A, bool menor) {
    int indx = 2;
    producto* p;
    // Paso 1: Reorganiza para que los k estén al inicio
    quickselect(A, 0, A->longitud() - 1, indx, menor);

    // Paso 2: Ordena esos k productos
    quicksort(A, 0, indx - 1, menor);


    for (int i = 0; i < indx; i++)
    {
        p = A->obtenerPos(i);
        cout << "Producto: " << p->getid() << ", Precio: " << p->getprecio() << ", Nombre: " << p->getnombre() << endl;
    }
}