#include "graphic_window.h"

Graphic_Window::Graphic_Window()
{
    Graphic::Add_Graphic(0, 0xFFFF, "Left");
    Graphic::Add_Graphic(0, 0xFFFF, "Right");
    Graphic::Add_Graphic(0, 0xFFFF, "Center");

   // Graphic::Add_Time_Label();
}

void Graphic_Window::Build_Graphic_Left(uint16_t voice)
{
    Graphic::Build_Graphic(voice,   1, 0);

   //     Graphic::Write_Time(POHPR.time);
}

void Graphic_Window::Build_Graphic_Right(uint16_t voice)
{
    Graphic::Build_Graphic(voice,   1, 1);

   //     Graphic::Write_Time(1);
}

void Graphic_Window::Build_Graphic_Center(uint16_t voice)
{
    Graphic::Build_Graphic(voice,   1, 2);

   //     Graphic::Write_Time(1);
}
