#include "Snake.h"

int Snake::sizeSnake = 100;
int Snake::rightBorderSnake = 1;
int Snake::pointerHead = 0;
int Snake::pointerTail = 0;
int Snake::pointerSecondHead = 0;
int Snake::point = 0;


Snake::Snake() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    locationSnake = new int*[sizeSnake];

    for (int i = 0; i < sizeSnake; ++i) {
        locationSnake[i] = new int[2];
    }

    locationSnake[0][0] = 50;
    locationSnake[0][1] = 25;

    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            if (i == 0 || j == 0 || i == BoardHeight - 1 || j == BoardWidth - 1) {
                board[i][j] = '#';
            }
            else {
                board[i][j] = ' ';
            }
        }
    }

}

void Snake::Play() {
    CastomSnake();
    system("cls");

    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            std::cout << board[i][j];
        }
        std::cout << '\n';
    }

    PrintDataForDisplay(51, 26, headSnake);
    CreateApple();
    Board();
    MonitorKeyBoard();
}

Snake::~Snake() {
    rightBorderSnake = 1;
    pointerHead = 0;
    pointerTail = 0;
    pointerSecondHead = 0;
    point = 0;
    flagPointerHead = false;
    lifeSnake = true;

    for (int i = 0; i < sizeSnake; ++i) {
        delete[] locationSnake[i];
    }

    delete[] locationSnake;
}

void Snake::Board(){
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            if (i == 0 || j == 0 || i == BoardHeight - 1 || j == BoardWidth - 1) {
                board[i][j] = '#';
            }
            else {
                board[i][j] = ' ';
            }
        }
    }

    board[positionApple_y][positionApple_x] = '$';

    for (int i = 0; i < point; ++i) {
        board[locationSnake[i][1]][locationSnake[i][0]] = bodySnake;
    }

    board[locationSnake[point][1]][locationSnake[point][0]] = headSnake;
}

void Snake::CastomSnake() {
    std::cout << "Ââåäèòå êàê âû õîòèòå âèäåòü ãîëîâó ñâîåé çìåè, ââåäèòå ñèìâîë ñ êëàâèàòóðû(íó èëè âñòàâüòå ñèìâîë èç òûðíåòà): ";
    std::cin >> headSnake;
    std::cout << "Ââåäèòå òàêèì æå îáðàçîì òåëî çìåè: ";
    std::cin >> bodySnake;
}

void Snake::MonitorKeyBoard() {
    char move;

    do
    {
        move = _getch();
        switch (move)
        {
        case 'w':
            if (MoveIsLegal(0, -1) && lifeSnake) {
                MoveSnake(0, -1);
                Board();
                break;
            }

            break;
        case 'a':
            if (MoveIsLegal(-1, 0) && lifeSnake) {
                MoveSnake(-1, 0);
                Board();
                break;
            }
            break;
        case 's':
            if (MoveIsLegal(0, 1) && lifeSnake) {
                MoveSnake(0, 1);
                Board();
                break;
            }
            break;
        case 'd':
            if (MoveIsLegal(1, 0) && lifeSnake) {
                MoveSnake(1, 0);
                Board();
                break;
            }
            break;
        default:
            break;
        }
    } while ((lifeSnake != false) && (move != 'b'));

    GameScore();
}

bool Snake::MoveIsLegal(int move_x, int move_y){
    int temp_x = locationSnake[pointerHead][0], temp_y = locationSnake[pointerHead][1];
    if(flagPointerHead){
        if (temp_y + move_y != BoardHeight - 1 && temp_y + move_y != 0 && temp_x + move_x != 0 &&
            temp_x + move_x != BoardWidth - 1 && (temp_x + move_x != locationSnake[pointerSecondHead][0] ||
            temp_y + move_y != locationSnake[pointerSecondHead][1])) {
            return true;
        }
        return false;
    }
    if (temp_y + move_y != BoardHeight - 1 && temp_y + move_y != 0 && temp_x + move_x != 0 &&
        temp_x + move_x != BoardWidth - 1) {
        return true;
    }
    return false;
}

bool Snake::CheckPositionSnake(int move_x, int move_y) {
    for (int i = 0; i < rightBorderSnake; i++) {
        if (locationSnake[pointerHead][0] + move_x == locationSnake[i][0] && 
            locationSnake[pointerHead][1] + move_y == locationSnake[i][1] && 
            pointerHead != i) {
            return true;
        }
    }
    return false;
}

