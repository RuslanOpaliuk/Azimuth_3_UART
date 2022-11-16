#include "base_station_coord.h"

#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QtGui/QMouseEvent>
#include <QtMath>

Base_Station_Coord::Base_Station_Coord(QWidget *parent) :
    QChartView(new QChart(), parent),
    coordX(0),
    coordY(0)
{
    QPointF Left = QPointF(23.5155868, 50.0797319);
    QPointF Right = QPointF(23.5155868, 50.0797319);
    QPointF Middle = QPointF(23.5155868, 50.0797319);

    QPointF Target = QPointF(23.5900122, 50.0608949);

    QPointF Middle_Left = QPointF((Middle.x()+Left.x())/2, (Middle.y()+Left.y())/2);
    QPointF Middle_Right = QPointF((Middle.x()+Right.x())/2, (Middle.y()+Right.y())/2);
    QPointF Left_Right = QPointF((Left.x()+Right.x())/2, (Left.y()+Right.y())/2);

    detector_series = new QScatterSeries();
    detector_series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    detector_series->setMarkerSize(10.0);
    detector_series->setColor(0x0000FF);
    detector_series->setPen(QColor(Qt::transparent));
    *(detector_series) << Left << Middle << Right;

    target = new QScatterSeries();
    target->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    target->setMarkerSize(15.0);
    target->setColor(0xAA0000);
    target->setPen(QColor(Qt::transparent));
    *(target) << Target;

    middle_series = new QScatterSeries();
    middle_series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    middle_series->setMarkerSize(8);
    middle_series->setColor(0xDCDC00);
    middle_series->setPen(QColor(Qt::transparent));
    *(middle_series) << Middle_Left << Middle_Right << Left_Right;

    ML_Target_Line = new QLineSeries();;
    MR_Target_Line = new QLineSeries();;
    LR_Target_Line = new QLineSeries();;
    ML_Target_Line->setColor(0xBB0000);
    MR_Target_Line->setColor(0xBB0000);
    LR_Target_Line->setColor(0xBB0000);

    ML_Target_Line->append(middle_series->at(0).x(), middle_series->at(0).y());
    MR_Target_Line->append(middle_series->at(1).x(), middle_series->at(1).y());
    LR_Target_Line->append(middle_series->at(2).x(), middle_series->at(2).y());

    setRenderHint(QPainter::Antialiasing);

    chart()->addSeries(detector_series);
    chart()->addSeries(target);
    chart()->addSeries(middle_series);
    chart()->addSeries(ML_Target_Line);
    chart()->addSeries(MR_Target_Line);
    chart()->addSeries(LR_Target_Line);
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

    resize(1000*cos(Target.x()*M_PI/180), 1000);
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


void Base_Station_Coord::mooveDetector(quint8 detector, double Longitude, double Latitude)
{
    detector_series->replace(detector, Longitude, Latitude);

    qreal Middle_Long = (detector_series->at(2).x() + detector_series->at(0).x())/2;
    qreal Middle_Lat  = (detector_series->at(2).y() + detector_series->at(0).y())/2;
    middle_series->replace(0, Middle_Long, Middle_Lat);
    ML_Target_Line->replace(0, Middle_Long, Middle_Lat);

    Middle_Long = (detector_series->at(2).x() + detector_series->at(1).x())/2;
    Middle_Lat  = (detector_series->at(2).y() + detector_series->at(1).y())/2;
    middle_series->replace(1, Middle_Long, Middle_Lat);
    MR_Target_Line->replace(0, Middle_Long, Middle_Lat);

    Middle_Long = (detector_series->at(0).x() + detector_series->at(1).x())/2;
    Middle_Lat  = (detector_series->at(0).y() + detector_series->at(1).y())/2;
    middle_series->replace(2, Middle_Long, Middle_Lat);
    LR_Target_Line->replace(0, Middle_Long, Middle_Lat);
}

void Base_Station_Coord::setTimeDiff(Time *s_Time)
{
    qreal ML_Diff = (s_Time[2].u16_microSecond - s_Time[0].u16_microSecond);
    ML_Diff /= 10000;
    ML_Diff += (s_Time[2].u8_Second - s_Time[0].u8_Second);
    ML_Diff *= sound_speed;

    qreal MR_Diff = (s_Time[2].u16_microSecond - s_Time[1].u16_microSecond);
    MR_Diff /= 10000;
    MR_Diff += (s_Time[2].u8_Second - s_Time[1].u8_Second);
    MR_Diff *= sound_speed;

    qreal LR_Diff = (s_Time[0].u16_microSecond - s_Time[1].u16_microSecond);
    LR_Diff /= 10000;
    LR_Diff += (s_Time[0].u8_Second - s_Time[1].u8_Second);
    LR_Diff *= sound_speed;

    calculate(399, -190, -589);
}

qreal Base_Station_Coord::CorrectAngle(qreal q_Angle)
{
    if ((2 * M_PI) <= q_Angle)
    {
        q_Angle -= (2 * M_PI);
    }
    else if (0 >= q_Angle)
    {
        q_Angle += (2 * M_PI);
    }

    return q_Angle;
}

qreal Base_Station_Coord::CalcAngle(qreal r_XDistance, qreal r_YDistance, qreal r_SoundDiff, qreal r_Azimuth)
{
    qreal q_Angle = r_Azimuth;

    if ((0 <= r_XDistance) && (0 >= r_YDistance))
    {
        q_Angle = 2 * M_PI + q_Angle;
    }
    else if (0 <= r_YDistance)
    {
        q_Angle += M_PI;
    }

    if (0 >= r_SoundDiff)
    {
        q_Angle = q_Angle + M_PI;

        if ((2 * M_PI) <= q_Angle)
        {
            q_Angle -= (2 * M_PI);
        }
    }

    return q_Angle;
}

QPointF Base_Station_Coord::CalcDirection(QPointF pointFirst, QPointF pointSecond, QPointF pointThird, qreal r_PoinsDiff, qreal r_FirstSecondDiff, qreal r_SecondThirdDiff)
{
    qreal r_XDistance = qDegreesToRadians(pointFirst.x() - pointSecond.x())*cos(qDegreesToRadians(pointFirst.y()));
    qreal r_YDistance = qDegreesToRadians(pointFirst.y() - pointSecond.y());
    qreal r_PointDistance = qSqrt((r_XDistance*r_XDistance) + (r_YDistance*r_YDistance))*R;

    qreal r_BaseAngle = qAtan(r_XDistance/r_YDistance);
    r_BaseAngle = CalcAngle(r_XDistance, r_YDistance, r_PoinsDiff, r_BaseAngle);

    r_PoinsDiff = qAbs(r_PoinsDiff);
    qreal r_TargetAngle = M_PI_2;

    if (r_PoinsDiff < r_PointDistance)
    {
        r_TargetAngle -= qAsin(r_PoinsDiff/r_PointDistance);

    //////////////////////////////////////////////////////////////////////////////////
        r_XDistance = qDegreesToRadians(pointFirst.x() - pointThird.x())*cos(qDegreesToRadians(pointFirst.y()));
        r_YDistance = qDegreesToRadians(pointFirst.y() - pointThird.y());
        qreal r_FirstCortAngle = qAtan(r_XDistance/r_YDistance);

        r_FirstCortAngle = CalcAngle(r_XDistance, r_YDistance, r_FirstSecondDiff, r_FirstCortAngle);
    //////////////////////////////////////////////////////////////////////////////////
        r_XDistance = qDegreesToRadians(pointSecond.x() - pointThird.x())*cos(qDegreesToRadians(pointSecond.y()));
        r_YDistance = qDegreesToRadians(pointSecond.y() - pointThird.y());
        qreal r_SecondCortAngle = qAtan(r_XDistance/r_YDistance);

        r_SecondCortAngle = CalcAngle(r_XDistance, r_YDistance, r_SecondThirdDiff, r_SecondCortAngle);
    //////////////////////////////////////////////////////////////////////////////////
        if (qAbs(r_SecondCortAngle - r_FirstCortAngle) > M_PI)
        {
            if (r_FirstCortAngle < r_SecondCortAngle)
            {
                r_FirstCortAngle += 2*M_PI;
            }
            else
            {
                r_SecondCortAngle += 2*M_PI;
            }
        }
        qreal q_CortAngle = (r_FirstCortAngle*qAbs(r_FirstSecondDiff) + r_SecondCortAngle*qAbs(r_SecondThirdDiff))/(qAbs(r_FirstSecondDiff) + qAbs(r_SecondThirdDiff));

    //////////////////////////////////////////////////////////////////////////////////
        qreal r_DiffMinus = CorrectAngle(r_BaseAngle - r_TargetAngle);
        r_DiffMinus = qAbs(r_DiffMinus - q_CortAngle);

        qreal r_DiffPlus = CorrectAngle(r_BaseAngle + r_TargetAngle);
        r_DiffPlus = qAbs(r_DiffPlus - q_CortAngle);

        if (r_DiffMinus <= r_DiffPlus)
        {
            r_TargetAngle = r_BaseAngle - r_TargetAngle;
        }
        else
        {
            r_TargetAngle = r_BaseAngle + r_TargetAngle;
        }

        r_TargetAngle = CorrectAngle(r_TargetAngle);
    }
    else
    {
        r_TargetAngle = r_BaseAngle;
    }

    qreal r_MiddleLong = (pointFirst.x() + pointSecond.x())/2;
    qreal r_MiddleLat  = (pointFirst.y() + pointSecond.y())/2;

    r_XDistance = distance*qSin(r_TargetAngle);
    r_YDistance = distance*qCos(r_TargetAngle);

    qreal X = r_XDistance/R;
    X /= cos(qDegreesToRadians(pointFirst.y()));
    qreal Y = r_YDistance/R;

    X = r_MiddleLong + qRadiansToDegrees(X);
    Y = r_MiddleLat + qRadiansToDegrees(Y);

    return QPointF(X, Y);
}

void Base_Station_Coord::calculate(qreal ML_Diff, qreal MR_Diff, qreal LR_Diff)
{
    QPointF Left = detector_series->at(0);
    QPointF Right = detector_series->at(1);
    QPointF Middle = detector_series->at(2);

    QPointF Z1 = CalcDirection(Middle, Left, Right, ML_Diff, MR_Diff, LR_Diff);
    ML_Target_Line->append(Z1);

    QPointF Z2 = CalcDirection(Middle, Right, Left, MR_Diff, ML_Diff, -LR_Diff);
    MR_Target_Line->append(Z2);

    QPointF Z3 = CalcDirection(Left, Right, Middle, LR_Diff, -ML_Diff, -MR_Diff);
    LR_Target_Line->append(Z3);
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

