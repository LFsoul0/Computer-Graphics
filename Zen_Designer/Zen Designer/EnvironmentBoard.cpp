#include "EnvironmentBoard.h"

EnvironmentBoard::EnvironmentBoard(QWidget *parent) : PropertyBoard(parent)
{
    title->setText("Environment");

    timeWidget = new QWidget(this);
    timeWidget->move(0, 100);
    timeLabel = new QLabel("Time", timeWidget);
    timeLabel->setFixedSize(200, 80);
    timeLabel->move(20, 0);
    timeLabel->setFont(Utils::menuFont());
    timeLabel->setStyleSheet("color: white");
    timeButtons.append(new QRadioButton("Day", timeWidget));
    timeButtons[0]->setFixedSize(130, 40);
    timeButtons[0]->move(20, 70);
    timeButtons[0]->setFont(Utils::menuFont(32));
    timeButtons[0]->setStyleSheet("color: white");
    connect(timeButtons[0], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) sceneWidget->envIndex = 0;
    });
    timeButtons.append(new QRadioButton("Night", timeWidget));
    timeButtons[1]->setFixedSize(130, 40);
    timeButtons[1]->move(170, 70);
    timeButtons[1]->setFont(Utils::menuFont(32));
    timeButtons[1]->setStyleSheet("color: white");
    connect(timeButtons[1], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) sceneWidget->envIndex = 1;
    });
    timeButtons.append(new QRadioButton("Sunrise", timeWidget));
    timeButtons[2]->setFixedSize(130, 40);
    timeButtons[2]->move(20, 110);
    timeButtons[2]->setFont(Utils::menuFont(32));
    timeButtons[2]->setStyleSheet("color: white");
    connect(timeButtons[2], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) sceneWidget->envIndex = 2;
    });
    timeButtons.append(new QRadioButton("Sunset", timeWidget));
    timeButtons[3]->setFixedSize(130, 40);
    timeButtons[3]->move(170, 110);
    timeButtons[3]->setFont(Utils::menuFont(32));
    timeButtons[3]->setStyleSheet("color: white");
    connect(timeButtons[3], &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) sceneWidget->envIndex = 3;
    });

    particleWidget = new QWidget(this);
    particleWidget->move(0, 310);
    particleLabel = new QLabel("Particle", particleWidget);
    particleLabel->setFixedSize(200, 80);
    particleLabel->move(20, 0);
    particleLabel->setFont(Utils::menuFont());
    particleLabel->setStyleSheet("color: white");
    particleOnButtons = new QRadioButton("On", particleWidget);
    particleOnButtons->setFixedSize(130, 40);
    particleOnButtons->move(20, 70);
    particleOnButtons->setFont(Utils::menuFont(32));
    particleOnButtons->setStyleSheet("color: white");
    connect(particleOnButtons, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) sceneWidget->setParticleOn();
    });
    particleOffButtons = new QRadioButton("Off", particleWidget);
    particleOffButtons->setFixedSize(130, 40);
    particleOffButtons->move(170, 70);
    particleOffButtons->setFont(Utils::menuFont(32));
    particleOffButtons->setStyleSheet("color: white");
    connect(particleOffButtons, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) sceneWidget->setParticleOff();
    });

    shadowWidget = new QWidget(this);
    shadowWidget->move(0, 480);
    shadowLabel = new QLabel("Shadow", shadowWidget);
    shadowLabel->setFixedSize(200, 80);
    shadowLabel->move(20, 0);
    shadowLabel->setFont(Utils::menuFont());
    shadowLabel->setStyleSheet("color: white");
    shadowOnButtons = new QRadioButton("On", shadowWidget);
    shadowOnButtons->setFixedSize(130, 40);
    shadowOnButtons->move(20, 70);
    shadowOnButtons->setFont(Utils::menuFont(32));
    shadowOnButtons->setStyleSheet("color: white");
    connect(shadowOnButtons, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) sceneWidget->addShadow = true;
    });
    shadowOffButtons = new QRadioButton("Off", shadowWidget);
    shadowOffButtons->setFixedSize(130, 40);
    shadowOffButtons->move(170, 70);
    shadowOffButtons->setFont(Utils::menuFont(32));
    shadowOffButtons->setStyleSheet("color: white");
    connect(shadowOffButtons, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) sceneWidget->addShadow = false;
    });
}

PropertyBoard::Type EnvironmentBoard::type()
{
    return Environment;
}

void EnvironmentBoard::setSceneWidget(SceneViewWidget* widget)
{
    sceneWidget = widget;
    timeButtons[widget->envIndex]->setChecked(true);
    if (widget->emitPartical) {
        particleOnButtons->setChecked(true);
    }
    else {
        particleOffButtons->setChecked(true);
    }

    if (widget->addShadow) {
        shadowOnButtons->setChecked(true);
    }
    else {
        shadowOffButtons->setChecked(true);
    }
}