void Snake::MoveSnake(int move_x, int move_y) {
    if (CheckSnakeAndPositionApple(move_x, move_y)) {
        ++point;
        ++rightBorderSnake;
        flagPointerHead = true;
        
        if (point % 100 == 0) {
           IncreaceSpaceArray();
        }


        locationSnake[point][0] = positionApple_x;
        locationSnake[point][1] = positionApple_y;
        
        PrintDataScore();
        PrintDataForDisplay(locationSnake[point][0], locationSnake[point][1], headSnake);
        PrintDataForDisplay(locationSnake[pointerHead][0], locationSnake[pointerHead][1], bodySnake);

        pointerSecondHead = pointerHead;
        pointerHead = point;

        CreateApple();
    }
    else {
        if (Death(move_x, move_y)) {
            lifeSnake = 0;
        }
        PrintDataForDisplay(locationSnake[pointerTail][0], locationSnake[pointerTail][1], ' ');

        locationSnake[pointerTail][0] = locationSnake[pointerHead][0] + move_x;
        locationSnake[pointerTail][1] = locationSnake[pointerHead][1] + move_y;

        PrintDataForDisplay(locationSnake[pointerHead][0], locationSnake[pointerHead][1], bodySnake);
        
        if (flagPointerHead) {
            pointerSecondHead = pointerHead;
        }
        pointerHead = pointerTail;

        PrintDataForDisplay(locationSnake[pointerHead][0], locationSnake[pointerHead][1], headSnake);

        if (pointerTail == 0) {
            pointerTail = point;
        }
        else {
            pointerTail--;
        }

    }
}

void Snake::CreateApple(){
    srand(time(0));

    int row = rand() % BoardHeight, col = rand() % BoardWidth;

    bool flag = false;

    while ((row >= BoardHeight - 1 || col >= BoardWidth - 1 || col == 0 || row == 0) && !flag) {
        for (int i = 0; i < rightBorderSnake; i++) {
            if (locationSnake[i][0] == row && locationSnake[i][1] == col ||
                col == 0 || row == 0 || col >= BoardWidth - 1 || row >= BoardHeight - 1) {
                
                
                row = 2 + rand() % (BoardHeight - 3);
                col = 2 + rand() % (BoardWidth - 3);
            }
        }



        flag = !flag;
    }
    flag = !flag;

    positionApple_x = col;
    positionApple_y = row;

    PrintDataForDisplay(positionApple_x, positionApple_y, '$');
    
    board[positionApple_y][positionApple_x] = '$';

}

bool Snake::CheckSnakeAndPositionApple(int move_x, int move_y){
    if (positionApple_x == locationSnake[pointerHead][0] + move_x && positionApple_y == locationSnake[pointerHead][1] + move_y) {
        return 1;
    }
    return 0;
}

bool Snake::Death(int move_x, int move_y){
    for (int i = 0; i < rightBorderSnake; i++) {
        if (locationSnake[pointerHead][0] + move_x == locationSnake[i][0] &&
            locationSnake[pointerHead][1] + move_y == locationSnake[i][1] && 
            pointerHead != i) {
            return true;
        }
    }
    return false;
}

void Snake::PrintDataForDisplay(int move_x, int move_y, char simbol){
    cursorPos.X = move_x;
    cursorPos.Y = move_y;

    SetConsoleCursorPosition(hConsole, cursorPos);
    fflush(stdout);

    std::cout << simbol;
}

void Snake::PrintDataScore() {
    cursorPos.X = 150;
    cursorPos.Y = 0;

    SetConsoleCursorPosition(hConsole, cursorPos);
    fflush(stdout);

    std::cout << "Ñ÷¸ò: " << point;
}

void Snake::IncreaceSpaceArray() {
    int** array = new int* [point + 100];

    for (int i = 0; i < point + 100; ++i) {
        array[i] = new int[2];
    }

    for (int i = 0; i < point; ++i) {
        array[i][0] = locationSnake[i][0];
        array[i][1] = locationSnake[i][1];
    }

    for (int i = 0; i < point; ++i) {
        delete[] locationSnake[i];
    }

    delete[] locationSnake;


    locationSnake = array;
}


