#include "Utils.h"

QFont Utils::titleFont(int pixelSize, bool bold, bool italic)
{
    QFont font("Segoe Script");
    font.setPixelSize(pixelSize);
    font.setBold(bold);
    font.setItalic(italic);
    return font;
}

QFont Utils::menuFont(int pixelSize, bool bold, bool italic)
{
    QFont font("Monotype Corsiva");
    font.setPixelSize(pixelSize);
    font.setBold(bold);
    font.setItalic(italic);
    //font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    return font;
}

QFont Utils::textFont(int pixelSize, bool bold, bool italic)
{
    QFont font("Times New Roman");
    font.setPixelSize(pixelSize);
    font.setBold(bold);
    font.setItalic(italic);
    return font;
}
