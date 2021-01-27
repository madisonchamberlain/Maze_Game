#include "game.hpp"
#include "curses_view.hpp"
#include "curses_controller.hpp"
#include "print_view.hpp"
#include "print_controller.hpp"

#define INVALID_GAME_FILE_MSG "Invalid game input file"
#define INVALID_LEVEL_FILE_MSG "Invalid level input file"
#define INVALID_VIEW_TYPE_MSG "Invalid view type"

Game::Game(const std::string& filename, InterfaceType interfaceType)
    : mView{nullptr}
    , mController{nullptr}
    , mInterfaceType{interfaceType}
{
	std::ifstream file(filename);
	if (!file)
	{
		throw std::runtime_error{INVALID_GAME_FILE_MSG};
	}
	std::string path;
	size_t found;
	found=filename.find_last_of("/\\");
	path = filename.substr(0,found);
	path.append("/");
	if (path == filename)
	{
		path.clear();
	}
	Game::game_good = 1;
	Game::win = 0;
	file>>Game::screenHeight>>Game::screenWidth;
	Game::playerX = ((Game::screenWidth - 1)/2) + 1;
	Game::playerY = ((Game::screenHeight - 1)/2) + 1;
	file>>Game::numLevels;	
	std::string nextLevelFile;
	Game::start_coords.push_back(0);
	Game::start_coords.push_back(0);
	for (int i=0; i<Game::numLevels; i++)
	{
		file>>nextLevelFile;
		std::string file_with_path;
		file_with_path.append(path);
		file_with_path.append(nextLevelFile);
		Game::readInLevel(file_with_path);
	}
}

void Game::draw()
{
	std::vector<std::string> current_map = Game::GameMaps[Game::currentLevel][Game::currentMap];
	Game::BoardAsLines.clear();
	std::string str1;
	std::string border_char = "*";
	std::string fill_char = "~";
	for (unsigned int i=0; i<(Game::screenWidth + 2); i++)
	{
		str1.append(border_char);
	}
	Game::BoardAsLines.push_back(str1);
	for (unsigned int i=0; i<Game::screenHeight; i++)
	{
		std::string strMid;
		strMid.append(border_char);
		for(unsigned int j=0; j<Game::screenWidth; j++)
		{
			strMid.append(fill_char);
		}
		strMid.append(border_char);
		Game::BoardAsLines.push_back(strMid);
	}
	std::string strLast;
	for (unsigned int i=0; i<(Game::screenWidth + 2); i++)
	{
		strLast.append(border_char);
	}
	Game::BoardAsLines.push_back(strLast);
	int map_x = Game::playerX - Game::player_coords[1];
	int map_y = Game::playerY - Game::player_coords[0];
	unsigned int height_map = current_map.size();
	unsigned int width_map = current_map[0].length();
	int k=0;
	if (map_y < 1)
	{
		k = (k - map_y)+1;
	}
	for (unsigned int i=std::max(map_y, 1); i<(std::min(Game::screenHeight+1, height_map+map_y)); i++)
	{
		int l = 0;
		if (map_x < 1)
		{
			l = (l-map_x) + 1;
		}
		for (unsigned int j=std::max(map_x, 1); j<(std::min(Game::screenWidth+1, width_map+map_x)); j++)
		{
			Game::BoardAsLines[i][j] = current_map[k][l];
			l++;
		}
		k++;
	}
	Game::BoardAsLines[Game::playerY][Game::playerX] = Game::character;
}

