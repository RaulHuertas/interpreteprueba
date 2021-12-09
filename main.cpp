#include <QCoreApplication>
#include "Nucleo.hpp"
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    if(argc!=2){
        cout<<"Invocar con <cmd> <archivo.ry>"<<endl;
        return 0;
    }

//    Variable varA(1.0);
//    Variable varB("hello");
//    cout<<varA<<endl;
//    cout<<varB<<endl;
//    cout<<varA+varB<<endl;
    ContextoRy ctx;
    ctx.cargarArchivo(argv[1]);
    ctx.ejecutar();
    return 0;
}
