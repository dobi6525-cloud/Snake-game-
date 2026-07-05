#include "Snake.h"

SnakeSingl::SnakeSingl() {
	for (int i = 0; i < BoardHeight; i++) {
		for (int j = 0; j < BoardWidth; j++) {
			if (i == 0 && j == 0 && i == BoardHeight - 1 && j == BoardWidth - 1) {
				board[i][j] = '#';
			}
			else {
				board[i][j] = ' ';
			}
		}
	}

	/*locationSnake = new int*[sizeSnake];
	for (int i = 0; i < sizeSnake / 2; i++) {
		locationSnake[i] = new int[2];
		locationSnake[i+1] = new int[2];
	}

	for (int i = 0; i < 99; i++) {
		for (int j = 0; j < 2; j++) {
			locationSnake[i][j] = 1+i;
		}
	}
	*/
	//for (int i = 0; i < sizeSnake; i++) {
	//	board[locationSnake[i][0]][locationSnake[i][1]] = '*';
	//}


	CastomSnake();
	PrintBoard();
	MonitorBoard();
}

SnakeSingl::~SnakeSingl() {

	/*for (int i = 0; i < sizeSnake; i++) {
		delete[] locationSnake[i];
	}

	delete[] locationSnake;*/
}

inline void SnakeSingl::PrintBoard() const {
	std::cout << board << std::endl;


}

inline void SnakeSingl::MoveSnake() {

}

inline void SnakeSingl::CastomSnake() {
	std::cout << "¬ведите как вы хотите видеть голову своей змеи, введите символ с клавиатуры(ну или вставьте символ из тырнета): ";
	std::cin >> headSnake;
	std::cout << "¬ведите таким же образом тело змеи: ";
	std::cin >> bodySnake;
}

inline void SnakeSingl::MonitorBoard() {
	char move;

	do
	{
		move = _getch();
		switch (move)
		{
		case 'w':

		case 'a':

		case 's':

		case 'd':

		default:
			break;
		}
	} while (true);
}

inline bool SnakeSingl::MoveIsLegal() {
	return false;
}