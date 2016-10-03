#ifndef BLANCO_H
#define BLANCO_H
#include <QString>

class Blanco
{
private:
    int id_blanco;
    QString etiqueta_blanco;
    QString clase_blanco;
    float frec_blanco[];
    double angulo;

public:
    Blanco();

    void visible();
    void guardar();
    void eliminar();
    void set_etiquetar_blanco(QString etiqueta_blanco);
    void set_clase_blanco(QString clase_blanco);
    void actualizar_informacion_blanco();

    int get_id_blanco();
    QString get_etiqueta_blanco();
    QString get_clase_blanco();
    float get_frec_blanco();
    float get_angulo();
};

#endif // BLANCO_H
