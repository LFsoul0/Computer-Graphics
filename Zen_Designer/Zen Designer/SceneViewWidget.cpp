#include "SceneViewWidget.h"

SceneViewWidget::SceneViewWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(3,3);
    format.setSamples(4);   // MSAA
    setFormat(format);

    initial = true;
    envIndex = 0;
    emitPartical = true;
    addShadow = false;
    selectedObject = nullptr;

    // day
    lights[0].direction = QVector3D(0, -0.866025, -0.5);
    lights[0].ambient = QVector3D(1, 1, 0.8784314) * 0.25;
    lights[0].diffuse = QVector3D(1, 1, 0.8784314) * 1.25;
    lights[0].specular = QVector3D(1, 1, 0.8784314) * 1.25;

    // night
    lights[1].direction = QVector3D(0.809017 * 0.275637, -0.587785, 0.809017 * 0.961262);
    lights[1].ambient = QVector3D(0.2156863, 0.372549, 0.5294118) * 0.3;
    lights[1].diffuse = QVector3D(0.2156863, 0.372549, 0.5294118) * 1.3;
    lights[1].specular = QVector3D(0.2156863, 0.372549, 0.5294118) * 1.3;

    // sunrise
    lights[2].direction = QVector3D(-0.913545, -0.406737, -1);
    lights[2].ambient = QVector3D(1, 0.8705882, 0.1960784) * 0.2;
    lights[2].diffuse = QVector3D(1, 0.8705882, 0.1960784) * 1.2;
    lights[2].specular = QVector3D(1, 0.8705882, 0.1960784) * 1.2;

    // sunset
    lights[3].direction = QVector3D(0.79229, -0.610145, 0);
    lights[3].ambient = QVector3D(0.9960785, 0.7058824, 0.1254902) * 0.6;
    lights[3].diffuse = QVector3D(0.9960785, 0.7058824, 0.1254902) * 1.6;
    lights[3].specular = QVector3D(0.9960785, 0.7058824, 0.1254902) * 1.6;

    skyboxShader = new QOpenGLShaderProgram(this);
    skyboxVAO = new QOpenGLVertexArrayObject(this);
    skyboxVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    skyboxEBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    skybox[0] = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    skybox[1] = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    skybox[2] = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    skybox[3] = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);

    objectShader = new QOpenGLShaderProgram(this);
    singleColorShader = new QOpenGLShaderProgram(this);
    diffTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 0. wood1
    diffTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 1. wood2
    diffTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 2. rock1
    diffTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 3. rock2
    diffTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 4. china1
    diffTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 5. china2
    normTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 0. wood1
    normTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 1. wood2
    normTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 2. rock1
    normTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 3. rock2
    specTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 0. rock1 (-2)
    specTexs.append(new QOpenGLTexture(QOpenGLTexture::Target2D)); // 1. rock2

    groundVAO = new QOpenGLVertexArrayObject(this);
    groundVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    groundDiff = new QOpenGLTexture(QOpenGLTexture::Target2D);
    groundNorm = new QOpenGLTexture(QOpenGLTexture::Target2D);
    groundSpec = new QOpenGLTexture(QOpenGLTexture::Target2D);

    shadowShader = new QOpenGLShaderProgram(this);
    shadowMap = new QOpenGLTexture(QOpenGLTexture::Target2D);

    particleShader = new QOpenGLShaderProgram(this);
    particleVAO = new QOpenGLVertexArrayObject(this);
    particleVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    particleTex[0] = new QOpenGLTexture(QOpenGLTexture::Target2D);
    particleTex[1] = new QOpenGLTexture(QOpenGLTexture::Target2D);
    particleTex[2] = new QOpenGLTexture(QOpenGLTexture::Target2D);
//    particleTimer = new QTimer(this);
//    particleTimer->setInterval(200);
//    connect(particleTimer, &QTimer::timeout, this, &SceneViewWidget::addParticle);
    currentFrameCount = 0;

    timer = new QTimer(this);
    timer->setInterval(20);
    connect(timer, &QTimer::timeout, this, &SceneViewWidget::update);

    camera = new Camera(this);

    setFocusPolicy(Qt::ClickFocus);
}

SceneViewWidget::~SceneViewWidget()
{
    delete skyboxVBO;
    delete skyboxEBO;
    for (int i = 0; i < 4; ++i) {
        delete skybox[i];
    }

    delete groundVBO;
    delete groundDiff;
    delete groundNorm;
    delete groundSpec;
}

