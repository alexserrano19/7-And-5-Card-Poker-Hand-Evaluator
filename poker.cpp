#include <cstdlib>
#include <iostream>
#include <map>
#include <iomanip>
#include <chrono>
#include <random>
#include <array>

/* 
Terminal command to compile on Mac: 
g++ poker.cpp -std=c++0x
Terminal command to execute on Mac: 
./a.out 
*/

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

char firstMenuOption();
char selectMenuOption(bool fiveCardGame);
void printByeMessage();
int selectPlayers7game(std::string question);
int selectPlayers5game(std::string question);
void printCards(const struct Card arr[], int possibleWinnerIndex, bool printingCommunityCards, 
                int startPosition, int arraySize, bool fiveCardGame);
void dynamicallyGrowBorder(int arr[], int playersTied);
void sortCardNumber(struct Card arr[], int sizeOfArray);
void setStats(struct Player_Hand arr[], int index, int pointsEarned, int highCard, int k1, int k2, int k3, int k4);
void sortBy(std::string choice, struct Player_Hand arr[], int sizeOfArray);
void changeCardValue(struct Card arr[], int valueCompared, int newValue);
int counterFor(std::string choice, const struct Player_Hand arr[], int sizeOfArray);
int compareKicker(std::string choice, const struct Player_Hand arr[], int sizeOfArray);
int* multipleWinners(const struct Player_Hand arr[], int sizeOfArray);

