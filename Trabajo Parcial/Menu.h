#include "Cliente.h"
#include "Repartidor.h"
#include "Carrito.h"
#include "Cola_.h"
#include "Ordenamientos.h"
#include <fstream>
#include <iostream>
#include <string>

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

    //Funciones Main
    void Menu();
    void InterfazUsuario();
    void PedidoRealizado();

    double CalcularTotal(Cola<Repartidor*>* Cola_Repartidor, int indx, double precio, int i = 0) {
        //// Validación básica
        //if (Cola_Repartidor == nullptr) {
        //    cout << "Cola no inicializada." << endl;
        //    return -1;
        //}

        //if (i > indx) {
        //    cout << "Índice fuera de rango." << endl;
        //    return -1;
        //}

        //Repartidor* repartidor = Cola_Repartidor->obtenerPos(i);
        //if (repartidor == nullptr) {
        //    cout << "Repartidor no encontrado." << endl;
        //    return -1;
        //}

        if (i == indx) {
            return precio + Cola_Repartidor->obtenerPos(indx)->gettarifa();
        }

        return CalcularTotal(Cola_Repartidor, indx, precio, i + 1);
    }

private:
    Lista<producto*>* List_Comida;
    Lista<producto*>* List_Salud;
    Lista<producto*>* List_Bebidas;
    Cola<Repartidor*>* Cola_Repartidor;
    Lista<producto*>* List_Carrito;
    Lista<producto*>* List_productos;
    Cola<producto*>* Cola_resumen;
    Cliente* user;
    Repartidor* repart;

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

    for (int i = 0; i < 5; i++)
    {
        repart = new Repartidor(nro_Repartidores);
        Cola_Repartidor->encolar(repart);
        nro_Repartidores++;
    }
    


}

inline void Controlador::ListarRepartidores()
{
    cout << "- - - - - - - - - - - - - - - - - - - LISTA DE REPARTIDORES - - - - - - - - - - - - - - - - - - - - -" << endl;

    Repartidor* repar;
    do
    {
        repar = Cola_Repartidor->desencolar();
        repar->MostrarTdo();
    } while (!Cola_Repartidor->esVacia());

    cout << endl << "Elije a tu repartidor:"; cin >> eleccionRepartidor;

    
}

