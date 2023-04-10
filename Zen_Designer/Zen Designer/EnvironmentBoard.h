#ifndef ENVIRONMENTBOARD_H
#define ENVIRONMENTBOARD_H

#include <QRadioButton>

#include "PropertyBoard.h"
#include "SceneViewWidget.h"

class EnvironmentBoard : public PropertyBoard
{
    Q_OBJECT
public:
    explicit EnvironmentBoard(QWidget *parent = nullptr);

    virtual Type type() override;

    void setSceneWidget(SceneViewWidget* widget);

private:
    SceneViewWidget* sceneWidget;

    QWidget* timeWidget;
    QLabel* timeLabel;
    QVector<QRadioButton *> timeButtons;

    QWidget* particleWidget;
    QLabel* particleLabel;
    QRadioButton* particleOnButtons;
    QRadioButton* particleOffButtons;

    QWidget* shadowWidget;
    QLabel* shadowLabel;
    QRadioButton* shadowOnButtons;
    QRadioButton* shadowOffButtons;

signals:

};

#endif // ENVIRONMENTBOARD_H
