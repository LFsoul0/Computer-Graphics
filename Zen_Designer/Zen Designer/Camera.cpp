#include "Camera.h"

Camera::Camera(QWidget *parent) : QObject(parent)
{
    widget = parent;

    yaw = 0;
    pitch = 0;
    border = QRectF(QPointF(-50, -50), QPointF(50, 50));
    cameraPos = QVector3D(0, 1, 0);
    updateDirection();

    timer = new QTimer(this);
    timer->setInterval(20);
    connect(timer, &QTimer::timeout, this, &Camera::updatePos);
}

float Camera::getYaw() const
{
    return yaw;
}

void Camera::setYaw(float value)
{
    yaw = value;
    updateDirection();
}

float Camera::getPitch() const
{
    return pitch;
}

void Camera::setPitch(float value)
{
    pitch = value;
    updateDirection();
}

QRectF Camera::getBorder() const
{
    return border;
}

void Camera::setBorder(const QRectF& rect)
{
    border = rect;
}

QVector3D Camera::getCameraPos() const
{
    return cameraPos;
}

void Camera::setCameraPos(const QVector3D& value)
{
    cameraPos = value;
    updateView();
}

void Camera::handle(QEvent *e)
{
    if(e->type()==QEvent::MouseMove){
        QMouseEvent *event=static_cast<QMouseEvent*>(e);

        float xoffset = event->localPos().x() - widget->rect().center().x();
        float yoffset = widget->rect().center().y() - event->localPos().y();
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;
        if(pitch > 1.55) // 89°
            pitch =  1.55;
        if(pitch < -1.55) // -89°
            pitch = -1.55;

        updateDirection();
        QCursor::setPos(widget->mapToGlobal(widget->rect().center()));
    }
    else if(e->type() == QEvent::KeyPress){
        QKeyEvent *event = static_cast<QKeyEvent*>(e);

        switch (event->key()) {
        case Qt::Key_Escape:
            widget->clearFocus();
            break;
        default:
            if (!event->isAutoRepeat()) {
                if (keys.isEmpty()) timer->start();
                keys.insert(event->key());
            }
            break;
        }
    }
    else if(e->type()==QEvent::KeyRelease){
        QKeyEvent *event=static_cast<QKeyEvent*>(e);
        if (!event->isAutoRepeat()) {
            keys.remove(event->key());
            if (keys.isEmpty()) timer->stop();
        }
    }
    else if(e->type() == QEvent::FocusIn){
        widget->setCursor(Qt::BlankCursor);
        QCursor::setPos(widget->mapToGlobal(widget->rect().center()));
        widget->setMouseTracking(true);
    }
    else if(e->type() == QEvent::FocusOut){
        widget->setCursor(Qt::ArrowCursor);
        widget->setMouseTracking(false);
    }
}

void Camera::init()
{
    updateView();
    widget->activateWindow();
    widget->setFocus();
}

QMatrix4x4 Camera::getView() const
{
    return view;
}

void Camera::updateView()
{
    view.setToIdentity();
    view.lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
}

void Camera::updateDirection()
{
    cameraDirection = QVector3D(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
    cameraRight = QVector3D(QVector3D::crossProduct(cameraDirection, QVector3D(0.0f,1.0f,0.0f)));
    cameraUp = QVector3D(QVector3D::crossProduct(cameraRight, cameraDirection));

    updateView();
}

void Camera::updatePos()
{
    if (keys.contains(Qt::Key_W))
        cameraPos += moveSpeed * QVector3D::crossProduct(QVector3D(0, 1, 0), cameraRight);
    if (keys.contains(Qt::Key_S))
        cameraPos -= moveSpeed * QVector3D::crossProduct(QVector3D(0, 1, 0), cameraRight);
    if (keys.contains(Qt::Key_A))
        cameraPos -= moveSpeed * cameraRight;
    if (keys.contains(Qt::Key_D))
        cameraPos += moveSpeed * cameraRight;

    if (cameraPos.x() < border.left()) cameraPos.setX(border.left());
    if (cameraPos.x() > border.right()) cameraPos.setX(border.right());
    if (cameraPos.z() < border.top()) cameraPos.setZ(border.top());
    if (cameraPos.z() > border.bottom()) cameraPos.setZ(border.bottom());

    updateView();
}