void SceneViewWidget::show()
{
    QOpenGLWidget::show();
    timer->start();
//    if (emitPartical) {
//        particleTimer->start();
//    }
}

void SceneViewWidget::hide()
{
    timer->stop();
    // particleTimer->stop();
    QOpenGLWidget::hide();
}

void SceneViewWidget::update()
{
    QOpenGLWidget::update();
}

void SceneViewWidget::setParticleOn()
{
    emitPartical = true;
    // particleTimer->start();
}

void SceneViewWidget::setParticleOff()
{
    emitPartical = false;
    // particleTimer->stop();
}

void SceneViewWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // shaders
    makeShader(skyboxShader, ":/shader/skybox.vert", ":/shader/skybox.frag");
    makeShader(objectShader, ":/shader/object.vert", ":/shader/object.frag");
    makeShader(singleColorShader, ":/shader/object.vert", ":/shader/singleColor.frag");
    makeShader(shadowShader, ":/shader/shadow.vert", ":/shader/shadow.frag");
    makeShader(particleShader, ":/shader/particle.vert", ":/shader/particle.frag");
    // skybox
    makeSkyBox();

    // ground
    makeGround();

    // object
    loadMaterialTexs();

    // shadow
    glGenFramebuffers(1, &shadowFBO);
    shadowMap->create();
    shadowMap->setSize(shadowSize, shadowSize);
    shadowMap->setFormat(QOpenGLTexture::D32F);
    shadowMap->allocateStorage();
    shadowMap->setMinificationFilter(QOpenGLTexture::Nearest);
    shadowMap->setMagnificationFilter(QOpenGLTexture::Nearest);
    shadowMap->setWrapMode(QOpenGLTexture::ClampToBorder);
    shadowMap->setBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap->textureId(), 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "Framebuffer not complete!" << Qt::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

    // particle
    makeParticle();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
//    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera->init();
    initial = false;
    timer->start();
    setParticleOn();
}

void SceneViewWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    projectionMat.setToIdentity();
    projectionMat.perspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
}

void SceneViewWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // shadow map
    QVector3D lightPos = camera->getCameraPos() - lights[envIndex].direction.normalized() * 5;
    QMatrix4x4 lightProjection, lightView, lightSpaceMatrix;
    lightProjection.ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
    lightView.lookAt(lightPos, camera->getCameraPos(), QVector3D(0, 1, 0));
    lightSpaceMatrix = lightProjection * lightView;

    if (addShadow) {
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, shadowSize, shadowSize);

        shadowShader->bind();
        shadowShader->setUniformValue("lightSpaceMatrix", lightSpaceMatrix);

//        glCullFace(GL_FRONT);

        // render objects
        for (SceneObject* obj : objects) {
            shadowShader->setUniformValue("model", obj->getModel());

            obj->VAO.bind();
            glDrawElements(GL_TRIANGLES, obj->indicesSize, GL_UNSIGNED_INT, 0);
            obj->VAO.release();
        }

