#include "Snake.h"

Snake::Snake(int startpos_x, int startpos_y, int gameSize_x, int gameSize_y) {
	for (int y_offset = 0; y_offset < Snake::INITIAL_LENGTH; y_offset++) {
		this->snakeParts.push_back({ startpos_x, startpos_y + y_offset, SNAKEPART_BODY, Snake::DIRECTION_UP});
	}
	this->snakeParts[0].snakePartType = Snake::SNAKEPART_HEAD;
	this->snakeParts[this->snakeParts.size()-1].snakePartType = Snake::SNAKEPART_TAIL;
	this->snakeParts[0].direction = Snake::DIRECTION_UP;
	this->gamesize_x = gameSize_x;
	this->gamesize_y = gameSize_y;
}

bool isRightOf(int partOne, int partTwo) {
	if (partOne == Snake::DIRECTION_UP)
		return partTwo == Snake::DIRECTION_LEFT;
	else if (partOne == Snake::DIRECTION_LEFT)
		return partTwo == Snake::DIRECTION_DOWN;
	return (partOne > partTwo);
}

void Snake::move() {
	for (int i = this->snakeParts.size() - 1;i > 0; i--) {
		this->snakeParts[i].x = this->snakeParts[i - 1].x;
		this->snakeParts[i].y = this->snakeParts[i - 1].y;
		this->snakeParts[i].direction = this->snakeParts[i - 1].direction;
	}

	this->snakeParts[this->snakeParts.size() - 1].direction = this->snakeParts[this->snakeParts.size() - 2].direction;
	this->snakeParts[0].direction = this->direction;

	for (int i = 1; i < this->snakeParts.size() -1; i++) {
		if (this->snakeParts[i].direction != this->snakeParts[i - 1].direction)
			if (isRightOf(this->snakeParts[i - 1].direction, this->snakeParts[i].direction)) 
				this->snakeParts[i].snakePartType = Snake::SNAKEPART_TURNRIGHT;
			else
				this->snakeParts[i].snakePartType = Snake::SNAKEPART_TURNLEFT;
		else
			this->snakeParts[i].snakePartType = Snake::SNAKEPART_BODY;
	}
	switch (this->direction)
	{
	case Snake::DIRECTION_DOWN:
		this->snakeParts[0].y ++;
		break;
	case Snake::DIRECTION_LEFT:
		this->snakeParts[0].x--;
		break;
	case Snake::DIRECTION_RIGHT:
		this->snakeParts[0].x ++;
		break;
	case Snake::DIRECTION_UP:
		this->snakeParts[0].y --;
		break;
	}
	this->alive = checkAlive();
}

void Snake::grow() {
	this->snakeParts.push_back(this->snakeParts.back());
	this->snakeParts[this->snakeParts.size() - 2].snakePartType = Snake::SNAKEPART_BODY;
}

bool Snake::checkAlive() {
	for (int i = 1; i < this->snakeParts.size(); i++) {
		if (this->snakeParts[0].x == this->snakeParts[i].x && this->snakeParts[0].y == this->snakeParts[i].y) {
			return false;
		}
	}
	return this->snakeParts[0].x < this->gamesize_x && this->snakeParts[0].x >= 0 && this->snakeParts[0].y < this->gamesize_y&& this->snakeParts[0].y >= 0;
}

bool Snake::isAlive() {
	return this->alive;
}

void Snake::turnRight() {
	if (this->direction < 3)
		this->direction++;
	else
		this->direction = 0;
}

void Snake::turnLeft() {
	if (this->direction > 0)
		this->direction--;
	else
		this->direction = 3;
}
