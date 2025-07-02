#include "Cliente.h"
#include "Repartidor.h"
#include "Carrito.h"
#include "Cola_.h"
#include "Ordenamientos.h"
#include <fstream>
#include <iostream>
#include "TablaHash.h"
#include <string>
#include "User.h"
#include "AVL.h"
#include "GestorUsuarios.h"
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

// Constantes para teclas
const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_ENTER = 13;
const int KEY_ESC = 27;
const int KEY_SPECIAL = 224;

// Colores para el menú
const WORD COLOR_SELECCIONADO = BACKGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD COLOR_NORMAL = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

// Función para ocultar/mostrar el cursor
void mostrarCursor(bool visible) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

// Función para mostrar menú con selección interactiva
int mostrarMenuInteractivo(const string titulo, const vector<string>& opciones, int opcionActual = 0) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int seleccion = opcionActual;

    mostrarCursor(false); // Ocultar cursor durante la navegación

    while (true) {
        system("cls");
        cout << titulo << endl << endl;

        for (int i = 0; i < opciones.size(); i++) {
            if (i == seleccion) {
                SetConsoleTextAttribute(hConsole, COLOR_SELECCIONADO);
                cout << "> " << opciones[i] << endl;
                SetConsoleTextAttribute(hConsole, COLOR_NORMAL);
            }
            else {
                cout << "  " << opciones[i] << endl;
            }
        }

        int tecla = _getch();
        if (tecla == KEY_SPECIAL) {
            tecla = _getch();
            switch (tecla) {
            case KEY_UP:
                seleccion--;
                if (seleccion < 0) seleccion = opciones.size() - 1;
                break;
            case KEY_DOWN:
                seleccion++;
                if (seleccion >= opciones.size()) seleccion = 0;
                break;
            }
        }
        else if (tecla == KEY_ENTER) {
            mostrarCursor(true); // Mostrar cursor al seleccionar
            system("cls"); // Limpiar pantalla al seleccionar
            return seleccion;
        }
        else if (tecla == KEY_ESC) {
            mostrarCursor(true); // Mostrar cursor al salir
            return -1; // Indica que se quiere salir
        }
    }
}

// Función para mostrar el logo
void mostrarLogo() {
    
    cout << "                          "; cout << "                       -=                " << endl;
    cout << "                          "; cout << "               .     -##:                " << endl;
    cout << "                          "; cout << "              :=     -###=          =:   " << endl;
    cout << "                          "; cout << "             +*      *####+.         *+  " << endl;
    cout << "                          "; cout << "            +#:      #######+.       :#+ " << endl;
    cout << "                          "; cout << "           -##.      -########.      .##-" << endl;
    cout << "                          "; cout << "           *##:       =#######*      :##*" << endl;
    cout << "                          "; cout << "           ###*        .+######      *###" << endl;
    cout << "                          "; cout << "           *###*         .*###+     *###*" << endl;
    cout << "                          "; cout << "           -#####-        :##*    -#####-" << endl;
    cout << "                          "; cout << "            +######=:     :#=  :=######+ " << endl;
    cout << "                          "; cout << "             =#########*++#+*#########=  " << endl;
    cout << "                          "; cout << "              :*####################*:   " << endl;
    cout << "                          "; cout << "                .=################=:     " << endl;
    cout << "                          "; cout << "                   :=*##########*+.      " << endl;
    cout << endl;
    cout << "                                                  Llevalo" << endl << endl << endl;
}

void ordenarListaPorPrecio(Lista<producto*>* lista) {
    if (lista->esVacia()) return;

    bool cambio;
    do {
        cambio = false;
        for (int i = 0; i < lista->longitud() - 1; i++) {
            producto* prod1 = lista->obtenerPos(i);
            producto* prod2 = lista->obtenerPos(i + 1);
            if (prod1->getprecio() < prod2->getprecio()) {
                lista->modificarPos(prod2, i);
                lista->modificarPos(prod1, i + 1);
                cambio = true;
            }
        }
    } while (cambio);
}

