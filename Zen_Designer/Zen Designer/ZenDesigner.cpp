#include "ZenDesigner.h"

ZenDesigner::ZenDesigner(QWidget *parent)
    : QWidget(parent)
{
    // set window
    setWindowIcon(QIcon(":/image/logo.png"));
    setWindowTitle(Utils::appName);
    setFixedSize(Utils::winWidth, Utils::winHeight);
    setFocusPolicy(Qt::StrongFocus);

    // bgm
    bgmSource.append(QUrl::fromLocalFile(":/audio/BGM/Forest_Birds.wav"));
    bgmSource.append(QUrl::fromLocalFile(":/audio/BGM/Night_Crickets.wav"));
    bgmSource.append(QUrl::fromLocalFile(":/audio/BGM/Wind_Calm.wav"));
    bgmSource.append(QUrl::fromLocalFile(":/audio/BGM/Wind_Forest.wav"));

    bgm = new QSoundEffect(this);
    bgm->setLoopCount(QSoundEffect::Infinite);
    bgm->setVolume(1);
    setRandomMusic();

    // create scene
    mainMenu = new MainMenu(this);
    connect(mainMenu, &MainMenu::start, this, [=]() {
        mainMenu->hide();
        mainScene->show();
        setRandomMusic();
    });
    connect(mainMenu, &MainMenu::musicVolChanged, bgm, &QSoundEffect::setVolume);
    connect(mainMenu, &MainMenu::soundVolChanged, this, [=](double vol) {
        MenuButton::sound->setVolume(vol);
    });
    connect(mainMenu, &MainMenu::exit, this, &ZenDesigner::close);

    mainScene = new MainScene(this);
    connect(mainScene, &MainScene::back, this, [=]() {
        mainScene->hide();
        mainMenu->show();
        setRandomMusic();
    });
    connect(mainScene, &MainScene::switchScene, this, &ZenDesigner::setRandomMusic);
    mainScene->hide();
}

ZenDesigner::~ZenDesigner()
{

}

void ZenDesigner::setRandomMusic()
{
    int idx = QRandomGenerator::global()->generate() % bgmSource.size();
    bgm->setSource(bgmSource[idx]);
    bgm->play();
}