//        glCullFace(GL_BACK);

        // render ground
        shadowShader->setUniformValue("model", QMatrix4x4());

        groundVAO->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        groundVAO->release();

        glViewport(0, 0, width(), height());

        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    }

    // render object
    objectShader->bind();
    objectShader->setUniformValue("projection", projectionMat);
    objectShader->setUniformValue("view", camera->getView());
    objectShader->setUniformValue("viewPos", camera->getCameraPos());
    objectShader->setUniformValue("light.direction", lights[envIndex].direction);
    objectShader->setUniformValue("light.ambient", lights[envIndex].ambient);
    objectShader->setUniformValue("light.diffuse", lights[envIndex].diffuse);
    objectShader->setUniformValue("light.specular", lights[envIndex].specular);
    objectShader->setUniformValue("haveShadowMap", addShadow);
    objectShader->setUniformValue("lightSpaceMatrix", lightSpaceMatrix);
    shadowMap->bind(3);
    objectShader->setUniformValue("shadowMap", 3);

    for (SceneObject* obj : objects) {
        if (obj->focus) {
            glEnable(GL_STENCIL_TEST);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        }

        objectShader->bind();
        objectShader->setUniformValue("model", obj->getModel());

        if (obj->material.texIndex < 0) {
            objectShader->setUniformValue("material.haveDiffTex", false);
            objectShader->setUniformValue("material.diffuse", obj->material.diffuse);
        }
        else {
            objectShader->setUniformValue("material.haveDiffTex", true);
            diffTexs[obj->material.texIndex]->bind(0);
            objectShader->setUniformValue("material.diffTex", 0);
        }
        if (obj->material.texIndex >= 0 && obj->material.texIndex < 4) {
            objectShader->setUniformValue("material.haveNormTex", true);
            normTexs[obj->material.texIndex]->bind(1);
            objectShader->setUniformValue("material.normTex", 1);
        }
        else {
            objectShader->setUniformValue("material.haveNormTex", false);
        }
        if (obj->material.texIndex >= 2 && obj->material.texIndex < 4) {
            objectShader->setUniformValue("material.haveSpecTex", true);
            specTexs[obj->material.texIndex - 2]->bind(2);
            objectShader->setUniformValue("material.specTex", 2);
        }
        else {
            objectShader->setUniformValue("material.haveSpecTex", false);
            objectShader->setUniformValue("material.specular", obj->material.specular);
        }

        objectShader->setUniformValue("material.ambient", obj->material.ambient);
        objectShader->setUniformValue("material.shininess", obj->material.shininess);

        obj->VAO.bind();
        glDrawElements(GL_TRIANGLES, obj->indicesSize, GL_UNSIGNED_INT, 0);
        obj->VAO.release();

        if (obj->focus) {
            glStencilMask(0x00);
            glDisable(GL_STENCIL_TEST);
        }
    }

    // render ground
    objectShader->bind();
    objectShader->setUniformValue("model", QMatrix4x4());

    objectShader->setUniformValue("material.haveDiffTex", true);
    groundDiff->bind(0);
    objectShader->setUniformValue("material.diffTex", 0);

    objectShader->setUniformValue("material.haveNormTex", true);
    groundNorm->bind(1);
    objectShader->setUniformValue("material.normTex", 1);

    objectShader->setUniformValue("material.haveSpecTex", true);
    groundSpec->bind(2);
    objectShader->setUniformValue("material.specTex", 2);

    objectShader->setUniformValue("material.ambient", QVector3D(0.2, 0.2, 0.2));
    objectShader->setUniformValue("material.shininess", 72.0f);

    groundVAO->bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    groundVAO->release();

    // render skybox
    glDepthFunc(GL_LEQUAL);
    skyboxShader->bind();
    skyboxShader->setUniformValue("view", QMatrix4x4(camera->getView().toGenericMatrix<3, 3>()));
    skyboxShader->setUniformValue("projection", projectionMat);
    skybox[envIndex]->bind(0);
    skyboxShader->setUniformValue("skybox", 0);

    skyboxVAO->bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    skyboxVAO->release();

    glDepthFunc(GL_LESS);

    if (selectedObject) {
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        QMatrix4x4 model = selectedObject->getModel();
        model.scale(1.02);
        singleColorShader->bind();
        singleColorShader->setUniformValue("model", model);
        singleColorShader->setUniformValue("view", camera->getView());
        singleColorShader->setUniformValue("projection", projectionMat);
        singleColorShader->setUniformValue("color", QColor(Qt::yellow));

        selectedObject->VAO.bind();
        glDrawElements(GL_TRIANGLES, selectedObject->indicesSize, GL_UNSIGNED_INT, 0);
        selectedObject->VAO.release();

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
    }

    if (emitPartical) {
        glDisable(GL_DEPTH_TEST);
        particleShader->bind();
        particleShader->setUniformValue("view", QMatrix4x4());
        particleShader->setUniformValue("projection", projectionMat);
        particleShader->setUniformValue("lightColor", lights[envIndex].diffuse);
        for (Particle* p : particles) {
            particleShader->setUniformValue("model", p->getModel());
            particleTex[p->texIndex]->bind(0);
            particleShader->setUniformValue("tex", 0);

            particleVAO->bind();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            particleVAO->release();

            p->update();
        }
        glEnable(GL_DEPTH_TEST);

        if (--currentFrameCount < 0) {
            addParticle();
            currentFrameCount = particleEmitGap;
        }
    }

}

void SceneViewWidget::addParticle()
{
    Particle* p = new Particle(this);
    particles.append(p);
    connect(p, &Particle::endLife, this, [=](Particle* p) {
        particles.removeOne(p);
        p->deleteLater();
    });
}

void SceneViewWidget::renderShadow()
{

}

