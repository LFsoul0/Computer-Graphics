#include "SceneObject.h"

unsigned long SceneObject::objectCount = 0;

SceneObject::SceneObject(QObject *parent)
    : QObject(parent),
      VAO(parent),
      VBO(QOpenGLBuffer::VertexBuffer),
      EBO(QOpenGLBuffer::IndexBuffer)
{
    name = QString("object %1").arg(objectCount++);
    pos = QVector3D(0, 0, 0);
    rotate = QVector3D(0, 0, 0);
    scale = QVector3D(1, 1, 1);

    focus = false;
}

SceneObject::~SceneObject()
{

}

QString SceneObject::getFilePath() const
{
    return filePath;
}

void SceneObject::setFilePath(const QString& path)
{
    this->filePath = path;
}

QString SceneObject::getName() const
{
    return name;
}

void SceneObject::setName(const QString& name)
{
    this->name = name;
    emit changeName(name);
}

QVector3D SceneObject::getPos() const
{
    return pos;
}

void SceneObject::setPos(const QVector3D& pos)
{
    this->pos = pos;
    transMat.setToIdentity();
    transMat.translate(pos);
    updateModel();
}

QVector3D SceneObject::getRotate() const
{
    return rotate;
}

void SceneObject::setRotate(const QVector3D& rotate)
{
    this->rotate = rotate;
    rotMat.setToIdentity();
    rotMat.rotate(rotate.z(), QVector3D(0, 0, 1));
    rotMat.rotate(rotate.x(), QVector3D(1, 0, 0));
    rotMat.rotate(rotate.y(), QVector3D(0, 1, 0));
    updateModel();
}

QVector3D SceneObject::getScale() const
{
    return scale;
}

void SceneObject::setScale(const QVector3D& scale)
{
    this->scale = scale;
    scaleMat.setToIdentity();
    scaleMat.scale(scale);
    updateModel();
}

QMatrix4x4 SceneObject::getModel() const
{
    return model;
}

void SceneObject::updateModel()
{
    model.setToIdentity();
    model = transMat * rotMat * scaleMat;
}

void SceneObject::setFocus(bool focus)
{
    if (focus) {
        focusIn();
    }
    else {
        focusOut();
    }
}

void SceneObject::focusIn()
{
    if (focus) return;
    focus = true;
    emit changeFocus(true);
}

void SceneObject::focusOut()
{
    if (!focus) return;
    focus = false;
    emit changeFocus(false);
}
