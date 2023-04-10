#include "Particle.h"

Particle::Particle(QObject *parent) : QObject(parent)
{
    lifetime = 200;

    texIndex = QRandomGenerator::global()->generate() % texNum;

    double dis = 1 * QRandomGenerator::global()->generateDouble() + 1;
    trans = QVector3D(2 * QRandomGenerator::global()->generateDouble(), dis, -1.5 * dis);
    rotate = QVector3D(0, 0, 360 * QRandomGenerator::global()->generateDouble());

    v = QVector3D(-0.02 * QRandomGenerator::global()->generateDouble(),
                  -0.05 * QRandomGenerator::global()->generateDouble(),
                  0.01 * QRandomGenerator::global()->generateDouble() - 0.005);

    a = QVector3D(-0.002 * QRandomGenerator::global()->generateDouble(),
                  -0.001 * QRandomGenerator::global()->generateDouble(),
                  0.0005 * QRandomGenerator::global()->generateDouble() - 0.00025);

    w = QVector3D(0, 0, 1 * QRandomGenerator::global()->generateDouble() - 0.5);

    b = QVector3D(0, 0, 0.1 * QRandomGenerator::global()->generateDouble() - 0.05);
}

void Particle::update()
{
    if (--lifetime < 0) {
        emit endLife(this);
        return;
    }

    trans += v;
    rotate += w;
    v += a;
    w += b;
}

QMatrix4x4 Particle::getModel()
{
    QMatrix4x4 rotMat, transMat;
    rotMat.rotate(rotate.z(), QVector3D(0, 0, 1));
    transMat.translate(trans);

    return transMat * rotMat;
}
