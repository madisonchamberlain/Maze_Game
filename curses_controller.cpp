#include "curses_controller.hpp"

#include <curses.h>
#include <string>

CursesController::~CursesController()
{
}

char CursesController::getInput()
{
	wchar_t input;
	char output;
	keypad(stdscr, true);
	mvaddstr(0, 0, "Enter input: ");
	refresh();
	input = getch();
	if (input == KEY_LEFT || input == 'a')
	{
		output = 'a';
	}
	else if (input == KEY_RIGHT || input == 'd')
	{
		output = 'd';
	}
	else if (input == KEY_UP || input == 'w')
	{
		output = 'w';
	}
	else if (input == 'q')
	{
		output = 'q';
	}
	else
	{
		output = 's';
	}
	return output;
}
