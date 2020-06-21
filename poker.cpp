#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <limits>

// Terminal command to compile: g++ poker.cpp -std=c++0x
// Terminal command to execute: ./a.out

struct Card
{
    int number;
    int suit;
};

struct Player_Hand
{
    int playerNumber;
    int highCard;      
    int points;
    int kicker1;
    int kicker2;
    int kicker3;
    int kicker4;
};

int printMenu();
int selectOpponents();
void printPlayerHand(struct Card arr[], int indexCounter);
void sortCardNumber(struct Card arr[], int sizeOfArray);
void setStats(struct Player_Hand arr[], int index, int pointsEarned, int highCard, int k1, int k2, int k3, int k4);
void sortBy(std::string choice, struct Player_Hand arr[], int sizeOfArray);
int counterFor(std::string choice, struct Player_Hand arr[], int sizeOfArray);
int compareKicker(std::string choice, struct Player_Hand arr[], int sizeOfArray);
int* multipleWinners(struct Player_Hand arr[], int sizeOfArray);
void changeCardValue(struct Card arr[], int valueCompared, int newValue);

// Card strength functions
int* highCard(struct Card arr[]);
int* pair(struct Card arr[]);
int* twoPair(struct Card arr[]);
int* trips(struct Card arr[]);
int* straight(struct Card arr[]);
int* flush(struct Card arr[]);
int* fullHouse(struct Card arr[]);
int* quads(struct Card arr[]);
int* straightflush(struct Card arr[]);

// Operator overloading for hash map
bool operator < (const Card& t1, const Card& t2)
{
    if (t1.number == t2.number)
        return (t1.suit < t2.suit);
    else
        return (t1.number < t2.number);
}

