#pragma once
#include "User.h"
#include "Grafo.h"
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;



vector<string> distritos = { "San Miguel","Barranco","Jesus Maria" };
vector<string> nombres = {
    "Valentina", "Mateo", "Sofía", "Sebastián", "Isabella",
    "Lucas", "Camila", "Santiago", "Emma", "Tomás",
    "Martina", "Diego", "Victoria", "Gabriel", "Renata",
    "Emilio", "Antonella", "Daniel", "Lucía", "Andrés"
};

class Repartidor {
public:
    Repartidor(int id); // CONSTRUCTOR

    void MostrarTdo();

    // 🎯 NUEVA FUNCIÓN: calcular tarifa según el grafo
    

    // GETTERS
    string getdistrito() { return distrito; };
    int getCalificacion() { return calificacion; }
    string getnombre() { return nombre; };
    double gettarifa() { return tarifa; };
    int getId() { return id; }
    void settarifa(int tarifa) {
        this->tarifa = tarifa;
    }

private:
    string distrito;
    string nombre;
    double tarifa;
    int calificacion;
    int id;
};

inline Repartidor::Repartidor(int ide) {
    int idxDistrito = rand() % distritos.size();
    int idxNombre = rand() % nombres.size();

    distrito = distritos[idxDistrito];
    nombre = nombres[idxNombre];
    calificacion = rand() % 10 + 1;
    tarifa = 0; // Se asignará luego con calcularTarifa()
    id = ide;
}

// 🎯 Calcular tarifa usando el grafo


inline void Repartidor::MostrarTdo() {
    cout << "\n------------- Repartidor " << getnombre() << " -----------\n";
    cout << " - ID: " << getId() << endl;
    cout << " - Distrito: " << getdistrito() << endl;
    cout << " - Calificación: " << getCalificacion() << endl;
    cout << " - Tarifa: " << gettarifa() << endl;
}
