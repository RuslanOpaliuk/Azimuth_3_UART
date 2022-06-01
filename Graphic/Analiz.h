#ifndef ANALIZ_H
#define ANALIZ_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "Data_Graf.h"

class Analiz : public QWidget
{
    Q_OBJECT
private:
    QSpinBox        *Sigma;
    QDoubleSpinBox  *Average;
    QDoubleSpinBox  *RMS;

    QLabel *Brack_RMS, *Brack_VRMS;

   // Data_Graf::D_V *D_Vector;
public:
    explicit Analiz(qint16, qint16, QWidget *parent = 0);
    //void Get_Data(Data_Graf::D_V &D_V);
signals:
    void Calculation(quint8);
private slots:
    void Calculation_Average_Slot();
public slots:
    void Show_Hide_Slot(bool);
    void AverageRMS_Slot(double, double, double);
};

#endif // ANALIZ_H
