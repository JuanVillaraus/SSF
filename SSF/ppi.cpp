#include "ppi.h"

PPI::PPI()
{
    visible = false;
    tipo_norte = true;
    tipo_origen = 0;
    escala_ppi = 10;
}

void PPI::set_visible(bool visible)
{
    this->visible = visible;
}

void PPI::set_tipo_norte(bool tipo_norte)
{
    this->tipo_norte = tipo_norte;
}

void PPI::set_tipo_origen(int tipo_origen)
{
    this->tipo_origen = tipo_origen;
}

void PPI::set_escala_ppi(float escala_ppi)
{
    this->escala_ppi = escala_ppi;
}

bool PPI::get_visible()
{
    return visible;
}

bool PPI::get_tipo_norte()
{
    return tipo_norte;
}

int PPI::get_tipo_origen()
{
    return tipo_origen;
}

float PPI::get_escala_ppi()
{
    return escala_ppi;
}