int main()
{
    std::cout << "\n========---------------------------------========\n";
    std::cout << " ***** !! pOkEr PoKeR POKER PoKeR pOkEr !! ***** \n";
    std::cout << "========---------------------------------========\n";

    ////////// DECLARATION
    // Seeds time for random number generation
    srand(time(0));
    int selection = 0;
    ////////// END DECLARATION

    do 
    {
        // Selects menu option
        selection = printMenu();

        if (selection == 1)
        {
            ////////// DECLARATION
            // Selects opponents to verse
            int opponents = selectOpponents();
            std::map <Card, int> availableCards;
            ////////// END DECLARATION

            // Generates hash map of 52 card deck
            for (int i = 0; i < 13; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    const Card cardToInsert = * new Card{i+1, j+1};
                    availableCards.insert(std::pair<Card, int>(cardToInsert, 1));
                }
            }
            
            ////////// DECLARATION
            int communityCounter = 0, playerCardCounter = 0;
            Card card, preEvaluation[5], evaluation[7];
            Card* playerCard = new Card[(opponents+1)*2];
            ////////// END DECLARATION

            // Creates distinct cards for all players and community cards
            do
            {
                card.number = rand() % 13 + 1;
                card.suit = rand() % 4 + 1;

                if (availableCards[card] == 1)
                {
                    if (communityCounter < 5)
                    {
                        preEvaluation[communityCounter] = card;
                        communityCounter++;
                    }
                    else
                    {
                        playerCard[playerCardCounter] = card;
                        playerCardCounter++;
                    }
                    availableCards[card] = 0;
                }
            } while (playerCardCounter < (opponents+1)*2);

            ////////// DECLARATION
            int indexCounter = -1;
            int *highCardPtr, *pairPtr, *twoPairPtr, *tripsPtr, *straightPtr, *flushPtr, *fullHousePtr, *quadsPtr, *straightFlushPtr;
            Player_Hand* possibleWinner = new Player_Hand[opponents+1];
            ////////// END DECLARATION

            playerCardCounter = 0;

            std::cout << "\nTHE FOLLOWING LIST SHOWS ALL PLAYER HANDS AND THEIR STRENGTH:\n";

            while (playerCardCounter < (opponents+1)*2)
            {
                // Sets the index for possibleWinner array
                indexCounter++;

                // Copies the communnity cards into evaluation array
                for (int i = 0; i < 5; i++)
                    evaluation[i] = preEvaluation[i];

                // Copies 2 cards of the playerCard array into evaluation array
                for (int i = 5; i < 7; i++)
                {
                    evaluation[i] = playerCard[playerCardCounter];
                    playerCardCounter++;
                }

                printPlayerHand(evaluation, indexCounter);

                // The pointers contain the values that evaluate each hand strength
                straightFlushPtr = straightflush(evaluation);
                quadsPtr = quads(evaluation);
                fullHousePtr = fullHouse(evaluation);
                flushPtr = flush(evaluation);
                straightPtr = straight(evaluation);
                tripsPtr = trips(evaluation);
                twoPairPtr = twoPair(evaluation);
                pairPtr = pair(evaluation);
                highCardPtr = highCard(evaluation);

                ///// The following finds the hand strength for each player /////
                if (straightFlushPtr[0] == 1)
                {
                    std::cout << "Hand strength: STRAIGHT FLUSH" << std::endl;
                    setStats(possibleWinner, indexCounter, 200, straightFlushPtr[1], 0, 0, 0, 0);
                }
                else if (quadsPtr[0] == 1)
                {
                    std::cout << "Hand strength: QUADS" << std::endl;
                    setStats(possibleWinner, indexCounter, 190, quadsPtr[1], quadsPtr[2], 0, 0, 0);
                }
                else if (fullHousePtr[0] == 1)
                {
                    std::cout << "Hand strength: FULL HOUSE" << std::endl;
                    setStats(possibleWinner, indexCounter, 180, fullHousePtr[1], fullHousePtr[2], 0, 0, 0);
                }
                else if (flushPtr[0] == 1)
                {
                    std::cout << "Hand strength: FLUSH" << std::endl;
                    setStats(possibleWinner, indexCounter, 170, flushPtr[1], flushPtr[2], flushPtr[3],
                            flushPtr[4], flushPtr[5]);
                }
                else if (straightPtr[0] == 1)
                {
                    std::cout << "Hand strength: STRAIGHT" << std::endl;
                    setStats(possibleWinner, indexCounter, 160, straightPtr[1], 0, 0, 0, 0);
                }
                else if (tripsPtr[0] == 1)
                {
                    std::cout << "Hand strength: TRIPS" << std::endl;
                    setStats(possibleWinner, indexCounter, 150, tripsPtr[1], tripsPtr[2], tripsPtr[3], 0, 0);
                }
                else if (twoPairPtr[0] == 1)
                {
                    std::cout << "Hand strength: TWO PAIR" << std::endl;
                    setStats(possibleWinner, indexCounter, 140, twoPairPtr[1], twoPairPtr[2], twoPairPtr[3], 0, 0);
                }
                else if (pairPtr[0] == 1)
                {
                    std::cout << "Hand strength: PAIR" << std::endl;
                    setStats(possibleWinner, indexCounter, 130, pairPtr[1], pairPtr[2], pairPtr[3], pairPtr[4], 0);
                }
                else
                {
                    std::cout << "Hand strength: HIGH CARD" << std::endl;
                    setStats(possibleWinner, indexCounter, highCardPtr[0], highCardPtr[0], highCardPtr[1],
                            highCardPtr[2], highCardPtr[3], highCardPtr[4]);
                }

                #pragma region Memory Deallocation

                // Deallocates memory from the functions
                // Sets pointers to null value
                delete [] straightFlushPtr; 
                straightFlushPtr = 0;
                delete [] quadsPtr; 
                quadsPtr = 0;
                delete [] fullHousePtr; 
                fullHousePtr = 0;
                delete [] flushPtr; 
                flushPtr = 0; 
                delete [] straightPtr; 
                straightPtr = 0;
                delete [] tripsPtr;
                tripsPtr = 0;
                delete [] twoPairPtr;
                twoPairPtr = 0;
                delete [] pairPtr; 
                pairPtr = 0;
                delete [] highCardPtr; 
                highCardPtr = 0;

                #pragma endregion
            }

            delete [] playerCard;
            playerCard = 0;

            ////////// DECLARATION
            int* wins;
            int samePointsCounter = 0, trueWinner = 0, numberOfPlayersTied = 0;
            ////////// END DECLARATION

            /*  The rest of this program determines the winner based on kickers and highcards.
            First the program is sorted by player points, and the players with the highest 
            points are compared to each other based on their highcard and then the kickers 
            (if need be). Some only require the highcards to be compared or some of the kickers. */

            // sortBy, Sorts player hands by whatever is written in the first parameter i.e. "points"
            sortBy("points", possibleWinner, opponents+1);
            // counterFor, Counts the amount of players that have the same amount of whatever is written in the first parameter i.e. "points"
            samePointsCounter = counterFor("points", possibleWinner, opponents+1);
            
            /**** The rest of the program follows a similar pattern to the one explained above ****/
            if (samePointsCounter == 1)
                trueWinner = possibleWinner[0].playerNumber;
            else
            {
                ////////// DECLARATION
                int sameHighCardCounter = 1;
                ////////// END DECLARATION

                sortBy("highcard", possibleWinner, samePointsCounter);
                sameHighCardCounter = counterFor("highcard", possibleWinner, samePointsCounter);

                // Finds winner for straightflush and straight
                if ((sameHighCardCounter > 1 && possibleWinner[0].points == 200) || 
                    (sameHighCardCounter > 1 && possibleWinner[0].points == 160))
                {
                    wins = multipleWinners(possibleWinner, sameHighCardCounter);
                    numberOfPlayersTied = sameHighCardCounter;
                }
                else if (sameHighCardCounter > 1)
                {
                    ////////// DECLARATION
                    int sameKicker1Counter = 1,  sameKicker2Counter = 1, sameKicker3Counter = 1, sameKicker4Counter = 1;
                    ////////// END DECLARATION

                    sortBy("kicker1", possibleWinner, sameHighCardCounter);
                    sameKicker1Counter = counterFor("kicker1", possibleWinner, sameHighCardCounter);

                    switch (possibleWinner[0].points)
                    {
                        case 190: // Finds winner for quads and fullhouse
                        case 180:
                        {
                            if (sameKicker1Counter > 1)
                            {
                                wins = multipleWinners(possibleWinner, sameHighCardCounter);
                                numberOfPlayersTied = sameHighCardCounter;
                            }
                            else
                                trueWinner = compareKicker("kicker1", possibleWinner, sameHighCardCounter);
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
                                    wins = multipleWinners(possibleWinner, sameKicker2Counter);
                                    numberOfPlayersTied = sameKicker2Counter;
                                }
                                else
                                    trueWinner = compareKicker("kicker2", possibleWinner, sameKicker1Counter);
                            }
                            else
                                trueWinner = compareKicker("kicker1", possibleWinner, sameHighCardCounter);
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
                                        wins = multipleWinners(possibleWinner, sameKicker3Counter);
                                        numberOfPlayersTied = sameKicker3Counter;
                                    }
                                    else
                                        trueWinner = compareKicker("kicker3", possibleWinner, sameKicker2Counter);
                                }
                                else
                                    trueWinner = compareKicker("kicker2", possibleWinner, sameKicker1Counter);
                            }
                            else
                                trueWinner = compareKicker("kicker1", possibleWinner, sameHighCardCounter);
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
                                            wins = multipleWinners(possibleWinner, sameKicker4Counter);
                                            numberOfPlayersTied = sameKicker4Counter;
                                        }
                                        else
                                            trueWinner = compareKicker("kicker4", possibleWinner, sameKicker3Counter);
                                    }
                                    else
                                        trueWinner = compareKicker("kicker3", possibleWinner, sameKicker2Counter);
                                }
                                else
                                    trueWinner = compareKicker("kicker2", possibleWinner, sameKicker1Counter);
                            }
                            else
                                trueWinner = compareKicker("kicker1", possibleWinner, sameHighCardCounter);
                            break;
                        }
                    }
                }
                else
                    trueWinner = possibleWinner[0].playerNumber;
            }

            delete [] possibleWinner;
            possibleWinner = 0;

            #pragma region Handles Output For Winning Hand(s)

            if (numberOfPlayersTied > 0)
            {
                std::cout << "\nITS A TIE BETWEEN PLAYERS";
                for (int i = 0; i < numberOfPlayersTied-1; i++)
                    std::cout << " #" << wins[i] << ",";
                std::cout << " and #" << wins[numberOfPlayersTied-1] << "!\n\n";

                delete [] wins;
                wins = 0;
            }
            else
            {
                std::cout << "\n    <******* THE WINNER IS *******>\n";
                std::cout << "      -----    PLAYER #" << trueWinner << "    -----" << std::endl;
                std::cout << "   <**** !! CONGRATULATIONS !! ****>\n\n";
            }

            #pragma endregion
        }

    } while (selection == 1);

    std::cout << "\n*** THANKS FOR PLAYING ***\n\n";
    
    return 0;
}


