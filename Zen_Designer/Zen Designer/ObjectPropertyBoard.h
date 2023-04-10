#ifndef OBJECTPROPERTYBOARD_H
#define OBJECTPROPERTYBOARD_H

#include <QLineEdit>
#include <QDoubleValidator>

#include "PropertyBoard.h"
#include "SceneObject.h"

class ObjectPropertyBoard : public PropertyBoard
{
    Q_OBJECT
public:
    explicit ObjectPropertyBoard(QWidget *parent = nullptr);

    virtual Type type() override;

    void bind(SceneObject* point);
    void unbind(SceneObject* point);

    void setPos(const QVector3D& pos);
    void setRotate(const QVector3D& rotate);
    void setScale(const QVector3D& scale);

private:
    SceneObject* obj;

    QLabel* nameLabel;
    QLineEdit* nameEdit;

    QLabel* posLabel;
    QLabel* posXLabel;
    QLineEdit* posXEdit;
    QLabel* posYLabel;
    QLineEdit* posYEdit;
    QLabel* posZLabel;
    QLineEdit* posZEdit;

    QLabel* rotateLabel;
    QLabel* rotateXLabel;
    QLineEdit* rotateXEdit;
    QLabel* rotateYLabel;
    QLineEdit* rotateYEdit;
    QLabel* rotateZLabel;
    QLineEdit* rotateZEdit;

    QLabel* scaleLabel;
    QLabel* scaleXLabel;
    QLineEdit* scaleXEdit;
    QLabel* scaleYLabel;
    QLineEdit* scaleYEdit;
    QLabel* scaleZLabel;
    QLineEdit* scaleZEdit;

    MenuButton* removeButton;

    void updatePos();
    void updateRotate();
    void updateScale();

signals:

};

#endif // OBJECTPROPERTYBOARD_H
