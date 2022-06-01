#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "console.h"
#include "graphic_window.h"

class Main_Window : public QMainWindow
{
    Q_OBJECT

private:
public:

    Main_Window();
    Console *console;
    Graphic_Window *GraphWindow;
};

#endif // MAIN_WINDOW_H
