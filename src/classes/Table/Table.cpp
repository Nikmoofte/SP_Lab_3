#include "Table.hpp"

#include <algorithm>
#include <utility>

Table::Table(int rowNum, int colNum, RECT rect) noexcept 
{
    resize(rowNum, colNum, rect);
    
}

Table::Table() noexcept : Table(1, 1, {0, 0, 0, 0})
{
}

void Table::setText(const std::string &str) noexcept
{
    for(auto& textArr : texts) 
        for(auto& text : textArr)
            text = str;
}

void Table::setText(const Text &other) noexcept
{
    for(auto& textArr : texts) 
        for(auto& text : textArr)
            text = other;
}

void Table::resize(RECT rect) noexcept
{
    tableRect = rect;
}

void Table::resize(int rowNum, int colNum) noexcept
{
    rows = rowNum;
    cols = colNum;

    cellsDimensions.resize(rows);
    for(auto& cellArr : cellsDimensions) cellArr.resize(cols);

    texts.resize(rows);
    for(auto& textArr : texts) textArr.resize(cols);
}

void Table::resize(int rowNum, int colNum, RECT rect) noexcept
{
    resize(rowNum, colNum);
    resize(rect);
}

void Table::calcCellsSize(HDC hdc)
{
    auto cellWidth = (tableRect.right - tableRect.left) / cols;
    int prevRowH = 0;
    for(int i = 0; i < rows; ++i)
    {
        int prevRowW = 0;
        int maxRowH = 0;
        for(int j = 0; j < cols; ++j)
        {
            auto& cell = cellsDimensions[i][j];
            cell = RECT{prevRowW, prevRowH, prevRowW + cellWidth, prevRowH};
            auto textHeight = texts[i][j].computeHeight(hdc, cell);

            if(maxRowH < textHeight)
                maxRowH = textHeight; 

            prevRowW += cellWidth;
        }
        for(auto& cell : cellsDimensions[i])
            cell.bottom = prevRowH + maxRowH;
        prevRowH += maxRowH;
    }
    resize({tableRect.left, tableRect.top, tableRect.right, prevRowH});
}



std::string& Table::getCellText(int i, int j)
{
    return texts[i][j].getStr();
}

RECT Table::getCellRect(int i, int j)
{
    return cellsDimensions[i][j];
}

std::pair<int, int> Table::getCellIndies(int x, int y)
{
    int i{}, j{};
    j = x / (cellsDimensions[0][0].right - cellsDimensions[0][0].left);
    for(int ind = 0; ind < cellsDimensions.size(); ++ind)
        if(y < cellsDimensions[ind][0].bottom)
        {
            i = ind;
            break;
        }

    return std::pair<int, int>(i, j);
}

void Table::draw(HDC hdc, int offset) noexcept
{
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
        {
            RECT rect = cellsDimensions[i][j];
            rect.bottom += offset;
            rect.top += offset;
            Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
            texts[i][j].draw(hdc, rect);
        }
}
