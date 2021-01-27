#ifndef GAME_HPP
#define GAME_HPP

#include "controller.hpp"
#include "view.hpp"
#include "interface_type.hpp"
#include "map_segment.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <curses.h>

class Game
{
public:  
    Game(const std::string& filename, InterfaceType interfaceType);
    ~Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    void run();

private:  
    void makeBoard(); 
    void draw();
    void doGameLoop();
    void update();
    void loadLevel();
    void readInLevel(const std::string& filename);

private:
    static const int MIN_VIEW_HEIGHT;
    static const int MIN_VIEW_WIDTH;
    static const char HERO_ICON_LEFT;
    static const char HERO_ICON_RIGHT;
    static const char HERO_ICON_UP;
    static const char HERO_ICON_DOWN;

    // things having to do with the game:
    unsigned int screenHeight, screenWidth;
    int numLevels, currLevel;
    //things having to do with each level
    int start_map;
    std::vector<int> start_maps;
    int player_start_x, player_start_y;
    std::vector<int> start_coords;
    std::vector<std::vector<int>> start_positions;
    int	max_moves, num_map_segments;
    int num_moves;
    std::vector<int> num_moves_level;
    int playerX, playerY;
    std::string start_dir;
    std::vector<std::string> start_directions;
    std::vector<std::vector<std::vector<std::string>>> GameMaps;
    std::vector<std::vector<std::vector<int>>> PortalLists;
    // things that have to do with the game
    int game_good;
    int win;
   
    std::vector<std::string> BoardAsLines;
    std::vector<int> player_coords; //in reference to location in map
    char character;
    int currentLevel;
    int currentMap;

    std::vector<int> ItemsLevels;
    int ItemsLeft;
    int ItemsTotal;
    int MovesRemaining;
    char input;	



    /**
     * Because one of the constructor parameters must be checked in order
     * to determine the subclass types for the below members, they can't
     * be references, because there's no default constructors for them.
     * (Recall that each member in a class must either be initialized by an
     * initializer list in the constructor OR have a default constructor.)
     */
    View* mView;
    Controller* mController;

    InterfaceType mInterfaceType;
};

#endif // GAME_HPP