void SceneViewWidget::makeSkyBox()
{
    // skybox VAO
    QVector<float> skyboxVertices = {
        // positions
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f
    };
    QVector<unsigned int> skyboxIndices = {
        0, 4, 6,
        6, 2, 0,
        5, 1, 3,
        3, 7, 5,
        1, 5, 4,
        4, 0, 1,
        2, 6, 7,
        7, 3, 2,
        1, 0, 2,
        2, 3, 1,
        4, 5, 7,
        7, 6, 4
    };

    if (!skyboxVAO->create()) qDebug() << "Fail to create VAO" << Qt::endl;
    if (!skyboxVBO->create()) qDebug() << "Fail to create VBO" << Qt::endl;
    if (!skyboxEBO->create()) qDebug() << "Fail to create EBO" << Qt::endl;

    skyboxVAO->bind();
    skyboxVBO->bind();
    skyboxVBO->allocate(skyboxVertices.data(), skyboxVertices.size() * sizeof(float));
    skyboxEBO->bind();
    skyboxEBO->allocate(skyboxIndices.data(), skyboxIndices.size() * sizeof(unsigned int));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    skyboxVAO->release();

    // textures
    // day
    skybox[0]->create();
    skybox[0]->setSize(1024, 1024);
    skybox[0]->setFormat(QOpenGLTexture::RGB8_UNorm);
    skybox[0]->allocateStorage();
    skybox[0]->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/day/right.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[0]->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/day/left.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[0]->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/day/top.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[0]->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/day/bottom.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[0]->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/day/front.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[0]->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/day/back.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[0]->setMinificationFilter(QOpenGLTexture::Linear);
    skybox[0]->setMagnificationFilter(QOpenGLTexture::Linear);
    skybox[0]->setWrapMode(QOpenGLTexture::ClampToEdge);

    // night
    skybox[1]->create();
    skybox[1]->setSize(1024, 1024);
    skybox[1]->setFormat(QOpenGLTexture::RGB8_UNorm);
    skybox[1]->allocateStorage();
    skybox[1]->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/night/right.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[1]->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/night/left.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[1]->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/night/top.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[1]->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/night/bottom.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[1]->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/night/front.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[1]->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/night/back.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[1]->setMinificationFilter(QOpenGLTexture::Linear);
    skybox[1]->setMagnificationFilter(QOpenGLTexture::Linear);
    skybox[1]->setWrapMode(QOpenGLTexture::ClampToEdge);

    // sunrise
    skybox[2]->create();
    skybox[2]->setSize(1024, 1024);
    skybox[2]->setFormat(QOpenGLTexture::RGB8_UNorm);
    skybox[2]->allocateStorage();
    skybox[2]->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunrise/right.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[2]->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunrise/left.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[2]->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunrise/top.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[2]->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunrise/bottom.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[2]->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunrise/front.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[2]->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunrise/back.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[2]->setMinificationFilter(QOpenGLTexture::Linear);
    skybox[2]->setMagnificationFilter(QOpenGLTexture::Linear);
    skybox[2]->setWrapMode(QOpenGLTexture::ClampToEdge);

    // sunset
    skybox[3]->create();
    skybox[3]->setSize(1024, 1024);
    skybox[3]->setFormat(QOpenGLTexture::RGB8_UNorm);
    skybox[3]->allocateStorage();
    skybox[3]->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunset/right.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[3]->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunset/left.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[3]->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunset/top.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[3]->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunset/bottom.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[3]->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunset/front.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[3]->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8,
                    QImage(":/image/skybox/sunset/back.png").convertToFormat(QImage::Format_RGB888).constBits());
    skybox[3]->setMinificationFilter(QOpenGLTexture::Linear);
    skybox[3]->setMagnificationFilter(QOpenGLTexture::Linear);
    skybox[3]->setWrapMode(QOpenGLTexture::ClampToEdge);

}