// Card strength functions
int* highCard(const struct Card originalArr[]);
int* pair(const struct Card originalArr[]);
int* twoPair(const struct Card originalArr[]);
int* trips(const struct Card originalArr[]);
int* straight(const struct Card originalArr[]);
int* flush(const struct Card originalArr[]);
int* fullHouse(const struct Card originalArr[]);
int* quads(const struct Card originalArr[]);
int* straightflush(const struct Card originalArr[]);

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
    std::cout << "\n\nMoNeYmOnEy======--------======mOnEyMoNeY\n";
    std::cout << "     **** Poker Hand Evaluator **** \n";
    std::cout << "MoNeYmOnEy======--------======mOnEyMoNeY\n\n";

    ////////// DECLARATION
    // START - Uniform and random number generation declaration
    std::random_device randomGeneration;
    std::array<unsigned int, std::mt19937::state_size> seed_data;
    std::generate_n(seed_data.begin(), seed_data.size(), std::ref(randomGeneration));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 engineNumberGenerator(seq);
    std::uniform_int_distribution<int> cardNumber(1, 13);
    std::uniform_int_distribution<int> cardSuit(1, 4);
    // END - Uniform and random number generation declaration
    char selection = 0;
    unsigned long long int statsArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // Activates/deactivates 'cout' output for higher efficiency
    std::streambuf* orig_buf = std::cout.rdbuf();
    // Continuous looping variables
    bool continuousLoopOn = false;
    int playersPerContinuousLoop = 0;
    int outputGames = 0;
    unsigned long long int numberOfLoops = 0;
    unsigned long long int loopingRequirement = 0;
    // Used for timer
    std::chrono::high_resolution_clock::time_point startTimer, endTimer;
    int duration;
    int tempDuration = 0;
    // Used for first menu option
    char firstSelection;
    // Boolean for 5/7 card poker 
    bool fiveCardGame = false;
    ////////// END DECLARATION

    // Choice between 5 or 7 card evaluator
    firstSelection = firstMenuOption();

    if (firstSelection == '5')
        fiveCardGame = true;
    else if (firstSelection == '7')
        fiveCardGame = false;
    else
    {
        printByeMessage();
        return 0;
    }

    do 
    {
        // Selects menu option, if it is in continous loop it doesn't ask
        if (!continuousLoopOn)
            selection = selectMenuOption(fiveCardGame);
        else
        {
            selection = '1';
            if (numberOfLoops == 0)
            {
                startTimer = std::chrono::high_resolution_clock::now();
                std::clog << "\n\n\nLOADING...\n\n\n";
                // Deactivates cout output until looping is over
                std::cout.rdbuf(NULL);
            }
            if (loopingRequirement-numberOfLoops == outputGames)
                std::cout.rdbuf(orig_buf);
            endTimer = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::seconds>(endTimer - startTimer).count(); 
            if (duration % 5 == 0 && duration > tempDuration)
            {
                tempDuration = duration;
                std::clog << "LOADING...\n";
                std::clog << "Time loading: " << duration << " seconds\n";
                std::clog << "Games completed so far: " << numberOfLoops << "\n\n\n";
            }
            numberOfLoops++;
        }
        
        if (selection == '1')
        {
            ////////// DECLARATION
            // Selects number of opponents to verse
            int players = 0;
            std::map <Card, int> availableCards;
            ////////// END DECLARATION

            // Selects opponents
            if (!fiveCardGame)
                players = (!continuousLoopOn) ? selectPlayers7game("How many players will you like to deals cards to?") : playersPerContinuousLoop;
            else
                players = (!continuousLoopOn) ? selectPlayers5game("How many players will you like to deals cards to?") : playersPerContinuousLoop;

            // Generates 52 card deck
            for (int i = 0; i < 13; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    const Card cardToInsert = * new Card{i+1, j+1};
                    availableCards.insert(std::pair<Card, int>(cardToInsert, 1));
                }
            }
            
            ////////// DECLARATION
            int communityCounter = 0, playerCardCounter = 0, loopEvaluatorRequirement = 0;
            Card card, communityCards[5], sevenCardHand[7];
            Card* playerCards = new Card[players*2]();
            Card* fiveCardHand = new Card[players*5]();
            ////////// END DECLARATION

            if (!fiveCardGame)
                loopEvaluatorRequirement = players*2;
            else
                loopEvaluatorRequirement = players*5;

            if (!fiveCardGame)
            {
                // Creates distinct cards for all players and community cards
                do
                {
                    card.number = cardNumber(engineNumberGenerator);
                    card.suit = cardSuit(engineNumberGenerator);    

                    if (availableCards[card] == 1)
                    {
                        if (communityCounter < 5)
                        {
                            communityCards[communityCounter] = card;
                            communityCounter++;
                        }
                        else
                        {
                            playerCards[playerCardCounter] = card;
                            playerCardCounter++;
                        }
                        availableCards[card] = 0;
                    }
                } while (playerCardCounter < loopEvaluatorRequirement);
            }
            else
            {
                // Creates distinct cards for all players and community cards
                do
                {
                    card.number = cardNumber(engineNumberGenerator);
                    card.suit = cardSuit(engineNumberGenerator);    

                    if (availableCards[card] == 1)
                    {
                        fiveCardHand[playerCardCounter] = card;
                        playerCardCounter++;
                        availableCards[card] = 0;
                    }
                } while (playerCardCounter < loopEvaluatorRequirement);
            }

            ////////// DECLARATION
            int possibleWinnerIndex = -1, cardIndex = 0;
            int* highCardPtr, *pairPtr, *twoPairPtr, *tripsPtr, *straightPtr, *flushPtr, *fullHousePtr, *quadsPtr, *straightFlushPtr;
            Player_Hand* possibleWinner = new Player_Hand[players]();
            ////////// END DECLARATION

            std::cout << "\n=================================================================\n";
            std::cout << "  THE FOLLOWING LIST SHOWS ALL PLAYER HANDS AND THEIR STRENGTH:";
            std::cout << "\n=================================================================\n";
            
            while (cardIndex < loopEvaluatorRequirement)
            {
                // Sets the index for possibleWinner array
                possibleWinnerIndex++;

                if (!fiveCardGame)
                {
                    // Copies the communnity cards into sevenCardHand array
                    for (int i = 0; i < 5; i++)
                        sevenCardHand[i] = communityCards[i];

                    // Copies 2 cards of the playerCards array into sevenCardHand array
                    for (int i = 5; i < 7; i++)
                    {
                        sevenCardHand[i] = playerCards[cardIndex];
                        cardIndex++;
                    }

                    printCards(sevenCardHand, possibleWinnerIndex, false, 5, 7, false);
                    printCards(sevenCardHand, possibleWinnerIndex, true, 0, 5, false);
                }
                else
                {
                    for (int i = 0; i < 5; i++)
                    {
                        sevenCardHand[i] = fiveCardHand[cardIndex];
                        cardIndex++;
                    }
                    sevenCardHand[5] = {-5, -5};
                    sevenCardHand[6] = {-10, -10};

                    printCards(sevenCardHand, possibleWinnerIndex, false, 0, 5, true);
                }

                // The pointers contain the values that evaluate each hand strength
                straightFlushPtr = straightflush(sevenCardHand);
                quadsPtr = quads(sevenCardHand);
                fullHousePtr = fullHouse(sevenCardHand);
                flushPtr = flush(sevenCardHand);
                straightPtr = straight(sevenCardHand);
                tripsPtr = trips(sevenCardHand);
                twoPairPtr = twoPair(sevenCardHand);
                pairPtr = pair(sevenCardHand);
                highCardPtr = highCard(sevenCardHand);

                ///// The following finds the hand strength for each player /////
                if (straightFlushPtr[0] == 1)
                {
                    setStats(possibleWinner, possibleWinnerIndex, 200, straightFlushPtr[1], 0, 0, 0, 0);
                    if (straightFlushPtr[1] == 14)
                    {
                        std::cout << "\nHand strength: ROYAL FLUSH\n";
                        statsArray[0] += 1;
                    }  
                    else
                    {
                        std::cout << "\nHand strength: STRAIGHT FLUSH\n";
                        statsArray[1] += 1;
                    }
                }
                else if (quadsPtr[0] == 1)
                {
                    std::cout << "\nHand strength: QUADS\n";
                    setStats(possibleWinner, possibleWinnerIndex, 190, quadsPtr[1], quadsPtr[2], 0, 0, 0);
                    statsArray[2] += 1;
                }
                else if (fullHousePtr[0] == 1)
                {
                    std::cout << "\nHand strength: FULL HOUSE\n";
                    setStats(possibleWinner, possibleWinnerIndex, 180, fullHousePtr[1], fullHousePtr[2], 0, 0, 0);
                    statsArray[3] += 1;
                }
                else if (flushPtr[0] == 1)
                {
                    std::cout << "\nHand strength: FLUSH\n";
                    setStats(possibleWinner, possibleWinnerIndex, 170, flushPtr[1], flushPtr[2], flushPtr[3],
                            flushPtr[4], flushPtr[5]);
                    statsArray[4] += 1;
                }
                else if (straightPtr[0] == 1)
                {
                    std::cout << "\nHand strength: STRAIGHT\n";
                    setStats(possibleWinner, possibleWinnerIndex, 160, straightPtr[1], 0, 0, 0, 0);
                    statsArray[5] += 1;
                }
                else if (tripsPtr[0] == 1)
                {
                    std::cout << "\nHand strength: TRIPS\n";
                    setStats(possibleWinner, possibleWinnerIndex, 150, tripsPtr[1], tripsPtr[2], tripsPtr[3], 0, 0);
                    statsArray[6] += 1;
                }
                else if (twoPairPtr[0] == 1)
                {
                    std::cout << "\nHand strength: TWO PAIR\n";
                    setStats(possibleWinner, possibleWinnerIndex, 140, twoPairPtr[1], twoPairPtr[2], twoPairPtr[3], 0, 0);
                    statsArray[7] += 1;
                }
                else if (pairPtr[0] == 1)
                {
                    std::cout << "\nHand strength: PAIR\n";
                    setStats(possibleWinner, possibleWinnerIndex, 130, pairPtr[1], pairPtr[2], pairPtr[3], pairPtr[4], 0);
                    statsArray[8] += 1;
                }
                else
                {
                    std::cout << "\nHand strength: HIGH CARD\n";
                    setStats(possibleWinner, possibleWinnerIndex, highCardPtr[0], highCardPtr[0], highCardPtr[1],
                            highCardPtr[2], highCardPtr[3], highCardPtr[4]);
                    statsArray[9] += 1;
                }
            }

            delete [] playerCards;
            playerCards = 0;
            delete [] fiveCardHand;
            fiveCardHand = 0;

            ////////// DECLARATION
            int* wins = 0;
            int samePointsCounter = 0, trueWinner = 0, numberOfPlayersTied = 0;
            ////////// END DECLARATION

            /*  The rest of this program determines the winner based on kickers and highcards.
            First the program is sorted by player points, and the players with the highest 
            points are compared to each other based on their highcard and then the kickers 
            (if need be). Some only require the points to be compared or some of the kickers. */

            // sortBy, Sorts player hands by whatever is written in the first parameter i.e. "points"
            sortBy("points", possibleWinner, players);
            // counterFor, Counts the amount of players that have the same amount of whatever is written in the first parameter i.e. "points"
            samePointsCounter = counterFor("points", possibleWinner, players);
            
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
                std::cout << "\n    ===---------------------";
                dynamicallyGrowBorder(wins, numberOfPlayersTied);
                std::cout << "------------------===\n";
                std::cout << "        ITS A TIE BETWEEN PLAYERS";
                for (int i = 0; i < numberOfPlayersTied-1; i++)
                    std::cout << " #" << wins[i] << ",";
                std::cout << " and #" << wins[numberOfPlayersTied-1] << "!\n";
                std::cout << "    =====-------------------";
                dynamicallyGrowBorder(wins, numberOfPlayersTied);
                std::cout << "----------------=====\n\n";

                delete [] wins;
                wins = 0;
            }
            else
            {
                std::cout << "\n    WINNER===WINNER===WINNER===WINNER===WINNER===WINNER===WINNER===WINNER\n";
                std::cout << "    ||                      *****"  << " PLAYER #" 
                                                        << trueWinner <<   " *****                      ||\n";
                std::cout << "    WINNER===WINNER===WINNER===WINNER===WINNER===WINNER===WINNER===WINNER\n\n";
            }

            #pragma endregion
        }
        else if (selection == '2')
        {
            continuousLoopOn = true;
            playersPerContinuousLoop = 0;
            numberOfLoops = 0;

            do
            {
                std::cout << "\nFor how many games will you like to speed deal cards?\n" << ">>> ";
                std::cin >> loopingRequirement;

                if (loopingRequirement < 2)
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "\nVALUE MUST BE GREATER THAN 1!";
                }

            } while (loopingRequirement < 2);

            if (!fiveCardGame)
                playersPerContinuousLoop = selectPlayers7game("How many players in each game?");
            else
                playersPerContinuousLoop = selectPlayers5game("How many players in each game?");
            
            std::cout << "\nFor how many games will you like player hand output?\n" << ">>> ";

            while (!(std::cin >> outputGames) || outputGames > loopingRequirement || outputGames < 0)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "\nINVALID AMOUNT! MAX: " << loopingRequirement << " MIN: 0";
                std::cout << "\nFor how many games will you like player hand output?\n" << ">>> ";
            }
        }
        else if (selection == '3' || selection == '4')
        {
            if (selection == '3')
            {
                ////////// DECLARATION
                long double handsDealt = 0;
                ////////// END DECLARATION

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
            else
            {
                for (int i = 0; i < 10; i++)
                    statsArray[i] = 0;
                
                std::cout << "\n** Statistics have been RESET **\n";
            }
        }

        if (numberOfLoops == loopingRequirement)
        {
            if (continuousLoopOn && outputGames == 0)
                std::clog << "FINISHED LOADING!\n\n\n";
            continuousLoopOn = false;
            // Activates output
            if (outputGames == 0)
                std::cout.rdbuf(orig_buf);
        }
        
    } while (selection != 'Q');

    printByeMessage();
    
    return 0;
}