////////////////////////////////////////////////////////////////////////////
/************************** FUNCTION DEFINITIONS **************************/
////////////////////////////////////////////////////////////////////////////


// Returns the menu selection
int printMenu()
{
    int selection = 0;

    std::cout << "\nMeNuMeNu%%%========%%%MeNuMeNu\n";
    std::cout << "     1 - Deal cards!\n";
    std::cout << "     2 - Quit\n";
    std::cout << "MeNuMeNu%%%========%%%MeNuMeNu\n" << ">>> ";;
    std::cin >> selection;

    // Forces a choice of 1 or 2
    while (selection != 1 && selection != 2)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nTRY AGAIN!\n" << ">>> ";
        std::cin >> selection;
    }

    return selection;
}


// Returns the amount of opponents that will be versed
int selectOpponents()
{
    int opponents = 0;

    do
    {
        std::cout << "\nHow many opponents do you want to play with?\n" << ">>> ";
        std::cin >> opponents;

        // Forces between 1 and 22 (inclusive) opponents
        if (opponents > 22)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nONLY A MAXIMUM OF 22 OPPONENTS ARE POSSIBLE!";
        }
        else if (opponents < 1)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nMORE OPPONENTS ARE NEEDED!";
        }
    } while (opponents > 22 || opponents < 1);

    return opponents;
}


