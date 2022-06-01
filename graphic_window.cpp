#include "graphic_window.h"

Graphic_Window::Graphic_Window()
{
    Graphic::Add_Graphic(0, 0xFFFF, "Left");
    Graphic::Add_Graphic(0, 0xFFFF, "Right");
    Graphic::Add_Graphic(0, 0xFFFF, "Center");

   Graphic::Add_Time_Label();
}

void Graphic_Window::Build_Graphic_Left(uint16_t voice)
{
    static uint64_t count = 0;

    Graphic::Build_Graphic(voice,   'A', 0);

    Graphic::Write_Time(count);
    count++;
}

void Graphic_Window::Build_Graphic_Right(uint16_t voice)
{
    static uint64_t count = 0;

    Graphic::Build_Graphic(voice,   'A', 1);

    Graphic::Write_Time(count);
    count++;
}

void Graphic_Window::Build_Graphic_Center(uint16_t voice)
{
    static uint64_t count = 0;

    Graphic::Build_Graphic(voice,   'A', 2);

    Graphic::Write_Time(count);
    count++;
}

void Graphic_Window::Clear_Data()
{
    Graphic::Clear_Slot();
}
