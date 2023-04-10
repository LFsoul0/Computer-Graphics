#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPainter>

#include "Utils.h"
#include "MenuButton.h"

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *ev) override;
    void show();

private:
    QLabel* title;

    // root menu
    QWidget* rootMenu;
    MenuButton* startButton;
    MenuButton* settingsButton;
    MenuButton* exitButton;

    // settings menu
    QWidget* settingsMenu;
    QLabel* musicLabel;
    QSlider* musicVol;
    QLabel* soundLabel;
    QSlider* soundVol;
    MenuButton* settingsBack;

signals:
    void start();
    void exit();
    void musicVolChanged(double vol);
    void soundVolChanged(double vol);

};

#endif // MAINMENU_H
