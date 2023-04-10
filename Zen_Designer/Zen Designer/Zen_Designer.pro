QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AttachLine.cpp \
    BezierWidget.cpp \
    Camera.cpp \
    ControlPoint.cpp \
    DesignerWidget.cpp \
    DragablePoint.cpp \
    EnvironmentBoard.cpp \
    LightBoard.cpp \
    ListScrollArea.cpp \
    MainMenu.cpp \
    MainScene.cpp \
    MaterialBoard.cpp \
    MenuButton.cpp \
    ObjectDesignerWidget.cpp \
    ObjectPropertyBoard.cpp \
    ObjectViewWidget.cpp \
    Particle.cpp \
    PointPropertyBoard.cpp \
    PropertyBoard.cpp \
    SceneDesignerWidget.cpp \
    SceneObject.cpp \
    SceneViewWidget.cpp \
    Utils.cpp \
    main.cpp \
    ZenDesigner.cpp

HEADERS += \
    AttachLine.h \
    BezierWidget.h \
    Camera.h \
    ControlPoint.h \
    DesignerWidget.h \
    DragablePoint.h \
    EnvironmentBoard.h \
    LightBoard.h \
    ListScrollArea.h \
    MainMenu.h \
    MainScene.h \
    MaterialBoard.h \
    MenuButton.h \
    ObjectDesignerWidget.h \
    ObjectPropertyBoard.h \
    ObjectViewWidget.h \
    Particle.h \
    PointPropertyBoard.h \
    PropertyBoard.h \
    SceneDesignerWidget.h \
    SceneObject.h \
    SceneViewWidget.h \
    Utils.h \
    ZenDesigner.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    audio.qrc \
    shader.qrc \
    skybox.qrc \
    texture.qrc \
    ui.qrc
