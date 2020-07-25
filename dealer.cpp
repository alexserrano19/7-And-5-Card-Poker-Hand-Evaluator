#include <iostream>
#include <random>
#include <algorithm>
#include "dealer.h"

const int Dealer::TRUTH_VALUE = 1;
const int Dealer::HAND_SIZE = 7;
const int Dealer::LOW_ACE_VALUE = 1;
const int Dealer::HIGH_ACE_VALUE = 14;

Dealer::Dealer() : cardIndex(0), players(0), possibleWinnerIndex(-1), randomNumberGenerator(rd())
{}

// Deallocates all memory and resets variables
// Deallocation upon game ending, not object destruction
void Dealer::deallocateMemoryAndResetMembers()
{
    // Resets values for new loop
    cardIndex = 0;
    players = 0;
    possibleWinnerIndex = -1;

    delete [] playerCards;
    playerCards = 0;

    delete [] fiveCardHand;
    fiveCardHand = 0;

    delete [] possibleWinner;
    possibleWinner = 0;
}

int Dealer::getLoopRequirement() const
{
    return loopRequirement;
}

int Dealer::getPossibleWinnerIndex() const
{
    return possibleWinnerIndex;
}

int Dealer::getCardIndex() const
{
    return cardIndex;
}

Card* Dealer::getPrintableHand()
{
    return printable;
}

Player* Dealer::getPossibleWinnerArray() const
{
    return possibleWinner;
}

void Dealer::setPlayers(int p)
{
    players = p;
    // Intializes dynamic arrays based on number of players
    playerCards = new Card[p*2]();
    fiveCardHand = new Card[p*5]();
    possibleWinner = new Player[p]();
}

// Generates and shuffles a deck based on numbers
void Dealer::generateShuffledDeck()
{   
    int deckIndexCounter = 0;
    // Generates 52 card deck with numbers and corresponding suits
    // Number range: 2-14, Suit range: 1-4
    for (int i = 1; i < 14; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Card temp = {i+1, j+1};
            deck[deckIndexCounter] = temp;
            deckIndexCounter++;
        }
    }

    // Shuffles the deck
    const int NUMBER_OF_SHUFFLES = 3;
    const int DECK_SIZE = 52;
    for (int i = 0; i < NUMBER_OF_SHUFFLES; i++)
    {
        for (int j = 0; j < DECK_SIZE; j++)
        {
            int randomIndex = randomNumberGenerator() % DECK_SIZE;
            Card hold = deck[j];
            deck[j] = deck[randomIndex];
            deck[randomIndex] = hold;
        }
    }
}

// Populates specific arrays based on game version
void Dealer::populateCardArrays(bool sevenCardGame)
{
    // 7/5 hand games use different arrays
    if (sevenCardGame)
    {
        for (int i = 0; i < HAND_SIZE-2; i++)
            communityCards[i] = deck[i];

        loopRequirement = players*2;
        for (int i = 0; i < loopRequirement; i++)
            playerCards[i] = deck[i+5];
    }
    else
    {
        loopRequirement = players*5;
        for (int i = 0; i < loopRequirement; i++)
            fiveCardHand[i] = deck[i];
    }
}

