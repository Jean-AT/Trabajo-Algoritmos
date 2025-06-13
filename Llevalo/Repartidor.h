#pragma once
#include "User.h"

class Repartidor
{
public:
	Repartidor(int pdistrito, string nnombre);
	void MostrarTdo();
	string getdistrito() {
		if (distrito == 1)
		{
			return "San Miguel";
		}if (distrito == 2)
		{
			return "Barranco";
		}
		else
		{
			return "Jesus Maria";
		}
	};
	void CalcularTarifa();
	string getnombre() { return nombre; };
	double gettarifa() { return tarifa; };
	void toString()
	{
		CalcularTarifa();
		cout << "Nombre: " << nombre << " | Precio: " << tarifa << " | Distrito: " << getdistrito() << endl;
	}
private:
	int distrito;
	string nombre;
	double tarifa;
};

inline Repartidor::Repartidor(int pdistrito, string nnombre) {

	distrito = pdistrito;
	nombre = nnombre;
}

void Repartidor::MostrarTdo()
{
	cout << "------------- Repartidor " << nombre << "----------- " << endl;

}

inline void Repartidor::CalcularTarifa()
{
	switch (distrito)
	{
	case 1:tarifa = 1.50; break;
	case 2:tarifa = 1.30; break;
	case 3:tarifa = 1.10; break;
	default:break;
	}
}
