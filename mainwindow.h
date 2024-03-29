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
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
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
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <ll_protocol/sources/ll_protocol.h>
#include <ll_protocol/sources/ll_Message.h>
#include <TextEditor.h>
#include <time.h>
#include "base_station_coord.h"
#include "sound_graph.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString title, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void readData();
    void saveData();

    void handleError(QSerialPort::SerialPortError error);

private:
    void initActionsConnections();
    QByteArray qByte_less;

    Ui::MainWindow *ui;
    Console *console;
    TextEditor *textEditor;

    SettingsDialog *settings;
    QSerialPort *serial;

    struct timeval tv;
    Base_Station_Coord *BS_Coord;
    SoundGraph *SoundGraphic;

    double d_detectorTime[3] = {0.0, 0.0, 0.0};
signals:
    void DrawDataLeft(uint16_t);
    void DrawDataCenter(uint16_t);
    void DrawDataRight(uint16_t);
    void SerialConnection(bool);
    void Clear_Signal();

private slots:
    void Clear_Slot();
public slots:
    void triger_connect(bool);
};

#endif // MAINWINDOW_H
