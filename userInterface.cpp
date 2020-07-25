#include <iostream>
#include <iomanip>
#include <vector>
#include "userInterface.h"

const int UserInterface::HIGH_ACE_VALUE = 14;

UserInterface::UserInterface()
{
    sevenCardGame = false;
    // Initalizes statsArray to values of 0
    for (unsigned long long int& element : statsArray)
        element = 0;
}

char UserInterface::getFirstSelection() const
{
    return firstSelection;
}

char UserInterface::getSecondSelection() const
{
    return secondSelection;
}

bool UserInterface::getSevenCardGameBool() const
{
    return sevenCardGame;
}

void UserInterface::setSecondSelection(char s)
{
    secondSelection = s;
}

void UserInterface::setSevenCardGameBool(bool s)
{
    sevenCardGame = s;
}

void UserInterface::printGameBanner() const
{
    std::cout << "\n\nMoNeYmOnEy======--------======mOnEyMoNeY\n";
    std::cout << "     **** Poker Hand Evaluator **** \n";
    std::cout << "MoNeYmOnEy======--------======mOnEyMoNeY\n\n";
}

void UserInterface::printHandBanner() const
{
    std::cout << "\n=================================================================\n";
    std::cout << "  THE FOLLOWING LIST SHOWS ALL PLAYER HANDS AND THEIR STRENGTH:";
    std::cout << "\n=================================================================\n";
}

void UserInterface::printByeMessage() const
{
    std::cout << "\n  <<****************************>>\n";
    std::cout << "<<ThAnKyOutHaNkYoUThAnKyOutHaNkYoU>>\n";
    std::cout << "     <** THANKS FOR PLAYING **>\n";
    std::cout << "<<ThAnKyOutHaNkYoUThAnKyOutHaNkYoU>>\n";
    std::cout << "  <<****************************>>\n\n";
}

// Returns and error checks first menu option
void UserInterface::selectFirstMenuOption()
{
    std::cout << "\nMeNuMeNu%%%===========%%%MeNuMeNu\n";
    std::cout << "    5 - Play 5 card evaluator\n";
    std::cout << "    7 - Play 7 card evaluator\n";
    std::cout << "    Q - Quit\n";
    std::cout << "MeNuMeNu%%%===========%%%MeNuMeNu\n" << ">>> ";;
    std::cin >> firstSelection;

    firstSelection = toupper(firstSelection);

    // Forces a choice of 5,7 or Q
    while (firstSelection != '5' && firstSelection != '7' && firstSelection != 'Q' )
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "\nTRY AGAIN!\n" << ">>> ";
        std::cin >> firstSelection;
        firstSelection = toupper(firstSelection);
    }
}

// Returns and error checks second menu option
void UserInterface::selectSecondMenuOption(bool sevenCardGame)
{
    std::cout << "\nMeNuMeNu%%%========%%%MeNuMeNu\n";
    if (sevenCardGame)
        std::cout << "     1 - Deal SEVEN cards\n";
    else
        std::cout << "     1 - Deal FIVE cards\n";
    std::cout << "     2 - Speed dealing\n";
    std::cout << "     3 - Statistics\n";
    std::cout << "     4 - Reset Statistics\n";
    std::cout << "     Q - Quit\n";
    std::cout << "MeNuMeNu%%%========%%%MeNuMeNu\n" << ">>> ";;
    std::cin >> secondSelection;

    secondSelection = toupper(secondSelection);

    // Forces a choice of 1, 2, 3, 4, or Q
    while (secondSelection != '1' && secondSelection != '2' && secondSelection != '3' 
            && secondSelection != '4' && secondSelection != 'Q' )
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "\nTRY AGAIN!\n" << ">>> ";
        std::cin >> secondSelection;
        secondSelection = toupper(secondSelection);
    }
}

