#include "building.hpp"

const int Building::width = 6;
const int Building::height = 4;

Building::Building(int y, int x)
    : mY(x+1), mX(y)
{
}

bool Building::contains(int y, int x) const
{
	bool return_bool = 0;
	int min_y = mY;
	int max_y = mY + 6;
	int min_x = mX;
	int max_x = mX + 6;
	if ((min_y <= y) && (y <= max_y) && (min_x <= x) && (x <= max_x))
	{
		return_bool = 1;
	}
	return return_bool;
}

void Building::drawIn(std::vector<std::string>& MapAsLines) const
{
	for (int i=mX; i<(mX+6); i++)
	{
		MapAsLines[mY][i] = '.';
	}
	for (int i=mX; i<(mX+6); i++)
	{
		MapAsLines[mY-1][i] = '.';
	}
	MapAsLines[mY+1][mX] = '.';
	MapAsLines[mY+1][mX+1] = '.';
	MapAsLines[mY+1][mX+2] = '&';
	MapAsLines[mY+1][mX+3] = '&';
	MapAsLines[mY+1][mX+4] = '.';
	MapAsLines[mY+1][mX+5] = '.';
	MapAsLines[mY+2][mX] = '.';
	MapAsLines[mY+2][mX+1] = '.';
	MapAsLines[mY+2][mX+2] = '&';
	MapAsLines[mY+2][mX+3] = '&';
	MapAsLines[mY+2][mX+4] = '.';
	MapAsLines[mY+2][mX+5] = '.';
}

