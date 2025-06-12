#include <iostream>
#include "Ordenamientos.h"
using namespace std;

int main()
{
	/*Controlador* incio = new Controlador();

	incio->Menu();*/
	int longitud = 0;
	int opt;
	Lista<producto*>* listaProductos;
	listaProductos = new Lista<producto*>();

	producto* p1 = new producto(01,"Producto1", 10.0, 5);
	producto* p2 = new producto(02,"Producto2", 20.0, 3);
	producto* p3 = new producto(03,"Producto3", 15.0, 8);
	producto* p4 = new producto(04,"Producto4", 25.0, 2);
	producto* p5 = new producto(05,"Producto5", 40.0, 3);

	listaProductos->agregaPos(p1, longitud); longitud++;
	listaProductos->agregaPos(p2, longitud); longitud++;
	listaProductos->agregaPos(p3, longitud); longitud++;
	listaProductos->agregaPos(p4, longitud); longitud++;
	listaProductos->agregaPos(p5, longitud);


	ObtenerMasCoB(listaProductos, true);

	/*for (int i = 0; i < listaProductos->longitud(); i++) {
		cout << "Producto: " << listaProductos->obtenerPos(i)->getid() << ", Precio: " << listaProductos->obtenerPos(i)->getprecio() << ", Nombre: " << listaProductos->obtenerPos(i)->getnombre() << endl;
	}*/


}