#include<iostream>
#include<utility>
#include<string>
#include<sstream>
#include<vector>
#include <stdlib.h>
#include <time.h>

std::vector<std::vector<char>> generateMaze(const std::pair<int, int> &maze_size, const int &number_of_mines);
std::vector<std::vector<char>> fill_initial_maze(const std::pair<int, int> &maze_size);
bool area_check(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &cord, const std::pair<int, int> &maze_size);
bool check_top(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &cord, const int top_check, const int left_check, const int right_check);
bool check_bot(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &cord, const int bot_check, const int left_check, const int right_check);
void print_maze(const std::vector<std::vector<char>> &maze);
bool check_for_full_maze(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &maze_size);
void place_mines(std::vector<std::vector<char>> &maze, const std::pair<int, int> &maze_size, int number_of_mines);
std::pair<int, int> generate_maze_entrance(const std::pair<int, int> &maze_size);
std::pair<int, int> generate_maze_exit(const std::pair<int, int> &maze_size);
void move_robot(std::vector<std::vector<char>> &maze, const char &movement, std::pair<int, int> &robot_position, const std::pair<int, int> &maze_entrance, const std::pair<int, int> &maze_exit);
bool movement_check(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &maze_size, const char &movement, const std::pair<int, int> &robot_position);
void play_game(std::vector<std::vector<char>> &maze, const std::pair<int, int> &maze_size, const std::pair<int, int> &maze_entrance, const std::pair<int, int> &maze_exit);

int main()
{
	srand(time(NULL));
	int x, y, user_mines;
	char sym;
	std::string user_input;
	std::cout << "Please enter the dimensions of the maze as A X B: ";
	std::cin >> user_input;
	std::istringstream inputstream(user_input);
	inputstream >> x >> sym >> y;
	std::pair<int, int> user_size = std::make_pair(x, y);
	std::cout << "Please enter the number of mines the AI should try to place in the maze: ";
	std::cin >> user_mines;
	std::vector<std::vector<char>> maze = generateMaze(user_size, user_mines);

	return 0;
}

std::vector<std::vector<char>> generateMaze(const std::pair<int, int> &maze_size, const int &number_of_mines)
{
	//add checks for entrance and exit
	std::vector<std::vector<char>> maze = fill_initial_maze(maze_size); //fill the maze with walls and empty spaces
	if (number_of_mines == 0){
		return maze;
	}
	if(check_for_full_maze(maze, maze_size)){ // check if the user entered a maze without any valid spaces for mines right off the bat
		return maze;
	}
	std::pair<int, int> maze_entrance = generate_maze_entrance(maze_size);
	maze[maze_entrance.first][maze_entrance.second] = 'X'; //block the entrance for now
	std::pair<int, int> maze_exit = generate_maze_exit(maze_size);
	maze[maze_exit.first][maze_exit.second] = 'X'; //block the exit for now
	place_mines(maze, maze_size, number_of_mines); //place all the mines we can in the maze
	maze[maze_entrance.first][maze_entrance.second] = 'R'; //un-block entrance
	maze[maze_exit.first][maze_exit.second] = '0'; //un-block exit
	print_maze(maze);
	play_game(maze, maze_size, maze_entrance, maze_exit);
	return maze;
}

void place_mines(std::vector<std::vector<char>> &maze, const std::pair<int, int> &maze_size, int number_of_mines)
{
	int x, y;
	do{
		x = rand() % maze_size.first;
		y = rand() % maze_size.second;
	}while(maze[x][y] == '#' || maze[x][y] == 'X');
	while(number_of_mines > 0){
		std::pair<int, int> cord = std::make_pair(x, y);
		if(area_check(maze, cord, maze_size)){
			maze[cord.first][cord.second] = 'X';
			number_of_mines--;
		}
		if(check_for_full_maze(maze, maze_size)){
			break;
		}
		do{
			x = rand() % maze_size.first;
			y = rand() % maze_size.second;
		}while(maze[x][y] == '#' || maze[x][y] == 'X');
	}
}

std::pair<int, int> generate_maze_entrance(const std::pair<int, int> &maze_size)
{
	int x, y;
	do{
		x = 0;
		y = rand() % maze_size.second;
	}while (y == 0 || y == maze_size.second - 1);
	return std::make_pair(x, y);
}

std::pair<int, int> generate_maze_exit(const std::pair<int, int> &maze_size)
{
	int x, y;
	do{
		x = maze_size.first - 1;
		y = rand() %  maze_size.second;
	}while((y == 0) || (y == maze_size.second - 1));
	return std::make_pair(x, y);
}

std::vector<std::vector<char>> fill_initial_maze(const std::pair<int, int> &maze_size)
{
	std::vector<std::vector<char>> maze;
	for(unsigned i = maze_size.first; i > 0; i--){
		std::vector<char> row;
		row.push_back('#');
		for(unsigned j = maze_size.second; j > 1; j--){
			if((i == maze_size.first) || (i == 1)){ //if we are doing the first or last row it must be all walls.
				row.push_back('#');
			}
			else if(j == 2){ // the last of every row must be a wall
				row.push_back('#');
			}
			else{ //if this isnt the first or last row of the first or last character of a row then we make it a empty space
				row.push_back('0');
			}
		}
		maze.push_back(row);
	}
	return maze;
}

