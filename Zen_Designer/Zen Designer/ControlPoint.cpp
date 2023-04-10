#include "ControlPoint.h"

const QSize ControlPoint::centralPointSize = QSize(30, 30);
const QSize ControlPoint::attachPointSize = QSize(20, 20);

ControlPoint::ControlPoint(QWidget *parent) : ControlPoint("New Point", parent) { }

ControlPoint::ControlPoint(const QString& name, QWidget *parent) : QObject(parent), name(name)
{
    normalPix = QPixmap(":/image/point/ControlPointNormal.png").scaled(centralPointSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    selectedPix = QPixmap(":/image/point/ControlPointSelected.png").scaled(centralPointSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    attachPix = QPixmap(":/image/point/AttachPoint.png").scaled(attachPointSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    focus = false;
    type = Curve;
    pos = QPoint(0, 0);
    offset = QPoint(100, 0);

    attachLine = new AttachLine(parent);
    attachLine->hide();

    centralPoint = new DragablePoint(false, parent);
    centralPoint->setFixedSize(centralPointSize);
    centralPoint->setPixmap(normalPix);
    connect(centralPoint, &DragablePoint::pressed, this, &ControlPoint::focusIn);
    connect(centralPoint, &DragablePoint::drag, this, [=](QPoint offset) {
        move(pos + offset, false);
        emit dragPos(getPos());
    });
    centralPoint->show();

    forwardPoint = new DragablePoint(false, parent);
    forwardPoint->setFixedSize(attachPointSize);
    forwardPoint->setPixmap(attachPix);
    connect(forwardPoint, &DragablePoint::drag, this, [=](QPoint off) {
        setOffset(offset + off, false);
        emit dragOffset(getOffset());
    });
    forwardPoint->hide();

    backwardPoint = new DragablePoint(false, parent);
    backwardPoint->setFixedSize(attachPointSize);
    backwardPoint->setPixmap(attachPix);
    connect(backwardPoint, &DragablePoint::drag, this, [=](QPoint off) {
        setOffset(offset - off, false);
        emit dragOffset(getOffset());
    });
    backwardPoint->hide();
}

ControlPoint::~ControlPoint()
{
    centralPoint->deleteLater();
    attachLine->deleteLater();
    forwardPoint->deleteLater();
    backwardPoint->deleteLater();
}

void ControlPoint::setOriginPoint(int x, int y)
{
    setOriginPoint(QPoint(x, y));
}

void ControlPoint::setOriginPoint(const QPoint& p)
{
    origin = p;
}

void ControlPoint::setDragRange(int left, int top, int width, int height)
{
    setDragRange(QRect(left, top, width, height));
}

void ControlPoint::setDragRange(const QRect& r)
{
    centralPoint->setRangeRect(r);
}

QString ControlPoint::getName()
{
    return name;
}

void ControlPoint::setName(const QString& name)
{
    this->name = name;
    emit changeName(name);
}

ControlPoint::Type ControlPoint::getType()
{
    return type;
}

void ControlPoint::setType(Type t)
{
    if (type == t) return;
    type = t;
    emit changeType(type);
}

QPoint ControlPoint::getPos(bool relative)
{
    if (relative) {
        return QPoint(pos.x() - origin.x(), origin.y() - pos.y());
    }
    else {
        return pos;
    }
}

void ControlPoint::move(int x, int y, bool relative)
{
    move(QPoint(x, y), relative);
}

void ControlPoint::move(const QPoint& p, bool relative)
{
    if (relative) {
        pos = QPoint(origin.x() + p.x(), origin.y() - p.y());
    }
    else {
        pos = p;
    }
    updateAllPos();
    emit changePos(getPos());
}

QPoint ControlPoint::getOffset(bool relative)
{
    if (relative) {
        return QPoint(offset.x(), -offset.y());
    }
    else {
        return offset;
    }
}

void ControlPoint::setOffset(int x, int y, bool relative)
{
    setOffset(QPoint(x, y), relative);
}

void ControlPoint::setOffset(const QPoint& p, bool relative)
{
    if (relative) {
        offset = QPoint(p.x(), -p.y());
    }
    else {
        offset = p;
    }
    updateAllPos();
    emit changeOffset(getOffset());
}

QPoint ControlPoint::getForwardPoint(bool relative)
{
    return getPos(relative) + getOffset(relative);
}

QPoint ControlPoint::getBackwardPoint(bool relative)
{
    return getPos(relative) - getOffset(relative);
}

void ControlPoint::setFocus(bool focus)
{
    if (focus) {
        focusIn();
    }
    else {
        focusOut();
    }
}

void ControlPoint::focusIn()
{
    if (focus) return;

    focus = true;
    centralPoint->setPixmap(selectedPix);
    attachLine->show();
    forwardPoint->show();
    backwardPoint->show();
    emit changeFocus(true);
}

void ControlPoint::focusOut()
{
    if (!focus) return;

    focus = false;
    centralPoint->setPixmap(normalPix);
    attachLine->hide();
    forwardPoint->hide();
    backwardPoint->hide();
    emit changeFocus(false);
}

void ControlPoint::updateAllPos()
{
    centralPoint->move(pos - QPoint(centralPointSize.width() / 2, centralPointSize.height() / 2));
    attachLine->setLine(pos - offset, pos + offset);
    forwardPoint->move(pos + offset - QPoint(attachPointSize.width() / 2, attachPointSize.height() / 2));
    backwardPoint->move(pos - offset - QPoint(attachPointSize.width() / 2, attachPointSize.height() / 2));
}
