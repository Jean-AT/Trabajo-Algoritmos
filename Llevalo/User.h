#pragma once
#include <iostream>
#include <string>

using namespace std;
class User
{
public:
	User();
	User(string pnombre, int pnumero, string pdistrito, string pcorreo);
	~User();
	void ListarCLiente();
	string getNombre() { return nombre; };
	string getDistrito();
protected:
	string nombre;
	int numero;
	string distrito;
	string correo;
	string contrase�a;
};

User::User()
{
	nombre = "";
	numero = 0;
	distrito = "";
	correo = "";
	contrase�a = "";
}

User::User(string pnombre, int pnumero, string pdistrito, string pcorreo){
	nombre = pnombre;
	numero = pnumero;
	distrito = pdistrito;
	correo = pcorreo;
}

User::~User()
{
}

inline void User::ListarCLiente()
{
	cout << "------------- Cliente "<< nombre <<"----------- " << endl;
	cout << "Correo:" << correo;
	cout << "Contrase�a:" << contrase�a;
	cout << "Distrito:" << distrito;
	cout << "Numero de Contacto:" << numero;
}

inline string User::getDistrito()
{
	return distrito;
}
