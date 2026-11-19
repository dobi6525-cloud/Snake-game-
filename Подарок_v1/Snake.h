#pragma once

#include <iostream>
#include <conio.h>
#include <ctime>
#include <Windows.h>
#include <vector>

#define BoardWidth 100
#define BoardHeight 50

class Network_Server;
class Network_Client;

class Snake{
public:
    Snake();

    void virtual Play();
    void CastomSnake();
    void MonitorKeyBoard();
    bool MoveIsLegal(int move_x, int move_y);
    bool CheckPositionSnake(int move_x, int move_y);
    void virtual Board();
    void MoveSnake(int move_x, int move_y);
    void CreateApple();
    bool CheckSnakeAndPositionApple(int move_x, int move_y);
    void PrintDataForDisplay(int move_x, int move_y, char simbol);
    void PrintDataScore();
    void IncreaceSpaceArray();
    void GameScore();
    bool Death(int mov_x, int move_y);
    ~Snake();

protected:
    char board[BoardHeight][BoardWidth];
    char boardV2[BoardHeight * BoardWidth]{};

    int** locationSnake;

    char headSnake;
    char bodySnake;

    static int point;

    bool flagPointerHead = false;

    static int sizeSnake;
    static int rightBorderSnake;
    static int pointerHead;
    static int pointerTail;
    static int pointerSecondHead;

    int positionApple_x;
    int positionApple_y;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPos;

    bool lifeSnake = true;
};

class MyltiplaySnake {
public:
    MyltiplaySnake(int countConnections, bool SendSenrverOrClient);

    void AlgorithmInitPositionsSnakeServer(std::vector<std::vector<int>>& arrayPlace);
    
    void InitData(std::vector<std::vector<int>>& allPositionsSnake, std::vector<std::vector<char>>& fashonSnake, std::vector<std::string>& namePlayers, unsigned int myPosition, unsigned countUsers);
    void InitOtherDataServer(SOCKET (&connection)[60]);
    void InitOtherDataClient(SOCKET& socket);
    void CreateAppleFullUsers(std::vector<std::vector<int>>& positionApple);
    void createAppleForOneUser();

    void InitApple(std::vector<std::vector<int>>& positionApple);
    bool ChechPositionApple(int row, int col);

    void StartGame();

    ~MyltiplaySnake();


    void SendMessageServer();
    void SendMessageClient();

    void RecvMessage(char* data);

private:
    void BinarySearch(std::vector<std::vector<int>>& arrayPlace, int Wiedth, int Height) const;

    void PrintBoard();
    void InitBoard();

    void PlayLoop();
    bool MoveIsLegal(int x, int y);
    void MoveSnake(int x, int y, int position, bool otherUsers);
    void Board();
    bool CheckSnakeAndPositionApple(int x, int y);
    void IncreaceSpaceArray(int position);
    void PrintDataScore();
    void PrintDataForDisplay(int x, int y, char simbol);
    bool Death(int x, int y);
    bool CountDead();
    void DeleteSnake(int position);
    
    void GenerateMessage();
    void ReadMessage(char* message, int& position, char& move);


    char board[BoardHeight][BoardWidth]{};
    std::vector<std::vector<std::vector<int>>> positionsSnake;
    std::vector<std::vector<int>> positionApple;
    int** pointerHeadSecondHeadAndTail = NULL;
    unsigned int* pointScore = NULL;
    int countConnections = 0;
    int countDeathSnake = 0;
    std::vector<std::string> nameUser;
    std::vector<std::vector<char>> fashonSnake;
    unsigned int countUsers = 0;
    int myPosition = 0;

    unsigned int* sizeSnakes = NULL;
    unsigned int pointerEatAppl = 0;
    unsigned int countApple = 0;
    int indexEatApple = 0;
    int AppleEat_x = 0;
    int AppleEat_y = 0;
    

    char move[256]{};

    SOCKET* connection;
    SOCKET server;


    bool flagLife = 1;
    bool flagPointerHead = false;
    bool You = false;
    bool EatApple = false;
    char moveThis;
    

    bool SendServerOrClient = NULL;


};