string leerContrasena() {
    string contrasena;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' es Enter
        if (ch == '\b') { // Retroceso
            if (!contrasena.empty()) {
                contrasena.pop_back();
                cout << "\b \b";
            }
        }
        else {
            contrasena += ch;
            cout << '*';
        }
    }
    cout << endl;
    return contrasena;
}

class Controlador
{
public:
    Controlador();
    //Funciones para repartidores
    void ListarRepartidores();
    //Funciones Para Productos
    void RegistrarProducto(int a);
    //Funciones Para Cliente
    void RegistarCliente();
    void Vercarrito();
    //Funciones de usuario
    void RegistrarUsuario();
    bool IniciarSesion();
    //Funciones Main
    void Menu();
    void InterfazUsuario();
    void PedidoRealizado();

    double CalcularTotal(Cola<Repartidor*>* Cola_Repartidor, int indx, double precio, int i = 0) {
        if (i == indx) {
            return precio + Cola_Repartidor->obtenerPos(indx)->gettarifa();
        }

        return CalcularTotal(Cola_Repartidor, indx, precio, i + 1);
    }
    void GuardarHistorial() {
        ofstream archivo("historial_Productos.txt", ios::app);
        if (archivo.is_open()) {
            for (int i = 0; i < List_Carrito->longitud(); i++) {
                archivo << List_Carrito->obtenerPos(i)->getid() << " "
                    << List_Carrito->obtenerPos(i)->getnombre() << " "
                    << List_Carrito->obtenerPos(i)->getprecio() << " "
                    << List_Carrito->obtenerPos(i)->getstock() << endl;
            }
            archivo.close();
        }
        else {
            cout << "No se pudo abrir el archivo de historial." << endl;
        }
    }
    void VerHistorial();

    void GenerarArbolBalanceado();
private:
    Lista<producto*>* List_Comida;
    Lista<producto*>* List_Salud;
    Lista<producto*>* List_Bebidas;
    Cola<Repartidor*>* Cola_Repartidor;
    Lista<producto*>* List_Carrito;
    Lista<producto*>* List_productos;
    Cola<producto*>* Cola_resumen;
    Lista<producto*>* Lista_Historial;

    Cliente* user;
    Repartidor* repart;
    AVLArbol<User> arbolUsuarios;
    AVLArbol<producto> arbol_historial;
    int nro_Productos = 5;
    int nro_Repartidores = 0;
    int nro_Carrito;
    int eleccionRepartidor;
};

inline Controlador::Controlador()
{
    srand(time(0));
    List_Bebidas = new Lista<producto*>();
    List_Carrito = new Lista<producto*>();
    List_Comida = new Lista<producto*>();
    List_Salud = new Lista<producto*>();
    Cola_Repartidor = new Cola<Repartidor*>();
    List_productos = new Lista<producto*>();
    Cola_resumen = new Cola<producto*>();
    Lista_Historial = new Lista<producto*>();

    for (int i = 0; i < 5; i++)
    {
        repart = new Repartidor(nro_Repartidores);
        Cola_Repartidor->encolar(repart);
        nro_Repartidores++;
    }

    cargarUsuariosDesdeArchivo(arbolUsuarios, "usuarios.txt");
}

inline void Controlador::ListarRepartidores()
{
    // Crear vector con las opciones de repartidores
    vector<string> opcionesRepartidores;
    Cola<Repartidor*>* temp_Cola = new Cola<Repartidor*>();

    cout << "- - - - - - - - - - - - - LISTA DE REPARTIDORES - - - - - - - - - - - - -" << endl;

    while (!Cola_Repartidor->esVacia()) {
        Repartidor* repar = Cola_Repartidor->desencolar();
        opcionesRepartidores.push_back("Repartidor " + to_string(repar->getId()) +
            " - Tarifa: $" + to_string(repar->gettarifa()));
        temp_Cola->encolar(repar);
    }

    // Reinsertar repartidores en la cola original
    while (!temp_Cola->esVacia()) {
        Cola_Repartidor->encolar(temp_Cola->desencolar());
    }
    delete temp_Cola;

    // Mostrar menú interactivo
    eleccionRepartidor = mostrarMenuInteractivo("Elije a tu repartidor:", opcionesRepartidores);
    if (eleccionRepartidor == -1) eleccionRepartidor = 0; // Valor por defecto si presiona ESC
}

