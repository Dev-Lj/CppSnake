#pragma once
#include <vector>

struct snakepart
{
	int x;
	int y;
	int snakePartType;
	int direction;
};

class Snake
{
private:
	std::vector<snakepart> snakeParts;
	int direction;
	bool isAlive = true;
	int gamesize_x;
	int gamesize_y;


	bool checkAlive();
public:
	static const int INITIAL_LENGTH = 3;
	enum Directions
	{
		DIRECTION_UP = 0,
		DIRECTION_RIGHT = 1,
		DIRECTION_DOWN = 2,
		DIRECTION_LEFT = 3
	};
	enum SnakePartType
	{
		SNAKEPART_HEAD = 0,
		SNAKEPART_BODY = 1,
		SNAKEPART_TURNRIGHT = 2,
		SNAKEPART_TURNLEFT = 3,
		SNAKEPART_TAIL = 4
	};

	Snake(int startpos_x, int startpos_y, int gameSize_x, int gameSize_y);
	
	void move();
	void turnRight();
	void turnLeft();
	void grow();
	bool getIsAlive() {
		return this->isAlive;
	}
	std::vector<snakepart> getSnakeParts() {
		return this->snakeParts;
	}
};

