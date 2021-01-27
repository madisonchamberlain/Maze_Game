#include "curses_view.hpp"

/**
 * Side note: This class should probably be what is known as a singleton
 * class, i.e. a class that uses some setup (e.g. a static variable) to
 * prevent more than one instance of it from being created. I did not do this.
 */

CursesView::CursesView(unsigned int height, unsigned int width)
    : View(height, width)
{

}

CursesView::~CursesView()
{

}

void CursesView::draw(int& currentLevel, int& ItemsLeft, int& MovesRemaining, const std::vector<std::string>& lines)
{
	initscr();
	cbreak();
	noecho();
	clear();
	std::string levels = "Level: ";
	levels.append(std::to_string(currentLevel));
	std::string items = "Items remaining: ";
	items.append(std::to_string(ItemsLeft));
	std::string moves = "Moves remaining: ";
	moves.append(std::to_string(MovesRemaining));
	mvaddstr(1, 0, (levels).c_str());
	mvaddstr(2, 0, (items).c_str());
	mvaddstr(3, 0, (moves).c_str());
	for (unsigned int i=0; i<lines.size(); i++)
	{
		mvaddstr(4+i, 0, (lines[i]).c_str());
	}
	refresh();
}
