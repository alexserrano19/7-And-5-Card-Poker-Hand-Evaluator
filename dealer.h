#ifndef DEALER_H
#define DEALER_H

#include <random>

class Card
{
public:
    int number;
    int suit;
};

class Player
{
public:
    int playerNumber;
    int points;
    int highCard;
    int kicker1;
    int kicker2;
    int kicker3;
    int kicker4;
};

class Dealer
{
public:
    Dealer();
    void deallocateMemoryAndResetMembers();
    int getLoopRequirement() const;
    int getPossibleWinnerIndex() const;
    int getCardIndex() const;
    Card* getPrintableHand();
    Player* getPossibleWinnerArray() const;
    void setPlayers(int p);
    void generateShuffledDeck();
    void populateCardArrays(bool sevenCardGame);
    void evaluatePlayerHand(bool sevenCardGame);
    std::vector<int> determineWinner();
    // Global variables in dealer class
    static const int TRUTH_VALUE;
    static const int HAND_SIZE;
    static const int LOW_ACE_VALUE;
    static const int HIGH_ACE_VALUE;
    
private:
    int players, loopRequirement, cardIndex, possibleWinnerIndex;
    Card deck[52], sevenCardHand[7], communityCards[5], printable[7];
    Card* playerCards;
    Card* fiveCardHand;
    Player* possibleWinner;
    // Used for random number generation
    std::random_device rd;
    std::mt19937 randomNumberGenerator;

    // Used to determine hand strength 
    /* All of the following member functions are static for optimization, they are using 
    parameters rather than accesing private members for greater usability and readability */
    static int* highCard(const Card originalArr[]);
    static int* pair(const Card originalArr[]);
    static int* twoPair(const Card originalArr[]);
    static int* trips(const Card originalArr[]);
    static int* straight(const Card originalArr[]);
    static int* flush(const Card originalArr[]);
    static int* fullHouse(const Card originalArr[]);
    static int* quads(const Card originalArr[]);
    static int* straightflush(const Card originalArr[], int* flushPtr);
    static void sortCardNumber(Card arr[]);
    static void setUpArrays(Card arr[], const Card originalArr[], int handScore[], int sizeHandScore);

    // Used to determine winner amongst all other player hands
    static std::vector<int> multipleWinners(const Player arr[], const int sizeOfVector);
    static void sortBy(std::string choice, Player arr[], int sizeOfArray);
    static int counterFor(std::string choice, const Player arr[], int sizeOfArray);
    static int compareKicker(std::string choice, const Player arr[], int sizeOfArray);
    static void setStats(Player arr[], int index, int points, int highCard, int k1, int k2, int k3, int k4);
};

#endif