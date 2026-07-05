#pragma once
#include <iostream>
#include <conio.h>

#define BoardWidth 100
#define BoardHeight 50


class SnakeSingl{
public:
	SnakeSingl();

	~SnakeSingl();

	inline void CastomSnake();
	inline void MonitorBoard();
	inline bool MoveIsLegal();
	inline void PrintBoard() const;
	inline void MoveSnake();


protected:
	char board[BoardWidth][BoardHeight]{};
	
	int* locationSnake;
	
	char headSnake;
	char bodySnake;
	
	int point = 0;
	
	bool left = 0, right = 0, up = 0, down = 0;

	int sizeSnake = 100;
};