#include "MainMenu.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    // set window size
    setFixedSize(parent->size());

    // draw title
    title = new QLabel(Utils::appName, this);
    title->setFixedSize(600, 100);
    title->move((width() - title->width()) / 2, 70);
    title->setFont(Utils::titleFont());
    title->setAlignment(Qt::AlignCenter);
    //title->setStyleSheet("color: white;");

    // create root menu
    rootMenu = new QWidget(this);
    startButton = new MenuButton("Start", rootMenu);
    startButton->move((width() - startButton->width()) / 2, 250);
    connect(startButton, &MenuButton::clicked, this, &MainMenu::start);
    settingsButton = new MenuButton("Settings", rootMenu);
    settingsButton->move((width() - settingsButton->width()) / 2, 400);
    connect(settingsButton, &MenuButton::clicked, this, [=]() {
        rootMenu->hide();
        settingsMenu->show();
    });
    exitButton = new MenuButton("Exit", rootMenu);
    exitButton->move((width() - settingsButton->width()) / 2, 550);
    connect(exitButton, &MenuButton::clicked, this, &MainMenu::exit);

    // create settings menu
    settingsMenu = new QWidget(this);
    musicLabel = new QLabel("Music", settingsMenu);
    musicLabel->setFixedSize(250, 100);
    musicLabel->move((width() * 0.7 - musicLabel->width()) / 2, 250);
    musicLabel->setFont(Utils::menuFont());
    musicLabel->setAlignment(Qt::AlignCenter);
    musicVol = new QSlider(Qt::Horizontal, settingsMenu);
    musicVol->setFixedSize(400, 50);
    musicVol->move((width() * 1.1 - musicVol->width()) / 2, 275);
    musicVol->setValue(100);
    connect(musicVol, &QSlider::valueChanged, this, [=](int value) { emit musicVolChanged((double)value / 100); });
    soundLabel = new QLabel("Sound", settingsMenu);
    soundLabel->setFixedSize(250, 100);
    soundLabel->move((width() * 0.7 - musicLabel->width()) / 2, 400);
    soundLabel->setFont(Utils::menuFont());
    soundLabel->setAlignment(Qt::AlignCenter);
    soundVol = new QSlider(Qt::Horizontal, settingsMenu);
    soundVol->setFixedSize(400, 50);
    soundVol->move((width() * 1.1 - soundVol->width()) / 2, 425);
    soundVol->setValue(100);
    connect(soundVol, &QSlider::valueChanged, this, [=](int value) { emit soundVolChanged((double)value / 100); });
    settingsBack = new MenuButton("Back", settingsMenu);
    settingsBack->move((width() - settingsBack->width()) / 2, 550);
    connect(settingsBack, &MenuButton::clicked, this, [=]() {
        settingsMenu->hide();
        rootMenu->show();
    });
    settingsMenu->hide();
}

void MainMenu::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    QPixmap pix;

    // draw background
    pix.load(":/image/background.png");
    pix = pix.scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    painter.drawPixmap((width() - pix.width()) / 2, (height() - pix.height()) / 2, pix);

    QWidget::paintEvent(ev);
}

void MainMenu::show()
{
    QWidget::show();
}