void print_maze(const std::vector<std::vector<char>> &maze)
{
	for(auto i = maze.begin(); i != maze.end(); i++){
		for(auto j = (*i).begin(); j != (*i).end(); j++){
			//if((*j) == 'X'){ //hide the mines
				//std::cout << "0 ";
			//} else {
				std::cout << (*j) << " ";
			//}
		}
		std::cout << "\n";
	}
}

bool area_check(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &cord, const std::pair<int, int> &maze_size)
{
	int bot_check = (cord.first + 1 >= maze_size.first) ? -1 : cord.first + 1;
	int top_check = (cord.first - 1 < 0) ? -1 : cord.first - 1;
	int left_check = (cord.second - 1 < 0) ? -1 : cord.second - 1;
	int right_check = (cord.second + 1 >= maze_size.second) ? -1 : cord.second + 1;
	if((top_check == -1) && (bot_check == -1) && (left_check == -1) && (right_check = -1)){
		return false;
	}
	if(top_check != -1){ // can check top
		if(!check_top(maze, cord, top_check, left_check, right_check)){
			return false;
		}
	}
	if(left_check != -1){ // can check left
		if(maze[cord.first][cord.second-1] == 'X'){
			return false;
		}
	}
	if(right_check != -1){ // can check right
		if(maze[cord.first][cord.second+1] == 'X'){
			return false;
		}
	}
	if(bot_check != -1){ // can check bot
		if(!check_bot(maze, cord, bot_check, left_check, right_check)){
			return false;
		}
	}
	return true;
}

bool check_top(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &cord, const int top_check, const int left_check, const int right_check)
{
	if((left_check != -1) && (right_check != -1)){ //left and right are valid
		if((maze[cord.first-1][cord.second-1] == 'X') || (maze[cord.first-1][cord.second+1] == 'X') || (maze[cord.first-1][cord.second] == 'X')){
			return false;
		}
	}
	else if(left_check != -1){
		if((maze[cord.first-1][cord.second-1] == 'X') || (maze[cord.first-1][cord.second] == 'X')){ //right isn't a valid check
			return false;
		}
	}
	else if(right_check != -1){
		if((maze[cord.first-1][cord.second+1] == 'X') || (maze[cord.first-1][cord.second] == 'X')){//left isn't a valid check
			return false;
		}
	}
	return true;
}

bool check_bot(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &cord, const int bot_check, const int left_check, const int right_check)
{
	if((left_check != -1) && (right_check != -1)){ //left and right are valid
		if((maze[cord.first+1][cord.second-1] == 'X') || (maze[cord.first+1][cord.second+1] == 'X') || (maze[cord.first+1][cord.second] == 'X')){
			return false;
		}
	}
	else if(left_check != -1){
		if((maze[cord.first+1][cord.second-1] == 'X') || (maze[cord.first+1][cord.second] == 'X')){ //right isn't a valid check
			return false;
		}
	}
	else if(right_check != -1){
		if((maze[cord.first+1][cord.second+1] == 'X') || (maze[cord.first+1][cord.second] == 'X')){//left isn't a valid check
			return false;
		}
	}
	return true;
}

bool check_for_full_maze(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &maze_size){
	for(int i = 0; i < maze.size(); i++){
		for(int j = 0; j < maze[i].size(); j++){
			std::pair<int, int> cord = std::make_pair(i, j);
			if(maze[i][j] == '0'){
				if(area_check(maze, cord, maze_size)){
					return false;
				}
			}
		}
	}
	return true;
}