void Game::update()
{
	if (Game::character == 'v')
	{
		if (Game::input == 'a')
		{
			Game::character = '>';
			return;
		}
		else if (Game::input == 'd')
		{
			Game::character = '<';
			return;
		}
		else if (Game::input == 's')
		{
			return;
		}
		else if (Game::input == 'w')
		{
			std::vector<std::string> current_map = Game::GameMaps[Game::currentLevel][Game::currentMap];
			int next_height = Game::player_coords[0] + 1;
			char next_char = current_map[Game::player_coords[0]+1][Game::player_coords[1]];
			if (next_char == ' ' || next_char == '$' || next_char == '@')
			{	
				Game::player_coords[0] = next_height;
				Game::MovesRemaining = Game::MovesRemaining - 1;
				if (MovesRemaining == 0)
				{
					Game::game_good = 0;
					return;
				}
				char current_char = current_map[Game::player_coords[0]][Game::player_coords[1]];
				if (current_char == '$')
				{	
					Game::ItemsLeft = Game::ItemsLeft - 1;
					Game::GameMaps[Game::currentLevel][Game::currentMap][Game::player_coords[0]][Game::player_coords[1]] = ' ';
					if (Game::ItemsLeft == 0)
					{
						if (Game::currentLevel < (Game::numLevels - 1))
						{
							Game::currentLevel = Game::currentLevel + 1;
							Game::loadLevel();
							return;
						}
						else
						{	
							Game::game_good = 0;
							Game::win = 1;
							return; 
						}
						return;
					}
					return;
				}
				else if (current_char == '@')
				{
					for (unsigned int i=0; i<Game::PortalLists[Game::currentLevel].size(); i++)
					{
						if ((Game::PortalLists[Game::currentLevel][i][0] == Game::currentMap) && (Game::PortalLists[Game::currentLevel][i][1] == Game::player_coords[0]) && (Game::PortalLists[Game::currentLevel][i][2] == Game::player_coords[1]))
						{
							Game::player_coords[0] = Game::PortalLists[Game::currentLevel][i][4];
							Game::player_coords[1] = Game::PortalLists[Game::currentLevel][i][5];
							Game::currentMap = Game::PortalLists[Game::currentLevel][i][3];
							if (Game::PortalLists[Game::currentLevel][i][6] == 0)
							{
								Game::character = '>';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 1)
							{
								Game::character = '<';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 2)
							{
								Game::character = 'v';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 3)
							{
								Game::character = '^';
							}
						}
					}
				}
				return;
			}
			return;
		
		}
	}
	if (Game::character == '^')
	{
		if (Game::input == 'a')
		{
			Game::character = '<';
			return;
		}
		else if (Game::input == 'd')
		{
			Game::character = '>';
			return;
		}
		else if (Game::input == 's')
		{
			return;
		}
		else if (Game::input == 'w')
		{
			std::vector<std::string> current_map = Game::GameMaps[Game::currentLevel][Game::currentMap];
			int next_height = Game::player_coords[0] - 1;
			char next_char = current_map[Game::player_coords[0]-1][Game::player_coords[1]];
			if (next_char == ' ' || next_char == '$'||next_char =='@')
			{	
				Game::player_coords[0] = next_height;
				Game::MovesRemaining = Game::MovesRemaining - 1;
				if (MovesRemaining == 0)
				{
					Game::game_good = 0;
					return;
				}
				char current_char = current_map[Game::player_coords[0]][Game::player_coords[1]];
				if (current_char == '$')
				{	
					Game::ItemsLeft = Game::ItemsLeft - 1;
					Game::GameMaps[Game::currentLevel][Game::currentMap][Game::player_coords[0]][Game::player_coords[1]] = ' ';
					if (Game::ItemsLeft == 0)
					{
						if (Game::currentLevel < (Game::numLevels - 1))
						{
							Game::currentLevel = Game::currentLevel + 1;
							Game::loadLevel();
							return;
						}
						else
						{
							Game::game_good = 0;
							Game::win = 1;
							return; 
						}
						return;
					}
					return;
				}
				else if (current_char == '@')
				{
					for (unsigned int i=0; i<Game::PortalLists[Game::currentLevel].size(); i++)
					{
						if ((Game::PortalLists[Game::currentLevel][i][0] == Game::currentMap) && (Game::PortalLists[Game::currentLevel][i][1] == Game::player_coords[0]) && (Game::PortalLists[Game::currentLevel][i][2] == Game::player_coords[1]))
						{
							Game::player_coords[0] = Game::PortalLists[Game::currentLevel][i][4];
							Game::player_coords[1] = Game::PortalLists[Game::currentLevel][i][5];
							Game::currentMap = Game::PortalLists[Game::currentLevel][i][3];
							if (Game::PortalLists[Game::currentLevel][i][6] == 0)
							{
								Game::character = '>';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 1)
							{
								Game::character = '<';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 2)
							{
								Game::character = 'v';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 3)
							{
								Game::character = '^';
							}
						}
					}
				}
				return;
			}
			return;
		
		}
	}
	if (Game::character == '>')
	{
		if (Game::input == 'a')
		{
			Game::character = '^';
			return;
		}
		else if (Game::input == 'd')
		{
			Game::character = 'v';
			return;
		}
		else if (Game::input == 's')
		{
			return;
		}
		else if (Game::input == 'w')
		{
			std::vector<std::string> current_map = Game::GameMaps[Game::currentLevel][Game::currentMap];
			int next_width = Game::player_coords[1] + 1;
			char next_char = current_map[Game::player_coords[0]][Game::player_coords[1]+1];
			if (next_char == ' ' || next_char == '$'||next_char == '@')
			{	
				Game::player_coords[1] = next_width;
				Game::MovesRemaining = Game::MovesRemaining - 1;
				if (MovesRemaining == 0)
				{
					Game::game_good = 0;
					return;
				}
				char current_char = current_map[Game::player_coords[0]][Game::player_coords[1]];
				if (current_char == '$')
				{	
					Game::ItemsLeft = Game::ItemsLeft - 1;
					Game::GameMaps[Game::currentLevel][Game::currentMap][Game::player_coords[0]][Game::player_coords[1]] = ' ';
					if (Game::ItemsLeft == 0)
					{
						if (Game::currentLevel < (Game::numLevels - 1))
						{
							Game::currentLevel = Game::currentLevel + 1;
							Game::loadLevel();
							return;
						}
						else
						{
							Game::game_good = 0;
							Game::win =1;
							return; 
						}
						return;
					}
					return;
				}
				else if (current_char == '@')
				{
					for (unsigned int i=0; i<Game::PortalLists[Game::currentLevel].size(); i++)
					{
						if ((Game::PortalLists[Game::currentLevel][i][0] == Game::currentMap) && (Game::PortalLists[Game::currentLevel][i][1] == Game::player_coords[0]) && (Game::PortalLists[Game::currentLevel][i][2] == Game::player_coords[1]))
						{
							Game::player_coords[0] = Game::PortalLists[Game::currentLevel][i][4];
							Game::player_coords[1] = Game::PortalLists[Game::currentLevel][i][5];
							Game::currentMap = Game::PortalLists[Game::currentLevel][i][3];
							if (Game::PortalLists[Game::currentLevel][i][6] == 0)
							{
								Game::character = '>';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 1)
							{
								Game::character = '<';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 2)
							{
								Game::character = 'v';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 3)
							{
								Game::character = '^';
							}
						}
					}
				}
				return;
			}
			return;		
		}
	}
	if (Game::character == '<')
	{
		if (Game::input == 'a')
		{
			Game::character = 'v';
			return;
		}
		else if (Game::input == 'd')
		{
			Game::character = '^';
			return;
		}
		else if (Game::input == 's')
		{
			return;
		}
		else if (Game::input == 'w')
		{
			std::vector<std::string> current_map = Game::GameMaps[Game::currentLevel][Game::currentMap];
			int next_width = Game::player_coords[1] - 1;
			char next_char = current_map[Game::player_coords[0]][Game::player_coords[1]-1];
			if (next_char == ' ' || next_char == '$'||next_char=='@')
			{	
				Game::player_coords[1] = next_width;
				Game::MovesRemaining = Game::MovesRemaining - 1;
				if (MovesRemaining == 0)
				{
					Game::game_good = 0;
					return;
				}
				char current_char = current_map[Game::player_coords[0]][Game::player_coords[1]];
				if (current_char == '$')
				{	
					Game::ItemsLeft = Game::ItemsLeft - 1;
					Game::GameMaps[Game::currentLevel][Game::currentMap][Game::player_coords[0]][Game::player_coords[1]] = ' ';
					if (Game::ItemsLeft == 0)
					{
						if (Game::currentLevel < (Game::numLevels - 1))
						{
							Game::currentLevel = Game::currentLevel + 1;
							Game::loadLevel();
							return;
						}
						else
						{
							Game::game_good = 0;
							Game::win = 1;
							return; 
						}
						return;
					}
					return;
				}
				else if (current_char == '@')
				{
					for (unsigned int i=0; i<Game::PortalLists[Game::currentLevel].size(); i++)
					{
						if ((Game::PortalLists[Game::currentLevel][i][0] == Game::currentMap) && (Game::PortalLists[Game::currentLevel][i][1] == Game::player_coords[0]) && (Game::PortalLists[Game::currentLevel][i][2] == Game::player_coords[1]))
						{
							Game::player_coords[0] = Game::PortalLists[Game::currentLevel][i][4];
							Game::player_coords[1] = Game::PortalLists[Game::currentLevel][i][5];
							Game::currentMap = Game::PortalLists[Game::currentLevel][i][3];
							if (Game::PortalLists[Game::currentLevel][i][6] == 0)
							{
								Game::character = '>';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 1)
							{
								Game::character = '<';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 2)
							{
								Game::character = 'v';
							}
							else if (Game::PortalLists[Game::currentLevel][i][6] == 3)
							{
								Game::character = '^';
							}
						}
					}
				}
				return;
			}
			return;
		}
	}
}

