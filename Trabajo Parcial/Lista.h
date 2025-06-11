#include "Nodo.h"
#include <functional>
using namespace std;

typedef unsigned int uint;
template <class T>
class Lista {
private:
    Nodo<T>* nodo;
    Nodo<T>* ini;
    uint lon; // número de elementos en la lista

public:
    Lista() : ini(nullptr), lon(0) { nodo = new Nodo<T>(); }
    ~Lista();
    /*Lista() {
        nodo = new Nodo<T>();
    };*/
    Lista(Nodo<T>* pNodo) {
        nodo = pNodo;
    };
    uint longitud();
    bool esVacia();
    void agregaInicial(T elem);
    void agregaPos(T elem, uint pos);
    void agregaFinal(T elem);
    void modificarInicial(T elem);
    void modificarPos(T elem, uint pos);
    void modificarFinal(T elem);
    void eliminaInicial();
    void eliminaPos(uint pos);
    void eliminaFinal();
    void eliminarTodo();
    void ordenar(function<bool(T, T)> compardor);
    T obtenerInicial();
    T obtenerPos(uint pos);
    T obtenerFinal();
    T buscar(function<bool(T)> predicado);
    void foreach(function<void(T)> funcion);
    void removeIf(function<bool(T)> predicado);


    //~Lista(void);
    void agregar(T pNodo); //sumar nodos a la lista
    bool esvacia(void);
    T cabeza(void); //retorna el dato del primer nodo
    Lista* resto(void); //retorna el puntero al "resto" de la lista
                        //resto= lo que queda de la lista sin la cabeza
    T suma(T i);
    int size();
    void borrar(void); //borra la cabeza
    void borrar_last();//borra el ultimo
    void concat(Lista<T>* l1);// le transfiere los datos de l1 a this
    Lista<T>* copy(void);// hace una copia de la lista
    void tomar(int n);//deja "vivos" los n primeros nodos y borra el resto
    void addOrdenado(T d);// suma nodos ordenados de menor a mayor
    bool esta(T d); //retorna true cuando d est� en la lista
    void borrarDato(T d);// borra el nodo que contiene a d

};
template <class T>
uint Lista<T>::longitud() {
    return lon;
}
template <class T>
bool Lista<T>::esVacia() {
    return lon == 0;
}
template <class T>
void Lista<T>::agregaInicial(T elem) {
    Nodo<T>* nuevo = new Nodo<T>(elem);
    if (nuevo != nullptr) {
        ini = nuevo;
        lon++;
    }
}
template <class T>
T Lista<T>::buscar(function<bool(T)> predicado) {
    Nodo<T>* aux = ini;
    while (aux != nullptr) {
        if (predicado(aux->get_Elem())) {
            return aux->get_Elem();
        }
        aux = aux->get_Sgte();
    }
    return nullptr; 
}

template <class T>
void Lista<T>::agregaPos(T elem, uint pos) {
    if (pos > lon) return;
    if (pos == 0) {
        agregaInicial(elem);
    }
    else {
        Nodo<T>* aux = ini;
        for (int i = 1; i < pos; i++) {
            aux = aux->get_Sgte();
        }
        Nodo<T>* nuevo = new Nodo<T>(elem);
        nuevo->set_Sgte(aux->get_Sgte());
        if (nuevo != nullptr) {

            aux->set_Sgte(nuevo);
            lon++;
        }
    }
}
template <class T>
void Lista<T>::agregaFinal(T elem) {
    agregarPos(elem, lon);
}

template <class T>
void Lista<T>::eliminaInicial() {
    if (lon > 0) {
        Nodo<T>* aux = ini;
        ini = ini->sgte;
        delete aux;
        lon--;
    }
}

template <class T>
void Lista<T>::eliminaPos(uint pos) {
    if (pos >= lon) return; // Fuera de rango

    Nodo<T>* anterior = ini;
    for (uint i = 1; i < pos; ++i) {
        anterior = anterior->get_Sgte();
    }
    Nodo<T>* aEliminar = anterior->get_Sgte();
    anterior->set_Sgte(aEliminar->get_Sgte());
    delete aEliminar;
    lon--;
}
template <class T>
void Lista<T>::eliminaFinal() {
}

template <class T>
void Lista<T>::modificarInicial(T elem) {
    if (lon > 0) {
        ini->elem = elem;
    }
}
template <class T>
void  Lista<T>::eliminarTodo()
{
    Nodo<T>* actual = ini;
    while (actual != nullptr) {
        Nodo<T>* temp = actual;
        actual = actual->get_Sgte();
        delete temp;
    }
    ini = nullptr;
    lon = 0;
}

template <class T>
void Lista<T>::modificarPos(T elem, uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo<T>* aux = ini;
        for (int i = 0; i < pos; i++) {
            aux = aux->get_Sgte();
        }
        aux->set_Elem(elem);
    }
}
template <class T>
void Lista<T>::modificarFinal(T elem) {
    modificar(elem, lon - 1);
}

template <class T>
T Lista<T>::obtenerInicial() {
    return obtenerPos(0);
}


template <class T>
T Lista<T>::obtenerPos(uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo<T>* aux = ini;
        for (int i = 0; i < pos; i++) {
            aux = aux->get_Sgte();
        }
        return aux->get_Elem();
    }
    else {
        return NULL;
    }
}
template <class T>
T Lista<T>::obtenerFinal() {
    return obtenerPos(lon - 1);
}

template <class T>
void Lista<T>::agregar(T d) //100
{
    Nodo<T>* nuevo = new Nodo<T>(d);
    nuevo->set_Sgte(nodo);
    nodo = nuevo;
}

template <typename T>
void Lista<T>::ordenar(std::function<bool(T, T)> comparador) {
    if (esVacia()) return;
    bool cambio;
    do {
        cambio = false;
        for (int i = 0; i < longitud() - 1; i++) {
            T a = obtenerPos(i);
            T b = obtenerPos(i + 1);
            if (comparador(a, b)) {
                modificarPos(b, i);
                modificarPos(a, i + 1);
                cambio = true;
            }
        }
    } while (cambio);
}

template <class T>
void Lista<T>::foreach(function<void(T)> funcion){
    Nodo<T>* aux = ini;
    while (aux != nullptr) {
        func(aux->get_Elem());
        aux = aux->get_Sgte();
    }
}

template <class T>
void Lista<T>::removeIf(function<bool(T)> predicado) {
	Nodo<T>* actual = ini;
	Nodo<T>* anterior = nullptr;
	while (actual != nullptr) {
		if (predicado(actual->get_Elem())) {
			if (anterior == nullptr) {
				// Eliminar el primer nodo
				ini = actual->get_Sgte();
				delete actual;
				actual = ini;
			}
			else {
				// Eliminar un nodo intermedio o final
				anterior->set_Sgte(actual->get_Sgte());
				delete actual;
				actual = anterior->get_Sgte();
			}
			lon--;
		}
		else {
			anterior = actual;
			actual = actual->get_Sgte();
		}
	}
}
