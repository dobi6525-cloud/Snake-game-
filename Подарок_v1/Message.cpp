#include "Message.h"

int Message::countPlayers = 0;

Message::Message(unsigned int countConnection){
    countPlayers = countConnection;
}

Message::Message(){}

void Message::DefoultDataCreate(std::vector<std::vector<int>>& Position, std::vector<std::vector<char>>& fashonSnake, std::vector<std::string>& name, char*& result, unsigned int youPosition, unsigned int countPlayers){
    char* resultData = new char[256];
    
    ConvertPositionToChar(Position, resultData);
    ConvertFashonSnake(fashonSnake, resultData);
    WriteName(name, resultData);
    
    
    ConvertOtherDataIntToChar(youPosition, resultData);
    resultData[placeWrite++] = char(3);

    ConvertOtherDataIntToChar(countPlayers, resultData);
    resultData[placeWrite++] = char(4);

    for(int i = 0; i < placeWrite; i++){
        result[i] = resultData[i];
    }

    delete[] resultData;
}

void Message::DefoultDataDecode(std::vector<std::vector<int>>& Position, std::vector<std::vector<char>>& fashonSnake, std::vector<std::string>& name, unsigned int& myPosition, unsigned int& countPlayers, char*& source){
    SearchCountUsers(source, countPlayers);
    this->countPlayers = countPlayers;

    SearchMyPosition(source, myPosition);
    DecodeCharToPosition(Position, source);
    ConvertFashonBack(fashonSnake, source);
    ConvertNameBack(name, source);


}

void Message::DefoultDataAppleConvert(std::vector<std::vector<int>>& Position, char* result){
    placeWrite = 0;
    ConvertPositionToChar(Position, result);
}

void Message::DefoultDataAppleDecode(std::vector<std::vector<int>>& Position, char* source) {
    placeDecode = 0;
    DecodeCharToPosition(Position, source);
}

void Message::SendMove(char identefi, char move, char*& result){
    result[0] = identefi;
    result[1] = move;
}

void Message::RecvMove(char identefi, char move, char*& result){
    result[0] = identefi;
    result[1] = move;
}

//void Message::ConvertPositionToChar(std::vector<std::vector<int>> position, char* result){
//    for (int i = 0; i < countPlayers; i++) {
//        for (int j = 0; j < 2; j++) {
//            result[placeWrite++] = (char)position[i][j];
//        }
//    }
//    result[placeWrite++] = char(0);
//}

void Message::DecodeCharToPosition(std::vector<std::vector<int>>& position, char* source){
    for (int i = 0; i < countPlayers; i++) {
        for (int j = 0; j < 2 && source[placeDecode] != char(0); j++) {
            position[i][j] = (int)(source[placeDecode++]);
        }
    }

    placeDecode++;
}

void Message::ConvertPositionToChar(std::vector<std::vector<int>>& position, char* result){
    for (int i = 0; i < countPlayers; i++) {
        result[placeWrite++] = (char)position[i][0];
        result[placeWrite++] = (char)position[i][1];
    }

}

//void Message::DecodeCharToPosition(std::vector<std::vector<int>>& position, char* source){
//    for (int i = 0; i < countPlayers; i++) {
//        for (int j = 0; j < 2; j++) {
//            position[i][j] = (int)source[placeDecode++];
//        }
//    }
//}

void Message::ConvertOtherDataIntToChar(int source, char* result){
    result[placeWrite++] = (char)source;
}

void Message::ConverOtherDataCharToInt(int result, char* source){
    result = (int)source[placeDecode++];
}

void Message::WriteName(std::vector<std::string>& name, char* result) {
    for (int i = 0; i < countPlayers; i++) {
        short unsigned int len = name[i].length();

        if (len > 0) {
            std::memcpy(&result[placeWrite], name[i].data(), len);
            placeWrite += len + 1;
        }
        result[placeWrite++] = '\0';
    }

}

void Message::ConvertNameBack(std::vector<std::string>& name, char* source){
    name.clear();
    name.resize(countPlayers); 

    for (int i = 0; i < countPlayers; i++) {
        while (source[placeDecode] != '\0') {
            name[i] += source[placeDecode++];
        }

        placeDecode++;
    }
}

void Message::ConvertFashonSnake(std::vector<std::vector<char>>& fashon, char* result){
    for (int i = 0; i < countPlayers; i++) {
        for (int j = 0; j < 2; j++) {
            result[placeWrite++] = fashon[i][j];
        }
    }
    result[placeWrite++] = char(1);
}

void Message::ConvertFashonBack(std::vector<std::vector<char>>& fashon, char* source){
    for (int i = 0; i < countPlayers; i++) {
        for (int j = 0; j < 2; j++) {
            fashon[i][j] = source[placeDecode++];
        }
    }
}

void Message::SearchCountUsers(char* message, unsigned int& countPlayers){
    for (int i = 0; i < 256; i++) {
        if(message[i] == char(3)) {
            countPlayers = int(message[++i]);
            return;
        }
    }
}

void Message::SearchMyPosition(char* source, unsigned int& position){
    for (int i = 0; i < 256; i++) {
        if (source[i] == char(3)) {
            position = int(source[--i]);
            return;
        }
    }
}
