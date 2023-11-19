#ifndef TABLE_HPP
#define TABLE_HPP

#include "Text/Text.hpp"

#include <vector>

class Table
{
public:
    Table(int rowNum, int colNum, RECT clientRect) noexcept;
    Table() noexcept;

    void setText(const std::string& str) noexcept;
    void setText(const Text& text) noexcept;

    inline void resize(RECT clientRect) noexcept;
    inline void resize(int rows, int cols) noexcept;
    void resize(int rows, int cols, RECT clientRect) noexcept;
    void calcCellsSize(HDC hdc);
    RECT getRect() { return tableRect; };
    RECT getCellRect(int i, int j);
    std::string& getCellText(int i, int j);

    std::pair<int, int> getCellIndies(int x, int y);

    void draw(HDC hdc, int offset) noexcept;
private:
    //number of cells 
    int rows;
    int cols;
    RECT tableRect;


    std::vector<std::vector<RECT>> cellsDimensions;
    std::vector<std::vector<Text>> texts;
    // Text text;
};

#endif