// Determines hand strength for each hand
void Dealer::evaluatePlayerHand(bool sevenCardGame)
{
    if (sevenCardGame)
    {
        // Copies the community cards into sevenCardHand array
        for (int i = 0; i < HAND_SIZE-2; i++)
            sevenCardHand[i] = communityCards[i];

        // Copies 2 cards of the playerCards array into sevenCardHand array
        for (int i = 5; i < HAND_SIZE; i++)
        {
            sevenCardHand[i] = playerCards[cardIndex];
            cardIndex++;
        }
    }
    else
    {
        // Copies 5 card into 7 card hand array
        for (int i = 0; i < HAND_SIZE-2; i++)
        {
            sevenCardHand[i] = fiveCardHand[cardIndex];
            cardIndex++;
        }
        // Last 2 card should be obsolete in 5 card game
        sevenCardHand[5] = {-5, -5};
        sevenCardHand[6] = {-10, -10};
    }

    // Copies to printable, shuffled version of array, before it is sorted
    for (int i = 0; i < HAND_SIZE; i++)
        printable[i] = sevenCardHand[i];

    // Sorts sevenCardHand since every hand strength function needs this to work properly
    sortCardNumber(sevenCardHand);

    // Used for possibleWinner array, to save all player hand strengths
    possibleWinnerIndex++;

    //////////////////////////////////////////////////////////////////////////////////////////////
    /* The following functions determine the hand strength, with values returned as pointers, the
    values in each index are specified at the end of every hand strength function definition */
    //////////////////////////////////////////////////////////////////////////////////////////////

    typedef int* PointerToInt;
    PointerToInt highCardPtr = 0, pairPtr = 0, twoPairPtr = 0, tripsPtr = 0, straightPtr = 0;
    PointerToInt flushPtr = 0, fullHousePtr = 0, quadsPtr = 0, straightFlushPtr = 0;

    // The flush function is used first since it is used in the straightflush function
    flushPtr = flush(sevenCardHand);
    straightFlushPtr = straightflush(sevenCardHand, flushPtr);
    if (straightFlushPtr[0] == TRUTH_VALUE && straightFlushPtr[1] == HIGH_ACE_VALUE)
    {
        setStats(possibleWinner, possibleWinnerIndex, 210, straightFlushPtr[1], 0, 0, 0, 0);
        return;
    }
    else if (straightFlushPtr[0] == TRUTH_VALUE)
    {
        setStats(possibleWinner, possibleWinnerIndex, 200, straightFlushPtr[1], 0, 0, 0, 0);
        return;
    }

    quadsPtr = quads(sevenCardHand);
    if (quadsPtr[0] == TRUTH_VALUE)
    {
        setStats(possibleWinner, possibleWinnerIndex, 190, quadsPtr[1], quadsPtr[2], 0, 0, 0);
        return;
    }

    fullHousePtr = fullHouse(sevenCardHand);
    if (fullHousePtr[0] == TRUTH_VALUE)
    {
        setStats(possibleWinner, possibleWinnerIndex, 180, fullHousePtr[1], fullHousePtr[2], 0, 0, 0);
        return;
    }

    if (flushPtr[0] == TRUTH_VALUE)
    {
        setStats(possibleWinner, possibleWinnerIndex, 170, flushPtr[1], flushPtr[2], flushPtr[3],
                flushPtr[4], flushPtr[5]);
        return;
    }

    straightPtr = straight(sevenCardHand);
    if (straightPtr[0] == TRUTH_VALUE)
    {
        setStats(possibleWinner, possibleWinnerIndex, 160, straightPtr[1], 0, 0, 0, 0);
        return;
    }

    tripsPtr = trips(sevenCardHand);
    if (tripsPtr[0] == TRUTH_VALUE)
    {
        setStats(possibleWinner, possibleWinnerIndex, 150, tripsPtr[1], tripsPtr[2], tripsPtr[3], 0, 0);
        return;
    }

    twoPairPtr = twoPair(sevenCardHand);
    if (twoPairPtr[0] == TRUTH_VALUE)
    {
        setStats(possibleWinner, possibleWinnerIndex, 140, twoPairPtr[1], twoPairPtr[2], twoPairPtr[3], 0, 0);
        return;
    }

    pairPtr = pair(sevenCardHand);
    if (pairPtr[0] == TRUTH_VALUE)
    {
        setStats(possibleWinner, possibleWinnerIndex, 130, pairPtr[1], pairPtr[2], pairPtr[3], pairPtr[4], 0);
        return;
    }
    else
    {
        highCardPtr = highCard(sevenCardHand);
        setStats(possibleWinner, possibleWinnerIndex, highCardPtr[0], highCardPtr[0], highCardPtr[1],
                highCardPtr[2], highCardPtr[3], highCardPtr[4]);
    }
}

// Sets the stats of the player hands into possibleWinner array
void Dealer::setStats(Player arr[], int index, int points, int highCard, int k1, int k2, int k3, int k4)
{
    arr[index].playerNumber = index+1;
    arr[index].points = points;
    arr[index].highCard = highCard;
    arr[index].kicker1 = k1;
    arr[index].kicker2 = k2; 
    arr[index].kicker3 = k3;
    arr[index].kicker4 = k4;
}

///////////////////////////////// HAND STRENGTH FUNCTIONS /////////////////////////////////
/* The following functions will return a pointer as an array that returns several values to 
determine serveral factors, which are mentioned specifically at the end of each function */

int* Dealer::highCard(const Card originalArr[])
{
    static int handScore[5];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 5);

    // Copies 5 highest cards into handScore array
    for (int i = 0; i < HAND_SIZE-2; i++)
        handScore[i] = arr[i].number;

    // handScore[0-4] = high card and kickers
    return handScore;
}

