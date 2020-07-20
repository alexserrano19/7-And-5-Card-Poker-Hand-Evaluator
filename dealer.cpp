#include <iostream>
#include <random>
#include "dealer.h"

const int Dealer::TRUTH_VALUE = 1;
const int Dealer::HAND_SIZE = 7;
const int Dealer::LOW_ACE_VALUE = 1;
const int Dealer::HIGH_ACE_VALUE = 14;

Dealer::Dealer() : cardIndex(0), players(0), possibleWinnerIndex(-1)
{}

// Resets and deallocates all variables and memory every loop
void Dealer::deallocate()
{
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

Card* Dealer::getSevenCardHand()
{
    return sevenCardHand;
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
    std::mt19937 randomNumberGenerator(random());
    const int DECK_SIZE = 52;
    const int NUMBER_OF_SHUFFLES = 2;
    int deckIndexCounter = 0;

    // Generates 52 card deck
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Card temp = {i+1, j+1};
            deck[deckIndexCounter] = temp;
            deckIndexCounter++;
        }
    }

    // Shuffles the deck array
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
void Dealer::populateHand(bool sevenCardGame)
{
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
void Dealer::evaluatePlayerHands(bool sevenCardGame)
{
    int* highCardPtr = 0, *pairPtr = 0, *twoPairPtr = 0, *tripsPtr = 0, *straightPtr = 0;
    int* flushPtr = 0, *fullHousePtr = 0, *quadsPtr = 0, *straightFlushPtr = 0;

    possibleWinnerIndex++;

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
        sevenCardHand[5] = {-5, -5};
        sevenCardHand[6] = {-10, -10};
    }

    // The following functions determine the hand strength, with values returned as pointers
    // The values in each index are specified at the end of every hand strength function definition
    straightFlushPtr = straightflush(sevenCardHand);
    if (straightFlushPtr[0] == TRUTH_VALUE)
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

    flushPtr = flush(sevenCardHand);
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

// Sets the stats of the player hands into a Player array
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

///////////////////////////////////// HAND STRENGTH FUNCTIONS /////////////////////////////////////
/* The following functions will return a pointer as an array that returns several values to 
determine serveral factors, which are mentioned specifically at the end of each function */
/* Remember: Aces can act as the number 1 and 14 */

int* Dealer::highCard(const Card originalArr[])
{
    static int handScore[5];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 5);
    changeCardValue(arr, LOW_ACE_VALUE, HIGH_ACE_VALUE);
    sortCardNumber(arr, HAND_SIZE);

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
    changeCardValue(arr, LOW_ACE_VALUE, HIGH_ACE_VALUE);
    sortCardNumber(arr, HAND_SIZE);

    // Finds the card number of the pair
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            handScore[0] = TRUTH_VALUE;
            handScore[1] = arr[i].number;
            break;
        }
    }

    // If a pair is present, this sets its 3 kickers
    if (handScore[0] == TRUTH_VALUE)
    {
        changeCardValue(arr, handScore[1], -1);
        sortCardNumber(arr, HAND_SIZE);
        handScore[2] = arr[0].number;
        handScore[3] = arr[1].number;
        handScore[4] = arr[2].number;
    }

    /* handScore[0] = wether or not there is a single pair (TRUTH_VALUE = true, 0 = false), handScore[1] = high card of single pair, 
    handScore[2] = first kicker handScore[3] = second kicker, handScore[4] = last kicker */
    return handScore;
}

int* Dealer::twoPair(const Card originalArr[])
{
    static int handScore[4];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 4);
    changeCardValue(arr, LOW_ACE_VALUE, HIGH_ACE_VALUE);
    sortCardNumber(arr, HAND_SIZE);

    bool checkSecondPair = false;
    // Finds two pairs and sets their values
    for (int i = 0; i < HAND_SIZE; i++)
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
        changeCardValue(arr, handScore[1], -1);
        changeCardValue(arr, handScore[2], -1);
        sortCardNumber(arr, HAND_SIZE);
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
    changeCardValue(arr, LOW_ACE_VALUE, HIGH_ACE_VALUE);
    sortCardNumber(arr, HAND_SIZE);

    // Finds if trips are present and sets value
    for (int i = 0; i < HAND_SIZE-2; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number)
        {
            handScore[0] = TRUTH_VALUE;
            handScore[1] = arr[i].number;
            break;
        }
    }

    // Sets two kickers if trips are present
    if (handScore[0] == TRUTH_VALUE)
    {
        changeCardValue(arr, handScore[1], -1);
        sortCardNumber(arr, HAND_SIZE);
        handScore[2] = arr[0].number;
        handScore[3] = arr[1].number;
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
    sortCardNumber(arr, HAND_SIZE);

    int counter = 0;
    // Sets all but one of any repeated cards as -1, to not interfer with following algorithm
    for (int i = 0; i < HAND_SIZE-counter; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            arr[i] = {-1, -1};
            sortCardNumber(arr, HAND_SIZE);
            counter++;
            i = 0;
        }
    }

    sortCardNumber(arr, HAND_SIZE);
    
    bool ace = false;
    // Determines if there is a straight taking into account the ace as number 1 and 14
    do
    {
        // If ace is present algorithm will check for ace high straight
        if (ace)
            sortCardNumber(arr, HAND_SIZE);

        // Checks for straight
        for (int i = 0; i < HAND_SIZE-4; i++)
        {
            if (arr[i].number == arr[i+1].number+1 && arr[i+1].number == arr[i+2].number+1 &&
                arr[i+2].number == arr[i+3].number+1 && arr[i+3].number == arr[i+4].number+1)
            {
                handScore[0] = TRUTH_VALUE;
                handScore[1] = arr[i].number;
                break;
            }
        }

        // Exits loop after checking for a straight with an ace
        if (ace)
            break;

        // Checks if ace is present
        if (!ace)
        {
            for (int i = 0; i < HAND_SIZE; i++)
            {
                if (arr[i].number == LOW_ACE_VALUE)
                {
                    arr[i].number = HIGH_ACE_VALUE;
                    ace = true;
                }
            }
        }

    } while (ace);

    /* handScore[0] = wether or not there is a straight (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of straight */
    return handScore;
}