// Prints out player hand and community cards
void printPlayerHand(struct Card arr[], int indexCounter)
{
    if (indexCounter == 0)
        std::cout << "\nPlayer #"<< indexCounter+1 << "(YOU)" << std::endl;
    else
        std::cout << "\nPlayer #"<< indexCounter+1 << std::endl;

    // Prints out community cards and player cards for each player
    std::cout << "Community Cards: {";
    for(int i = 0; i < 4; i++)
        std::cout << "{" << arr[i].number << ", " << arr[i].suit << "}, ";
    std::cout << "{" << arr[4].number << ", " << arr[4].suit << "}}";
    std::cout << "\nPlayer Hand: {{" << arr[5].number << ", " << arr[5].suit << "}, ";
    std::cout << "{" << arr[6].number << ", " << arr[6].suit << "}}\n";
}


// Sorts array from highest card number to lowest card number regardless of suit
void sortCardNumber(struct Card arr[], int sizeOfArray)
{
    for (int i = 0; i < sizeOfArray-1; i++)
    {
        for (int j = 0; j < sizeOfArray-i-1; j++)
        {
            if (arr[j].number < arr[j+1].number)
            {
                Card temp;
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}


// Sets the stats of the player hands into a new array
void setStats(struct Player_Hand arr[], int index, int pointsEarned, int highCard, int k1, int k2, int k3, int k4)
{
    arr[index].playerNumber = index+1;
    arr[index].points = pointsEarned;
    arr[index].highCard = highCard;
    arr[index].kicker1 = k1;
    arr[index].kicker2 = k2; 
    arr[index].kicker3 = k3;
    arr[index].kicker4 = k4;
}


// Sorts player hand array based on specified parameters from highest value to lowest value
void sortBy(std::string choice, struct Player_Hand arr[], int sizeOfArray)
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
                Player_Hand temp;
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}


// Returns the amount of players that have the same value of the "choice" parameter
int counterFor(std::string choice, struct Player_Hand arr[], int sizeOfArray)
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
int compareKicker(std::string choice, struct Player_Hand arr[], int sizeOfArray)
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


// Returns the player numbers that tied
int* multipleWinners(struct Player_Hand arr[], int sizeOfArray)
{
    int *status = new int[sizeOfArray];

    for (int i = 0; i < sizeOfArray; i++)
        status[i] = arr[i].playerNumber;

    return status;
}


// Change value of card to new value, i.e. changing ace value from 1 to 14
void changeCardValue(struct Card arr[], int valueCompared, int newValue)
{
    for (int i = 0; i < 7; i++)
        if (arr[i].number == valueCompared)
            arr[i].number = newValue;
}


//////////////////// HAND STRENGTH FUNCTIONS ////////////////////
/* SIDE NOTE: The following functions will return a pointer as an
array that returns several values to determine serveral factors, 
which are mentioned specifically at the end of each function */
/* Remember: Aces can act as the number 1 and 14 */
 

int* highCard(struct Card arr[])
{
    int* status = new int[5];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    // Copies 5 highest cards into status array
    for (int i = 0; i < 5; i++)
        status[i] = arr[i].number;

    // status[0-4] = high card and kickers
    return status;
}


int* pair(struct Card arr[])
{
    int* status = new int[5];
    Card hold[7];
    
    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    // Finds the card number of the pair
    for (int i = 0; i < 7; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            status[0] = 1;
            status[1] = arr[i].number;
            break;
        }
    }

    // If a pair is present, this sets its 3 kickers
    if (status[0] == 1)
    {
        changeCardValue(arr, status[1], -1);
        sortCardNumber(arr, 7);
        status[2] = arr[0].number;
        status[3] = arr[1].number;
        status[4] = arr[2].number;
    }

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether or not there is a single pair, status[1] = high card of single pair, status[2] = first kicker
    status[3] = second kicker, status[4] = last kicker */
    return status;
}


int* twoPair(struct Card arr[])
{
    int* status = new int[4];
    Card hold[7];
    
    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    bool checkSecondPair = false;
    // Finds two pairs and sets their values
    for (int i = 0; i < 7; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            if (checkSecondPair)
            {
                status[0] = 1;
                status[2] = arr[i].number;
                break;
            }
            else
            {
                status[1] = arr[i].number;
                checkSecondPair = true;
            }
        }
    }

    // Sets kicker if two pairs are present
    if (status[0] == 1)
    {
        changeCardValue(arr, status[1], -1);
        changeCardValue(arr, status[2], -1);
        sortCardNumber(arr, 7);
        status[3] = arr[0].number;
    }

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether or not there is a two pair, status[1] = value of highest pair, 
    status[2] = value of second highest pair, status[3] = kicker */
    return status;
}