int* Dealer::pair(const Card originalArr[])
{
    static int handScore[5];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 5);

    // Finds is single pair is present
    for (int i = 0; i < HAND_SIZE-1; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            handScore[0] = TRUTH_VALUE;
            handScore[1] = arr[i].number;
            // Sets the kickers
            switch(i)
            {
                case 0:
                    handScore[2] = arr[2].number;
                    handScore[3] = arr[3].number;
                    handScore[4] = arr[4].number;
                    break;
                case 1:
                    handScore[2] = arr[0].number;
                    handScore[3] = arr[3].number;
                    handScore[4] = arr[4].number;
                    break;
                case 2:
                    handScore[2] = arr[0].number;
                    handScore[3] = arr[1].number;
                    handScore[4] = arr[4].number;
                    break;
                default:
                    handScore[2] = arr[0].number;
                    handScore[3] = arr[1].number;
                    handScore[4] = arr[2].number;
            }
            break;
        }
    }

    /* handScore[0] = wether or not there is a single pair (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of single pair, handScore[2-4] = all kickers */
    return handScore;
}

int* Dealer::twoPair(const Card originalArr[])
{
    static int handScore[4];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 4);

    // Finds if two pairs are present
    bool checkSecondPair = false;
    for (int i = 0; i < HAND_SIZE-1; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            if (!checkSecondPair)
            {
                handScore[1] = arr[i].number;
                checkSecondPair = true;
            }
            else
            {
                handScore[0] = TRUTH_VALUE;
                handScore[2] = arr[i].number;
                break;
            }
        }
    }

    // Sets kicker if two pairs are present
    if (handScore[0] == TRUTH_VALUE)
    {
        // Removes pairs to find kicker
        for (int i = 0; i < HAND_SIZE; i++)
            if (arr[i].number == handScore[1] || arr[i].number == handScore[2])
                arr[i].number = -1;
        sortCardNumber(arr);
        // Sets kicker
        handScore[3] = arr[0].number;
    }

    /* handScore[0] = wether or not there is a two pair (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = value of highest pair, handScore[2] = value of second highest pair, handScore[3] = kicker */
    return handScore;
}

int* Dealer::trips(const Card originalArr[])
{
    static int handScore[4];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 4);

    // Finds if trips are present
    for (int i = 0; i < HAND_SIZE-2; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number)
        {
            handScore[0] = TRUTH_VALUE;
            handScore[1] = arr[i].number;
            // Sets the kickers
            switch(i)
            {
                case 0:
                    handScore[2] = arr[3].number;
                    handScore[3] = arr[4].number;
                    break;
                case 1:
                    handScore[2] = arr[0].number;
                    handScore[3] = arr[4].number;
                    break;
                default:
                    handScore[2] = arr[0].number;
                    handScore[3] = arr[1].number;
            }
            break;
        }
    }

    /* handScore[0] = wether or not there are trips (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = trips high card, handScore[2] = first kicker, handScore[3] = second kicker */
    return handScore;
}

int* Dealer::straight(const Card originalArr[])
{
    static int handScore[2];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 2);

    // Sets all but one of any repeated cards as -1, to not interfer with following algorithm
    bool shouldItSort = false;
    for (int i = 0; i < HAND_SIZE-1; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            arr[i].number = -1;
            shouldItSort = true;
        }
    }

    if (shouldItSort)
        sortCardNumber(arr);

    // Checks for straight with ace as value 14
    for (int i = 0; i < HAND_SIZE-4; i++)
    {
        if (arr[i].number == arr[i+1].number+1 && arr[i+1].number == arr[i+2].number+1 &&
            arr[i+2].number == arr[i+3].number+1 && arr[i+3].number == arr[i+4].number+1)
        {
            handScore[0] = TRUTH_VALUE;
            handScore[1] = arr[i].number;
            return handScore;
        }
    }
    
    // Checks for lowest possible straight Ace -> 5 if ace is present
    if (arr[0].number == HIGH_ACE_VALUE)
    {
        for (int i = 1; i < HAND_SIZE-3; i++)
        {
            if (arr[i].number == 5 && arr[i+1].number == 4 
                && arr[i+2].number == 3 && arr[i+3].number == 2)
            {
                handScore[0] = TRUTH_VALUE;
                handScore[1] = 5;
            }
        } 
    }

    /* handScore[0] = wether or not there is a straight (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of straight */
    return handScore;
}

