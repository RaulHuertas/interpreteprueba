#include "Nucleo.hpp"
#include <QString>
#include <QFile>
#include <QDebug>
#include <cmath>
#include <iostream>

using namespace std;

std::ostream& operator<<(std::ostream& os, const Variable& var)
{
    if(var.tipo==Variable::Cadena){
        os<<var.cadena;
    }else{
        os<<var.numero;
    }
    return os;
}

void ContextoRy::cargarArchivo(const std::string& nombreArchivo){
    QFile file(QString(nombreArchivo.c_str()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    int lineN = 0;
    bool enFunc = false;
    Funcion funcActual;
    Operacion opActual;
    //int tempN = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString linea(line);
        linea = linea.trimmed();
        auto comentsStart = linea.indexOf(";");
        if(comentsStart>=0){//comentario encontrado, descartarlo
            linea = linea.left(comentsStart);
        }
        if(linea.length()<=0){//linea vacía
            //qDebug()<<Q_FUNC_INFO<<"línea vacía"<<linea;
            lineN++;
            continue;
        }
        auto tokens = linea.split(" ");
        if(tokens.size()<1){
            qDebug()<<Q_FUNC_INFO<<"CODIGO INVALIDO!!!"<<linea;
            return;
        }
        //evaluar(tokens);

        auto primerToken = tokens[0];

        if(primerToken.compare("func")==0){//inicio funcion
            enFunc = true;
            funcActual.nombre = tokens[1].toStdString();
            lineN++;
            continue;
        }
        if(primerToken.compare("end")==0){
            enFunc = false;
            funciones.emplace(funcActual.nombre, funcActual);
            lineN++;
            continue;
        }
        if(!enFunc){
            lineN++;
            continue;
        }
        if(primerToken.compare("print")==0){
            funcActual.operaciones.emplace_back(Operacion::Tipo::Print, tokens[1].toStdString());
            lineN++;
            continue;
        }
        auto segundoToken = tokens[1];
        if(segundoToken.compare("=")==0){
            //funcActual.operaciones.emplace_back(Operacion::Tipo::Asignar, tokens[0], );
            if(tokens.size()==3){//Asignaciones
                if(esLiteralQ(tokens[2])){
                    funcActual.operaciones.emplace_back( Operacion::Tipo::AsignarLiteral, tokens[0].toStdString(), literalDeToken(tokens[2]) );
                }else{
                    funcActual.operaciones.emplace_back( Operacion::Tipo::AsignarVar, tokens[0].toStdString(), tokens[2].toStdString());
                }
            }else if(tokens.size()==5){//Verificar funciones aritméticas
                if(esLiteralQ(tokens[2])&&esLiteralQ(tokens[4])){
                    funcActual.operaciones.emplace_back(Operacion::Tipo::AsignarLiteral, tokens[0].toStdString(), operar(tokens[2], tokens[3], tokens[4]));
                }if(!esLiteralQ(tokens[2])&&esLiteralQ(tokens[4])){
                    funcActual.operaciones.emplace_back(Operacion::Tipo::AsignarLiteral, tokens[0].toStdString(), operar(variables[tokens[2].toStdString()], tokens[3], tokens[4]));
                }if(esLiteralQ(tokens[2])&&!esLiteralQ(tokens[4])){
                    funcActual.operaciones.emplace_back(Operacion::Tipo::AsignarLiteral, tokens[0].toStdString(), operar(tokens[2], tokens[3], variables[tokens[4].toStdString()]));
                }
            }else{//Verificar funciones sin, sqrt
                 if(esLiteralQ(tokens[3])){
                    funcActual.operaciones.emplace_back(Operacion::Tipo::AsignarLiteral, tokens[0].toStdString(), operar(tokens[2], tokens[3], tokens[4].toStdString()));
                 }else{
                    funcActual.operaciones.emplace_back(Operacion::Tipo::AsignarLiteral, tokens[0].toStdString(), operar(tokens[2], tokens[3], variables[tokens[4].toStdString()]));
                 }
                //funcActual.operaciones.emplace_back(Operacion::Tipo::Asignar, tokens[0], tokens[2]);
            }
        }


    }
}

void ContextoRy::evaluar(QStringList tokens){
    Q_UNUSED(tokens)
}

bool ContextoRy::esNumeroQ(const QString& token){
    bool okQ = false;
    auto numb = token.toDouble(&okQ);
    Q_UNUSED(numb)
    return okQ;
}

bool ContextoRy::esLiteralQ(const QString& token){
    bool okQ = false;
    auto numb = token.toDouble(&okQ);
    Q_UNUSED(numb);
    if(okQ){
        return true;
    }
    auto comillas = '\"';
    auto nQ = token.count();
    auto test1 = token.startsWith(comillas);
    auto test2 = token.endsWith  (comillas);
    auto test3 = token.size()>3;
    return (nQ&&test1&&test2&&test3);
}

double ContextoRy::numb(const QString& token){
    return token.toDouble();
}

std::string ContextoRy::str(const QString& token){
    return token.mid(1,token.size()-2).toStdString();
}

Variable ContextoRy::literalDeToken(const QString& token){
    Variable ret;
    if(esNumeroQ(token)){
        ret.tipo = Variable::Numero;
        ret.numero = numb(token);
    }else{
        ret.tipo = Variable::Cadena;
        ret.cadena = str(token);
    }
    return ret;
}

Variable ContextoRy::operar(const Variable varA, const QString op, const Variable varB){
    Variable res;
    if(op.compare("+")==0){
        res = varA+varB;
    }else if(op.compare("-")==0){
        res = varA-varB;
    }else if(op.compare("*")==0){
        res = varA*varB;
    }else if(op.compare("/")==0){
        res = varA/varB;
    }
    return res;
}

Variable ContextoRy::operar(const QString a, const QString op, const QString b){
    auto varA = literalDeToken(a);
    auto varB = literalDeToken(b);
    return operar(varA, op, varB);
}

Variable ContextoRy::operar(const Variable a, const QString op, const QString b){
    auto varB = literalDeToken(b);
    return operar(a, op, varB);
}

Variable ContextoRy::operar(QString a, const QString op, const Variable b){
    auto varA = literalDeToken(a);
    return operar(varA, op, b);
}

Variable ContextoRy::operar(const QString op, const Variable b){
    if(op.compare("sin")==0){
        return Variable(std::sin(b.numero));
    }else if(op.compare("cos")==0){
        return Variable(std::cos(b.numero));
    }else if(op.compare("sqrt")==0){
        return Variable(std::sqrt(b.numero));
    }
    return Variable(0.0);
}

Variable ContextoRy::operar(const QString op, const QString varB){
    auto b = variables[varB.toStdString()];
    if(op.compare("sin")==0){
        return Variable(std::sin(b.numero));
    }else if(op.compare("cos")==0){
        return Variable(std::cos(b.numero));
    }else if(op.compare("sqrt")==0){
        return Variable(std::sqrt(b.numero));
    }
    return Variable(0.0);
}

void ContextoRy::ejecutar(){
    for(auto& f : funciones){
        if(f.first==std::string("main")){
            ejecutar(f.second);
        }
    }
}

void ContextoRy::ejecutar(const Funcion& func){
    for(const auto& op : func.operaciones){
        switch(op.tipo){
            case Operacion::Pass:{
                break;
            }
            case Operacion::AsignarLiteral:{
                variables[op.op1] = op.literal;
                break;
            }
            case Operacion::Print:{
                QString tok = QString(op.op1.c_str());
                if(esLiteralQ(tok)){
                    cout<< literalDeToken(tok)<<endl;
                }else{
                    cout<< variables[op.op1]<<endl;
                }

                break;
            }
            default:{

            }
        }
    }
}
