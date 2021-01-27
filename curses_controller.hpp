#ifndef CURSES_CONTROLLER_HPP
#define CURSES_CONTROLLER_HPP

#include "controller.hpp"

class CursesController : public Controller
{
public:
    CursesController() = default;
    virtual ~CursesController() override;
    CursesController(const CursesController&) = delete;
    CursesController& operator=(const CursesController&) = delete;
    virtual char getInput() override;
};

#endif // CURSES_CONTROLLER_HPP
