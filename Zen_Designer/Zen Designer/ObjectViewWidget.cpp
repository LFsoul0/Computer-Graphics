#include "ObjectViewWidget.h"

ObjectViewWidget::ObjectViewWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      objectVBO(QOpenGLBuffer::VertexBuffer),
      objectEBO(QOpenGLBuffer::IndexBuffer),
      quadVBO(QOpenGLBuffer::VertexBuffer)
{
    QSurfaceFormat format;
    format.setVersion(3,3);
    format.setSamples(4);   // MSAA
    setFormat(format);

    initial = true;
    isDragging = false;
    textureRepeat = 2;

    objectShader = new QOpenGLShaderProgram(this);
    blendShader = new QOpenGLShaderProgram(this);

    material.ambient = QVector3D(0.25, 0.25, 0.25);
    material.diffuse = QVector3D(0.829, 0.829, 0.829);
    material.specular = QVector3D(0.297, 0.297, 0.297);
    material.shininess = 128 * 0.88f;
    material.texIndex = 0;

    light.direction = QVector3D(1, -1, -1);
    light.ambient = QVector3D(0.2, 0.2, 0.2);
    light.diffuse = QVector3D(1, 1, 1);
    light.specular = QVector3D(1, 1, 1);

    viewPos = QVector3D(0.0f, 0.0f, 3.0f);
    modelMat.translate(QVector3D(0, -0.5, 0));
    viewMat.lookAt(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));

    timer = new QTimer(this);
    timer->setInterval(20);
    connect(timer, &QTimer::timeout, this, &ObjectViewWidget::update);

    background = new QOpenGLTexture(QOpenGLTexture::Target2D);
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
}

ObjectViewWidget::~ObjectViewWidget()
{
    //delete objectFBO;
    delete background;
    for (QOpenGLTexture* tex : diffTexs) {
        delete tex;
    }
    for (QOpenGLTexture* tex : normTexs) {
        delete tex;
    }
    for (QOpenGLTexture* tex : specTexs) {
        delete tex;
    }
}

void ObjectViewWidget::setRotatorLine(const QVector<QVector3D>& line)
{
    vertices.clear();
    indices.clear();
    normals.clear();
    tangents.clear();
    texcoords.clear();

    const int lineSize = line.size();
    const float angleStep = 360.0 / (rotateSampleNum - 1);
    const float texXStep = (float)textureRepeat / (rotateSampleNum - 1);
    const float texYStep = -1.0 / lineSize;
    QVector2D tex = QVector2D(0, 1);
    for (int i = 0; i < lineSize; ++i) {
        QMatrix4x4 rotMat;
        tex.setX(0);
        tex.setY(tex.y() + texYStep);
        for (int j = 0; j < rotateSampleNum; ++j) {
            QVector4D p = rotMat * QVector4D(line[i], 1);
            vertices.append(p.x() * scale);
            vertices.append(p.y() * scale);
            vertices.append(p.z() * scale);

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

    this->line = line;

    if (!initial) {
        makeCurrent();
        makeObject();
        doneCurrent();
    }
}

void ObjectViewWidget::setTextureRepeat(int times)
{
    textureRepeat = times;
    setRotatorLine(line);
}

void ObjectViewWidget::show()
{
    QOpenGLWidget::show();
    timer->start();
}

void ObjectViewWidget::hide()
{
    timer->stop();
    QOpenGLWidget::hide();
}

void ObjectViewWidget::update()
{
    QOpenGLWidget::update();
}

void ObjectViewWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // shaders
    makeShader(objectShader, ":/shader/object.vert", ":/shader/object.frag");
    makeShader(blendShader, ":/shader/blend.vert", ":/shader/blend.frag");

    // object VAO
    if (!objectVAO.create()) qDebug() << "Fail to create VAO" << Qt::endl;
    if (!objectVBO.create()) qDebug() << "Fail to create VBO" << Qt::endl;
    if (!objectEBO.create()) qDebug() << "Fail to create EBO" << Qt::endl;

    makeObject();
    //objectFBO = new QOpenGLFramebufferObject(size(), QOpenGLFramebufferObject::Depth, GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F);

    // quad VAO
    if (!quadVAO.create()) qDebug() << "Fail to create VAO" << Qt::endl;
    if (!quadVBO.create()) qDebug() << "Fail to create VBO" << Qt::endl;

    QVector<float> quadVertices = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    quadVAO.bind();
    quadVBO.bind();
    quadVBO.allocate(quadVertices.data(), quadVertices.size() * sizeof(float));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    quadVAO.release();

    // textures
    background->create();
    QImage backgroundImage = QImage(":/image/background.png").mirrored();
    backgroundImage = backgroundImage.scaled(parentWidget()->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    backgroundImage = backgroundImage.copy((backgroundImage.width() - parentWidget()->width()) / 2 + pos().x(),
                                           (backgroundImage.height() - parentWidget()->height()) / 2 + pos().y(),
                                           width(), height());
    background->setData(backgroundImage);
    background->setMinificationFilter(QOpenGLTexture::Linear);
    background->setMagnificationFilter(QOpenGLTexture::Linear);
    background->setWrapMode(QOpenGLTexture::ClampToEdge);

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

    initial = false;
}

void ObjectViewWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    projectionMat.setToIdentity();
    projectionMat.perspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
}

void ObjectViewWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // render background
    blendShader->bind();
    blendShader->setUniformValue("mask", QVector4D(0.0, 0.0, 0.0, 0.5));
    background->bind(0);
    blendShader->setUniformValue("scene", 0);

    quadVAO.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    quadVAO.release();

    // render object
    //objectFBO->bind();
    glEnable(GL_DEPTH_TEST);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    objectShader->bind();
    objectShader->setUniformValue("model", modelMat);
    objectShader->setUniformValue("view", viewMat);
    objectShader->setUniformValue("projection", projectionMat);
    objectShader->setUniformValue("viewPos", viewPos);

    if (material.texIndex < 0) {
        objectShader->setUniformValue("material.haveDiffTex", false);
        objectShader->setUniformValue("material.diffuse", material.diffuse);
    }
    else {
        objectShader->setUniformValue("material.haveDiffTex", true);
        diffTexs[material.texIndex]->bind(0);
        objectShader->setUniformValue("material.diffTex", 0);
    }
    if (material.texIndex >= 0 && material.texIndex < 4) {
        objectShader->setUniformValue("material.haveNormTex", true);
        normTexs[material.texIndex]->bind(1);
        objectShader->setUniformValue("material.normTex", 1);
    }
    else {
        objectShader->setUniformValue("material.haveNormTex", false);
    }
    if (material.texIndex >= 2 && material.texIndex < 4) {
        objectShader->setUniformValue("material.haveSpecTex", true);
        specTexs[material.texIndex - 2]->bind(2);
        objectShader->setUniformValue("material.specTex", 2);
    }
    else {
        objectShader->setUniformValue("material.haveSpecTex", false);
        objectShader->setUniformValue("material.specular", material.specular);
    }

    objectShader->setUniformValue("material.ambient", material.ambient);
    objectShader->setUniformValue("material.shininess", material.shininess);
    objectShader->setUniformValue("light.direction", light.direction);
    objectShader->setUniformValue("light.ambient", light.ambient);
    objectShader->setUniformValue("light.diffuse", light.diffuse);
    objectShader->setUniformValue("light.specular", light.specular);
    objectShader->setUniformValue("haveShadowMap", false);

    objectVAO.bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    objectVAO.release();

    glDisable(GL_DEPTH_TEST);
    //objectFBO->release();

    //QOpenGLFramebufferObject::blitFramebuffer(nullptr, rect(), objectFBO, QRect(0, 0, objectFBO->width(), objectFBO->height()));
}