int* Dealer::flush(const Card originalArr[])
{
    static int handScore[7];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 7);

    // Counts how many of each suit there are
    int suit1 = 0, suit2 = 0, suit3 = 0, suit4 = 0;
    for (int i = 0; i < HAND_SIZE; i++)
    {
        switch(arr[i].suit)
        {
            case 1: suit1++; break;
            case 2: suit2++; break;
            case 3: suit3++; break;
            case 4: suit4++; break;
        }
    }

    // Exits function if no flush is present
    if (suit1 < 5 && suit2 < 5 && suit3 < 5 && suit4 < 5)
        return handScore;

    // Sets the suit value
    if (suit1 >= 5)
        handScore[6] = 1;
    else if (suit2 >= 5)
        handScore[6] = 2;
    else if (suit3 >= 5)
        handScore[6] = 3;
    else if (suit4 >= 5)
        handScore[6] = 4;

    // Sets flush to true
    handScore[0] = TRUTH_VALUE;
    
    // Sets the high card and kickers
    int indexCounter = 0;
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (arr[i].suit == handScore[6])
        {
            indexCounter++;
            if (indexCounter > 5)
                break;
            handScore[indexCounter] = arr[i].number;
        }
    }

    /* handScore[0] = wether or not there is a flush (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of flush, handScore[2-5] = kickers, handScore[6] = suit of flush */
    return handScore;
}

int* Dealer::fullHouse(const Card originalArr[])
{
    static int handScore[3];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 3);

    // Finds if trips are present
    for (int i = 0; i < HAND_SIZE-2; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number)
        {
            handScore[1] = arr[i].number;
            // Removes trips so trips arent confused as pairs
            arr[i].number = -15;
            arr[i+1].number = -15;
            arr[i+2].number = -15;
            sortCardNumber(arr);
            // Checks for a pair apart from trips
            for (int i = 0; i < HAND_SIZE-4; i++)
            {
                if (arr[i].number == arr[i+1].number)
                {
                    handScore[0] = TRUTH_VALUE;
                    handScore[2] = arr[i].number;
                    break;
                }
            }
            break;
        }
    }

    /* handScore[0] = wether or not there is a full house (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of trips, handScore[2] = high card of pair */
    return handScore;
}

int* Dealer::quads(const Card originalArr[])
{
    static int handScore[3];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 3);

    // Checks if there are quads present
    for (int i = 0; i < HAND_SIZE-3; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number &&
            arr[i].number == arr[i+3].number)
        {
            handScore[0] = TRUTH_VALUE;
            handScore[1] = arr[i].number;
            // Sets kicker
            if (i == 0)
                handScore[2] = arr[4].number;
            else
                handScore[2] = arr[0].number;
            break;
        }
    }

    /* handScore[0] = wether or not there are quads (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of quads, handScore[2] = kicker */
    return handScore;
}

int* Dealer::straightflush(const Card originalArr[], int* flushPtr)
{
    static int handScore[2];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 2);

    // Checks if flush is present
    if (flushPtr[0] == TRUTH_VALUE)
    {
        // Only keeps cards that correspond to the flush suit
        for (int i = 0; i < HAND_SIZE; i++)
            if (arr[i].suit != flushPtr[6])
                arr[i].number = -1;

        sortCardNumber(arr);

        // Checks for a straight only with cards corresponding to flush
        int* straightPtr = straight(arr);
        if (straightPtr[0] == TRUTH_VALUE)
        {
            handScore[0] = TRUTH_VALUE;
            handScore[1] = straightPtr[1];
        }
    }

    /* handScore[0] = wether or not there is a straightflush (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of straightflush */
    return handScore;
}

// Sorts array from highest card number to lowest card number
void Dealer::sortCardNumber(Card arr[])
{
    for (int i = 0; i < HAND_SIZE-1; i++)
    {
        for (int j = 0; j < HAND_SIZE-i-1; j++)
        {
            if (arr[j].number < arr[j+1].number)
            {
                Card hold = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = hold;
            }
        }
    }
}

