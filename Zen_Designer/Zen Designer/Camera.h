#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QSet>
#include <QVector3D>
#include <QEvent>
#include <QWidget>
#include <QtMath>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QTimer>

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QWidget *parent = nullptr);

    float getYaw() const;
    void setYaw(float value);

    float getPitch() const;
    void setPitch(float value);

    QRectF getBorder() const;
    void setBorder(const QRectF& rect);

    QVector3D getCameraPos() const;
    void setCameraPos(const QVector3D& value);

    void init();

    void handle(QEvent *event);

    QMatrix4x4 getView() const;

private:
    static constexpr float sensitivity = 0.002;
    static constexpr float moveSpeed = 0.02;

    QWidget *widget;

    float yaw;
    float pitch;

    QRectF border;
    QVector3D cameraPos;

    QVector3D cameraDirection;
    QVector3D cameraRight;
    QVector3D cameraUp;

    QSet<int> keys;

    QTimer* timer;

    QMatrix4x4 view;

    void updateView();
    void updateDirection();
    void updatePos();

signals:

};

#endif // CAMERA_H