////////////////////////////////////////////////////////////////////////////
/************************** FUNCTION DEFINITIONS **************************/
////////////////////////////////////////////////////////////////////////////


// Returns first menu option
char firstMenuOption()
{
    char selection = 0;

    std::cout << "\nMeNuMeNu%%%===========%%%MeNuMeNu\n";
    std::cout << "    5 - Play 5 card evaluator\n";
    std::cout << "    7 - Play 7 card evaluator\n";
    std::cout << "    Q - Quit\n";
    std::cout << "MeNuMeNu%%%===========%%%MeNuMeNu\n" << ">>> ";;
    std::cin >> selection;

    selection = toupper(selection);

    // Forces a choice of 5,7 or Q
    while (selection != '5' && selection != '7' && selection != 'Q' )
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "\nTRY AGAIN!\n" << ">>> ";
        std::cin >> selection;
        selection = toupper(selection);
    }

    return selection;
}

// Returns the menu selection
char selectMenuOption(bool fiveCardGame)
{
    char selection = 0;

    std::cout << "\nMeNuMeNu%%%========%%%MeNuMeNu\n";
    if (!fiveCardGame)
        std::cout << "     1 - Deal SEVEN cards\n";
    else
        std::cout << "     1 - Deal FIVE cards\n";
    std::cout << "     2 - Speed dealing\n";
    std::cout << "     3 - Statistics\n";
    std::cout << "     4 - Reset Statistics\n";
    std::cout << "     Q - Quit\n";
    std::cout << "MeNuMeNu%%%========%%%MeNuMeNu\n" << ">>> ";;
    std::cin >> selection;

    selection = toupper(selection);

    // Forces a choice of 1, 2, 3, 4, or Q
    while (selection != '1' && selection != '2' && selection != '3' && selection != '4' && selection != 'Q' )
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "\nTRY AGAIN!\n" << ">>> ";
        std::cin >> selection;
        selection = toupper(selection);
    }

    return selection;
}


