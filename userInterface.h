#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "dealer.h"

class UserInterface
{
public:
    UserInterface();
    char getFirstSelection() const;
    char getSecondSelection() const;
    bool getSevenCardGameBool() const;
    void setSecondSelection(char s);
    void setSevenCardGameBool(bool s);
    void printGameBanner() const;
    void printHandBanner() const;
    void printByeMessage() const;
    void selectFirstMenuOption();
    void selectSecondMenuOption(bool sevenCardGame);
    int numberPlayersPerGame(bool sevenCardGame, bool continuousLoopOn, 
                            int playersPerContinuousLoop, bool speedDealingOn = false);
    void processPlayerHand(const Card arr[], const Player playerArr[], int index, bool setStats);
    void processStatistics(char menuSelection);
    void winningHandOutput(std::vector<int> winners) const;
    static const int HIGH_ACE_VALUE;

private:
    unsigned long long int statsArray[10];
    char firstSelection, secondSelection;
    bool sevenCardGame;

    static int selectPlayers5game(std::string question);
    static int selectPlayers7game(std::string question);
    void processHandInfo(const Card arr[], const Player playerArr[], int start, int end, int index, bool sevenCardGame);
    static void dynamicallyGrowBorder(std::vector<int> v);
};

#endif