void SceneViewWidget::makeGround()
{
    // ground VAO
    QVector<float> groundVertices = {
         // positions           // normals        // tangents        // texcoords
        -100.0f, 0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -100.0f,  100.0f,
         100.0f, 0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  100.0f,  100.0f,
        -100.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -100.0f, -100.0f,
         100.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  100.0f, -100.0f
    };

    if (!groundVAO->create()) qDebug() << "Fail to create VAO" << Qt::endl;
    if (!groundVBO->create()) qDebug() << "Fail to create VBO" << Qt::endl;

    groundVAO->bind();
    groundVBO->bind();
    groundVBO->allocate(groundVertices.data(), groundVertices.size() * sizeof(float));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));

    groundVAO->release();

    // textures
    groundDiff->create();
    groundDiff->setData(QImage(":/image/material/ground_diffuse.png").mirrored());
    groundDiff->setMinificationFilter(QOpenGLTexture::Linear);
    groundDiff->setMagnificationFilter(QOpenGLTexture::Linear);
    groundDiff->setWrapMode(QOpenGLTexture::Repeat);

    groundNorm->create();
    groundNorm->setData(QImage(":/image/material/ground_normal.png").mirrored());
    groundNorm->setMinificationFilter(QOpenGLTexture::Linear);
    groundNorm->setMagnificationFilter(QOpenGLTexture::Linear);
    groundNorm->setWrapMode(QOpenGLTexture::Repeat);

    groundSpec->create();
    groundSpec->setData(QImage(":/image/material/ground_spec.png").mirrored());
    groundSpec->setMinificationFilter(QOpenGLTexture::Linear);
    groundSpec->setMagnificationFilter(QOpenGLTexture::Linear);
    groundSpec->setWrapMode(QOpenGLTexture::Repeat);
}

void SceneViewWidget::loadMaterialTexs()
{
    // diffuse textures
    // 0. wood1
    diffTexs[0]->create();
    diffTexs[0]->setData(QImage(":/image/material/wood1_diffuse.png").mirrored());
    diffTexs[0]->setMinificationFilter(QOpenGLTexture::Linear);
    diffTexs[0]->setMagnificationFilter(QOpenGLTexture::Linear);
    diffTexs[0]->setWrapMode(QOpenGLTexture::Repeat);

    // 1. wood2
    diffTexs[1]->create();
    diffTexs[1]->setData(QImage(":/image/material/wood2_diffuse.png").mirrored());
    diffTexs[1]->setMinificationFilter(QOpenGLTexture::Linear);
    diffTexs[1]->setMagnificationFilter(QOpenGLTexture::Linear);
    diffTexs[1]->setWrapMode(QOpenGLTexture::Repeat);

    // 2. rock1
    diffTexs[2]->create();
    diffTexs[2]->setData(QImage(":/image/material/rock1_diffuse.png").mirrored());
    diffTexs[2]->setMinificationFilter(QOpenGLTexture::Linear);
    diffTexs[2]->setMagnificationFilter(QOpenGLTexture::Linear);
    diffTexs[2]->setWrapMode(QOpenGLTexture::Repeat);

    // 3. rock2
    diffTexs[3]->create();
    diffTexs[3]->setData(QImage(":/image/material/rock2_diffuse.png").mirrored());
    diffTexs[3]->setMinificationFilter(QOpenGLTexture::Linear);
    diffTexs[3]->setMagnificationFilter(QOpenGLTexture::Linear);
    diffTexs[3]->setWrapMode(QOpenGLTexture::Repeat);

    // 4. china1
    diffTexs[4]->create();
    diffTexs[4]->setData(QImage(":/image/material/china1_diffuse.png").mirrored());
    diffTexs[4]->setMinificationFilter(QOpenGLTexture::Linear);
    diffTexs[4]->setMagnificationFilter(QOpenGLTexture::Linear);
    diffTexs[4]->setWrapMode(QOpenGLTexture::Repeat);

    // 5. china2
    diffTexs[5]->create();
    diffTexs[5]->setData(QImage(":/image/material/china2_diffuse.png").mirrored());
    diffTexs[5]->setMinificationFilter(QOpenGLTexture::Linear);
    diffTexs[5]->setMagnificationFilter(QOpenGLTexture::Linear);
    diffTexs[5]->setWrapMode(QOpenGLTexture::Repeat);

    // normal textures
    // 0. wood1
    normTexs[0]->create();
    normTexs[0]->setData(QImage(":/image/material/wood1_normal.png").mirrored());
    normTexs[0]->setMinificationFilter(QOpenGLTexture::Linear);
    normTexs[0]->setMagnificationFilter(QOpenGLTexture::Linear);
    normTexs[0]->setWrapMode(QOpenGLTexture::Repeat);

    // 1. wood2
    normTexs[1]->create();
    normTexs[1]->setData(QImage(":/image/material/wood2_normal.png").mirrored());
    normTexs[1]->setMinificationFilter(QOpenGLTexture::Linear);
    normTexs[1]->setMagnificationFilter(QOpenGLTexture::Linear);
    normTexs[1]->setWrapMode(QOpenGLTexture::Repeat);

    // 2. rock1
    normTexs[2]->create();
    normTexs[2]->setData(QImage(":/image/material/rock1_normal.png").mirrored());
    normTexs[2]->setMinificationFilter(QOpenGLTexture::Linear);
    normTexs[2]->setMagnificationFilter(QOpenGLTexture::Linear);
    normTexs[2]->setWrapMode(QOpenGLTexture::Repeat);

    // 3. rock2
    normTexs[3]->create();
    normTexs[3]->setData(QImage(":/image/material/rock2_normal.png").mirrored());
    normTexs[3]->setMinificationFilter(QOpenGLTexture::Linear);
    normTexs[3]->setMagnificationFilter(QOpenGLTexture::Linear);
    normTexs[3]->setWrapMode(QOpenGLTexture::Repeat);

    // spec textures
    // 0. rock1
    specTexs[0]->create();
    specTexs[0]->setData(QImage(":/image/material/rock1_spec.png").mirrored());
    specTexs[0]->setMinificationFilter(QOpenGLTexture::Linear);
    specTexs[0]->setMagnificationFilter(QOpenGLTexture::Linear);
    specTexs[0]->setWrapMode(QOpenGLTexture::Repeat);

    // 1. rock2
    specTexs[1]->create();
    specTexs[1]->setData(QImage(":/image/material/rock2_spec.png").mirrored());
    specTexs[1]->setMinificationFilter(QOpenGLTexture::Linear);
    specTexs[1]->setMagnificationFilter(QOpenGLTexture::Linear);
    specTexs[1]->setWrapMode(QOpenGLTexture::Repeat);
}

