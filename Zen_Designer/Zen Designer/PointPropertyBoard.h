#ifndef POINTPROPERTYBOARD_H
#define POINTPROPERTYBOARD_H

#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QIntValidator>

#include "PropertyBoard.h"
#include "ControlPoint.h"

class PointPropertyBoard : public PropertyBoard
{
    Q_OBJECT
public:
    explicit PointPropertyBoard(QWidget *parent = nullptr);

    virtual Type type() override;

    void bind(ControlPoint* point);
    void unbind(ControlPoint* point);

    void setPos(const QPoint& p);
    void setOffset(const QPoint& p);

private:
    ControlPoint* point;

    QLabel* nameLabel;
    QLineEdit* nameEdit;

    QLabel* typeLabel;
    QRadioButton* straightButton;
    QRadioButton* curveButton;

    QLabel* posLabel;
    QLabel* posXLabel;
    QLineEdit* posXEdit;
    QLabel* posYLabel;
    QLineEdit* posYEdit;

    QLabel* offsetLabel;
    QLabel* offsetXLabel;
    QLineEdit* offsetXEdit;
    QLabel* offsetYLabel;
    QLineEdit* offsetYEdit;

    MenuButton* removeButton;

    void updatePos();
    void updateOffset();

signals:

};

#endif // POINTPROPERTYBOARD_H
