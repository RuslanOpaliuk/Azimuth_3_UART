/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE
//! [0]
MainWindow::MainWindow(QString title, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(title);

    tv = {0, 0};

    console = new Console;
    console->setEnabled(false);

    textEditor = new TextEditor();

    setCentralWidget(textEditor);
    //setCentralWidget(console);

    serial = new QSerialPort(this);
    settings = new SettingsDialog;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    BS_Coord = new Base_Station_Coord();
    BS_Coord->show();

    SoundGraphic = new SoundGraph();
    SoundGraphic->show();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}
//! [3]

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

//! [4]
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    if (serial->open(QIODevice::ReadWrite)) {
        if (serial->setBaudRate(p.baudRate)
                && serial->setDataBits(p.dataBits)
                && serial->setParity(p.parity)
                && serial->setStopBits(p.stopBits)
                && serial->setFlowControl(p.flowControl)) {

            console->setEnabled(true);
            console->setLocalEchoEnabled(p.localEchoEnabled);
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

            emit SerialConnection(true);
        } else {
            serial->close();
            QMessageBox::critical(this, tr("Error"), serial->errorString());

            ui->statusBar->showMessage(tr("Open error"));
        }
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Configure error"));
    }
}
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    serial->close();
    //console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
    emit SerialConnection(false);
}
//! [5]

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();

    qByte_less += data;

    size_t remainder;
    uint8_t tmp[100] = {0};
    ll_status_t status;

    ll_message_info_t message_for_deserialize[] = {s_TimeInfo, s_CoordInfo, s_SoundInfo};

    const quint8 u8_DeserializeSize = sizeof(message_for_deserialize)/sizeof(ll_message_info_t);

    Coordinate s_Coordinate;
    Time s_Time;
    Sound s_Sound;

    while (0 < qByte_less.size())
    {
        int j = u8_DeserializeSize;

        for (int i = 0; i < u8_DeserializeSize; i++)
        {
            if ((quint8)message_for_deserialize[i].begin_byte == (quint8)qByte_less.data()[0])
            {
                j = i;
                break;
            }
        }

        if (u8_DeserializeSize == j)
        {
            qByte_less.remove(0, 1);
            continue;
        }

        status = ll_deserialize(message_for_deserialize[j], (uint8_t*)qByte_less.data(), qByte_less.size(), tmp, &remainder);

        tv.tv_usec += 10;

        if(LL_STATUS_SUCCESS == status)
        {
            if (s_SoundInfo.begin_byte == message_for_deserialize[j].begin_byte)
            {
                s_Sound = s_ParseSoundMessage(tmp);
                textEditor->putSound(s_Sound.u8_detector, s_Sound.u16_Power);
                SoundGraphic->DrawSound(s_Sound.u8_detector, d_detectorTime[s_Sound.u8_detector], s_Sound.u16_Power);
                d_detectorTime[s_Sound.u8_detector] += 0.000005;
            }

            if (s_TimeInfo.begin_byte == message_for_deserialize[j].begin_byte)
            {
                s_Time = s_ParseTimeMessage(tmp);
                textEditor->putTime(s_Time.u8_detector, s_Time.u8_Hour, s_Time.u8_Minute, s_Time.u8_Second, s_Time.u16_microSecond);

                double Time = (s_Time.u8_Hour*60*60) + (s_Time.u8_Minute*60) + s_Time.u8_Second;
                Time += double(s_Time.u16_microSecond) / 100000;

                d_detectorTime[s_Time.u8_detector] = Time;
            }

            if (s_CoordInfo.begin_byte == message_for_deserialize[j].begin_byte)
            {
                s_Coordinate = s_ParseCoordMessage(tmp);
                BS_Coord->mooveDetector(s_Coordinate.u8_detector, s_Coordinate.f_Longitude, s_Coordinate.f_Latitude);
                textEditor->putCoord(s_Coordinate.u8_detector, s_Coordinate.f_Longitude, s_Coordinate.f_Latitude);
            }

            if (0 == remainder)
            {
                qByte_less.clear();
                data.clear();
            }
            else
            {
                qByte_less.remove(0, remainder);
            }
        }
        else if (LL_STATUS_NO_MESSAGE == status)
        {
            qByte_less.remove(0, remainder);
            break;
        }
        else if (LL_STATUS_MESSAGE_TOO_SHORT == status || LL_STATUS_MESSAGE_TOO_LONG == status)
        {
            qByte_less.remove(0, remainder);
        }
        else if (LL_STATUS_NO_ENOUGH_BYTES == status)
        {
            break;
        }
    }
}

void MainWindow::saveData()
{

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
//! [8]

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(Clear_Slot()));
    connect(ui->actionClear, SIGNAL(triggered()), textEditor, SLOT(clearData()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::triger_connect(bool trigger)
{
    if (true == trigger)
    {
        openSerialPort();
    }
    else
    {
        closeSerialPort();
    }
}

void MainWindow::Clear_Slot()
{
    emit Clear_Signal();
    SoundGraphic->clear();

    BS_Coord->mooveDetector(0, 23.5164367, 50.081694);
    BS_Coord->mooveDetector(1, 23.5095169, 50.0847046);
    BS_Coord->mooveDetector(2, 23.5091296, 50.079602);

    Time s_Time[3];

    /*
typedef struct Time{
    uint8_t u8_detector;
    uint8_t u8_Hour;
    uint8_t u8_Minute;
    uint8_t u8_Second;
    uint16_t u16_microSecond;
}Time;
     * */

    s_Time[0] = {(uint8_t)0, (uint8_t)13, (uint8_t)5, (uint8_t)30, (uint16_t)7848};
    s_Time[1] = {(uint8_t)1, (uint8_t)13, (uint8_t)5, (uint8_t)31, (uint16_t)545};
    s_Time[2] = {(uint8_t)2, (uint8_t)13, (uint8_t)5, (uint8_t)30, (uint16_t)0};

    BS_Coord->setTimeDiff(s_Time);
}
