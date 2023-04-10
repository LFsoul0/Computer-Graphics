#ifndef PARTICLE_H
#define PARTICLE_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QRandomGenerator>

class SceneViewWidget;

class Particle : public QObject
{
    friend SceneViewWidget;

    Q_OBJECT
public:
    explicit Particle(QObject *parent = nullptr);

    void update();

    QMatrix4x4 getModel();

private:
    static constexpr int texNum = 3;

    int lifetime;

    int texIndex;

    QVector3D trans;
    QVector3D rotate; // xy for scale, z for rotate

    QVector3D v; // velocity
    QVector3D a; // accelerate
    QVector3D w; // angular velocity
    QVector3D b; // angular accelerate

signals:
    void endLife(Particle* p);

};

#endif // PARTICLE_H
