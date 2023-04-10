#include "DragablePoint.h"

DragablePoint::DragablePoint(QWidget *parent) : DragablePoint(true, parent) { }

DragablePoint::DragablePoint(bool autoDrag, QWidget *parent) : QLabel(parent)
{
    range = parent->rect();

    if (autoDrag) {
        connect(this, &DragablePoint::drag, this, [=](QPoint offset) {
            move(pos() + offset);
        });
    }
}

void DragablePoint::setRangeRect(const QRect& r)
{
    range = r;
}

void DragablePoint::mousePressEvent(QMouseEvent* ev)
{
    QRect rect = this->rect();
    if (rect.contains(ev->pos())) {
        isDragging = true;
        emit pressed();
    }

    QWidget::mousePressEvent(ev);
}

void DragablePoint::mouseMoveEvent(QMouseEvent* ev)
{
    if (isDragging) {
        QPoint p = pos() + ev->pos();
        p.setX(qMin(qMax(p.x(), range.left()), range.right())); // clamp X
        p.setY(qMin(qMax(p.y(), range.top()), range.bottom())); // clamp Y
        emit drag(p - pos() - QPoint(width() / 2, height() / 2));
    }

    QWidget::mouseMoveEvent(ev);
}

void DragablePoint::mouseReleaseEvent(QMouseEvent* ev)
{
    if (isDragging) {
        isDragging = false;
    }

    QWidget::mouseReleaseEvent(ev);
}
