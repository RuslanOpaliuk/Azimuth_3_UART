#include "main_window.h"
#include "mainwindow.h"

Main_Window::Main_Window()
{
    console = new Console;
    console->setEnabled(false);
    setCentralWidget(console);

    GraphWindow = new Graphic_Window();
    GraphWindow->show();

    MainWindow *w0 = new MainWindow();
    w0->show();

    MainWindow *w1 = new MainWindow();
    w1->show();

    MainWindow *w2 = new MainWindow();
    w2->show();

    connect(w0, SIGNAL(DrawData(uint16_t)), GraphWindow, SLOT(Build_Graphic_Left(uint16_t)));
    connect(w1, SIGNAL(DrawData(uint16_t)), GraphWindow, SLOT(Build_Graphic_Right(uint16_t)));
    connect(w2, SIGNAL(DrawData(uint16_t)), GraphWindow, SLOT(Build_Graphic_Center(uint16_t)));

    connect(w0, SIGNAL(SerialConnection(bool)), w1, SLOT(triger_connect(bool)));
}