inline void Controlador::RegistrarProducto(int a)
{
    int seleccion;
    int Ordyaf;
    int codigo, codigoBuscado, inventario;
    string nombre;
    float precio;
    bool menor;
    
    bool encontrado = false;
    ifstream nomArch;
    ofstream tempArch;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - SECCIONES DE PRODUCTOS - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "                                                         1.Comida" << endl;
    cout << "                                                         2.Salud" << endl;
    cout << "                                                        3.Bebidas" << endl;
    cin >> seleccion;
    int i = 0;
    
    switch (seleccion)
    {
    case 1:
        i = 0;
        List_productos->eliminarTodo();
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - SECCIONES DE COMIDA - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << endl;
        //ACA VA LA LISTAR SALUD
        nomArch.open("productos_Comida.txt", ios::in);
        if (nomArch.is_open())
        {
            cout << "Lista de productos disponibles:\n";
            cout << "----------------------------------\n";
            while (nomArch >> codigo >> nombre >> precio >> inventario)
            {
                
                
                List_productos->agregaPos(new producto(codigo, nombre, precio, inventario),i);
                i++;
            }

            cout << "Ordenar de mayor a menor? (0 = Si || 1 = No:\n";
            cin >> menor;
            quicksort(List_productos, 0, List_productos->longitud() - 1, menor);
            for (int i = 0; i < List_productos->longitud(); i++)
            {
                
                List_productos->obtenerPos(i)->toString();
            }
            nomArch.close();

            cout << "\nIngrese el código del producto que desea seleccionar: ";
            cin >> codigoBuscado;

            // Leer y actualizar inventario
            nomArch.open("productos_Comida.txt", ios::in);
            tempArch.open("temp.txt", ios::out);

            //

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                if (List_productos->obtenerPos(i)->getid()==codigoBuscado)
                {
                    if (List_productos->obtenerPos(i)->getstock() > 0)
                    {
                        List_productos->obtenerPos(i)->disminuir_stock();
                        cout << "\nProducto seleccionado:\n";
                        cout << "Código: " << List_productos->obtenerPos(i)->getid() << endl;
                        cout << "Nombre: " << List_productos->obtenerPos(i)->getnombre() << endl;
                        cout << "Precio: $" << List_productos->obtenerPos(i)->getprecio() << endl;
                        cout << "Inventario restante: " << List_productos->obtenerPos(i)->getstock() << endl;
                        encontrado = true;
                        List_Carrito->agregaPos(new producto(List_productos->obtenerPos(i)->getid(), List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i)->getprecio(), List_productos->obtenerPos(i)->getstock()), a);
                       

                    }
                    else
                    {
                        cout << "\nProducto sin stock disponible." << endl;
                    }
                }
                tempArch << List_productos->obtenerPos(i)->getid() << " " << List_productos->obtenerPos(i)->getnombre() << " " << List_productos->obtenerPos(i)->getprecio() << " " << List_productos->obtenerPos(i)->getstock() << endl;
            }
            //
                       

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

    case 2:

        i = 0;
        List_productos->eliminarTodo();
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - SECCIONES DE SALUD - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << endl;
        //ACA VA LA LISTAR SALUD
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
            ObtenerMasCoB(List_productos, false);
            cout << "Ordenar de mayor a menor?:\n";
            cin >> menor;
            quicksort(List_productos, 0, List_productos->longitud() - 1, menor);

            for (int i = 0; i < List_productos->longitud(); i++)
            {

                List_productos->obtenerPos(i)->toString();
            }
            nomArch.close();

            cout << "\nIngrese el código del producto que desea seleccionar: ";
            cin >> codigoBuscado;

            // Leer y actualizar inventario
            nomArch.open("productos_Salud.txt", ios::in);
            tempArch.open("temp.txt", ios::out);

            //

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                if (List_productos->obtenerPos(i)->getid() == codigoBuscado)
                {
                    if (List_productos->obtenerPos(i)->getstock() > 0)
                    {
                        List_productos->obtenerPos(i)->disminuir_stock();
                        cout << "\nProducto seleccionado:\n";
                        cout << "Código: " << List_productos->obtenerPos(i)->getid() << endl;
                        cout << "Nombre: " << List_productos->obtenerPos(i)->getnombre() << endl;
                        cout << "Precio: $" << List_productos->obtenerPos(i)->getprecio() << endl;
                        cout << "Inventario restante: " << List_productos->obtenerPos(i)->getstock() << endl;
                        encontrado = true;
                        List_Carrito->agregaPos(new producto(List_productos->obtenerPos(i)->getid(), List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i)->getprecio(), List_productos->obtenerPos(i)->getstock()), a);
                        a++;
                    }
                    else
                    {
                        cout << "\nProducto sin stock disponible." << endl;
                    }
                }
                tempArch << List_productos->obtenerPos(i)->getid() << " " << List_productos->obtenerPos(i)->getnombre() << " " << List_productos->obtenerPos(i)->getprecio() << " " << List_productos->obtenerPos(i)->getstock() << endl;
            }
            //


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
    case 3:

        i = 0;
        List_productos->eliminarTodo();
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - SECCIONES DE BEBIDAS - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << endl;
        //ACA VA LA LISTAR SALUD
        nomArch.open("productos_Bebidas.txt", ios::in);
        if (nomArch.is_open())
        {
            cout << "Lista de productos disponibles:\n";
            cout << "----------------------------------\n";
            while (nomArch >> codigo >> nombre >> precio >> inventario)
            {
                
                
                List_productos->agregaPos(new producto(codigo, nombre, precio, inventario),i);
                i++;
            }
            cout << "Ordenar de mayor a menor?:\n";
            cin >> menor;
            quicksort(List_productos, 0, List_productos->longitud() - 1, menor);

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                
                List_productos->obtenerPos(i)->toString();
            }
            nomArch.close();

            cout << "\nIngrese el código del producto que desea seleccionar: ";
            cin >> codigoBuscado;

            // Leer y actualizar inventario
            nomArch.open("productos_Bebidas.txt", ios::in);
            tempArch.open("temp.txt", ios::out);

            //

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                if (List_productos->obtenerPos(i)->getid()==codigoBuscado)
                {
                    if (List_productos->obtenerPos(i)->getstock() > 0)
                    {
                        List_productos->obtenerPos(i)->disminuir_stock();
                        cout << "\nProducto seleccionado:\n";
                        cout << "Código: " << List_productos->obtenerPos(i)->getid() << endl;
                        cout << "Nombre: " << List_productos->obtenerPos(i)->getnombre() << endl;
                        cout << "Precio: $" << List_productos->obtenerPos(i)->getprecio() << endl;
                        cout << "Inventario restante: " << List_productos->obtenerPos(i)->getstock() << endl;
                        encontrado = true;
                        List_Carrito->agregaPos(new producto(List_productos->obtenerPos(i)->getid(), List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i)->getprecio(), List_productos->obtenerPos(i)->getstock()), a);
                        a++;
                    }
                    else
                    {
                        cout << "\nProducto sin stock disponible." << endl;
                    }
                }
                tempArch << List_productos->obtenerPos(i)->getid() << " " << List_productos->obtenerPos(i)->getnombre() << " " << List_productos->obtenerPos(i)->getprecio() << " " << List_productos->obtenerPos(i)->getstock() << endl;
            }
            //
                       

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
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - REGISTRO DE USUARIO - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "Ingrese su correo :"; cin >> Correo;
    cout << "Ingrese su contraseña :"; cin >> contraseña;
    cout << "Ingrese su nombre:"; cin >> nombre;
    cout << "Ingrese su numero de telefono :"; cin >> telefono;
    cout << "Ingrese su distrito :" << endl;
    cout << "1.San Miguel" << endl;
    cout << "2.Barranco" << endl;
    cout << "3.Jesus Maria" << endl;
    cin >> distrito;
    Cliente* user = new Cliente(nombre, telefono, distrito, Correo);
}