void SceneViewWidget::makeParticle()
{
    // particle VAO
    QVector<float> particleVertices = {
         // positions         // texcoords
        -0.05f,  0.05f, 0.0f, 0.0f, 1.0f,
        -0.05f, -0.05f, 0.0f, 0.0f, 0.0f,
         0.05f,  0.05f, 0.0f, 1.0f, 1.0f,
         0.05f, -0.05f, 0.0f, 1.0f, 0.0f
    };

    if (!particleVAO->create()) qDebug() << "Fail to create VAO" << Qt::endl;
    if (!particleVBO->create()) qDebug() << "Fail to create VBO" << Qt::endl;

    particleVAO->bind();
    particleVBO->bind();
    particleVBO->allocate(particleVertices.data(), particleVertices.size() * sizeof(float));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    particleVAO->release();

    // textures
    particleTex[0]->create();
    particleTex[0]->setSize(64, 64);
    particleTex[0]->setFormat(QOpenGLTexture::RGBA8_UNorm);
    particleTex[0]->allocateStorage();
    particleTex[0]->setData(0, 0, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                            QImage(":/image/particle/Petal1.png").convertToFormat(QImage::Format_RGBA8888).constBits());
    particleTex[0]->setMinificationFilter(QOpenGLTexture::Linear);
    particleTex[0]->setMagnificationFilter(QOpenGLTexture::Linear);
    particleTex[0]->setWrapMode(QOpenGLTexture::ClampToEdge);

    particleTex[1]->create();
    particleTex[1]->setSize(64, 64);
    particleTex[1]->setFormat(QOpenGLTexture::RGBA8_UNorm);
    particleTex[1]->allocateStorage();
    particleTex[1]->setData(0, 0, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                            QImage(":/image/particle/Petal2.png").convertToFormat(QImage::Format_RGBA8888).constBits());
    particleTex[1]->setMinificationFilter(QOpenGLTexture::Linear);
    particleTex[1]->setMagnificationFilter(QOpenGLTexture::Linear);
    particleTex[1]->setWrapMode(QOpenGLTexture::ClampToEdge);

    particleTex[2]->create();
    particleTex[2]->setSize(64, 64);
    particleTex[2]->setFormat(QOpenGLTexture::RGBA8_UNorm);
    particleTex[2]->allocateStorage();
    particleTex[2]->setData(0, 0, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                            QImage(":/image/particle/Petal3.png").convertToFormat(QImage::Format_RGBA8888).constBits());
    particleTex[2]->setMinificationFilter(QOpenGLTexture::Linear);
    particleTex[2]->setMagnificationFilter(QOpenGLTexture::Linear);
    particleTex[2]->setWrapMode(QOpenGLTexture::ClampToEdge);
}