void Game::readInLevel(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file)
	{
		throw std::runtime_error{INVALID_LEVEL_FILE_MSG};
	}
	file>>Game::start_map;
	Game::start_maps.push_back(Game::start_map);
	file>>Game::player_start_y>>Game::player_start_x;
	Game::start_coords[0] = Game::player_start_y;
	Game::start_coords[1] = Game::player_start_x;
	Game::start_positions.push_back(Game::start_coords);
	file>>Game::start_dir;
	Game::start_directions.push_back(Game::start_dir);
	int items_total = 0;
	char letter;
	std::vector<std::vector<std::string>> levelMaps;
	std::vector<std::vector<int>> PortalList;
	while(file>>letter)
	{
		if (letter == 'M')
		{	
			int height, width;
			file>>height>>width;
			MapSegment map{height, width}; 
			levelMaps.push_back(map.getAsLines());
		}
		if (letter == 'B')
		{
			int int_1, int_2, int_3;
			file>>int_1>>int_2>>int_3;
			Building building{int_3, int_2}; 
			building.drawIn(levelMaps[int_1]); 		
		}
		else if (letter == 'I')
		{
			int int_1, int_2, int_3;
			file>>int_1>>int_2>>int_3;
			Item item{int_3, int_2}; 
			item.drawIn(levelMaps[int_1]);
			items_total ++;
		}
		else if (letter == 'N')
		{
			int num_moves;
			file>>num_moves;
			Game::num_moves_level.push_back(num_moves);
		}
		else if (letter == 'P')
		{	
			std::vector<int> portal_1;   
			std::vector<int> portal_2;
			for (int i=0; i<7; i++)
			{
				portal_1.push_back(0);
				portal_2.push_back(0);
			}
			int map_a, map_b;
			std::string dir_a;
			std::string dir_b;
			file>>map_a>>dir_a>>map_b>>dir_b;
			portal_1[0] = map_a;
			portal_1[3] = map_b; 
			portal_2[0] = map_b;
			portal_2[3] = map_a;
			std::string left = "left";
			std::string right = "right";
			std::string up = "up";
			std::string down = "down";
			char at_sign = '@';
			if (dir_a == left) 
			{	
				levelMaps[map_a][(levelMaps[map_a].size() - 1)/2][0] = at_sign;
				portal_2[6] = 0;
				portal_1[1]=(levelMaps[map_a].size()-1)/2;
				portal_1[2]=0;
				portal_2[4]=(levelMaps[map_a].size()-1)/2;
				portal_2[5]=1;
			}
			if (dir_a == right)
			{
				levelMaps[map_a][(levelMaps[map_a].size() - 1)/2][levelMaps[map_a][0].length()-1] = at_sign;
				portal_2[6] = 1;
				portal_1[1]=(levelMaps[map_a].size()-1)/2;
				portal_1[2]=(levelMaps[map_a][0].length()-1);
				portal_2[4]=(levelMaps[map_a].size()-1)/2;
				portal_2[5]=(levelMaps[map_a][0].length()-1)-1;
			}
			if (dir_a == up)
			{
				levelMaps[map_a][0][(levelMaps[map_a][0].length()-1)/2] = at_sign;
				portal_2[6] = 2;
				portal_1[1] = 0;
				portal_1[2] = (levelMaps[map_a][0].length()-1)/2;
				portal_2[4] = 1;
				portal_2[5] = (levelMaps[map_a][0].length()-1)/2;
			}
			if (dir_a == down)
			{
				levelMaps[map_a][levelMaps[map_a].size()-1][(levelMaps[map_a][0].length() - 1)/2] = at_sign;
				portal_2[6] = 3;
				portal_1[1] = levelMaps[map_a].size()-1;
				portal_1[2] = (levelMaps[map_a][0].length() -1)/2;
				portal_2[4] = levelMaps[map_a].size()-2;
				portal_2[5] = (levelMaps[map_a][0].length()-1)/2;
			}
			if (dir_b == left)
			{
				levelMaps[map_b][(levelMaps[map_b].size()-1)/2][0] = at_sign;
				portal_1[6] = 0; 
				portal_2[1]=(levelMaps[map_b].size()-1)/2;
				portal_2[2]=0;
				portal_1[4]=(levelMaps[map_b].size()-1)/2;
				portal_1[5]=1;
			}
			if (dir_b == right)
			{	
				levelMaps[map_b][(levelMaps[map_b].size() - 1)/2][(levelMaps[map_b][0].length()-1)] = at_sign;
				portal_1[6] = 1;
				portal_2[1]=(levelMaps[map_b].size()-1)/2;
				portal_2[2]=(levelMaps[map_b][0].length()-1);
				portal_1[4]=(levelMaps[map_b].size()-1)/2;
				portal_1[5]=(levelMaps[map_b][0].length()-1)-1;
			}
			if (dir_b == up)
			{
				levelMaps[map_b][0][(levelMaps[map_b][0].length()-1)/2] = at_sign;
				portal_1[6] = 2;
				portal_2[1] = 0;
				portal_2[2] = (levelMaps[map_b][0].length()-1)/2;
				portal_1[4] = 1;
				portal_1[5] = (levelMaps[map_b][0].length()-1)/2;
			}
			if (dir_b == down)
			{
				levelMaps[map_b][levelMaps[map_b].size()-1][(levelMaps[map_b][0].length() - 1)/2] = at_sign;
				portal_1[6] = 3;
				portal_2[1] = levelMaps[map_b].size()-1;
				portal_2[2] = (levelMaps[map_b][0].length() -1)/2;
				portal_1[4] = levelMaps[map_b].size()-2;
				portal_1[5] = (levelMaps[map_b][0].length()-1)/2;
			}
			PortalList.push_back(portal_1);
			PortalList.push_back(portal_2);
		}
	}
	Game::PortalLists.push_back(PortalList);
	Game::GameMaps.push_back(levelMaps);
	Game::ItemsLevels.push_back(items_total);
}

