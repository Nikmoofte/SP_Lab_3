#include "Text.hpp"

Text::Text(const std::string &text) : text(text)
{
}

Text::Text() : text{"new text"}
{
}

const std::string &Text::operator=(const std::string &_text)
{
    text = _text;
    
    return text;
}

const std::string &Text::operator=(const Text &other)
{
    text = other.text;
    return text;
}

int Text::draw(HDC hdc, RECT& rect) noexcept
{
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    auto fontHeight = tm.tmHeight;
    float fontWidth = tm.tmAveCharWidth;
    fontWidth += 1.05;
    auto symbInLine = (rect.right - rect.left) / fontWidth;
    auto lines = text.size() / symbInLine;
    for(int i = 0; i <= lines && fontHeight * (i + 1) < (rect.bottom - rect.top); ++i)
    {
        RECT newRect = rect;
        newRect.bottom += fontHeight * i;
        newRect.top += fontHeight * i;
        DrawText(hdc, text.substr(symbInLine * i, symbInLine).c_str(), -1, &newRect, DT_CENTER);
    }

    return fontHeight * lines;
}

int Text::computeHeight(HDC hdc, RECT &rect) noexcept
{
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    auto fontHeight = tm.tmHeight;
    float fontWidth = tm.tmAveCharWidth;
    fontWidth += 1.05;
    auto symbInLine = (rect.right - rect.left) / fontWidth;
    float lines = max(text.size() / symbInLine, 1.2f);
    
    rect.bottom = (float)fontHeight * lines; 
    return rect.bottom;
}
