#pragma once
#include <iostream>
#include <vector>

class Message{
public:
    Message(unsigned int countConnection);
    Message();

    void DefoultDataCreate(std::vector<std::vector<int>>& Position, std::vector<std::vector<char>>& fashonSnake, std::vector<std::string>& name, char*& result, unsigned int youPosition, unsigned int countPlayers = 0);
    void DefoultDataDecode(std::vector<std::vector<int>>& Position, std::vector<std::vector<char>>& fashonSnake, std::vector<std::string>& name, unsigned int& myPosition, unsigned int& countPlayers, char*& source);

    void DefoultDataAppleConvert(std::vector<std::vector<int>>& Position, char* result);
    void DefoultDataAppleDecode(std::vector<std::vector<int>>& Position, char* source);

    void SendMove(char identefi, char move, char*& result);
    void RecvMove(char identefi, char move, char*& result);

private:
    void ConvertPositionToChar(std::vector<std::vector<int>>& position, char* result);
    void DecodeCharToPosition(std::vector<std::vector<int>>& position, char* source);

    //void ConvertPositionToChar(std::vector<std::vector<int>>& position, char* result);
    //void DecodeCharToPosition(std::vector<std::vector<int>>& position, char* source);

    void ConvertOtherDataIntToChar(int source, char* result);
    void ConverOtherDataCharToInt(int result, char* source);

    void WriteName(std::vector<std::string>& name, char* result);
    void ConvertNameBack(std::vector<std::string>& name, char* source);

    void ConvertFashonSnake(std::vector<std::vector<char>>& fashon, char* result);
    void ConvertFashonBack(std::vector<std::vector<char>>& fashon, char* source);

    void SearchCountUsers(char* message, unsigned int& countPlayers);
    void SearchMyPosition(char* source, unsigned int& position);

    static int countPlayers;

    int placeWrite = 0;
    int placeDecode = 0;
};

  