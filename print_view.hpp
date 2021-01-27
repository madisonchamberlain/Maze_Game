#ifndef PRINT_VIEW_HPP
#define PRINT_VIEW_HPP

#include "view.hpp"

/**
 * You shouldn't have to change this header file.
 * Implement the functions defined in the source file.
 */

class PrintView : public View
{
public:  // public methods
    PrintView(unsigned int height, unsigned int width);
    virtual ~PrintView();
    PrintView(const PrintView&) = delete;
    PrintView& operator=(const PrintView&) = delete;
    virtual void draw(int& currentLevel, int& ItemsLeft, int& MovesRemaining, const std::vector<std::string>& lines) override;
};

#endif // PRINT_VIEW_HPP