void Snake::GameScore() {
    system("cls");
    std::cout << "Ñïàñèáî çà âàøó èãðó, âû íàáðàëè:" << point << std::endl;

    _getch();
}


MyltiplaySnake::MyltiplaySnake(int countConnections, bool SendServerOrClient){
    this->SendServerOrClient = SendServerOrClient;
    this->countConnections = countConnections;
    //positionsSnake = new int** [countConnections];//ïåðåïèñàòü èíèöèàëèçàöèþ ïîçèöèè èãðîêà
    

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD bufferSize = { BoardWidth + 100, BoardHeight+1};
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, BoardWidth + 99, BoardHeight };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

}

void MyltiplaySnake::AlgorithmInitPositionsSnakeServer(std::vector<std::vector<int>>& arrayPlace) {
    BinarySearch(arrayPlace, BoardWidth, BoardHeight);
}

void MyltiplaySnake::BinarySearch(std::vector<std::vector<int>>& arrayPlace, int Width, int Height) const {
    arrayPlace.resize(countConnections);
    int countPlaceRow = 1, countPlaceCol = 1;
    int iterRow = 0, iterCol = 0;
    int countPlace = 1;
    int iterator = 0;

    while (countPlace < countConnections) {
        countPlaceRow *= 2;
        countPlace *= 2;
        if (countPlace >= countConnections) {
            break;
        }
        countPlaceCol *= 2;
        countPlace *= 2;
    }

    for (int i = (Height / countPlaceCol) / 2; iterCol < countPlaceCol; i = ((Height / countPlaceCol) / 2) * (iterCol * 2 + 1)) {
        for (int j = (Width / countPlaceRow) / 2; iterRow < countPlaceRow; j = ((Width / countPlaceRow) / 2) * (iterRow * 2 + 1)) {
            arrayPlace[iterator].push_back(j);
            arrayPlace[iterator].push_back(i);
            ++iterator;
            ++iterRow;
        }
        ++iterCol;
        iterRow = 0;
    }

}

void MyltiplaySnake::InitData(std::vector<std::vector<int>>& allPositionsSnake, std::vector<std::vector<char>>& fashonSnake, std::vector<std::string>& namePlayers, unsigned int myPosition, unsigned countUsers){
    countConnections = countUsers;

    sizeSnakes = new unsigned int[countConnections];
    positionApple.resize(countConnections);
    pointerHeadSecondHeadAndTail = new int* [countConnections];
    pointScore = new unsigned int[countConnections];

    this->myPosition = myPosition;

    move[0] = (char)myPosition;

    positionsSnake.resize(countConnections);
    for (int i = 0; i < countConnections; i++) {
        positionsSnake[i].resize(100);
        for (int j = 0; j < 100; j++) {
            positionsSnake[i][j].resize(2);
        }
    }

    for (int i = 0; i < countConnections; ++i) {
        positionApple[i].resize(2);
    }
    
    
    for (int i = 0; i < countConnections; i++) {
        positionsSnake[i][0][1] = allPositionsSnake[i][0];
        positionsSnake[i][0][0] = allPositionsSnake[i][1];
    }


    nameUser.resize(countUsers);
    for (int i = 0; i < countConnections; i++){
        nameUser[i] = namePlayers[i];
    }

    this->fashonSnake.resize(countUsers);
    for (int i = 0; i < countConnections; i++) {
        this->fashonSnake[i].resize(2);
        for (int j = 0; j < 2; j++) {
            this->fashonSnake[i][j] = fashonSnake[i][j];
        }
    }

    for (int i = 0; i < countConnections; i++) {
        sizeSnakes[i] = 1;
    }

    for (int i = 0; i < countConnections; i++) {
        pointerHeadSecondHeadAndTail[i] = new int[3];
        pointerHeadSecondHeadAndTail[i][0] = 0;
        pointerHeadSecondHeadAndTail[i][1] = 0;
        pointerHeadSecondHeadAndTail[i][2] = 0;
    }


    for (int i = 0; i < countConnections; i++) {
        pointScore[i] = 0;
    }

}

void MyltiplaySnake::InitOtherDataServer(SOCKET (&connection)[60]) {
    if (connection != NULL) {
        for (int i = 0; connection[i] != 0; i++) {
            this->connection = connection;
        }
    }
}
void MyltiplaySnake::InitOtherDataClient(SOCKET& socket) {
    if (socket != NULL) {
        this->server = socket;
    }
}

