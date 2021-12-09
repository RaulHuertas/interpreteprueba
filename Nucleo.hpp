#ifndef NUCLEO_HPP
#define NUCLEO_HPP
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <QStringList>
#include <queue>
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
    Variable() = default;
    Variable(const double& numero){
        tipo = Numero;
        this->numero = numero;
    }
    Variable(const std::string& cadena){
        tipo = Cadena;
        this->cadena = cadena;
    }
    Variable(const Variable& otro){
        tipo = otro.tipo;
        numero = otro.numero;
        cadena = otro.cadena;
    }
    /**
      Reglas de mi lenguaje:
      - Si ambos son número, devuelve un número
      - Si ambos son cadenas, devuelve una cadena
      - Si tipos son distintos, devuelve una cadena
    */
    Variable operator+(const Variable& otro)const{
        Variable resultado;
        if(this->tipo==Numero && otro.tipo==Numero){
            resultado.tipo = Numero;
            resultado.numero = this->numero + otro.numero;
        }else if(this->tipo==Cadena && otro.tipo==Cadena){
            resultado.tipo = Cadena;
            resultado.cadena = this->cadena + otro.cadena;
        }else if(this->tipo==Numero && otro.tipo==Cadena){
            resultado.tipo = Cadena;
            resultado.cadena = std::to_string(numero) + otro.cadena;
        }else if(this->tipo==Cadena && otro.tipo==Numero){
            resultado.tipo = Cadena;
            resultado.cadena = cadena + std::to_string(otro.numero);
        }
        return resultado;
    }

    //El resto de funciones están definidas de forma similar
    //Casos especiales, como <cadena>/<cadena> se resuelven devolviendo el
    //primer operando
    Variable operator-(const Variable& otro)const{
        Variable resultado;
        if(this->tipo==Numero && otro.tipo==Numero){
            resultado.tipo = Numero;
            resultado.numero = this->numero - otro.numero;
        }else if(this->tipo==Cadena && otro.tipo==Cadena){
            resultado.tipo = Cadena;
            if(terminaEn(cadena, otro.cadena)){
                resultado.cadena = cadena.substr(0, (cadena.size()-1)-otro.cadena.size() );
            }
        }
        return resultado;
    }

    Variable operator*(const Variable& otro)const{
        Variable resultado;
        if(this->tipo==Numero && otro.tipo==Numero){
            resultado.tipo = Numero;
            resultado.numero = this->numero * otro.numero;
        }else if(this->tipo==Cadena && otro.tipo==Numero){
            resultado.tipo = Cadena;
            for(int j = 0; j<otro.numero; j++){
                resultado.cadena += this->cadena;
            }
        }else{
            resultado.tipo = tipo;
            resultado.cadena = cadena;
            resultado.numero = numero;
        }
        return resultado;
    }

    Variable operator/(const Variable& otro)const{
        Variable resultado;
        if(this->tipo==Numero && otro.tipo==Numero){
            resultado.tipo = Numero;
            resultado.numero = this->numero / otro.numero;
        }else{
            resultado.tipo = tipo;
            resultado.cadena = cadena;
            resultado.numero = numero;
        }
        return resultado;
    }

    bool operator==(const Variable& otro)const{
        if(tipo!=otro.tipo){return false;}
        if(tipo==Numero){
            return numero==otro.numero;
        }else{
            return cadena==otro.cadena;
        }
        return true;
    }

    Variable& operator=(const Variable& otro){
        tipo = otro.tipo;
        numero = otro.numero;
        cadena = otro.cadena;
        return *this;
    }

};

class Operacion{
public:
    enum Tipo {Pass, Print, IniciarPlot, FinPlot, AsignarVar, AsignarLiteral, Invocar, Sumar, Restar, Multiplicar, Dividir, Sin, Sqrt};
    Tipo tipo = Pass;
    //Tipo tipo2 = Pass;
    std::string op1, op2, op3;
    //Variable res;
    Variable literal;
    Operacion()=default;
    Operacion(Tipo tipo, std::string op1){
        this->tipo = tipo;
        this->op1 = op1;
    }
    Operacion(Tipo tipo, std::string op1, std::string op2){
        this->tipo = tipo;
        this->op1 = op1;
        this->op2 = op2;
    }
    Operacion(Tipo tipo, std::string op1, Variable op2){
        this->tipo = tipo;
        this->op1 = op1;
        this->literal = op2;
    }
//    Operacion(Tipo tipo, std::string op1, std::string op2, std::string op3, Tipo tipo2){
//        this->tipo = tipo;
//        this->tipo2 = tipo2;
//        this->op1 = op1;
//        this->op2 = op2;
//        this->op3 = op3;
//    }
    Operacion(Variable lit){
        this->tipo = AsignarLiteral;
        literal = lit;
    }
};

class Funcion{
public:
    std::string nombre;
    std::vector<Operacion> operaciones;
    Variable resultado;
    Funcion()=default;
};

class ContextoRy{
    std::map<std::string, Variable> variables;
    std::map<std::string, Funcion> funciones;
    std::queue<Variable> stack;//keyword
public:
    void cargarArchivo(const std::string& nombreArchivo);
    void ejecutar();
    void ejecutar(const Funcion& func);
    void evaluar(QStringList tokens);
    ContextoRy(){
        stack.emplace(Variable(0.0));
    }
private:
    Variable literalDeToken(const QString& token);
    bool esNumeroQ(const QString& token);
    bool esLiteralQ(const QString& token);
    double numb(const QString& token);
    std::string str(const QString& token);

    Variable operar(const Variable a, const QString op, const Variable b);
    Variable operar(const QString  a, const QString op, const QString  b);//literal op literal
    Variable operar(const Variable a, const QString op, const QString  b);
    Variable operar(const QString  a, const QString op, const Variable b);
    //funciones de un solo argumento
    Variable operar(const QString op, const Variable b);
    Variable operar(const QString op, const QString b);

};



std::ostream& operator<<(std::ostream& os, const Variable& var);



#endif // NUCLEO_HPP