void play_game(std::vector<std::vector<char>> &maze, const std::pair<int, int> & maze_size, const std::pair<int, int> &maze_entrance, const std::pair<int, int> &maze_exit)
{
	bool gameOver = false;
	bool started = false;
	int count = 0;
	std::pair<int, int> robot_position = maze_entrance;
	std::pair<int, int> robot_last_position = robot_position;
	std::string orders;
	std::cout << "\nPlease enter the robots orders ((I) start, - stop, (L)eft, (R)ight, (U)p, (D)own)\n";
	std::cout << "The robot's commands must start with a start (I) command and end with a stop (-) command!: ";
	std::cin >> orders;
	if (orders[0] == 'i'){
		orders[0] = 'I';
	}
	while(orders[0] != 'I' || orders[orders.size() - 1] != '-'){
		std::cout << "The robot does not have a valid start (I) and stop (-) command!";
		std::cout << "Please enter the robots orders ((I) start, - stop, (L)eft, (R)ight, (U)p, (D)own): ";
		std::cin >> orders;
	}
	std::istringstream inputstream(orders);
	while (!gameOver){
		count++;
		char movement;
		inputstream >> movement;
		if(movement == 'I' && !started){
			std::cout << "Robot started!\n";
			started = true;
		}
		else if(movement == 'I' && started){
			std::cout << "Robot is already on!\n";
		}
		if(movement == '-' && started){
			std::cout << "Robot shut down!\n";
			if(robot_position == maze_exit){
				gameOver = true;
				std::cout << "You win!\n";
				break;
			}
			started = false;
		}
		else if(movement == '-' && !started){
			std::cout << "Robot is already shut down!\n";
			if(robot_position == maze_exit){
				gameOver = true;
				std::cout << "You win!\n";
				break;
			}
		}
		if(robot_position == maze_exit && !started){
			gameOver = true;
			std::cout << "You win!\n";
			break;
		}
		if(count == orders.size()){
			gameOver = true;
			std::cout << "You Lose!\n";
			break;
		}
		if (movement == 'l'|| movement == 'L'|| movement == 'r'|| movement == 'R'|| movement == 'u'|| movement == 'U'|| movement == 'd'|| movement == 'D'){
			if(movement_check(maze, maze_size, movement, robot_position)){ // if the movement is valid
				move_robot(maze, movement, robot_position, maze_entrance, maze_exit); //move the robot to the new position or back to entrance if the robot stepped on a mine
				maze[robot_last_position.first][robot_last_position.second] = '0';
				print_maze(maze);
				robot_last_position = robot_position;
				continue;
			}
			else{
				std::cout << "You cannot move onto that tile\n";
				continue;
			}
		} else {
			continue;
		}
	}
}

bool movement_check(const std::vector<std::vector<char>> &maze, const std::pair<int, int> &maze_size, const char &movement, const std::pair<int, int> &robot_position)
{
	if(movement == 'l' || movement == 'L'){
		if (robot_position.second - 1 >= 0){
			if(maze[robot_position.first][robot_position.second - 1] != '#') {
				return true;
			} else {
				return false;
			}
		}
		return false;
	}
	else if (movement == 'r' || movement == 'R'){
		if (robot_position.second + 1 < maze_size.second){
			if(maze[robot_position.first][robot_position.second + 1] != '#'){
				return true;
			} else {
				return false;
			}
		}
		return false;
	}
	else if(movement == 'u' || movement == 'U'){
		if (robot_position.first - 1 >= 0){
			if(maze[robot_position.first - 1][robot_position.second] != '#'){
				return true;
			} else {
				return false;
			}
		}
		return false;
	}
	else if(movement == 'd' || movement == 'D'){
		if(robot_position.first + 1 < maze_size.first){
			if(maze[robot_position.first + 1][robot_position.second] != '#'){
				return true;
			} else {
				return false;
			}
		}
		return false;
	}
}

void move_robot(std::vector<std::vector<char>> &maze, const char &movement, std::pair<int, int> &robot_position, const std::pair<int, int> &maze_entrance, const std::pair<int, int> &maze_exit)
{
	if(movement == 'l' || movement == 'L'){
		std::cout << "\nCurrent: (" << robot_position.first << ',' << robot_position.second <<')';
		std::cout << "\nMoving to: (" << robot_position.first << ',' << robot_position.second - 1 <<")\n";
		if(maze[robot_position.first][robot_position.second - 1] == 'X'){
			maze[maze_entrance.first][maze_entrance.second] = 'R';
			robot_position = maze_entrance;
			return;
		}
		maze[robot_position.first][robot_position.second - 1] = 'R';
		robot_position.second -= 1;
	}
	else if(movement == 'r' || movement == 'R'){
		std::cout << "\nCurrent: (" << robot_position.first << ',' << robot_position.second <<')';
		std::cout << "\nMoving to: (" << robot_position.first << ',' << robot_position.second + 1 <<")\n";
		if(maze[robot_position.first][robot_position.second + 1] == 'X'){
			maze[maze_entrance.first][maze_entrance.second] = 'R';
			robot_position = maze_entrance;
			return;
		}
		maze[robot_position.first][robot_position.second + 1] = 'R';
		robot_position.second += 1;
	}
	else if(movement == 'u' || movement == 'U'){
		std::cout << "\nCurrent: (" << robot_position.first << ',' << robot_position.second <<')';
		std::cout << "\nMoving to: (" << robot_position.first - 1 << ',' << robot_position.second <<")\n";
		if(maze[robot_position.first - 1][robot_position.second] == 'X'){
			maze[maze_entrance.first][maze_entrance.second] = 'R';
			robot_position = maze_entrance;
			return;
		}
		maze[robot_position.first - 1][robot_position.second] = 'R';
		robot_position.first -= 1;
	}
	else if(movement == 'd' || movement == 'D'){
		std::cout << "\nCurrent: (" << robot_position.first << ',' << robot_position.second <<')';
		std::cout << "\nMoving to: (" << robot_position.first + 1 << ',' << robot_position.second <<")\n";
		if(maze[robot_position.first + 1][robot_position.second] == 'X'){
			maze[maze_entrance.first][maze_entrance.second] = 'R';
			robot_position = maze_entrance;
			return;
		}
		maze[robot_position.first + 1][robot_position.second] = 'R';
		robot_position.first += 1;
	}
}