inline void Controlador::RegistrarProducto(int a)
{
    vector<string> opcionesSecciones = {
        "Comida",
        "Salud",
        "Bebidas"
    };

    int seleccionSeccion = mostrarMenuInteractivo("- - - - - - - - - - - SECCIONES DE PRODUCTOS - - - - - - - - - - -", opcionesSecciones);
    if (seleccionSeccion == -1) return; // Si presionó ESC

    int codigo, codigoBuscado, inventario;
    string nombre;
    float precio;
    bool menor;
    int stockelegido = 0;
    bool encontrado = false;
    ifstream nomArch;
    ofstream tempArch;
    string nombreBuscar;

    int i = 0;

    switch (seleccionSeccion + 1) // +1 porque el vector empieza en 0
    {
    case 1: // Comida
        i = 0;
        system("cls");
        List_productos->eliminarTodo();
        cout << "- - - - - - - - - - - - - SECCIONES DE COMIDA - - - - - - - - - - - - -" << endl;
        cout << endl;

        nomArch.open("productos_Comida.txt", ios::in);
        if (nomArch.is_open())
        {
            cout << "Lista de productos disponibles:\n";
            cout << "----------------------------------\n";
            while (nomArch >> codigo >> nombre >> precio >> inventario)
            {
                List_productos->agregaPos(new producto(codigo, nombre, precio, inventario), i);
                i++;
            }

            vector<string> opciones = {
                "Ordenar de mayor a menor/menor a mayor",
                "Ver productos mas Baratos o Caros"
            };
            int seleccionOpciones = mostrarMenuInteractivo("Opciones de ordenamiento:", opciones) + 1;
            vector<string> opcionesOrden = {
                            "Mayor a Menor (0)",
                            "Menor a Mayor (1)"
            };

            vector<string> opcionesPrecios = {
                "Mas caros",
                "Mas baratos"
            };
            switch (seleccionOpciones)
            {
            case 1:{
                int seleccionOrden = mostrarMenuInteractivo("Seleccione el orden de los productos:", opcionesOrden);
                menor = seleccionOrden;
                quicksort(List_productos, 0, List_productos->longitud() - 1, menor);
                break;}
            case 2:{
                int selecionPrecio = mostrarMenuInteractivo("Seleccione lo deseado:", opcionesPrecios);
                menor = selecionPrecio;
                int indiceBaratosCaros;
                cout << "Cuantos productos desea ver? (Ingrese un numero):\n";
                cin >> indiceBaratosCaros;
                ObtenerMasCoB(List_productos, menor, indiceBaratosCaros);
                cout << "\nVolviendo a mostrar la lista...\n";}
            }

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                List_productos->obtenerPos(i)->toString();
            }
            nomArch.close();

            // Hash Table de productos
            HashTablaString* ht = new HashTablaString(100);
            for (int i = 0; i < List_productos->longitud(); i++)
            {
                ht->insertar(List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i));
            }

            while (true)
            {
                cout << "Desea ingresar un producto por nombre? (1 = Si || 0 = No): ";
                cin >> menor;
                if (menor == 1)
                {
                    std::cout << "\nBuscar nombre: ";
                    std::cin >> nombreBuscar;

                    int index = ht->buscar(nombreBuscar);
                    if (index != -1)
                        std::cout << nombreBuscar << " se encuentra en la posicion " << index << std::endl;
                    else
                        std::cout << nombreBuscar << " no se encontro en la tabla." << std::endl;
                }
                else
                {
                    break;
                }
            }
            delete ht;

            cout << "\nIngrese el código del producto que desea seleccionar: ";
            cin >> codigoBuscado;
            cout << "Ingrese la cantidad de stock que desea: ";
            cin >> stockelegido;

            // Leer y actualizar inventario 
            nomArch.open("productos_Comida.txt", ios::in);
            tempArch.open("temp.txt", ios::out);

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                if (List_productos->obtenerPos(i)->getid() == codigoBuscado)
                {
                    if (List_productos->obtenerPos(i)->getstock() > 0)
                    {
                        List_productos->obtenerPos(i)->disminuir_stock(stockelegido);
                        cout << "\nProducto seleccionado:\n";
                        cout << "Código: " << List_productos->obtenerPos(i)->getid() << endl;
                        cout << "Nombre: " << List_productos->obtenerPos(i)->getnombre() << endl;
                        cout << "Precio: $" << List_productos->obtenerPos(i)->getprecio() << endl;
                        cout << "Inventario restante: " << List_productos->obtenerPos(i)->getstock() << endl;
                        encontrado = true;
                        List_Carrito->agregaPos(new producto(List_productos->obtenerPos(i)->getid(),
                            List_productos->obtenerPos(i)->getnombre(),
                            List_productos->obtenerPos(i)->getprecio(),
                            List_productos->obtenerPos(i)->getstock()), a);
                    }
                    else
                    {
                        cout << "\nProducto sin stock disponible." << endl;
                    }
                }
                tempArch << List_productos->obtenerPos(i)->getid() << " "
                    << List_productos->obtenerPos(i)->getnombre() << " "
                    << List_productos->obtenerPos(i)->getprecio() << " "
                    << List_productos->obtenerPos(i)->getstock() << endl;
            }

            nomArch.close();
            tempArch.close();

            // Reemplazar archivo original
            remove("productos_Comida.txt");
            rename("temp.txt", "productos_Comida.txt");

            if (!encontrado)
            {
                cout << "Producto con código " << codigoBuscado << " no encontrado o sin inventario." << endl;
            }
        }
        else
        {
            cout << "No se pudo abrir el archivo." << endl;
        }
        break;

    case 2: // Salud
        system("cls");
        i = 0;
        List_productos->eliminarTodo();
        cout << "- - - - - - - - - - - - - SECCIONES DE SALUD - - - - - - - - - - - - -" << endl;
        cout << endl;

        nomArch.open("productos_Salud.txt", ios::in);
        if (nomArch.is_open())
        {
            cout << "Lista de productos disponibles:\n";
            cout << "----------------------------------\n";
            while (nomArch >> codigo >> nombre >> precio >> inventario)
            {
                List_productos->agregaPos(new producto(codigo, nombre, precio, inventario), i);
                i++;
            }

            vector<string> opciones = {
                "Ordenar de mayor a menor/menor a mayor",
                "Ver productos mas Baratos o Caros"
            };
            int seleccionOpciones = mostrarMenuInteractivo("Opciones de ordenamiento:", opciones) + 1;
            vector<string> opcionesOrden = {
                "Mayor a Menor (0)",
                "Menor a Mayor (1)"
            };

            vector<string> opcionesPrecios = {
                "Mas caros",
                "Mas baratos"
            };
            switch (seleccionOpciones)
            {
            case 1: {
                int seleccionOrden = mostrarMenuInteractivo("Seleccione el orden de los productos:", opcionesOrden);
                menor = seleccionOrden;
                quicksort(List_productos, 0, List_productos->longitud() - 1, menor);
                break;
            }
            case 2: {
                int selecionPrecio = mostrarMenuInteractivo("Seleccione lo deseado:", opcionesPrecios);
                menor = selecionPrecio;
                int indiceBaratosCaros;
                cout << "Cuantos productos desea ver? (Ingrese un numero):\n";
                cin >> indiceBaratosCaros;
                ObtenerMasCoB(List_productos, menor, indiceBaratosCaros);
                cout << "\nVolviendo a mostrar la lista...\n";
            }
            }

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                List_productos->obtenerPos(i)->toString();
            }
            nomArch.close();

            // Hash Table de productos
            HashTablaString* ht = new HashTablaString(100);
            for (int i = 0; i < List_productos->longitud(); i++)
            {
                ht->insertar(List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i));
            }

            while (true)
            {
                cout << "Desea ingresar un producto por nombre? (1 = Si || 0 = No): ";
                cin >> menor;
                if (menor == 1)
                {
                    std::cout << "\nBuscar nombre: ";
                    std::cin >> nombreBuscar;

                    int index = ht->buscar(nombreBuscar);
                    if (index != -1)
                        std::cout << nombreBuscar << " se encuentra en la posicion " << index << std::endl;
                    else
                        std::cout << nombreBuscar << " no se encontro en la tabla." << std::endl;
                }
                else
                {
                    break;
                }
            }
            delete ht;

            cout << "\nIngrese el código del producto que desea seleccionar: ";
            cin >> codigoBuscado;
            cout << "Ingrese la cantidad de stock que desea: ";
            cin >> stockelegido;

            // Leer y actualizar inventario
            nomArch.open("productos_Salud.txt", ios::in);
            tempArch.open("temp.txt", ios::out);

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                if (List_productos->obtenerPos(i)->getid() == codigoBuscado)
                {
                    if (List_productos->obtenerPos(i)->getstock() > 0)
                    {
                        List_productos->obtenerPos(i)->disminuir_stock(stockelegido);
                        cout << "\nProducto seleccionado:\n";
                        cout << "Código: " << List_productos->obtenerPos(i)->getid() << endl;
                        cout << "Nombre: " << List_productos->obtenerPos(i)->getnombre() << endl;
                        cout << "Precio: $" << List_productos->obtenerPos(i)->getprecio() << endl;
                        cout << "Inventario restante: " << List_productos->obtenerPos(i)->getstock() << endl;
                        encontrado = true;
                        List_Carrito->agregaPos(new producto(List_productos->obtenerPos(i)->getid(),
                            List_productos->obtenerPos(i)->getnombre(),
                            List_productos->obtenerPos(i)->getprecio(),
                            List_productos->obtenerPos(i)->getstock()), a);
                        a++;
                    }
                    else
                    {
                        cout << "\nProducto sin stock disponible." << endl;
                    }
                }
                tempArch << List_productos->obtenerPos(i)->getid() << " "
                    << List_productos->obtenerPos(i)->getnombre() << " "
                    << List_productos->obtenerPos(i)->getprecio() << " "
                    << List_productos->obtenerPos(i)->getstock() << endl;
            }

            nomArch.close();
            tempArch.close();

            // Reemplazar archivo original
            remove("productos_Salud.txt");
            rename("temp.txt", "productos_Salud.txt");

            if (!encontrado)
            {
                cout << "Producto con código " << codigoBuscado << " no encontrado o sin inventario." << endl;
            }
        }
        else
        {
            cout << "No se pudo abrir el archivo." << endl;
        }
        break;

    case 3: // Bebidas
        system("cls");
        i = 0;
        List_productos->eliminarTodo();
        cout << "- - - - - - - - - - - - - SECCIONES DE BEBIDAS - - - - - - - - - - - - -" << endl;
        cout << endl;

        nomArch.open("productos_Bebidas.txt", ios::in);
        if (nomArch.is_open())
        {
            cout << "Lista de productos disponibles:\n";
            cout << "----------------------------------\n";
            while (nomArch >> codigo >> nombre >> precio >> inventario)
            {
                List_productos->agregaPos(new producto(codigo, nombre, precio, inventario), i);
                i++;
            }

            vector<string> opciones = {
                "Ordenar de mayor a menor/menor a mayor",
                "Ver productos mas Baratos o Caros"
            };
            int seleccionOpciones = mostrarMenuInteractivo("Opciones de ordenamiento:", opciones) + 1;
            vector<string> opcionesOrden = {
                "Mayor a Menor (0)",
                "Menor a Mayor (1)"
            };

            vector<string> opcionesPrecios = {
                "Mas caros",
                "Mas baratos"
            };
            switch (seleccionOpciones)
            {
            case 1: {
                int seleccionOrden = mostrarMenuInteractivo("Seleccione el orden de los productos:", opcionesOrden);
                menor = seleccionOrden;
                quicksort(List_productos, 0, List_productos->longitud() - 1, menor);
                break;
            }
            case 2: {
                int selecionPrecio = mostrarMenuInteractivo("Seleccione lo deseado:", opcionesPrecios);
                menor = selecionPrecio;
                int indiceBaratosCaros;
                cout << "Cuantos productos desea ver? (Ingrese un numero):\n";
                cin >> indiceBaratosCaros;
                ObtenerMasCoB(List_productos, menor, indiceBaratosCaros);
                cout << "\nVolviendo a mostrar la lista...\n";
            }
            }

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                List_productos->obtenerPos(i)->toString();
            }
            nomArch.close();

            // Hash Table de productos
            HashTablaString* ht = new HashTablaString(100);
            for (int i = 0; i < List_productos->longitud(); i++)
            {
                ht->insertar(List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i));
            }

            while (true)
            {
                cout << "Desea ingresar un producto por nombre? (1 = Si || 0 = No): ";
                cin >> menor;
                if (menor == 1)
                {
                    std::cout << "\nBuscar nombre: ";
                    std::cin >> nombreBuscar;

                    int index = ht->buscar(nombreBuscar);
                    if (index != -1)
                        std::cout << nombreBuscar << " se encuentra en la posicion " << index << std::endl;
                    else
                        std::cout << nombreBuscar << " no se encontro en la tabla." << std::endl;
                }
                else
                {
                    break;
                }
            }
            delete ht;

            cout << "\nIngrese el código del producto que desea seleccionar: ";
            cin >> codigoBuscado;
            cout << "Ingrese la cantidad de stock que desea: ";
            cin >> stockelegido;

            // Leer y actualizar inventario
            nomArch.open("productos_Bebidas.txt", ios::in);
            tempArch.open("temp.txt", ios::out);

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                if (List_productos->obtenerPos(i)->getid() == codigoBuscado)
                {
                    if (List_productos->obtenerPos(i)->getstock() > 0)
                    {
                        List_productos->obtenerPos(i)->disminuir_stock(stockelegido);
                        cout << "\nProducto seleccionado:\n";
                        cout << "Código: " << List_productos->obtenerPos(i)->getid() << endl;
                        cout << "Nombre: " << List_productos->obtenerPos(i)->getnombre() << endl;
                        cout << "Precio: $" << List_productos->obtenerPos(i)->getprecio() << endl;
                        cout << "Inventario restante: " << List_productos->obtenerPos(i)->getstock() << endl;
                        encontrado = true;
                        List_Carrito->agregaPos(new producto(List_productos->obtenerPos(i)->getid(),
                            List_productos->obtenerPos(i)->getnombre(),
                            List_productos->obtenerPos(i)->getprecio(),
                            List_productos->obtenerPos(i)->getstock()), a);
                        a++;
                    }
                    else
                    {
                        cout << "\nProducto sin stock disponible." << endl;
                    }
                }
                tempArch << List_productos->obtenerPos(i)->getid() << " "
                    << List_productos->obtenerPos(i)->getnombre() << " "
                    << List_productos->obtenerPos(i)->getprecio() << " "
                    << List_productos->obtenerPos(i)->getstock() << endl;
            }

            nomArch.close();
            tempArch.close();

            // Reemplazar archivo original
            remove("productos_Bebidas.txt");
            rename("temp.txt", "productos_Bebidas.txt");

            if (!encontrado)
            {
                cout << "Producto con código " << codigoBuscado << " no encontrado o sin inventario." << endl;
            }
        }
        else
        {
            cout << "No se pudo abrir el archivo." << endl;
        }
        break;
    }
}