// Returns number of players depedngind on game time and menu option
int UserInterface::numberPlayersPerGame(bool sevenCardGame, bool continuousLoopOn, 
                                        int playersPerContinuousLoop, bool speedDealingOn)
{
    if (sevenCardGame && !continuousLoopOn)
    {
        if (speedDealingOn)
            return selectPlayers7game("How many players will you like to deal cards to in each game?");
        else
            return selectPlayers7game("How many players will you like to deal cards to?");
    }
    else if (!sevenCardGame && !continuousLoopOn)
    {
        if (speedDealingOn)
            return selectPlayers5game("How many players will you like to deal cards to in each game?");
        else
            return selectPlayers5game("How many players will you like to deal cards to?");
    }
    else
        return playersPerContinuousLoop;
}

// In seven player game, returns and error checks the amount of opponents that will be versed
int UserInterface::selectPlayers7game(std::string question)
{
    int players = 0;

    do 
    {
        std::cout << "\n" << question << "\n" << ">>> ";
        std::cin >> players;

        if (players > 23 || players < 1)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "\nINVALID AMOUNT! MAX: 23, MIN: 1";
        }

    } while (players > 23 || players < 1);

    return players;
}

// In five player game, returns and error checks the amount of opponents that will be versed
int UserInterface::selectPlayers5game(std::string question)
{
    int players = 0;

    do 
    {
        std::cout << "\n" << question << "\n" << ">>> ";
        std::cin >> players;

        if (players > 10 || players < 1)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "\nINVALID AMOUNT! MAX: 10, MIN: 1";
        }

    } while (players > 10 || players < 1);

    return players;
}

// Prints hand output
void UserInterface::processPlayerHand(const Card arr[], const Player playerArr[], int index, bool sevenCardGame)
{
    std::cout << "\n=-- #" << index+1 << " --=";
    if (sevenCardGame)
    {
        std::cout << "\nPLAYER HAND:"; 
        processCardInfo(arr, playerArr, 5, 7, index, false);
        std::cout << "\nCOMMUNITY CARDS:\n" << " ";
    }
    else
        std::cout << "\nFIVE CARD HAND:"; 

    processCardInfo(arr, playerArr, 0, 5, index, true);
}

// Prints and records strength of every hand
void UserInterface::processCardInfo(const Card arr[], const Player playerArr[], int start, int end, int index, bool setStats)
{
    // Outputs each card
    for (int i = start; i < end; i++)
    {   
        std::string handInfo = " * ";
        switch(arr[i].number)
        {
            case 11: handInfo += "Jack"; break;
            case 12: handInfo += "Queen"; break;
            case 13: handInfo += "King"; break;
            case 14: handInfo += "Ace"; break;
            default: handInfo += std::to_string(arr[i].number);
        }
        handInfo += " of ";
        switch(arr[i].suit)
        {
            case 1: handInfo += "Spades"; break;
            case 2: handInfo += "Cloves"; break;
            case 3: handInfo += "Hearts"; break;
            case 4: handInfo +=  "Diamonds"; break;
        }
        std::cout << handInfo;
    }

    // Sets the statistics of each hand strength and ouptuts hand strength
    if (setStats)
    {
        switch(playerArr[index].points)
        {
            case 210: std::cout << "\nHand strength: ROYAL FLUSH\n"; statsArray[0] += 1; break;
            case 200: std::cout << "\nHand strength: STRAIGHT FLUSH\n"; statsArray[1] += 1; break;
            case 190: std::cout << "\nHand strength: QUADS\n"; statsArray[2] += 1; break;
            case 180: std::cout << "\nHand strength: FULL HOUSE\n"; statsArray[3] += 1; break;
            case 170: std::cout << "\nHand strength: FLUSH\n"; statsArray[4] += 1; break;
            case 160: std::cout << "\nHand strength: STRAIGHT\n"; statsArray[5] += 1; break;
            case 150: std::cout << "\nHand strength: TRIPS\n"; statsArray[6] += 1; break;
            case 140: std::cout << "\nHand strength: TWO PAIR\n"; statsArray[7] += 1; break;
            case 130: std::cout << "\nHand strength: PAIR\n"; statsArray[8] += 1; break;
            default: std::cout << "\nHand strength: HIGH CARD\n"; statsArray[9] += 1; break;
        }
    }
}

