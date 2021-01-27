#ifndef CURSES_VIEW_HPP
#define CURSES_VIEW_HPP

#include <curses.h>
#include "view.hpp"

/**
 * You shouldn't have to change this header file.
 * Implement the functions defined in the source file.
 */

class CursesView : public View
{
public:  // public methods
    CursesView(unsigned int height, unsigned int width);
    virtual ~CursesView();
    CursesView(const CursesView&) = delete;
    CursesView& operator=(const CursesView&) = delete;
    virtual void draw(int& currentLevel, int& ItemsLeft, int& MovesRemaining, const std::vector<std::string>& lines) override;
};

#endif // CURSES_VIEW_HPP
