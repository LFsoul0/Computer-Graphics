#ifndef ZENDESIGNER_H
#define ZENDESIGNER_H

#include <QWidget>
#include <QIcon>
#include <QSoundEffect>
#include <QRandomGenerator>

#include "Utils.h"
#include "MainMenu.h"
#include "MainScene.h"

class ZenDesigner : public QWidget
{
    Q_OBJECT

public:
    explicit ZenDesigner(QWidget *parent = nullptr);
    ~ZenDesigner();

private:
    MainMenu* mainMenu;
    MainScene* mainScene;

    QSoundEffect* bgm;
    QVector<QUrl> bgmSource;

    void setRandomMusic();

};
#endif // ZENDESIGNER_H