int* trips(struct Card arr[])
{
    int *status = new int[4];
    Card hold[7];

    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    // Finds if trips are present and sets value
    for (int i = 0; i < 5; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number)
        {
            status[0] = 1;
            status[1] = arr[i].number;
            break;
        }
    }

    // Sets two kickers if trips are present
    if (status[0] == 1)
    {
        changeCardValue(arr, status[1], -1);
        sortCardNumber(arr, 7);
        status[2] = arr[0].number;
        status[3] = arr[1].number;
    }

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether or not there are trips, status[1] = trips high card, 
    status[2] = first kicker, status[3] = second kicker */
    return status;
}


int* straight(struct Card arr[])
{
    int *status = new int[2];
    Card hold[7];

    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    sortCardNumber(arr, 7);

    int counter = 0;
    // Sets all but one of any repeated cards as -1, to not interfer with following algorithm
    for (int i = 0; i < 7-counter; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            Card temp = {-1, -1};
            arr[i] = temp;
            sortCardNumber(arr, 7);
            counter++;
            i = 0;
        }
    }

    sortCardNumber(arr, 7);
    
    bool ace = false;
    // Determines if there is a straight taking into account the ace as number 1 and 14
    do
    {
        // If ace is present algorithm will check for ace high straight
        if (ace)
            sortCardNumber(arr, 7);

        // Checks for straight
        for (int i = 0; i < 3; i++)
        {
            if (arr[i].number == arr[i+1].number+1 && arr[i+1].number == arr[i+2].number+1 &&
                arr[i+2].number == arr[i+3].number+1 && arr[i+3].number == arr[i+4].number+1)
            {
                status[0] = 1;
                status[1] = arr[i].number;
                break;
            }
        }

        // Exits loop after checking for a straight with an ace
        if (ace)
            break;

        // Checks if ace is present
        if (!ace)
        {
            for (int i = 0; i < 7; i++)
            {
                if (arr[i].number == 1)
                {
                    arr[i].number = 14;
                    ace = true;
                }
            }
        }

    } while (ace);

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    // status[0] = wether or not there is a straight, status[1] = high card of straight
    return status;
}


