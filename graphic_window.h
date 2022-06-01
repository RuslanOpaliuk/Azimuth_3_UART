#ifndef GRAPHIC_WINDOW_H
#define GRAPHIC_WINDOW_H

#include "Graphic/graphic.h"

class Graphic_Window : public Graphic
{
    Q_OBJECT

public:
    Graphic_Window();

public slots:
    void Build_Graphic_Left(uint16_t voice);
    void Build_Graphic_Right(uint16_t voice);
    void Build_Graphic_Center(uint16_t voice);
};

#endif // GRAPHIC_WINDOW_H