// Prints message upon exiting
void printByeMessage()
{
    std::cout << "\n  <<****************************>>\n";
    std::cout << "<<ThAnKyOutHaNkYoUThAnKyOutHaNkYoU>>\n";
    std::cout << "     <** THANKS FOR PLAYING **>\n";
    std::cout << "<<ThAnKyOutHaNkYoUThAnKyOutHaNkYoU>>\n";
    std::cout << "  <<****************************>>\n\n";
}


// In seven player game, returns the amount of opponents that will be versed
int selectPlayers7game(std::string question)
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


// In five player game, returns the amount of opponents that will be versed
int selectPlayers5game(std::string question)
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


// Prints out player hand and community cards
void printCards (const struct Card arr[], int possibleWinnerIndex, bool printingCommunityCards, int startPosition, int arraySize, bool fiveCardGame)
{
    std::string handInfo = "";
    if (!printingCommunityCards)
    {
        handInfo += "\n=-- #" + std::to_string(possibleWinnerIndex+1) + " --=";
        if (!fiveCardGame)
            handInfo += "\nPLAYER HAND: "; 
        else
            handInfo += "\nFIVE CARD HAND:"; 
    }

    if (printingCommunityCards)
        handInfo += "\nCOMMUNITY CARDS: \n";

    for (int i = startPosition; i < arraySize; i++)
    {
        if (i == startPosition && printingCommunityCards)
            handInfo += "  * ";
        else if (printingCommunityCards || fiveCardGame)
            handInfo += " * ";
        
        switch(arr[i].number)
        {
            case 1: handInfo += "Ace"; break;
            case 11: handInfo += "Jack"; break;
            case 12: handInfo += "Queen"; break;
            case 13: handInfo += "King"; break;
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

        if (!printingCommunityCards && i != 6 && !fiveCardGame)
            handInfo += ", ";
    }
    std::cout << handInfo;
}


// Dynamically grows border when players tie
void dynamicallyGrowBorder(int arr[], int playersTied)
{
    if ((arr[0] <= 9 && arr[1] >= 10) || (arr[0] >= 10 && arr[1] <= 9))
        std::cout << "-";
    if (arr[0] >= 10 && arr[1] >= 10)
        std::cout << "--";
    if (playersTied > 2)
    {
        for (int i = 2; i < playersTied; i++)
        {
            bool twoDigitNumber = false;

            if (arr[i] >= 10)
                twoDigitNumber = true;

            if (!twoDigitNumber)
                std::cout << "----";
            else
                std::cout << "-----";
        }
    }
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
                Card hold;
                hold = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = hold;
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


// Change value of card to new value, i.e. changing ace value from 1 to 14
void changeCardValue(struct Card arr[], int valueCompared, int newValue)
{
    for (int i = 0; i < 7; i++)
        if (arr[i].number == valueCompared)
            arr[i].number = newValue;
}


// Returns the amount of players that have the same value of the "choice" parameter
int counterFor(std::string choice, const struct Player_Hand arr[], int sizeOfArray)
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
int compareKicker(std::string choice, const struct Player_Hand arr[], int sizeOfArray)
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
int* multipleWinners(const struct Player_Hand arr[], int sizeOfArray)
{
    int* winners = new int[sizeOfArray]();

    for (int i = 0; i < sizeOfArray; i++)
        winners[i] = arr[i].playerNumber;

    return winners;
}


//////////////////// HAND STRENGTH FUNCTIONS ////////////////////
/* SIDE NOTE: The following functions will return a pointer as an
array that returns several values to determine serveral factors, 
which are mentioned specifically at the end of each function */
/* Remember: Aces can act as the number 1 and 14 */
 

int* highCard(const struct Card originalArr[])
{
    static int handScore[5];
    Card arr[7];

    // Resets handScore array to values of 0
    for (int i = 0; i < 5; i++)
        handScore[i]= 0;
    
    // Copies to 'arr' array, so 'originalArr' doesn't change
    for (int i = 0; i < 7; i++)
        arr[i] = originalArr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    // Copies 5 highest cards into handScore array
    for (int i = 0; i < 5; i++)
        handScore[i] = arr[i].number;

    // handScore[0-4] = high card and kickers
    return handScore;
}


int* pair(const struct Card originalArr[])
{
    static int handScore[5];
    Card arr[7];

    // Resets handScore array to values of 0
    for (int i = 0; i < 5; i++)
        handScore[i]= 0;
    
    // Copies to 'arr', so 'originalArr' doesn't change
    for (int i = 0; i < 7; i++)
        arr[i] = originalArr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    // Finds the card number of the pair
    for (int i = 0; i < 7; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            handScore[0] = 1;
            handScore[1] = arr[i].number;
            break;
        }
    }

    // If a pair is present, this sets its 3 kickers
    if (handScore[0] == 1)
    {
        changeCardValue(arr, handScore[1], -1);
        sortCardNumber(arr, 7);
        handScore[2] = arr[0].number;
        handScore[3] = arr[1].number;
        handScore[4] = arr[2].number;
    }

    /* handScore[0] = wether or not there is a single pair, handScore[1] = high card of single pair, handScore[2] = first kicker
    handScore[3] = second kicker, handScore[4] = last kicker */
    return handScore;
}


int* twoPair(const struct Card originalArr[])
{
    static int handScore[4];
    Card arr[7];

    // Resets handScore array to values of 0
    for (int i = 0; i < 4; i++)
        handScore[i]= 0;
    
    // Copies to 'arr', so 'originalArr' doesn't change
    for (int i = 0; i < 7; i++)
        arr[i] = originalArr[i];

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
                handScore[0] = 1;
                handScore[2] = arr[i].number;
                break;
            }
            else
            {
                handScore[1] = arr[i].number;
                checkSecondPair = true;
            }
        }
    }

    // Sets kicker if two pairs are present
    if (handScore[0] == 1)
    {
        changeCardValue(arr, handScore[1], -1);
        changeCardValue(arr, handScore[2], -1);
        sortCardNumber(arr, 7);
        handScore[3] = arr[0].number;
    }

    /* handScore[0] = wether or not there is a two pair, handScore[1] = value of highest pair, 
    handScore[2] = value of second highest pair, handScore[3] = kicker */
    return handScore;
}


