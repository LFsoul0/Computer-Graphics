#include "AttachLine.h"

AttachLine::AttachLine(QWidget *parent) : QLabel(parent)
{
    orient = Backslash;
    lineColor = Qt::darkBlue;
    lineStyle = Qt::SolidLine;
    lineWidth = 3;
}

void AttachLine::setLine(const QPoint& start, const QPoint& end)
{
    QPoint topLeft = QPoint(qMin(start.x(), end.x()) - lineWidth, qMin(start.y(), end.y()) - lineWidth);
    QPoint bottomRight = QPoint(qMax(start.x(), end.x()) + lineWidth, qMax(start.y(), end.y()) + lineWidth);

    move(topLeft);
    resize(bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y());
    if ((start.x() - end.x()) * (start.y() - end.y()) >= 0) {
        orient = Backslash;
    }
    else {
        orient = Slash;
    }

    update();
}

void AttachLine::setLineColor(const QColor& color)
{
    lineColor = color;
    update();
}

void AttachLine::setLineStyle(Qt::PenStyle style)
{
    lineStyle = style;
    update();
}

void AttachLine::setLineWidth(int width)
{
    lineWidth = width;
    update();
}

void AttachLine::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(lineColor);
    pen.setStyle(lineStyle);
    pen.setWidth(lineWidth);
    painter.setPen(pen);
    QRect rect = this->rect();
    if (orient == Backslash) {
        painter.drawLine(rect.topLeft() + QPoint(lineWidth, lineWidth), rect.bottomRight() + QPoint(-lineWidth, -lineWidth));
    }
    else {
        painter.drawLine(rect.topRight() + QPoint(-lineWidth, lineWidth), rect.bottomLeft() + QPoint(lineWidth, - lineWidth));
    }

    QLabel::paintEvent(ev);
}
