#ifndef NUCLEO_HPP
#define NUCLEO_HPP
#include <string>

//funciona para comparar cadenas
inline bool terminaEn(std::string const& valor, std::string const& final)
{
    if (valor.size() > final.size()) return false;
    return std::equal(final.rbegin(), final.rend(), valor.rbegin());
}


class Variable{
public:
    enum Tipo {Numero, Cadena};
    //Tipo de variable
    Tipo tipo;
    //Valor real de la variable
    double numero;
    std::string cadena;

    /**
      Reglas de mi lenguaje:
      - Si ambos son número, devuelve un número
      - Si ambos son cadenas, devuelve una cadena
      - Si tipos son distintos, devuelve una cadena
    */
    Variable operator+(const Variable& otro){
        Variable resultado;
        if(this->tipo==Numero && otro.tipo==Numero){
            resultado.tipo = Numero;
            resultado.numero = this->numero + otro.numero;
        }if(this->tipo==Cadena && otro.tipo==Cadena){
            resultado.tipo = Cadena;
            resultado.cadena = this->cadena + otro.cadena;
        }
        return resultado;
    }

    //El resto de funciones están definidas de forma similar

    Variable operator-(const Variable& otro){
        Variable resultado;
        if(this->tipo==Numero && otro.tipo==Numero){
            resultado.tipo = Numero;
            resultado.numero = this->numero - otro.numero;
        }if(this->tipo==Cadena && otro.tipo==Cadena){
            resultado.tipo = Cadena;
            if(terminaEn(cadena, otro.cadena)){
                resultado.cadena = cadena.substr(0, (cadena.size()-1)-otro.cadena.size() );
            }

        }
        return resultado;
    }

};




#endif // NUCLEO_HPP