inline void Controlador::RegistarCliente()
{
    string Correo, contraseña, nombre;
    int telefono;
    string distrito;
    cout << "- - - - - - - - - - - - - REGISTRO DE USUARIO - - - - - - - - - - - - -" << endl;
    cout << "Ingrese su correo :"; cin >> Correo;
    cout << "Ingrese su contraseña :"; cin >> contraseña;
    cout << "Ingrese su nombre:"; cin >> nombre;
    cout << "Ingrese su numero de telefono :"; cin >> telefono;

    vector<string> opcionesDistritos = {
        "San Miguel",
        "Barranco",
        "Jesus Maria"
    };
    int seleccionDistrito = mostrarMenuInteractivo("Ingrese su distrito:", opcionesDistritos);
    if (seleccionDistrito == -1) seleccionDistrito = 0;

    distrito = opcionesDistritos[seleccionDistrito];
    Cliente* user = new Cliente(nombre, telefono, distrito, Correo);
}

inline void Controlador::RegistrarUsuario() {
    string nombre, correo, contraseña, distrito;
    int telefono;
    cout << "Nombre: "; cin >> nombre;
    cout << "Telefono: "; cin >> telefono;

    vector<string> opcionesDistritos = {
        "San Miguel",
        "Barranco",
        "Jesus Maria"
    };
    int seleccionDistrito = mostrarMenuInteractivo("Seleccione su distrito:", opcionesDistritos);
    if (seleccionDistrito == -1) seleccionDistrito = 0;
    distrito = opcionesDistritos[seleccionDistrito];

    cout << "Correo: "; cin >> correo;
    cout << "Contraseña: ";
    contraseña = leerContrasena();

    // Verifica si ya existe el usuario
    if (arbolUsuarios.buscar(nombre) != nullptr) {
        cout << "El usuario ya existe." << endl;
        return;
    }

    User* nuevo = new User(nombre, telefono, distrito, correo, contraseña);
    arbolUsuarios.insertar(nuevo);
    guardarUsuariosEnArchivo(arbolUsuarios, "usuarios.txt");
    cout << "Usuario registrado correctamente." << endl;
}

