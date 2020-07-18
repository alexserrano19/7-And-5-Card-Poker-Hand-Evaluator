#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "dealer.h"

class UserInterface
{
public:
    UserInterface();
    char getFirstSelection() const;
    char getSecondSelection() const;
    void setSecondSelection(char s);
    void printGameBanner() const;
    void printHandBanner() const;
    void printByeMessage() const;
    void selectFirstMenuOption();
    void selectSecondMenuOption(bool sevenCardGame);
    int numberPlayersPerGame(bool sevenCardGame, bool continuousLoopOn, int playersPerContinuousLoop);
    void processPlayerHand(const Card arr[], const Player playerArr[], int index, bool setStats);
    void processStatistics(char menuSelection);
    void winningHandOutput(std::vector<int> winners) const;
    static const int HIGH_ACE_VALUE;

private:
    unsigned long long int statsArray[10];
    char firstSelection;
    char secondSelection;

    int selectPlayers5game(std::string question);
    int selectPlayers7game(std::string question);
    void processCardInfo(const Card arr[], const Player playerArr[], int start, int end, int index, bool sevenCardGame);
    void dynamicallyGrowBorder(std::vector<int> v) const;
};

#endif