#pragma once
#include "User.h"
#include "AVL.h"
#include <fstream>
#include <string>

using namespace std;

// Guarda todos los usuarios del arbol en el archivo
inline void guardarUsuariosEnArchivo(AVLTree<User>& arbol, const string& filename) {
    ofstream archivo(filename, ios::trunc);
    if (!archivo.is_open()) return;

    arbol.enOrden([&archivo](User* usuario) {
        archivo << usuario->serializar() << endl;
        });

    archivo.close();
}

// Carga todos los usuarios del archivo y los inserta en el arbol
inline void cargarUsuariosDesdeArchivo(AVLTree<User>& arbol, const string& filename) {
    ifstream archivo(filename);
    string linea;
    while (getline(archivo, linea)) {
        User* usuario = User::deserializar(linea);
        arbol.insertar(usuario);
    }
    archivo.close();
}