void MyltiplaySnake::CreateAppleFullUsers(std::vector<std::vector<int>>& positionApple){
    srand(time(0));

    for (int i = 0; i < countConnections;) {
        int row = 2 + rand() % (BoardHeight - 3);
        int col = 2 + rand() % (BoardWidth - 3);

        if (row != BoardHeight && col != BoardWidth && row != 0 && col != 0 && ChechPositionApple(row, col)) {
            positionApple[i][0] = row;
            positionApple[i][1] = col;
            i++;
            countApple++;
        }
    }

}

void MyltiplaySnake::createAppleForOneUser(){
    int row, col;
    do {
        row = 2 + rand() % (BoardHeight - 3);
        col = 2 + rand() % (BoardWidth - 3);
    } while (!ChechPositionApple(row, col));

    positionApple[indexEatApple][0] = row;
    positionApple[indexEatApple][1] = col;

    move[0] = char(999);
    move[1] = (char)indexEatApple;
    move[2] = (char)row;
    move[3] = (char)col;
    You = true;

    countApple = countConnections;
    board[col][row] = '$';
    PrintDataForDisplay(col, row, '$');
}

void MyltiplaySnake::InitApple(std::vector<std::vector<int>>& positionApple){
    for (int i = 0; i < countConnections; i++) {
        this->positionApple[i][0] = positionApple[i][0];
        this->positionApple[i][1] = positionApple[i][1];
    }
    countApple = countConnections;
}

bool MyltiplaySnake::ChechPositionApple(int row, int col){
    for (int i = 0; i < countConnections; i++) {
        for (int j = 0; j < sizeSnakes[i]; j++) {
            if (positionsSnake[i][j][0] == col && positionsSnake[i][j][1] == row) {
                return false;
            }
        }
    }
    return true;
}

void MyltiplaySnake::StartGame() {
    system("cls");
    InitBoard();
    PrintBoard();
    PlayLoop();
    
}

void MyltiplaySnake::PrintBoard(){
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD cursorPos;
    cursorPos.X = BoardWidth + 50;
    cursorPos.Y = 0;

    SetConsoleCursorPosition(hConsole, cursorPos);
    fflush(stdout);

    std::cout << "Èãðîêè: \n";

    PrintDataScore();

}

void MyltiplaySnake::InitBoard(){
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            if (i == 0 || j == 0 || i == BoardHeight - 1 || j == BoardWidth - 1) {
                board[i][j] = '#';
            }
            else {
                board[i][j] = ' ';
            }
        }
    }

    for (int i = 0; i < countConnections; i++) {
        for (int j = 0; j < sizeSnakes[i]; j++) {
            board[positionsSnake[i][j][0]][positionsSnake[i][j][1]] = fashonSnake[i][0];
        }
    }

    for (int i = 0; i < countApple; i++) {
        board[positionApple[i][0]][positionApple[i][1]] = '$';
    }


}

void MyltiplaySnake::PlayLoop(){

    char move;
    do
    {
        move = _getch();
        switch (move)
        {
        case 'w':
            if (MoveIsLegal(0, -1) && flagLife) {
                this->moveThis = 'w';
                MoveSnake(0, -1, myPosition, 0);
                Board();
                break;
            }

            break;
        case 'a':
            if (MoveIsLegal(-1, 0) && flagLife) {
                this->moveThis = 'a';
                MoveSnake(-1, 0, myPosition, 0);
                Board();
                break;
            }
            break;
        case 's':
            if (MoveIsLegal(0, 1) && flagLife) {
                this->moveThis = 's';
                MoveSnake(0, 1, myPosition, 0);
                Board();
                break;
            }
            break;
        case 'd':
            if (MoveIsLegal(1, 0) && flagLife) {
                this->moveThis = 'd';
                MoveSnake(1, 0, myPosition, 0);
                Board();
                break;
            }
            break;
        default:
            break;
        }
    } while (flagLife);

}

