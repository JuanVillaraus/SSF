#ifndef PPI_H
#define PPI_H

class PPI
{
private:
    bool visible;
    bool tipo_norte;
    int tipo_origen;
    float escala_ppi;

public:
    PPI();

    void set_visible(bool visible);
    void set_tipo_norte(bool tipo_norte);
    void set_tipo_origen(int tipo_origen);
    void set_escala_ppi(float escala_ppi);

    bool get_visible();
    bool get_tipo_norte();
    int get_tipo_origen();
    float get_escala_ppi();
};

#endif // PPI_H