Game::~Game()
{
    // TODO: Usually, deallocate anything that was dynamically
    // allocated in the ctor (constructor).
    
}

void Game::loadLevel()
{
	Game::currentMap = start_maps[Game::currentLevel];
	Game::MovesRemaining = Game::num_moves_level[Game::currentLevel];
	Game::player_coords = Game::start_positions[Game::currentLevel];	
	std::string start_direction = start_directions[Game::currentLevel];
	Game::ItemsTotal = Game::ItemsLevels[Game::currentLevel];
	Game::ItemsLeft = Game::ItemsTotal;
	std::string left = "left";
	std::string right = "right";
	std::string up = "up";
	std::string down = "down";
	if (start_direction == left)
	{
		Game::character = '<';
	}
	else if (start_direction == right)
	{
		Game::character = '>';
	}
	if (start_direction == up)
	{
		Game::character = '^';
	}
	if (start_direction == down)
	{
		Game::character = 'v';
	}
}


void Game::run()
{
	Game::currentLevel = 0;
    	Game::loadLevel();
	Game::doGameLoop();
}

void Game::doGameLoop()
{
	if (mInterfaceType == InterfaceType::Print)
	{	
		Game::draw();
		PrintView view{Game::screenHeight, Game::screenWidth};
		PrintController controller;

		view.draw(Game::currentLevel, Game::ItemsLeft, Game::MovesRemaining, Game::BoardAsLines);
		while (Game::game_good == 1)
		{
			Game::input = controller.getInput();
			if (Game::input == 'q')
			{
				std::cout<<"You quit the game."<<std::endl;
				Game::game_good = 0;
				return;
			}
			Game::update();
			if (Game::MovesRemaining == 0)
			{	
				std::cout<<"You lost the game."<<std::endl;
				return;
			}
			if (Game::win == 1)
			{
				Game::draw();
				view.draw(Game::currentLevel, Game::ItemsLeft, Game::MovesRemaining, Game::BoardAsLines);
				std::cout<<"You won the game."<<std::endl;
				return;
			}
			Game::draw();
			view.draw(Game::currentLevel, Game::ItemsLeft, Game::MovesRemaining, Game::BoardAsLines);
		}
	}
	if (mInterfaceType == InterfaceType::Curses)
	{	
		Game::draw();
		CursesView view{Game::screenHeight, Game::screenWidth};
		CursesController controller;
		initscr();
		clear();
		noecho();
		cbreak();
		clear();
		refresh();
		view.draw(Game::currentLevel, Game::ItemsLeft, Game::MovesRemaining, Game::BoardAsLines);
		while (Game::game_good == 1)
		{
			Game::input = controller.getInput();
			if (Game::input == 'q')
			{
				mvaddstr(Game::screenHeight+6, 0, "You quit the game.");
				Game::game_good = 0;
				refresh();
				clear();
				refresh();
				return;
			}
			Game::update();
			if (Game::MovesRemaining == 0)
			{
				mvaddstr(Game::screenHeight+6, 0, "You lost the game.");
				refresh();
				clear();
				refresh();
				return;
			}
			if (Game::win == 1)
			{
				Game::draw();
				view.draw(Game::currentLevel, Game::ItemsLeft, Game::MovesRemaining, Game::BoardAsLines);
				mvaddstr(Game::screenHeight + 6 , 0, "You won the game.");
				refresh();
				clear();
				refresh();
				return;
			}
			Game::draw();
			view.draw(Game::currentLevel, Game::ItemsLeft, Game::MovesRemaining, Game::BoardAsLines);
		}
		clear();
		endwin();
		refresh();
	}
}
