#include "main_window.h"
#include "mainwindow.h"

Main_Window::Main_Window()
{
    console = new Console;
    console->setEnabled(false);
    setCentralWidget(console);

    GraphWindow = new Graphic_Window();
    GraphWindow->show();

    MainWindow *w0 = new MainWindow("Window 0", GraphWindow);
    w0->show();

 //   MainWindow *w1 = new MainWindow("Window 1");
  //  w1->show();

  //  MainWindow *w2 = new MainWindow("Window 2");
  //  w2->show();

  //  connect(w0, SIGNAL(DrawDataLeft(uint16_t)),   GraphWindow, SLOT(Build_Graphic_Left(uint16_t)));
  //  connect(w0, SIGNAL(DrawDataCenter(uint16_t)), GraphWindow, SLOT(Build_Graphic_Right(uint16_t)));
  //  connect(w0, SIGNAL(DrawDataRight(uint16_t)),  GraphWindow, SLOT(Build_Graphic_Center(uint16_t)));
/*
    connect(w0, SIGNAL(SerialConnection(bool)), w1, SLOT(triger_connect(bool)));
    connect(w0, SIGNAL(SerialConnection(bool)), w2, SLOT(triger_connect(bool)));
*/
    connect(w0, SIGNAL(Clear_Signal()), GraphWindow, SLOT(Clear_Data()));
 //   connect(w1, SIGNAL(Clear_Signal()), GraphWindow, SLOT(Clear_Data()));
 //   connect(w2, SIGNAL(Clear_Signal()), GraphWindow, SLOT(Clear_Data()));
}