// Handles everything involving statistics
void UserInterface::processStatistics(char menuSelection)
{
    if (menuSelection == '3')
    {
        long double handsDealt = 0;

        // Counts the total number of poker hands dealt
        for (int i = 0; i < 10; i++)
            handsDealt += (long double)statsArray[i];
        
        if (handsDealt > 0)
        {
            std::cout << std::fixed << std::showpoint << std::setprecision(6);
            std::cout << "\n=====-------------------------------=====\n";
            std::cout << " EXPERIMENTAL PROBABILITY OF POKER HANDS\n";
            std::cout << "=====-------------------------------=====\n";
            std::cout << "      Royal flush:    " << std::setw(10) << (statsArray[0]/handsDealt)*100.0 << "%\n";
            std::cout << "      Straight flush: " << std::setw(10) << (statsArray[1]/handsDealt)*100.0 << "%\n";
            std::cout << "      Quads:          " << std::setw(10) << (statsArray[2]/handsDealt)*100.0 << "%\n";
            std::cout << "      Full house:     " << std::setw(10) << (statsArray[3]/handsDealt)*100.0 << "%\n";
            std::cout << "      Flush:          " << std::setw(10) << (statsArray[4]/handsDealt)*100.0 << "%\n";
            std::cout << "      Straight:       " << std::setw(10) << (statsArray[5]/handsDealt)*100.0 << "%\n";
            std::cout << "      Trips:          " << std::setw(10) << (statsArray[6]/handsDealt)*100.0 << "%\n";
            std::cout << "      Two pair:       " << std::setw(10) << (statsArray[7]/handsDealt)*100.0 << "%\n";
            std::cout << "      Pair:           " << std::setw(10) << (statsArray[8]/handsDealt)*100.0 << "%\n";
            std::cout << "      High card:      " << std::setw(10) << (statsArray[9]/handsDealt)*100.0 << "%\n";
            std::cout << "=========================================\n";
            std::cout << "Total number of hands: " << (unsigned long long int)handsDealt << std::endl;
        }
        else
            std::cout << "\n** NO HANDS HAVE BEEN DEALT **\n";
    }
    else if (menuSelection == '4')
    {
        // Resets the statsArray to all values of 0
        for (unsigned long long int& element : statsArray)
            element = 0;
        
        std::cout << "\n** Statistics have been RESET **\n";
    }
}


// Outputs winning hand(s) with fancy borders
void UserInterface::winningHandOutput(std::vector<int> winners) const
{
    if ((int)winners.size() > 1)
    {
        std::cout << "\n    ===---------------------";
        dynamicallyGrowBorder(winners);
        std::cout << "------------------===\n";
        std::cout << "        ITS A TIE BETWEEN PLAYERS";
        for (int i = 0; i < (int)winners.size()-1; i++)
            std::cout << " #" << winners[i] << ",";
        std::cout << " and #" << winners[(int)winners.size()-1] << "!\n";
        std::cout << "    =====-------------------";
        dynamicallyGrowBorder(winners);
        std::cout << "----------------=====\n\n";
    }
    else
    {
        std::cout << "\n    WINNER===WINNER===WINNER===WINNER==";
        // Makes border 1 character longer if winner is double digit number
        if (winners[0] > 9)
            std::cout << "="; 
        std::cout << "=WINNER===WINNER===WINNER===WINNER\n";
        std::cout << "    ||                      *****"  << " PLAYER #" 
                            << winners[0] <<   " *****                      ||\n";
        std::cout << "    WINNER===WINNER===WINNER===WINNER==";
        if (winners[0] > 9)
            std::cout << "="; 
        std::cout << "=WINNER===WINNER===WINNER===WINNER\n\n";
    }
}

// Dynamically grows border when players tie
void UserInterface::dynamicallyGrowBorder(std::vector<int> v) const
{
    if ((v[0] <= 9 && v[1] >= 10) || (v[0] >= 10 && v[1] <= 9))
        std::cout << "-";
    if (v[0] >= 10 && v[1] >= 10)
        std::cout << "--";
    if ((int)v.size() > 2)
    {
        for (int i = 2; i < (int)v.size(); i++)
        {
            bool twoDigitNumber = false;

            if (v[i] >= 10)
                twoDigitNumber = true;

            if (!twoDigitNumber)
                std::cout << "----";
            else
                std::cout << "-----";
        }
    }
}