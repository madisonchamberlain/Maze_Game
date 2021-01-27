#ifndef ITEM_HPP
#define ITEM_HPP
#include <vector>
#include <string>

class Item
{
public:
    Item(int y, int x);

    int getY() const { return mY; }
    int getX() const { return mX; }
    void drawIn(std::vector<std::string>& MapAsLines) const;

    static char ICON;

private:
    /**
     * Each item is one unit, so only need to store coordinate,
     * not size.
     */
    int mY;
    int mX;
};

#endif // ITEM_HPP