bool MyltiplaySnake::MoveIsLegal(int x, int y) {
    if (myPosition > countConnections || myPosition < 0) {
        return false;
    }
    int temp_x = positionsSnake[myPosition][pointerHeadSecondHeadAndTail[myPosition][0]][1], temp_y = positionsSnake[myPosition][pointerHeadSecondHeadAndTail[myPosition][0]][0];
    if (flagPointerHead) {
        if (temp_y + y != BoardHeight - 1 && temp_y + y != 0 && temp_x + x != 0 &&
            temp_x + x != BoardWidth - 1 && (temp_x + x != positionsSnake[myPosition][pointerHeadSecondHeadAndTail[myPosition][1]][1] ||
                temp_y + y != positionsSnake[myPosition][pointerHeadSecondHeadAndTail[myPosition][1]][0])) {
            return true;
        }
        return false;
    }
    if (temp_y + y != BoardHeight - 1 && temp_y + y != 0 && temp_x + x != 0 &&
        temp_x + x != BoardWidth - 1) {
        return true;
    }
    return false;
}

void MyltiplaySnake::MoveSnake(int x, int y, int position, bool otherUsers) {
    int temp_x = positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][1] + x, temp_y = positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][0] + y;
    if (CheckSnakeAndPositionApple(temp_x, temp_y)) {
        if (!otherUsers) {
            createAppleForOneUser();

            GenerateMessage();
            SendServerOrClient ? SendMessageServer() : SendMessageClient();

        }
        else {
            positionApple[indexEatApple][1] = AppleEat_x;
            positionApple[indexEatApple][0] = AppleEat_y;
        }

        
        pointScore[position] += 1;
        sizeSnakes[position] += 1;
        flagPointerHead = true;

        if (sizeSnakes[position] % 100 == 0) {
            IncreaceSpaceArray(position);
        }

        positionsSnake[position][pointScore[position]][0] = temp_y;
        positionsSnake[position][pointScore[position]][1] = temp_x;

        PrintDataForDisplay(temp_x, temp_y, fashonSnake[position][0]);
        PrintDataForDisplay(positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][1], positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][0], fashonSnake[position][1]);

        pointerHeadSecondHeadAndTail[position][1] = pointerHeadSecondHeadAndTail[position][0];
        pointerHeadSecondHeadAndTail[position][0] = pointScore[position];

        PrintDataScore();
        EatApple = false;
    }
    else {
        if (!otherUsers) {
            GenerateMessage();
            SendServerOrClient ? SendMessageServer() : SendMessageClient();
        }

        if (Death(temp_x, temp_y)) {
            if (CountDead() || position == myPosition) {
                system("cls");
                flagLife = false;
                return;
            }
            DeleteSnake(position);

        }
        PrintDataForDisplay(positionsSnake[position][pointerHeadSecondHeadAndTail[position][2]][1], positionsSnake[position][pointerHeadSecondHeadAndTail[position][2]][0], ' ');

        positionsSnake[position][pointerHeadSecondHeadAndTail[position][2]][1] = positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][1] + x;
        positionsSnake[position][pointerHeadSecondHeadAndTail[position][2]][0] = positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][0] + y;

        PrintDataForDisplay(positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][1], positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][0], fashonSnake[position][1]);

        if (flagPointerHead) {
            pointerHeadSecondHeadAndTail[position][1] = pointerHeadSecondHeadAndTail[position][0];
        }
        pointerHeadSecondHeadAndTail[position][0] = pointerHeadSecondHeadAndTail[position][2];

        PrintDataForDisplay(positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][1], positionsSnake[position][pointerHeadSecondHeadAndTail[position][0]][0], fashonSnake[position][0]);

        if (pointerHeadSecondHeadAndTail[position][2] == 0) {
            pointerHeadSecondHeadAndTail[position][2] = pointScore[position];
        }
        else {
            pointerHeadSecondHeadAndTail[position][2]--;
        }
    }
}

bool MyltiplaySnake::CheckSnakeAndPositionApple(int x, int y) {
    for (int i = 0; i < countConnections; i++) {
        if (x == positionApple[i][1] && y == positionApple[i][0]) {
            indexEatApple = i;
            return true;
        }
    }
    return false;
}

