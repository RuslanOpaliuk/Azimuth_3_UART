#ifndef BASE_STATION_COORD_H
#define BASE_STATION_COORD_H

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsItem>
#include <QPointF>
#include "callout.h"

QT_CHARTS_USE_NAMESPACE

class Base_Station_Coord : public QChartView
{
public:
    explicit Base_Station_Coord(QWidget *parent = 0);
    void mooveDetector(quint8 detector, double Latitude, double Longitude);
protected:
    bool viewportEvent(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    bool m_isTouching;
    QScatterSeries *detector_series;
    QGraphicsSimpleTextItem *coordX;
    QGraphicsSimpleTextItem *coordY;
    Callout *ptr_tooltip = nullptr;
    QList<Callout *> callouts;
public slots:
    void keepCallout();
    void tooltip(QPointF point, bool state);
};

#endif // BASE_STATION_COORD_H