bool Controlador::IniciarSesion() {
    string nombre, contraseña;
    cout << "Nombre: "; cin >> nombre;
    cout << "Contraseña: ";
    contraseña = leerContrasena();

    User* usuario = arbolUsuarios.buscar(nombre);
    if (usuario && usuario->getContraseña() == contraseña) {
        cout << "¡Bienvenido, " << usuario->getNombre() << "!" << endl;
        return true;
    }
    else {
        cout << "Usuario o contraseña incorrectos." << endl;
        return false;
    }
}

inline void Controlador::Vercarrito()
{
    bool menor = false;
    int eliminar;

    if (List_Carrito == nullptr || List_Carrito->longitud() == 0) {
        cout << "El carrito está vacío." << endl;
        system("pause");
        return;
    }

    cout << "- - - - - - - - - - - - - CARRITO - - - - - - - - - - - - -" << endl;

    cout << "Ordenar de mayor a menor(0: Mayor A Menor || 1: Menor a Mayor):\n";
    cin >> menor;
    mergeSort(List_Carrito, 0, List_Carrito->longitud() - 1, menor);

    for (int i = 0; i < List_Carrito->longitud(); i++)
    {
        List_Carrito->obtenerPos(i)->toString();
    }

    vector<string> opcionesCarrito = {
        "Elegir repartidor",
        "Eliminar Producto"
    };
    int opccarito = mostrarMenuInteractivo("Opciones del carrito:", opcionesCarrito) + 1;

    switch (opccarito)
    {
    case 1:
        system("cls");
        cout << "Resumen del pedido: " << endl;

        for (int i = 0; i < List_Carrito->longitud(); i++)
        {
            Cola_resumen->encolar(new producto(List_Carrito->obtenerPos(i)->getid(),
                List_Carrito->obtenerPos(i)->getnombre(), List_Carrito->obtenerPos(i)->getprecio(), List_Carrito->obtenerPos(i)->getstock()));
        }

        producto* Producto;
        do
        {
            Producto = Cola_resumen->desencolar();
            Producto->toString();
        } while (!Cola_resumen->esVacia());

        ListarRepartidores();
        system("pause");
        break;

    case 2:
        cout << "Ingrese el código del producto a eliminar: ";
        cin >> eliminar;

        for (int i = 0; i < List_Carrito->longitud(); i++)
        {
            if (List_Carrito->obtenerPos(i)->getid() == eliminar)
            {
                List_Carrito->eliminaPos(i);
            }
        }
        break;
    }
    system("pause");
}

