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
    void deallocate();
    int getLoopRequirement() const;
    int getPossibleWinnerIndex() const;
    int getCardIndex() const;
    Card* getSevenCardHand();
    Player* getPossibleWinnerArray() const;
    void setPlayers(int p);
    void generateShuffledDeck();
    void populateHand(bool sevenCardGame);
    void evaluatePlayerHands(bool sevenCardGame);
    std::vector<int> determineWinner();
    // Global variables in dealer class
    static const int TRUTH_VALUE;
    static const int HAND_SIZE;
    static const int LOW_ACE_VALUE;
    static const int HIGH_ACE_VALUE;
    
private:
    int players, loopRequirement, cardIndex, possibleWinnerIndex;
    Card deck[52], sevenCardHand[7], communityCards[5];
    Card* playerCards;
    Card* fiveCardHand;
    Player* possibleWinner;
    // Random number generation
    std::random_device random;

    // Used to determine hand strength
    int* highCard(const Card originalArr[]);
    int* pair(const Card originalArr[]);
    int* twoPair(const Card originalArr[]);
    int* trips(const Card originalArr[]);
    int* straight(const Card originalArr[]);
    int* flush(const Card originalArr[]);
    int* fullHouse(const Card originalArr[]);
    int* quads(const Card originalArr[]);
    int* straightflush(const Card originalArr[]);
    void sortCardNumber(Card arr[], int sizeOfArray);
    void changeCardValue(Card arr[], int valueCompared, int newValue);
    void setUpArrays(Card arr[], const Card originalArr[], int handScore[], int sizeHandScore);

    // Used to determine winner amongst all other player hands
    std::vector<int> multipleWinners(const Player arr[], const int sizeOfVector);
    void sortBy(std::string choice, Player arr[], int sizeOfArray);
    int counterFor(std::string choice, const Player arr[], int sizeOfArray);
    int compareKicker(std::string choice, const Player arr[], int sizeOfArray);
    void setStats(Player arr[], int index, int points, int highCard, int k1, int k2, int k3, int k4);
};

#endif