void MyltiplaySnake::IncreaceSpaceArray(int position){
    /*int oldSize = sizeSnakes[position];
    int newCapacity = oldSize + 100;


    int** temp = new int* [oldSize];
    for (int i = 0; i < oldSize; ++i) {
        temp[i] = new int[2];
        temp[i][0] = positionsSnake[position][i][0];
        temp[i][1] = positionsSnake[position][i][1];
    }


    for (int i = 0; i < oldSize; ++i) {
        delete[] positionsSnake[position][i];
    }
    delete[] positionsSnake[position];


    positionsSnake[position] = new int* [newCapacity];
    for (int i = 0; i < newCapacity; ++i) {
        positionsSnake[position][i] = new int[2];
    }


    for (int i = 0; i < oldSize; ++i) {
        positionsSnake[position][i][0] = temp[i][0];
        positionsSnake[position][i][1] = temp[i][1];
        delete[] temp[i];
    }
    delete[] temp;*/
}

void MyltiplaySnake::PrintDataForDisplay(int x, int y, char simbol) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD cursorPos;
    cursorPos.X = x;
    cursorPos.Y = y;

    SetConsoleCursorPosition(hConsole, cursorPos);
    fflush(stdout);

    if (!SetConsoleCursorPosition(hConsole, cursorPos)) {
        
        DWORD err = GetLastError();
        std::cerr << "Îøèáêà ïîçèöèîíèðîâàíèÿ: " << err << std::endl;
    }
    else {
        std::cout << simbol;
    }
}

void MyltiplaySnake::Board() {

}

void MyltiplaySnake::PrintDataScore() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPos;

    cursorPos.X = 150;
    cursorPos.Y = 1;

    for (int i = 0; i < countConnections; i++) {
        SetConsoleCursorPosition(hConsole, cursorPos);
        fflush(stdout);

        if (!SetConsoleCursorPosition(hConsole, cursorPos)) {

            DWORD err = GetLastError();
            std::cerr << "Îøèáêà ïîçèöèîíèðîâàíèÿ: " << err << std::endl;
        }
        else {
            std::cout << nameUser[i][0] << ": " << sizeSnakes[i] << std::flush;
            cursorPos.X = 150;
            cursorPos.Y = cursorPos.Y + 1;
        }
    }
}

bool MyltiplaySnake::Death(int x, int y) {
    //for (int i = 0; i < countConnections; i++) {
    //    for (int j = 0; j < sizeSnakes[i]; j++) {//ïåðåïèñàòü ôóíêöèþ ñìåðòè
    //        if (x == positionsSnake[i][j][1] && y == positionsSnake[i][j][0]) {
    //            return true;
    //        }
    //    }
    //}
    return false;
}

bool MyltiplaySnake::CountDead(){
    if (countDeathSnake == countConnections) {
        return true;
    }
    return false;
}

void MyltiplaySnake::DeleteSnake(int position){
    for (int i = 0; i < sizeSnakes[position]; i++) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cursorPos;

        cursorPos.X = positionsSnake[position][i][1];
        cursorPos.Y = positionsSnake[position][i][0];
        
        std::cout << ' ';
    }
}

MyltiplaySnake::~MyltiplaySnake(){
    
}

void MyltiplaySnake::SendMessageServer(){
    for (int i = 0; i < countConnections - 1; i++) {
        send(connection[i], move, 256, NULL);
    }
}

void MyltiplaySnake::SendMessageClient(){
    send(server, move, 256, NULL);
}

void MyltiplaySnake::RecvMessage(char* data){
    int position;
    char move;

    ReadMessage(data, position, move);

    switch (move)
    {
    case 'w':
        MoveSnake(0, -1, position, 1);
        break;
    case 'a':
        MoveSnake(-1, 0, position, 1);
        break;
    case 's':
        MoveSnake(0, 1, position, 1);
        break;
    case 'd':
        MoveSnake(1, 0, position, 1);
        break;
    default:
        break;
    }
}

void MyltiplaySnake::GenerateMessage() {
    if (You) {
        You = false;
        move[4] = (char)(myPosition+1);
        move[5] = moveThis;
    }
    else {
        move[0] = (char)(myPosition+1);
        move[1] = moveThis;
    }
}

void MyltiplaySnake::ReadMessage(char* message, int& position, char& move) {
    if (message[0] == char(999)) {
        indexEatApple = (int)message[1];
        AppleEat_y = (int)message[2];
        AppleEat_x = (int)message[3];
        PrintDataForDisplay(AppleEat_x, AppleEat_y, '$');

        position = (int)message[4];
        position--;
        move = message[5];
    }
    else {
        position = (int)message[0];
        position--;
        move = message[1];
    }
}
