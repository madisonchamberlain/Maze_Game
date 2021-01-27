#include "print_view.hpp"

#include <iostream>

PrintView::PrintView(unsigned int height, unsigned int width)//height and width of board
    : View(height, width)
{
}

PrintView::~PrintView()
{
}

void PrintView::draw(int& currentLevel, int& itemsLeft, int& movesRemaining, const std::vector<std::string>& lines)
{   
	std::cout<<"Level: " <<currentLevel+1<<std::endl;
	std::cout<<"Items remaining: "<<itemsLeft<<std::endl;
	std::cout<<"Moves remaining: "<<movesRemaining<<std::endl;
        for (unsigned int i=0; i<lines.size(); i++)
	{
		std::cout<<lines[i]<<std::endl;
	}
}
