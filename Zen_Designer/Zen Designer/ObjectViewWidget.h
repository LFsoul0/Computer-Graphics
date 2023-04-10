#ifndef OBJECTVIEWWIDGET_H
#define OBJECTVIEWWIDGET_H

#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QDataStream>

class MaterialBoard;
class LightBoard;

class ObjectViewWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    friend MaterialBoard;
    friend LightBoard;

    Q_OBJECT
public:
    explicit ObjectViewWidget(QWidget *parent = nullptr);
    virtual ~ObjectViewWidget();

    void setRotatorLine(const QVector<QVector3D>& line);
    void setTextureRepeat(int times);
    void show();
    void hide();
    void update();

    void save(QDataStream& ds);
    void load(QDataStream& ds);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void mousePressEvent(QMouseEvent* ev) override;
    virtual void mouseMoveEvent(QMouseEvent* ev) override;
    virtual void mouseReleaseEvent(QMouseEvent* ev) override;
    virtual void wheelEvent(QWheelEvent* ev) override;

private:
    static constexpr int rotateSampleNum = 72 + 1;
    static constexpr float scale = 1.0 / 600;
    static constexpr float mouseSensitivity = 0.2;
    static constexpr float wheelSensitivity = 0.0002;

    bool initial;
    bool isDragging;
    QPointF mousePos;

    struct Material {
        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
        float shininess;
        int texIndex;
    } material;
    int textureRepeat;

    struct Light {
        QVector3D direction;
        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
    } light;

    QVector<QVector3D> line;

    // object
    QOpenGLShaderProgram* objectShader;
    QVector<float> vertices;
    QVector<float> normals;
    QVector<float> tangents;
    QVector<float> texcoords;
    QVector<unsigned int> indices;
    QOpenGLVertexArrayObject objectVAO;
    QOpenGLBuffer objectVBO;
    QOpenGLBuffer objectEBO;
    //QOpenGLFramebufferObject* objectFBO;

    // screen quad
    QOpenGLShaderProgram* blendShader;
    QOpenGLVertexArrayObject quadVAO;
    QOpenGLBuffer quadVBO;

    // textures
    QOpenGLTexture* background;
    // materials list
    // 0. wood1 +norm
    // 1. wood2 +norm
    // 2. rock1 +norm +spec
    // 3. rock2 +norm +spec
    // 4. china1
    // 5. china2
    QVector<QOpenGLTexture *> diffTexs;
    QVector<QOpenGLTexture *> normTexs;
    QVector<QOpenGLTexture *> specTexs;

    QVector3D viewPos;
    QMatrix4x4 modelMat, viewMat, projectionMat;
    QTimer* timer;  // for render loop

    void makeObject();
    void makeShader(QOpenGLShaderProgram* shader, const QString& vertexSourcePath, const QString& fragmentSourcePath);

signals:

};

#endif // OBJECTVIEWWIDGET_H