void Controlador::Menu()
{
    vector<string> opciones = {
        "Ingresar Sesion",
        "Registrar Nuevo Usuario",
        "Salir"
    };

    // Mostrar logo solo una vez al principio
    

    while (true) {
        int seleccion = mostrarMenuInteractivo("MENU PRINCIPAL", opciones);
        
        switch (seleccion) {
        case 0: // Ingresar Sesion
            if (IniciarSesion()) {
                InterfazUsuario();
            }
            else {
                // Mostrar mensaje de error y volver al menú
                cout << "Usuario o contraseña incorrectos. Presione cualquier tecla para continuar...";
                _getch();
            }
            break;
        case 1: // Registrar Nuevo Usuario
            RegistrarUsuario();
            cout << "Usuario registrado exitosamente. Presione cualquier tecla para continuar...";
            _getch();
            break;
        case 2: // Salir
        case -1: // ESC presionado
            // Limpiar memoria
            List_Bebidas = nullptr;
            List_Carrito = nullptr;
            List_Comida = nullptr;
            List_productos = nullptr;
            Cola_Repartidor = nullptr;
            List_Salud = nullptr;
            exit(0);
            break;
        }
    }
}

inline void Controlador::InterfazUsuario()
{
    int a = 0;
    vector<string> opciones = {
        "Pedir",
        "Ver Carrito",
        "Ver Mis Pedidos",
        "Ver Historial de compra",
        "Salir"
    };

    while (true) {
        int seleccion = mostrarMenuInteractivo("- - - - - - - - - - - - - BIENVENIDO - - - - - - - - - - - - -", opciones);

        switch (seleccion) {
        case 0:
            system("cls");
            RegistrarProducto(a);
            a++;
            break;
        case 1:
            system("cls");
            Vercarrito();
            break;
        case 2:
            PedidoRealizado();
            break;
        case 3:
            VerHistorial();
            GenerarArbolBalanceado();
            break;
        case 4:
        case -1: // ESC presionado
            cout << "\nSaliendo del programa..";
            exit(0);
            break;
        }
    }
}