int* trips(const struct Card originalArr[])
{
    static int handScore[4];
    Card arr[7];

    // Resets handScore array to values of 0
    for (int i = 0; i < 4; i++)
        handScore[i]= 0;

    // Copies to 'arr', so 'originalArr' doesn't change
    for (int i = 0; i < 7; i++)
        arr[i] = originalArr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    // Finds if trips are present and sets value
    for (int i = 0; i < 5; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number)
        {
            handScore[0] = 1;
            handScore[1] = arr[i].number;
            break;
        }
    }

    // Sets two kickers if trips are present
    if (handScore[0] == 1)
    {
        changeCardValue(arr, handScore[1], -1);
        sortCardNumber(arr, 7);
        handScore[2] = arr[0].number;
        handScore[3] = arr[1].number;
    }

    /* handScore[0] = wether or not there are trips, handScore[1] = trips high card, 
    handScore[2] = first kicker, handScore[3] = second kicker */
    return handScore;
}


int* straight(const struct Card originalArr[])
{
    static int handScore[2];
    Card arr[7];

    // Resets handScore array to values of 0
    for (int i = 0; i < 2; i++)
        handScore[i]= 0;

    // Copies to 'arr', so 'originalArr' doesn't change
    for (int i = 0; i < 7; i++)
        arr[i] = originalArr[i];

    sortCardNumber(arr, 7);

    int counter = 0;
    // Sets all but one of any repeated cards as -1, to not interfer with following algorithm
    for (int i = 0; i < 7-counter; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            Card newCard = {-1, -1};
            arr[i] = newCard;
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
                handScore[0] = 1;
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

    // handScore[0] = wether or not there is a straight, handScore[1] = high card of straight
    return handScore;
}


int* flush(const struct Card originalArr[])
{
    static int handScore[7];
    Card arr[7];

    // Resets handScore array to values of 0
    for (int i = 0; i < 7; i++)
        handScore[i]= 0;

    // Copies to 'arr', so 'originalArr' doesn't change
    for (int i = 0; i < 7; i++)
        arr[i] = originalArr[i];

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
        return handScore;

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
            handScore[0] = 1;
            // Sets the suit of the flush
            handScore[6] = i;
            int indexCounter = 0;
            // Sets the high card and kickers
            for (int j = 0; j < 7; j++)
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

    /* handScore[0] = wether or not there is a flush, handScore[1] = high card of flush, 
    handScore[2-5] = kickers, handScore[6] = suit of flush */
    return handScore;
}


int* fullHouse(const struct Card originalArr[])
{
    static int handScore[3];
    Card arr[7];

    // Resets handScore array to values of 0
    for (int i = 0; i < 3; i++)
        handScore[i]= 0;

    // Copies to 'arr', so 'originalArr' doesn't change
    for (int i = 0; i < 7; i++)
        arr[i] = originalArr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    bool tripsPresent = false, pairPresent = false;
    // Finds if trips are present
    for (int i = 0; i < 5; i++)
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
        sortCardNumber(arr, 7);
        
        // Checks for a pair
        for (int i = 0; i < 3; i++)
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
        handScore[0] = 1;

    /* handScore[0] = wether or not there is a full house, handScore[1] = high card of trips,
    handScore[2] = high card of pair */
    return handScore;
}


int* quads(const struct Card originalArr[])
{
    static int handScore[3];
    Card arr[7];

    // Resets handScore array to values of 0
    for (int i = 0; i < 3; i++)
        handScore[i]= 0;

    // Copies to 'arr', so 'originalArr' doesn't change
    for (int i = 0; i < 7; i++)
        arr[i] = originalArr[i];

    changeCardValue(arr, 1, 14);
    sortCardNumber(arr, 7);

    // Checks if there are quads present
    for (int i = 0; i < 4; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number &&
            arr[i].number == arr[i+3].number)
        {
            handScore[0] = 1;
            handScore[1] = arr[i].number;
        }
    }

    // If quads are present, the kicker is added
    if (handScore[0] == 1)
    {
        changeCardValue(arr, handScore[1], -1);
        sortCardNumber(arr, 7);
        handScore[2] = arr[0].number;
    }

    // handScore[0] = wether or not there are quads, handScore[1] = high card of quads, handScore[2] = kicker
    return handScore;
}


int* straightflush(const struct Card originalArr[])
{
    static int handScore[2];
    Card arr[7];

    // Resets handScore array to values of 0
    for (int i = 0; i < 2; i++)
        handScore[i]= 0;

    // Copies to 'arr', so 'originalArr' doesn't change
    for (int i = 0; i < 7; i++)
        arr[i] = originalArr[i];

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

        if (straightPtr[0] == 1)
        {
            handScore[1] = straightPtr[1];
            straightPresent = true;
        }
    }

    if (straightPresent && flushPresent)
        handScore[0] = 1;

    // handScore[0] = wether or not there is a straightflush, handScore[1] = high card of straightflush
    return handScore;
}