void SceneViewWidget::makeShader(QOpenGLShaderProgram* shader, const QString& vertexPath, const QString& fragmentPath)
{
    if(!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexPath)){
        qDebug() << "ERROR:" << shader->log();
    }
    if(!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentPath)){
        qDebug() << "ERROR:" << shader->log();
    }
    if(!shader->link()){
        qDebug() << "ERROR:" << shader->log();
    }
}

bool SceneViewWidget::event(QEvent* ev)
{
    camera->handle(ev);
    return QOpenGLWidget::event(ev);
}

void SceneViewWidget::focusInEvent(QFocusEvent* ev)
{
    emit focusIn();
    QOpenGLWidget::focusInEvent(ev);
}

SceneObject* SceneViewWidget::addObject(QDataStream& ds)
{
    struct ControlPoint {
        enum Type {
            Curve,
            StraightLine
        } type;
        QPoint pos, offset;
    };
    QVector<ControlPoint> controlPoints;
    SceneObject* obj = new SceneObject(this);

    // load data
    int controlPointNum = 0;
    ds >> controlPointNum;
    controlPoints.resize(controlPointNum);
    QString name;
    for (ControlPoint& p : controlPoints) {
        ds >> name; // ignore name
        ds >> p.type;
        ds >> p.pos;
        ds >> p.offset;
    }
    ds >> obj->material.texIndex;
    ds >> obj->material.ambient;
    ds >> obj->material.diffuse;
    ds >> obj->material.specular;
    ds >> obj->material.shininess;
    ds >> obj->textureRepeat;
    // ignore the rest

    // generate bezier
    QVector<QVector3D> line;
    if (controlPointNum > 1) {
        const double step = (double)(controlPointNum - 1) / (bezierSampleNum - 1);

        int current = 0;
        double t = 0;
        QVector3D p0 = QVector3D(controlPoints[0].pos);
        QVector3D p1 = QVector3D(controlPoints[0].pos + controlPoints[0].offset);
        QVector3D p2 = QVector3D(controlPoints[1].pos - controlPoints[1].offset);
        QVector3D p3 = QVector3D(controlPoints[1].pos);
        for (unsigned int i = 0; i < bezierSampleNum; ++i) {
            switch (controlPoints[current].type) {
            case ControlPoint::StraightLine:
                line.append((1 - t) * p0 + t * p3);
                break;
            case ControlPoint::Curve:
                line.append(qPow(1 - t, 3) * p0 + 3 * t * qPow(1 - t, 2) * p1 + 3 * qPow(t, 2) * (1 - t) * p2 + qPow(t, 3) * p3);
                break;
            }

            t += step;
            if (t > 1 && current < controlPointNum - 2) {
                t -= 1;
                current++;
                p0 = QVector3D(controlPoints[current].pos);
                p1 = QVector3D(controlPoints[current].pos + controlPoints[current].offset);
                p2 = QVector3D(controlPoints[current + 1].pos - controlPoints[current + 1].offset);
                p3 = QVector3D(controlPoints[current + 1].pos);
            }
        }
    }
    else {
        line = QVector<QVector3D>(bezierSampleNum, QVector3D(0, 0, 0));
    }

    // make rotator
    QVector<float> vertices;
    QVector<float> normals;
    QVector<float> tangents;
    QVector<float> texcoords;
    QVector<unsigned int> indices;

    const int lineSize = line.size();
    const float angleStep = 360.0 / (rotateSampleNum - 1);
    const float texXStep = (float)obj->textureRepeat / (rotateSampleNum - 1);
    const float texYStep = -1.0 / lineSize;
    QVector2D tex = QVector2D(0, 1);
    for (int i = 0; i < lineSize; ++i) {
        QMatrix4x4 rotMat;
        tex.setX(0);
        tex.setY(tex.y() + texYStep);
        for (int j = 0; j < rotateSampleNum; ++j) {
            QVector4D p = rotMat * QVector4D(line[i], 1);
            vertices.append(p.x() * modelScale);
            vertices.append(p.y() * modelScale);
            vertices.append(p.z() * modelScale);

            QVector4D prev = rotMat * QVector4D(i > 0 ? line[i - 1] : line[i], 1);
            QVector4D next = rotMat * QVector4D(i < lineSize - 1 ? line[i + 1] : line[i], 1);
            QVector3D lineT = (next - prev).toVector3D();
            QVector3D rotT = (rotMat * QVector4D(0, 0, -1, 1)).toVector3D();
            QVector3D normal = QVector3D::crossProduct(lineT, rotT);
            lineT.normalize();
            tangents.append(lineT.x());
            tangents.append(lineT.y());
            tangents.append(lineT.z());
            normal.normalize();
            normals.append(normal.x());
            normals.append(normal.y());
            normals.append(normal.z());

            texcoords.append(tex.x());
            texcoords.append(tex.y());

            tex.setX(tex.x() + texXStep);
            rotMat.rotate(angleStep, QVector3D(0, 1, 0));
        }
    }

    const int totalNum = lineSize * rotateSampleNum;
    for (int i = 0; i < totalNum - 1; ++i) {
        if (i + 1 - rotateSampleNum >= 0) {
            indices.append(i);
            indices.append(i + 1);
            indices.append(i + 1 - rotateSampleNum);
        }
        if (i + rotateSampleNum < totalNum) {
            indices.append(i + 1);
            indices.append(i);
            indices.append(i + rotateSampleNum);
        }
    }
    obj->indicesSize = indices.size();

    // object VAO
    makeCurrent();

    if (!obj->VAO.create()) qDebug() << "Fail to create VAO" << Qt::endl;
    if (!obj->VBO.create()) qDebug() << "Fail to create VBO" << Qt::endl;
    if (!obj->EBO.create()) qDebug() << "Fail to create EBO" << Qt::endl;

    obj->VAO.bind();
    obj->VBO.bind();
    obj->VBO.allocate((vertices.size() + normals.size() + tangents.size() + texcoords.size()) * sizeof(float));
    obj->VBO.write(0, vertices.data(), vertices.size() * sizeof(float));
    obj->VBO.write(vertices.size() * sizeof(float), normals.data(), normals.size() * sizeof(float));
    obj->VBO.write((vertices.size() + normals.size()) * sizeof(float), tangents.data(), tangents.size() * sizeof(float));
    obj->VBO.write((vertices.size() + normals.size() + tangents.size()) * sizeof(float),
                    texcoords.data(), texcoords.size() * sizeof(float));
    obj->EBO.bind();
    obj->EBO.allocate(indices.data(), indices.size() * sizeof(unsigned int));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertices.size() * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((vertices.size() + normals.size()) * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void*)((vertices.size() + normals.size() + tangents.size()) * sizeof(float)));

    obj->VAO.release();
    doneCurrent();

    objects.append(obj);
    connect(obj, &SceneObject::changeFocus, this, [=](bool focus) {
        if (focus) {
            selectedObject = obj;
        }
        else if (selectedObject == obj) {
            selectedObject = nullptr;
        }
    });
    connect(obj, &SceneObject::destroyed, this, [=](QObject* obj) { removeObject(dynamic_cast<SceneObject *>(obj)); });

    return obj;
}

