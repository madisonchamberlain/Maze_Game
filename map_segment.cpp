#include "map_segment.hpp"

char MapSegment::VERTICAL_BORDER_CHAR = '|';
char MapSegment::HORIZONTAL_BORDER_CHAR = '=';
char MapSegment::PORTAL_CHAR = '@';

MapSegment::MapSegment(int height, int width)
    : mHeight(height), mWidth(width)
    , mPortalY{(mHeight - 1) / 2}
    , mPortalX{(mWidth - 1) / 2}
{
	mHeight = height;
	mWidth = width;
	mPortalY = (height - 1)/2;
	mPortalX = (width - 1)/2;
}


std::vector<std::string> MapSegment::getAsLines() 
{
	// sets up the blank map
	std::vector<std::string> MapAsLines; 
	std::string str1;
	std::string horizontal = "=";
	std::string vertical = "|";
	std::string space = " ";
	for (int i=0; i<mWidth; i++)
	{
		str1.append(horizontal);
	}
	MapAsLines.push_back(str1);
	for (int i=1; i<(mHeight - 1); i++)
	{
		std::string strMid;
		strMid.append(vertical);
		for (int j=1; j<(mWidth - 1); j++)
		{
			strMid.append(space);
		}
		strMid.append(vertical);
		MapAsLines.push_back(strMid);
	}
	std::string strLast;
	for (int i=0; i<mWidth; i++)
	{
		strLast.append(horizontal);
	}
	MapAsLines.push_back(strLast);
	return MapAsLines;
}
