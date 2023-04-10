#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <QTimer>
#include <QtMath>
#include <QDataStream>

#include "Camera.h"
#include "SceneObject.h"
#include "Particle.h"

class EnvironmentBoard;

class SceneViewWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    friend EnvironmentBoard;

    Q_OBJECT
public:
    explicit SceneViewWidget(QWidget *parent = nullptr);
    virtual ~SceneViewWidget();

    void show();
    void hide();
    void update();

    SceneObject* addObject(QDataStream& ds);
    void removeObject(SceneObject* obj);

    void save(QDataStream& ds);
    void load(QDataStream& ds);
    void clear();

    void setParticleOn();
    void setParticleOff();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual bool event(QEvent* ev) override;
    virtual void focusInEvent(QFocusEvent* ev) override;

private:
    static constexpr float mouseSensitivity = 0.2;
    static constexpr unsigned int bezierSampleNum = 200;
    static constexpr int rotateSampleNum = 72 + 1;
    static constexpr float modelScale = 1.0 / 600;
    static constexpr int shadowSize = 2048;
    static constexpr int particleEmitGap = 5;

    bool initial;

    struct Light {
        QVector3D direction;
        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
    };
    Light lights[4]; // fit to skybox
    int envIndex;
    bool emitPartical;
    bool addShadow;

    // skybox
    QOpenGLShaderProgram* skyboxShader;
    QOpenGLVertexArrayObject* skyboxVAO;
    QOpenGLBuffer* skyboxVBO;
    QOpenGLBuffer* skyboxEBO;
    // skyboxes list
    // 0. day
    // 1. night
    // 2. sunrise
    // 3. sunset
    QOpenGLTexture* skybox[4];

    // objects
    QOpenGLShaderProgram* objectShader;
    QOpenGLShaderProgram* singleColorShader;
    QVector<SceneObject *> objects;
    SceneObject* selectedObject;
    // materials list
    // 0. wood1 +norm
    // 1. wood2 +norm
    // 2. rock1 +norm +spec(0)
    // 3. rock2 +norm +spec(1)
    // 4. china1
    // 5. china2
    QVector<QOpenGLTexture *> diffTexs;
    QVector<QOpenGLTexture *> normTexs;
    QVector<QOpenGLTexture *> specTexs;

    // ground
    QOpenGLVertexArrayObject* groundVAO;
    QOpenGLBuffer* groundVBO;
    QOpenGLTexture* groundDiff;
    QOpenGLTexture* groundNorm;
    QOpenGLTexture* groundSpec;

    // shadow
    QOpenGLShaderProgram* shadowShader;
    unsigned int shadowFBO;
    QOpenGLTexture* shadowMap;

    // particle
    QOpenGLShaderProgram* particleShader;
    QVector<Particle *> particles;
    QOpenGLVertexArrayObject* particleVAO;
    QOpenGLBuffer* particleVBO;
    QOpenGLTexture* particleTex[3];
    int currentFrameCount;
    //QTimer* particleTimer; // for generate particle

    QMatrix4x4 projectionMat;
    QTimer* timer; // for render loop

    Camera* camera;

    void addParticle();
    void renderShadow();

    void makeSkyBox();
    void makeGround();
    void loadMaterialTexs();
    void makeParticle();
    void makeShader(QOpenGLShaderProgram* shader, const QString& vertexSourcePath, const QString& fragmentSourcePath);

signals:
    void focusIn();
    void loadObject(SceneObject* obj);

};

#endif // SCENEWIDGET_H
