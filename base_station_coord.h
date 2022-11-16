#ifndef BASE_STATION_COORD_H
#define BASE_STATION_COORD_H

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsItem>
#include <QPointF>
#include <QtCharts/QLineSeries>
#include "callout.h"
#include <ll_protocol/sources/ll_Message.h>

QT_CHARTS_USE_NAMESPACE

class Base_Station_Coord : public QChartView
{
public:
    explicit Base_Station_Coord(QWidget *parent = 0);
    void mooveDetector(quint8 detector, double Longitude, double Latitude);
    void setTimeDiff(Time *s_Time);
protected:
    bool viewportEvent(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    const quint32 R = 6371000;
    const quint32 distance = 10000;
    const quint16 sound_speed = (quint16)330;

    qreal CorrectAngle(qreal q_Angle);
    qreal CalcAngle(qreal r_XDistance, qreal r_YDistance, qreal r_SoundDiff, qreal r_Azimuth);
    QPointF CalcDirection(QPointF pointFirst, QPointF pointSecond, QPointF pointThird, qreal r_PoinsDiff, qreal r_FirstSecondDiff, qreal r_SecondThirdDiff);
    void calculate(qreal ML_Diff, qreal MR_Diff, qreal LR_Diff);
    bool m_isTouching;
    QScatterSeries *detector_series;
    QScatterSeries *target;
    QScatterSeries *middle_series;
    QGraphicsSimpleTextItem *coordX;
    QGraphicsSimpleTextItem *coordY;
    Callout *ptr_tooltip = nullptr;
    QList<Callout *> callouts;

    QLineSeries *ML_Target_Line;
    QLineSeries *MR_Target_Line;
    QLineSeries *LR_Target_Line;

public slots:
    void keepCallout();
    void tooltip(QPointF point, bool state);
};

#endif // BASE_STATION_COORD_H
