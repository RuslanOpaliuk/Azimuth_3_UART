#include "base_station_coord.h"

#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QtGui/QMouseEvent>

Base_Station_Coord::Base_Station_Coord(QWidget *parent) :
    QChartView(new QChart(), parent),
    coordX(0),
    coordY(0)
{
    resize(500, 500);

    detector_series = new QScatterSeries();
    detector_series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    detector_series->setMarkerSize(15.0);
    detector_series->setColor(0x008800);

    *(detector_series) << QPointF(49.0994, 23.5731) << QPointF(49.83559, 24.03214) << QPointF(49.81633, 24.07918);

    detector_series->setPen(QColor(Qt::transparent));

    setRenderHint(QPainter::Antialiasing);

    chart()->addSeries(detector_series);
    chart()->createDefaultAxes();
    chart()->setDropShadowEnabled(false);
    chart()->legend()->hide();
    chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

    coordX = new QGraphicsSimpleTextItem(chart());
    coordX->setPos(chart()->size().width()/2 - 50, chart()->size().height()-30);
    coordX->setText("X: ");
    coordY = new QGraphicsSimpleTextItem(chart());
    coordY->setPos(chart()->size().width()/2 + 50, chart()->size().height()-30);
    coordY->setText("Y: ");

    connect(detector_series, &QScatterSeries::clicked, this, &Base_Station_Coord::keepCallout);
    connect(detector_series, &QScatterSeries::hovered, this, &Base_Station_Coord::tooltip);
}

void Base_Station_Coord::resizeEvent(QResizeEvent *event)
{
    if (scene())
    {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         chart()->resize(event->size());
         coordX->setPos(chart()->size().width()/2 - 50, chart()->size().height() - 30);
         coordY->setPos(chart()->size().width()/2 + 50, chart()->size().height() - 30);
    }
    QGraphicsView::resizeEvent(event);
}


void Base_Station_Coord::mooveDetector(quint8 detector,  double Latitude, double Longitude)
{
    int X1 = (int)Latitude/100;
    int Y1 = (int)Longitude/100;

    X1 *= 100;
    Y1 *= 100;

    double X = Latitude - X1;
    double Y = Longitude - Y1;

    X = X/60;
    Y = Y/60;

    X += (int)Latitude/100;
    Y += (int)Longitude/100;

    detector_series->replace(detector, X, Y);
}

bool Base_Station_Coord::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        m_isTouching = true;
        chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void Base_Station_Coord::mouseMoveEvent(QMouseEvent *event)
{
    coordX->setText(QString("X: %1").arg(chart()->mapToValue(event->pos()).x()));
    coordY->setText(QString("Y: %1").arg(chart()->mapToValue(event->pos()).y()));
    QGraphicsView::mouseMoveEvent(event);
}

void Base_Station_Coord::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -10);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void Base_Station_Coord::keepCallout()
{
    if (nullptr == ptr_tooltip)
        ptr_tooltip = new Callout(chart());

    callouts.append(ptr_tooltip);
    ptr_tooltip = new Callout(chart());
}

void Base_Station_Coord::tooltip(QPointF point, bool state)
{
    if (nullptr == ptr_tooltip)
        ptr_tooltip = new Callout(chart());

    if (state) {
        ptr_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        ptr_tooltip->setAnchor(point);
        ptr_tooltip->setZValue(11);
        ptr_tooltip->updateGeometry();
        ptr_tooltip->show();
    } else {
        ptr_tooltip->hide();
    }
}

