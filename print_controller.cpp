#include "print_controller.hpp"

#include <iostream>

PrintController::~PrintController()
{
}

char PrintController::getInput()
{
    char input;
    std::cout<<"Enter input: ";
    std::cin>>input;
    return input;
}
