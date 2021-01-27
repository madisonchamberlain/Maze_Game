#include "item.hpp"

char Item::ICON = '$';

Item::Item(int y, int x)
    : mY(y), mX(x)
{
}

void Item::drawIn(std::vector<std::string>& MapAsLines) const
{
	MapAsLines[mX][mY] = '$';//make sure x and y are not in wrong order
}
