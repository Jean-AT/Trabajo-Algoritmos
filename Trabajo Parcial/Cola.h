#pragma once
#include "NodoC.h"
#include <iostream>
using namespace std;
template <class T>
class Cola
{
private:
	NodoC<T>* inicio;
	NodoC<T>* fin;
public:
	Cola()
	{
		this->inicio = NULL;
		this->fin = NULL;
	}
	void encolar(T v);
	T desencolar();
	bool esVacia();
	T obtenerPos(int pos);
};

//IMPLEMENTACI�N

template <class T>
bool Cola<T>::esVacia()
{
	return (inicio == NULL);
}

template <class T>
void Cola<T>::encolar(T v)
{
	NodoC<T>* nodo = new NodoC<T>(v);
	if (esVacia())
	{
		inicio = nodo;
		fin = inicio;
	}
	else
	{
		fin->siguiente = nodo;
		fin = nodo;
	}

	nodo = NULL;
}

template <class T>
T Cola<T>::desencolar()
{
	T dato = inicio->dato;
	if (inicio == fin)
	{
		inicio = NULL;
		fin = NULL;
	}
	else
	{
		inicio = inicio->siguiente;
	}
	return dato;
}

template <class T>
T Cola<T>::obtenerPos(int pos)
{
	if (esVacia()) {
		throw std::out_of_range("La cola est� vac�a.");
	}

	NodoC<T>* actual = inicio;
	int contador = 0;

	while (actual != nullptr) {
		if (contador == pos) {
			return actual->dato;
		}
		actual = actual->siguiente;
		contador++;
	}

	throw std::out_of_range("�ndice fuera de rango.");
}
