#include "signal.h"

Signal::Signal()
{
    frec = 83;
    bw = 7.2;
    it = 5;
    dt = 9;
    ganancia_sensor = 3;
    prob_falsa = 0.3;
    prob_deteccion = 0.57;
    edo_mar = 2;
    info = "vacio";
}

void Signal::set_frec(int frec)
{
    this->frec = frec;
}

void Signal::set_bw(float bw)
{
    this->bw = bw;
}

void Signal::set_it(float it)
{
    this->it = it;
}

void Signal::set_dt(int dt)
{
    this->dt = dt;
}

void Signal::set_ganancia_sensor(int ganancia_sensor)
{
    this->ganancia_sensor = ganancia_sensor;
}

void Signal::set_prob_falsa(float prob_falsa)
{
    this->prob_falsa = prob_falsa;
}

void Signal::set_prob_deteccion(float prob_deteccion)
{
    this->prob_deteccion = prob_deteccion;
}

void Signal::set_edo_mar(int edo_mar)
{
    this->edo_mar = edo_mar;
}

void Signal::set_info_signal(QString info)
{
    this->info = info;
}

void Signal::receive_of_sensor(QString message)
{
    this->info = message;
}

int Signal::get_frec()
{
    return frec;
}

float Signal::get_bw()
{
    return bw;
}

float Signal::get_it()
{
    return it;
}

int Signal::get_dt()
{
    return dt;
}

int Signal::get_ganancia_sensor()
{
    return ganancia_sensor;
}

float Signal::get_prob_falsa()
{
    return prob_falsa;
}

float Signal::get_prob_deteccion()
{
    return prob_deteccion;
}

int Signal::get_edo_mar()
{
    return edo_mar;
}

QString Signal::get_info_signal()
{
    QString s;
    s = "frec= ";
    s += QString::number(get_frec());
    s += " ; bw= ";
    s += QString::number(get_bw());
    s += " ; it= ";
    s += QString::number(get_it());
    s += " ; dt= ";
    s += QString::number(get_dt());
    s += " ; gan sen= ";
    s += QString::number(get_ganancia_sensor());
    s += " ; prob falsa= ";
    s += QString::number(get_prob_falsa());
    s += " ; prob detec= ";
    s += QString::number(get_prob_deteccion());
    s += " ; edo mar= ";
    s += QString::number(get_edo_mar());
    return s;
}

QString Signal::send_to_sensor()
{
    QString s;
        s = "$";
        if(get_it()!=10)
            s += "0";
        s += QString::number(get_it());
        s += ";";
        s += QString::number(get_edo_mar());
        s += ";";
        if(get_prob_falsa()==1)
            s += "1.00";
        else
            if(QString::number(get_prob_falsa())=="0.1")
                s += "0.10";
            else
                if(QString::number(get_prob_falsa())=="0.2")
                    s += "0.20";
                else
                    if(QString::number(get_prob_falsa())=="0.3")
                        s += "0.30";
                    else
                        if(QString::number(get_prob_falsa())=="0.4")
                            s += "0.40";
                        else
                            if(QString::number(get_prob_falsa())=="0.5")
                                s += "0.50";
                            else
                                if(QString::number(get_prob_falsa())=="0.6")
                                    s += "0.60";
                                else
                                    if(QString::number(get_prob_falsa())=="0.7")
                                        s += "0.70";
                                    else
                                        if(QString::number(get_prob_falsa())=="0.8")
                                            s += "0.80";
                                        else
                                            if(QString::number(get_prob_falsa())=="0.9")
                                                s += "0.90";
                                            else
                                                if(QString::number(get_prob_falsa())=="0")
                                                    s += "0.00";
                                                else
                                                    s += QString::number(get_prob_falsa());
        s += ";";
        if(get_prob_deteccion()==1)
            s += "1.00";
        else
            if(QString::number(get_prob_deteccion())=="0.1")
                s += "0.10";
            else
                if(QString::number(get_prob_deteccion())=="0.2")
                    s += "0.20";
                else
                    if(QString::number(get_prob_deteccion())=="0.3")
                        s += "0.30";
                    else
                        if(QString::number(get_prob_deteccion())=="0.4")
                            s += "0.40";
                        else
                            if(QString::number(get_prob_deteccion())=="0.5")
                                s += "0.50";
                            else
                                if(QString::number(get_prob_deteccion())=="0.6")
                                    s += "0.60";
                                else
                                    if(QString::number(get_prob_deteccion())=="0.7")
                                        s += "0.70";
                                    else
                                        if(QString::number(get_prob_deteccion())=="0.8")
                                            s += "0.80";
                                        else
                                            if(QString::number(get_prob_deteccion())=="0.9")
                                                s += "0.90";
                                            else
                                                if(QString::number(get_prob_deteccion())=="0")
                                                    s += "0.00";
                                                else
                                                    s += QString::number(get_prob_deteccion());
        s += ";";
        s += QString::number(get_ganancia_sensor());
        return s;

}