inline void Controlador::Vercarrito()
{
    int eliminar;
    int opccarito;
    cout << "- - - - - - - - - - - - - - - - - - - - - CARRITO - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "1.Terminar Pedido  || 2. Eliminar Pedido" << endl;
    ordenarListaPorPrecio(List_Carrito);

    for (int i = 0; i < List_Carrito->longitud(); i++)
    {
        List_Carrito->obtenerPos(i)->toString();
    }
    cin >> opccarito;
    switch (opccarito)
    {
    case 1:
        system("cls");
        cout << "Resumen del pedido: "<<endl;
        
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

        break;
        
        




    case 2:

        cout << "Ingrese el código del producto a eliminar: "; cin >> eliminar;


        for (int i = 0; i < List_Carrito->longitud(); i++)
        {
            if (List_Carrito->obtenerPos(i)->getid() == eliminar)
            {
                List_Carrito->eliminaPos(i);
            }
            
        }

        break;
    default:
        break;
    }
}

void Controlador::Menu()
{
    int mainmenu;
    
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
    cout << "                                                                                                                     " << endl;

    cout << "                                                  Llevalo" << endl << endl << endl;
    cout << "                                            1.Ingresar Sesion" << endl;
    cout << "                                                  2.Salir" << endl;

    cin >> mainmenu;
    do
    {
        switch (mainmenu)
        {
        case 6:
            ListarRepartidores();
            CalcularTotal(Cola_Repartidor, eleccionRepartidor, 10);

        case 1:

            system("cls");
            RegistarCliente();
            system("cls");
            InterfazUsuario();
            break;

        case 2:
            List_Bebidas = nullptr;
            List_Carrito = nullptr;
            List_Comida = nullptr;
            List_productos = nullptr;
            Cola_Repartidor = nullptr;
            List_Salud = nullptr;
            exit(0);
            break;
        default:

            break;
        }

    } while (0<mainmenu<3);
    
}

inline void Controlador::InterfazUsuario()
{
    int a=0;
    
    int interfaz;
    do
    {
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - BIENVENIDO  - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << endl << endl << endl << endl << endl;
        cout << "                                                          1.Pedir" << endl;
        cout << "                                                        2.Ver Carrito" << endl;
        cout << "                                                      3.Ver Mis Pedidos" << endl;
        cin >> interfaz;
        switch (interfaz)
        {
        case 1:
            system("cls");
            RegistrarProducto(a);
            a++;
            break;
        case 2:
            system("cls");
            Vercarrito();
            break;
        case 3:
            PedidoRealizado();
            break;
        default:
            break;
        }

    } while (0<interfaz<3);
    
}

inline void Controlador::PedidoRealizado()
{
    system("cls");

    double precio= 0.0;
    cout << "- - - - - - - - - - - - - - - - - - - - - PEDIDOS REALIZADOS - - - - - - - - - - - - -" << endl;
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

    cout << " EL TOTAL SERIA :" << precio << " " << eleccionRepartidor<< endl;;
}