// Resets handscore values and copies 'originalArr' values to 'arr'
void Dealer::setUpArrays(Card arr[], const Card originalArr[], int handScore[], int sizeHandScore)
{
    // Resets handScore array to values of 0
    for (int i = 0; i < sizeHandScore; i++)
        handScore[i] = 0;
    
    // Copies to 'arr' array, so 'originalArr' doesn't change
    for (int i = 0; i < HAND_SIZE; i++)
        arr[i] = originalArr[i];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// END HAND STRENGTH FUNCTIONS /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<int> Dealer::determineWinner()
{
    std::vector<int> winners;
    int samePointsCounter = 0, numberOfPlayersTied = 0;

    /* This function determines the winner based on kickers and highcards. First 
    the possibleWinner array is sorted by player points, and the players with the highest 
    points are compared to each other based on their highcard and then the kickers 
    (if need be). Some only require the points to be compared or some of the kickers. */

    sortBy("points", possibleWinner, players);
    samePointsCounter = counterFor("points", possibleWinner, players);
    
    if (samePointsCounter == 1)
        winners.push_back(possibleWinner[0].playerNumber);
    else
    {
        int sameHighCardCounter = 1;

        sortBy("highcard", possibleWinner, samePointsCounter);
        sameHighCardCounter = counterFor("highcard", possibleWinner, samePointsCounter);

        // Finds winner for straightflush and straight
        if ((sameHighCardCounter > 1 && possibleWinner[0].points == 200) || 
            (sameHighCardCounter > 1 && possibleWinner[0].points == 160))
        {
            winners = multipleWinners(possibleWinner, sameHighCardCounter);
            numberOfPlayersTied = sameHighCardCounter;
        }
        else if (sameHighCardCounter > 1)
        {
            int sameKicker1Counter = 1,  sameKicker2Counter = 1, sameKicker3Counter = 1, sameKicker4Counter = 1;

            sortBy("kicker1", possibleWinner, sameHighCardCounter);
            sameKicker1Counter = counterFor("kicker1", possibleWinner, sameHighCardCounter);

            switch(possibleWinner[0].points)
            {
                case 190: // Finds winner for quads and fullhouse
                case 180:
                {
                    if (sameKicker1Counter > 1)
                    {
                        winners = multipleWinners(possibleWinner, sameHighCardCounter);
                        numberOfPlayersTied = sameHighCardCounter;
                    }
                    else
                        winners.push_back(compareKicker("kicker1", possibleWinner, sameHighCardCounter));
                    break;
                }
                case 150: // Finds winner for trips and two pair
                case 140:
                {
                    if (sameKicker1Counter > 1)
                    {
                        sortBy("kicker2", possibleWinner, sameKicker1Counter);
                        sameKicker2Counter = counterFor("kicker2", possibleWinner, sameKicker1Counter);

                        if (sameKicker2Counter > 1)
                        {
                            winners = multipleWinners(possibleWinner, sameKicker2Counter);
                            numberOfPlayersTied = sameKicker2Counter;
                        }
                        else
                            winners.push_back(compareKicker("kicker2", possibleWinner, sameKicker1Counter));
                    }
                    else
                        winners.push_back(compareKicker("kicker1", possibleWinner, sameHighCardCounter));
                    break;
                }
                case 130: // Finds winner for single pair
                {
                    if (sameKicker1Counter > 1)
                    {
                        sortBy("kicker2", possibleWinner, sameKicker1Counter);
                        sameKicker2Counter = counterFor("kicker2", possibleWinner, sameKicker1Counter);

                        if (sameKicker2Counter > 1)
                        {
                            sortBy("kicker3", possibleWinner, sameKicker2Counter);
                            sameKicker3Counter = counterFor("kicker3", possibleWinner, sameKicker2Counter);

                            if (sameKicker3Counter > 1)
                            {
                                winners = multipleWinners(possibleWinner, sameKicker3Counter);
                                numberOfPlayersTied = sameKicker3Counter;
                            }
                            else
                                winners.push_back(compareKicker("kicker3", possibleWinner, sameKicker2Counter));
                        }
                        else
                            winners.push_back(compareKicker("kicker2", possibleWinner, sameKicker1Counter));
                    }
                    else
                        winners.push_back(compareKicker("kicker1", possibleWinner, sameHighCardCounter));
                    break;
                }
                case 170: // Finds winner for flush and high cards
                default:
                {
                    if (sameKicker1Counter > 1)
                    {
                        sortBy("kicker2", possibleWinner, sameKicker1Counter);
                        sameKicker2Counter = counterFor("kicker2", possibleWinner, sameKicker1Counter);

                        if (sameKicker2Counter > 1)
                        {
                            sortBy("kicker3", possibleWinner, sameKicker2Counter);
                            sameKicker3Counter = counterFor("kicker3", possibleWinner, sameKicker2Counter);

                            if (sameKicker3Counter > 1)
                            {
                                sortBy("kicker4", possibleWinner, sameKicker3Counter);
                                sameKicker4Counter = counterFor("kicker4", possibleWinner, sameKicker3Counter);

                                if (sameKicker4Counter > 1)
                                {
                                    winners = multipleWinners(possibleWinner, sameKicker4Counter);
                                    numberOfPlayersTied = sameKicker4Counter;
                                }
                                else
                                    winners.push_back(compareKicker("kicker4", possibleWinner, sameKicker3Counter));
                            }
                            else
                                winners.push_back(compareKicker("kicker3", possibleWinner, sameKicker2Counter));
                        }
                        else
                            winners.push_back(compareKicker("kicker2", possibleWinner, sameKicker1Counter));
                    }
                    else
                        winners.push_back(compareKicker("kicker1", possibleWinner, sameHighCardCounter));
                    break;
                }
            }
        }
        else
            winners.push_back(possibleWinner[0].playerNumber);
    }

    return winners;
}

// Returns the player numbers that tied
std::vector<int> Dealer::multipleWinners(const Player arr[], const int sizeOfVector)
{
    std::vector<int> winners;

    for (int i = 0; i < sizeOfVector; i++)
        winners.push_back(arr[i].playerNumber);

    return winners;
}

// Sorts player hand array based on specified parameters from highest value to lowest value
void Dealer::sortBy(std::string choice, Player arr[], int sizeOfArray)
{
    for (int i = 0; i < sizeOfArray-1; i++)
    {
        for (int j = 0; j < sizeOfArray-i-1; j++)
        {
            if ((choice == "highcard" && arr[j].highCard < arr[j+1].highCard) ||
                (choice == "points" && arr[j].points < arr[j+1].points)    ||
                (choice == "kicker1" && arr[j].kicker1 < arr[j+1].kicker1) ||
                (choice == "kicker2" && arr[j].kicker2 < arr[j+1].kicker2) ||
                (choice == "kicker3" && arr[j].kicker3 < arr[j+1].kicker3) ||
                (choice == "kicker4" && arr[j].kicker4 < arr[j+1].kicker4))
            {
                Player temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Returns the amount of players that have the same value of the "choice" parameter
int Dealer::counterFor(std::string choice, const  Player arr[], int sizeOfArray)
{
    int newCounter = 1;
    for (int i = 0; i < sizeOfArray-1; i++)
    {
        if ((choice == "highcard" && arr[i].highCard == arr[i+1].highCard) ||
            (choice == "points" && arr[i].points == arr[i+1].points)    ||
            (choice == "kicker1" && arr[i].kicker1 == arr[i+1].kicker1) ||
            (choice == "kicker2" && arr[i].kicker2 == arr[i+1].kicker2) ||
            (choice == "kicker3" && arr[i].kicker3 == arr[i+1].kicker3) ||
            (choice == "kicker4" && arr[i].kicker4 == arr[i+1].kicker4))
        {
            newCounter++;
        }
        else
            break;
    }

    return newCounter;
}

// Compares kickers and returns the player number who holds the larger value
int Dealer::compareKicker(std::string choice, const  Player arr[], int sizeOfArray)
{
    int trueWinner = 0;
    for (int i = 0; i < sizeOfArray-1; i++)
    {
        if (choice == "kicker1")
        {
            if (arr[i].kicker1 > arr[i+1].kicker1)
                trueWinner = arr[i].playerNumber;
            else if (arr[i].kicker1 < arr[i+1].kicker1)
                trueWinner = arr[i+1].playerNumber;
        }
        else if (choice == "kicker2")
        {
            if (arr[i].kicker2 > arr[i+1].kicker2)
                trueWinner = arr[i].playerNumber;
            else if (arr[i].kicker2 < arr[i+1].kicker2)
                trueWinner = arr[i+1].playerNumber;
        }
        else if (choice == "kicker3")
        {
            if (arr[i].kicker3 > arr[i+1].kicker3)
                trueWinner = arr[i].playerNumber;
            else if (arr[i].kicker3 < arr[i+1].kicker3)
                trueWinner = arr[i+1].playerNumber;
        }
        else if (choice == "kicker4")
        {
            if (arr[i].kicker4 > arr[i+1].kicker4)
                trueWinner = arr[i].playerNumber;
            else if (arr[i].kicker4 < arr[i+1].kicker4)
                trueWinner = arr[i+1].playerNumber;
        }

        if (trueWinner > 0)
            break;
    }

    return trueWinner;
}