inline void Controlador::PedidoRealizado()
{
    system("cls");

    if (List_Carrito->esVacia())
    {
        cout << "No hay productos en el carrito." << endl;
        system("pause");
        return;
    }

    double precio = 0.0;
    cout << "- - - - - - - - - - - - - PEDIDOS REALIZADOS - - - - - - - - - - - - -" << endl;
    cout << "Resumen del pedido: " << endl;

    for (int i = 0; i < List_Carrito->longitud(); i++)
    {
        Cola_resumen->encolar(new producto(List_Carrito->obtenerPos(i)->getid(),
            List_Carrito->obtenerPos(i)->getnombre(), List_Carrito->obtenerPos(i)->getprecio(), List_Carrito->obtenerPos(i)->getstock()));
        precio += List_Carrito->obtenerPos(i)->getprecio();
    }

    producto* Producto;
    do
    {
        Producto = Cola_resumen->desencolar();
        Producto->toString();
    } while (!Cola_resumen->esVacia());

    double total = CalcularTotal(Cola_Repartidor, eleccionRepartidor, precio);

    GuardarHistorial();

    cout << " EL TOTAL SERIA :" << total << endl;
    system("pause");
}

inline void Controlador::VerHistorial()
{
    system("cls");
    int codigo, inventario;
    string nombre;
    float precio;
    ifstream nomArch("historial_productos.txt", ios::in);

    Lista_Historial->eliminarTodo();

    if (nomArch.is_open()) {
        int i = 0;
        while (nomArch >> codigo >> nombre >> precio >> inventario) {
            producto* p = new producto(codigo, nombre, precio, inventario);
            Lista_Historial->agregaPos(p, i);
            i++;
        }
        nomArch.close();
    }
    else {
        cout << "No se pudo abrir el archivo historial_productos.txt\n";
    }
    system("pause");
}

inline void Controlador::GenerarArbolBalanceado()
{
    if (Lista_Historial->esVacia()) {
        cout << "El historial de productos está vacío. No se puede generar un árbol AVL." << endl;
        return;
    }
    cout << "\n--- Generando Árbol AVL de Historial de Productos ---\n";
    for (int i = 0; i < Lista_Historial->longitud(); ++i) {
        producto* p = Lista_Historial->obtenerPos(i);
        arbol_historial.insertar(p);
    }
    cout << "Árbol AVL del historial de productos generado correctamente.\n";
    cout << "\n--- Contenido del Árbol AVL (ordenado por nombre) ---\n";
    arbol_historial.enOrden([](producto* p) {
        cout << "  - " << p->getnombre() << " (ID: " << p->getid() << ", Precio: $" << p->getprecio() << ")\n";
        });
    cout << "-----------------------------------------------------\n";
    system("pause");
}