int* flush(struct Card arr[])
{
    int *status = new int[7];
    Card hold[7];

    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    int counter1 = 0, counter2 = 0, counter3 = 0, counter4 = 0;
    // Finds how many of each suit there are
    for (int i = 0; i < 7; i++)
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
        return status;

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    // Finds flush card numbers and suit number
    for (int i = 1; i < 5; i++)
    {
        if ((counter1 >= 5 && i == 1) || 
            (counter2 >= 5 && i == 2) ||
            (counter3 >= 5 && i == 3) || 
            (counter4 >= 5 && i == 4))
        {
            status[0] = 1;
            // Sets the suit of the flush
            status[6] = i;
            int indexCounter = 0;
            // Sets the high card and kickers
            for (int j = 0; j < 7; j++)
            {
                if (arr[j].suit == i)
                {
                    indexCounter++;
                    if (indexCounter > 5)
                        break;
                    status[indexCounter] = arr[j].number;
                }
            }
            break;
        }
    }

    // Hold array copies old values back after collecting information about the array for the function
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether or not there is a flush, status[1] = high card of flush, 
    status[2-5] = kickers, status[6] = suit of flush */
    return status;
}


int* fullHouse(struct Card arr[])
{
    int *status = new int[3];
    Card hold[7];

    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    bool tripsPresent = false, pairPresent = false;
    // Finds if trips are present
    for (int i = 0; i < 5; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number)
        {
            status[1] = arr[i].number;
            tripsPresent = true;
            break;
        }
    }

    if (tripsPresent)
    {
        // Removes trips so trips arent confused as pairs
        changeCardValue(arr, status[1], -1);
        sortCardNumber(arr, 7);
        
        // Checks for a pair
        for (int i = 0; i < 3; i++)
        {
            if (arr[i].number == arr[i+1].number)
            {
                status[2] = arr[i].number;
                pairPresent = true;
                break;
            }
        }
    }

    if (pairPresent && tripsPresent)
        status[0] = 1;

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether or not there is a full house, status[1] = high card of trips,
    status[2] = high card of pair */
    return status;
}


int* quads(struct Card arr[])
{
    int* status = new int[3];
    Card hold[7];

    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    // Checks if there are quads present
    for (int i = 0; i < 4; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number &&
            arr[i].number == arr[i+3].number)
        {
            status[0] = 1;
            status[1] = arr[i].number;
        }
    }

    // If quads are present, the kicker is added
    if (status[0] == 1)
    {
        changeCardValue(arr, status[1], -1);
        sortCardNumber(arr, 7);
        status[2] = arr[0].number;
    }

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    // status[0] = wether or not there are quads, status[1] = high card of quads, status[2] = kicker
    return status;
}


int* straightflush(struct Card arr[])
{
    int* status = new int[2];
    Card hold[7];

    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    int* flushPtr = flush(arr);
    bool flushPresent = false;
    // Checks for a flush
    if (flushPtr[0] == 1)
    {
        flushPresent = true;
        // If a card suit is not corresponding to the flush it is removed to prevent interference with algorithm
        for (int i = 0; i < 7; i++)
        {
            if (flushPtr[6] != arr[i].suit)
            {
                Card temp = {-1, -1};
                arr[i] = temp;
            }
        }
    }
    
    delete [] flushPtr;
    flushPtr = 0;

    bool straightPresent = false;
    // Checks for a straight
    if (flushPresent)
    {
        int* straightPtr = straight(arr);

        if (straightPtr[0] == 1)
        {
            status[1] = straightPtr[1];
            straightPresent = true;
        }
        
        delete [] straightPtr;
        straightPtr = 0;
    }

    if (straightPresent && flushPresent)
        status[0] = 1;
    
    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    // status[0] = wether or not there is a straightflush, status[1] = high card of straightflush
    return status;
}