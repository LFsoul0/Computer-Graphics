#include "MenuButton.h"

QSoundEffect* MenuButton::sound = nullptr;

MenuButton::MenuButton(QWidget *parent) : MenuButton("Menu Button", parent) { }

MenuButton::MenuButton(const QString& text, QWidget *parent) : QLabel(text, parent)
{
    if (!sound) sound = setSound();

    setFixedSize(300, 80);
    setFont(Utils::menuFont());
    setAlignment(Qt::AlignCenter);

    checkable = false;
    autoRaise = false;
    isEntered = false;
    isPressed = false;
    isChecked = false;

    normalFontSize = font().pixelSize();
    triggeredFontSize = normalFontSize * 1.5;

    normalColor = Qt::black;
    pressedColor = Qt::white;
    checkedColor = Qt::yellow;
    normalBackgroundColor = Qt::transparent;
    raisedBackgroundColor = Qt::darkCyan;

    fontColor = normalColor;
    backgroundColor = normalBackgroundColor;

    enterAnim = new QPropertyAnimation(this, "fontSize", this);
    enterAnim->setDuration(200);
    enterAnim->setStartValue(normalFontSize);
    enterAnim->setEndValue(triggeredFontSize);

    leaveAnim = new QPropertyAnimation(this, "fontSize", this);
    leaveAnim->setDuration(200);
    leaveAnim->setStartValue(triggeredFontSize);
    leaveAnim->setEndValue(normalFontSize);

    updateStyleSheet();
}

MenuButton::~MenuButton() { }

int MenuButton::getFontSize()
{
    return font().pixelSize();
}

void MenuButton::setFontSize(int size)
{
    static QFont f = font();
    f.setPixelSize(size);
    setFont(f);
}

void MenuButton::setNormalFontSize(int pixelSize, bool current)
{
    normalFontSize = pixelSize;
    enterAnim->setStartValue(pixelSize);
    leaveAnim->setEndValue(pixelSize);
    if (current) {
        setFontSize(pixelSize);
    }
}

void MenuButton::setTriggeredFontSize(int pixelSize, bool current)
{
    triggeredFontSize = pixelSize;
    enterAnim->setEndValue(pixelSize);
    leaveAnim->setStartValue(pixelSize);
    if (current) {
        setFontSize(pixelSize);
    }
}

void MenuButton::setTriggeredFontSizeByRatio(double ratio, bool current)
{
    triggeredFontSize = normalFontSize * ratio;
    enterAnim->setEndValue(triggeredFontSize);
    leaveAnim->setStartValue(triggeredFontSize);
    if (current) {
        setFontSize(triggeredFontSize);
    }
}

void MenuButton::setNormalColor(QColor color, bool current)
{
    normalColor = color;
    if (current) {
        fontColor = color;
        updateStyleSheet();
    }
}

void MenuButton::setPressedColor(QColor color, bool current)
{
    pressedColor = color;
    if (current) {
        fontColor = color;
        updateStyleSheet();
    }
}

void MenuButton::setCheckedColor(QColor color, bool current)
{
    checkedColor = color;
    if (current) {
        fontColor = color;
        updateStyleSheet();
    }
}

void MenuButton::setNormalBackgroundColor(QColor color, bool current)
{
    normalBackgroundColor = color;
    if (current) {
        backgroundColor = color;
        updateStyleSheet();
    }
}

void MenuButton::setRaisedBackgroundColor(QColor color, bool current)
{
    raisedBackgroundColor = color;
    if (current) {
        backgroundColor = color;
        updateStyleSheet();
    }
}

void MenuButton::setCheckable(bool checkable)
{
    this->checkable = checkable;
}

void MenuButton::setCheck(bool check)
{
    if (isChecked != check) {
        this->check();
    }
}

void MenuButton::check()
{
    isChecked = !isChecked;

    if (isChecked) {
        fontColor = checkedColor;
        if (autoRaise) {
            backgroundColor = raisedBackgroundColor;
        }
    }
    else {
        fontColor = normalColor;
        if (!isEntered) {
            backgroundColor = normalBackgroundColor;
        }
    }
    updateStyleSheet();

    emit checked(isChecked);
}

void MenuButton::setAutoRaise(bool autoRaise)
{
    this->autoRaise = autoRaise;
}

void MenuButton::updateStyleSheet()
{
    setStyleSheet("color: " + fontColor.name(QColor::HexArgb) + "; background: " + backgroundColor.name(QColor::HexArgb) + ";");
}

void MenuButton::enterEvent(QEvent* ev)
{
    isEntered = true;
    enterAnim->start();

    if (autoRaise) {
        backgroundColor = raisedBackgroundColor;
    }
    updateStyleSheet();

    QLabel::enterEvent(ev);
}

void MenuButton::leaveEvent(QEvent* ev)
{
    isEntered = false;
    leaveAnim->start();

    if (!isChecked) {
        backgroundColor = normalBackgroundColor;
    }
    updateStyleSheet();

    QLabel::leaveEvent(ev);
}

void MenuButton::mousePressEvent(QMouseEvent *ev)
{
    QRect rect = this->rect();
    if (rect.contains(ev->pos())) {
        isPressed = true;
        fontColor = pressedColor;
        updateStyleSheet();
        emit pressed();
    }

    sound->play();

    QLabel::mousePressEvent(ev);
}

void MenuButton::mouseReleaseEvent(QMouseEvent *ev)
{
    if (isPressed) {
        isPressed = false;
        fontColor = normalColor;
        emit released();

        QRect rect = this->rect();
        if (rect.contains(ev->pos())) {
            emit clicked();
            if (checkable) {
                check();
            }
        }
        updateStyleSheet();
    }

    QLabel::mouseReleaseEvent(ev);
}

QSoundEffect* MenuButton::setSound()
{
    QSoundEffect* s = new QSoundEffect();
    s->setSource(QUrl::fromLocalFile(":/audio/SFX/button.wav"));
    s->setLoopCount(1);
    s->setVolume(1);

    return s;
}