void SceneViewWidget::removeObject(SceneObject* obj)
{
    if (selectedObject == obj) selectedObject = nullptr;
    objects.removeOne(obj);
}

void SceneViewWidget::save(QDataStream& ds)
{
    // global properties
    ds << envIndex;
    ds << emitPartical;

    // objects
    ds << objects.size();
    for (SceneObject* obj : objects) {
        ds << obj->filePath;
        ds << obj->name;
        ds << obj->pos;
        ds << obj->rotate;
        ds << obj->scale;
    }
}

void SceneViewWidget::load(QDataStream& ds)
{
    ds >> envIndex;
    ds >> emitPartical;
    if (emitPartical) {
        setParticleOn();
    }
    else {
        setParticleOff();
    }
    clear();

    int num = 0;
    ds >> num;
    QString str;
    QVector3D vec;
    for (int i = 0; i < num; ++i) {
        ds >> str;
        QFile file(str);
        file.open(QIODevice::ReadWrite);
        QDataStream data(&file);

        SceneObject* obj = addObject(data);
        emit loadObject(obj);

        obj->filePath = str;
        ds >> str;
        obj->setName(str);
        ds >> vec;
        obj->setPos(vec);
        ds >> vec;
        obj->setRotate(vec);
        ds >> vec;
        obj->setScale(vec);
    }

    SceneObject::objectCount = num;
}

void SceneViewWidget::clear()
{
    for (SceneObject* obj : objects) {
        obj->deleteLater();
    }
}
