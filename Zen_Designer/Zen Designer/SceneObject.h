#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QObject>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

class SceneViewWidget;

class SceneObject : public QObject
{
    friend SceneViewWidget;

    Q_OBJECT
public:
    explicit SceneObject(QObject *parent = nullptr);
    virtual ~SceneObject();

    QString getFilePath() const;
    void setFilePath(const QString& path);
    QString getName() const;
    void setName(const QString& name);
    QVector3D getPos() const;
    void setPos(const QVector3D& pos);
    QVector3D getRotate() const;
    void setRotate(const QVector3D& rotate);
    QVector3D getScale() const;
    void setScale(const QVector3D& scale);

    QMatrix4x4 getModel() const;

    void setFocus(bool focus);
    void focusIn();
    void focusOut();

private:
    static unsigned long objectCount;

    bool focus;

    QString filePath;
    QString name;
    QVector3D pos;
    QVector3D rotate;
    QVector3D scale;

    QMatrix4x4 model;
    QMatrix4x4 transMat;
    QMatrix4x4 rotMat;
    QMatrix4x4 scaleMat;

    struct Material {
        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
        float shininess;
        int texIndex;
    } material;
    int textureRepeat;
    int indicesSize;

    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO;
    QOpenGLBuffer EBO;

    void updateModel();

signals:
    void changeName(const QString& name);
    void changeFocus(bool focus);

};

#endif // SCENEOBJECT_H
