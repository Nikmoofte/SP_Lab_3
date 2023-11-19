#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include <Windows.h>

class Text
{
public:
    Text(const std::string&);
    Text();

    const std::string& operator=(const std::string&); 
    const std::string& operator=(const Text&); 
    const std::string& getStr() const { return text; }; 
    std::string& getStr() { return text; }; 

    int draw(HDC hdc, RECT& Rect) noexcept;
    int computeHeight(HDC hdc, RECT& Rect) noexcept;
private:
    std::string text;
};

#endif