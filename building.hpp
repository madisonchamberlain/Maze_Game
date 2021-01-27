#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <string>
#include <vector>

class Building
{
public:
    Building(int y, int x);
    void drawIn(std::vector<std::string>& MapAsLines) const;
    bool contains(int y, int x) const;

private:
    static const int width;
    static const int height;
    int mY;
    int mX;
};

#endif // BUILDING_HPP
