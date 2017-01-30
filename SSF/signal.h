#ifndef SIGNAL_H
#define SIGNAL_H
#include <qfile.h>

class Signal
{
private:
    int frec;
    int bw;
    int it;
    int dt;
    int frecP;
    int nP;
    int anchoP;
    int rangoDetec;
    int ganancia_sensor;
    int prob_falsa;
    int prob_deteccion;
    int edo_mar;
    QString info;

public:
    Signal();

    void set_frec(int frec);
    void set_bw(float bw);
    void set_it(float it);
    void set_dt(int dt);
    void set_ganancia_sensor(int ganancia_sensor);
    void set_prob_falsa(float prob_falsa);
    void set_prob_deteccion(float prob_deteccion);
    void set_edo_mar(int edo_mar);
    void set_info_signal(QString info);
    void set_frecP(int frecP);
    void set_nP(int nP);
    void set_anchoP(int anchoP);
    void receive_of_sensor(QString message);

    int get_frec();
    float get_bw();
    float get_it();
    int get_dt();
    int get_ganancia_sensor();
    float get_prob_falsa();
    float get_prob_deteccion();
    int get_edo_mar();
    QString get_info_signal();
    QString send_to_sensor();
    int get_frecP();
    int get_nP();
    int get_anchoP();
};

#endif // SIGNAL_H