int* Dealer::flush(const Card originalArr[])
{
    static int handScore[7];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 7);
    
    int counter1 = 0, counter2 = 0, counter3 = 0, counter4 = 0;
    // Finds how many of each suit there are
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (arr[i].suit == 4)
            counter4++;
        else if (arr[i].suit == 3)
            counter3++;
        else if (arr[i].suit == 2)
            counter2++;
        else if (arr[i].suit == 1)
            counter1++;
    }

    // Exits function if no flush is present
    if (counter1 < 5 && counter2 < 5 && counter3 < 5 && counter4 < 5)
        return handScore;

    changeCardValue(arr, LOW_ACE_VALUE, HIGH_ACE_VALUE);
    sortCardNumber(arr, HAND_SIZE);

    // Finds card numbers and suit corresponding to flush
    for (int i = 1; i < 5; i++)
    {
        if ((counter1 >= 5 && i == 1) || 
            (counter2 >= 5 && i == 2) ||
            (counter3 >= 5 && i == 3) || 
            (counter4 >= 5 && i == 4))
        {
            handScore[0] = TRUTH_VALUE;
            // Sets the suit of the flush
            handScore[6] = i;
            int indexCounter = 0;
            // Sets the high card and kickers
            for (int j = 0; j < HAND_SIZE; j++)
            {
                if (arr[j].suit == i)
                {
                    indexCounter++;
                    if (indexCounter > 5)
                        break;
                    handScore[indexCounter] = arr[j].number;
                }
            }
            break;
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
    changeCardValue(arr, LOW_ACE_VALUE, HIGH_ACE_VALUE);
    sortCardNumber(arr, HAND_SIZE);

    bool tripsPresent = false, pairPresent = false;
    // Finds if trips are present
    for (int i = 0; i < HAND_SIZE-2; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number)
        {
            handScore[1] = arr[i].number;
            tripsPresent = true;
            break;
        }
    }

    if (tripsPresent)
    {
        // Removes trips so trips arent confused as pairs
        changeCardValue(arr, handScore[1], -100);
        sortCardNumber(arr, HAND_SIZE);
        
        // Checks for a pair apart from trips
        for (int i = 0; i < HAND_SIZE-4; i++)
        {
            if (arr[i].number == arr[i+1].number)
            {
                handScore[2] = arr[i].number;
                pairPresent = true;
                break;
            }
        }
    }

    if (pairPresent && tripsPresent)
        handScore[0] = TRUTH_VALUE;

    /* handScore[0] = wether or not there is a full house (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of trips, handScore[2] = high card of pair */
    return handScore;
}

int* Dealer::quads(const Card originalArr[])
{
    static int handScore[3];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 3);
    changeCardValue(arr, LOW_ACE_VALUE, HIGH_ACE_VALUE);
    sortCardNumber(arr, HAND_SIZE);

    // Checks if there are quads present
    for (int i = 0; i < HAND_SIZE-3; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number &&
            arr[i].number == arr[i+3].number)
        {
            handScore[0] = TRUTH_VALUE;
            handScore[1] = arr[i].number;
        }
    }

    // If quads are present, the kicker is added
    if (handScore[0] == TRUTH_VALUE)
    {
        changeCardValue(arr, handScore[1], -1);
        sortCardNumber(arr, HAND_SIZE);
        handScore[2] = arr[0].number;
    }

    /* handScore[0] = wether or not there are quads (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of quads, handScore[2] = kicker */
    return handScore;
}

int* Dealer::straightflush(const Card originalArr[])
{
    static int handScore[2];
    Card arr[HAND_SIZE];

    setUpArrays(arr, originalArr, handScore, 2);

    int* flushPtr = flush(arr);
    bool flushPresent = false;
    // Checks for a flush
    if (flushPtr[0] == TRUTH_VALUE)
    {
        flushPresent = true;
        // Only keeps cards that correspond to the flush
        for (int i = 0; i < HAND_SIZE; i++)
        {
            if (flushPtr[6] != arr[i].suit)
            {
                Card newCard = {-1, -1};
                arr[i] = newCard;
            }
        }
    }

    bool straightPresent = false;
    // Checks for a straight
    if (flushPresent)
    {
        int* straightPtr = straight(arr);

        if (straightPtr[0] == TRUTH_VALUE)
        {
            handScore[1] = straightPtr[1];
            straightPresent = true;
        }
    }

    if (straightPresent && flushPresent)
        handScore[0] = TRUTH_VALUE;

    /* handScore[0] = wether or not there is a straightflush (TRUTH_VALUE = true, 0 = false), 
    handScore[1] = high card of straightflush */
    return handScore;
}

// Sorts array from highest card number to lowest card number regardless of suit
void Dealer::sortCardNumber(Card arr[], int sizeOfArray)
{
    for (int i = 0; i < sizeOfArray-1; i++)
    {
        for (int j = 0; j < sizeOfArray-i-1; j++)
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

// Change value of card to new value, i.e. changing ace value from 1 to 14
void Dealer::changeCardValue(Card arr[], int valueCompared, int newValue)
{
    for (int i = 0; i < HAND_SIZE; i++)
        if (arr[i].number == valueCompared)
            arr[i].number = newValue;
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

            switch (possibleWinner[0].points)
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