void ObjectViewWidget::makeObject()
{
    objectVAO.bind();
    objectVBO.bind();
    objectVBO.allocate((vertices.size() + normals.size() + tangents.size() + texcoords.size()) * sizeof(float));
    objectVBO.write(0, vertices.data(), vertices.size() * sizeof(float));
    objectVBO.write(vertices.size() * sizeof(float), normals.data(), normals.size() * sizeof(float));
    objectVBO.write((vertices.size() + normals.size()) * sizeof(float), tangents.data(), tangents.size() * sizeof(float));
    objectVBO.write((vertices.size() + normals.size() + tangents.size()) * sizeof(float),
                    texcoords.data(), texcoords.size() * sizeof(float));
    objectEBO.bind();
    objectEBO.allocate(indices.data(), indices.size() * sizeof(unsigned int));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertices.size() * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)((vertices.size() + normals.size()) * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void*)((vertices.size() + normals.size() + tangents.size()) * sizeof(float)));

    objectVAO.release();
}

void ObjectViewWidget::makeShader(QOpenGLShaderProgram* shader, const QString& vertexPath, const QString& fragmentPath)
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

void ObjectViewWidget::mousePressEvent(QMouseEvent* ev)
{
    isDragging = true;
    mousePos = ev->localPos();

    QOpenGLWidget::mousePressEvent(ev);
}

void ObjectViewWidget::mouseMoveEvent(QMouseEvent* ev)
{
    if (isDragging) {
        QPointF prevPos = mousePos;
        mousePos = ev->localPos();
        QVector3D moveVec = QVector3D(mousePos.x() - prevPos.x(), prevPos.y() - mousePos.y(), 0);
        QVector3D axisVec = QVector3D::crossProduct(QVector3D(0, 0, 1), moveVec).normalized();
        QMatrix4x4 rotMat;
        rotMat.rotate(moveVec.length() * mouseSensitivity, axisVec);
        modelMat = rotMat * modelMat;
    }

    QOpenGLWidget::mouseMoveEvent(ev);
}

void ObjectViewWidget::mouseReleaseEvent(QMouseEvent* ev)
{
    isDragging = false;

    QOpenGLWidget::mouseReleaseEvent(ev);
}

void ObjectViewWidget::wheelEvent(QWheelEvent* ev)
{
    float dis = ev->angleDelta().y() * wheelSensitivity;
    float factor = qExp(dis);

    modelMat.scale(factor, factor, factor);

    QOpenGLWidget::wheelEvent(ev);
}

void ObjectViewWidget::save(QDataStream& ds)
{
    ds << material.texIndex;
    ds << material.ambient;
    ds << material.diffuse;
    ds << material.specular;
    ds << material.shininess;
    ds << textureRepeat;

    ds << light.direction;
    ds << light.ambient;
    ds << light.diffuse;
    ds << light.specular;
}

void ObjectViewWidget::load(QDataStream& ds)
{
    ds >> material.texIndex;
    ds >> material.ambient;
    ds >> material.diffuse;
    ds >> material.specular;
    ds >> material.shininess;
    ds >> textureRepeat;

    ds >> light.direction;
    ds >> light.ambient;
    ds >> light.diffuse;
    ds >> light.specular;
}
