#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <limits>

// Terminal command to compile: g++ poker1.cpp -std=c++0x
// Terminal command to execute: ./a.out

struct Card
{
    int number;
    int suit;
};

struct Player_Hand
{
    int player_number;
    int high_card;
    int points;
    int kicker1;
    int kicker2;
    int kicker3;
    int kicker4;
};

void sort_card_number(struct Card arr[], int size_of_array);
void set_stats(struct Player_Hand arr[], int index, int points_earned, int high_card, int k1, int k2, int k3, int k4);
void sort_by(std::string choice, struct Player_Hand arr[], int size_of_array);
int counter_for(std::string choice, struct Player_Hand arr[], int size_of_array);
int compare_kicker(std::string choice, struct Player_Hand arr[], int size_of_array);
int* multiple_winners(struct Player_Hand arr[], int size_of_array);

// Card strength functions
int* high_card(struct Card arr[]);
int* pair(struct Card arr[]);
int* two_pair(struct Card arr[]);
int* trips(struct Card arr[]);
int* straight(struct Card arr[]);
int* flush(struct Card arr[]);
int* full_house(struct Card arr[]);
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
    std::cout << "========---------------------------------========\n\n";

    ////////// DECLARATION
    // Seeds time for random number generation
    srand(time(0));
    int opponents = 0;
    ////////// END DECLARATION

    #pragma region Inputting Opponent Number

    do
    {
        std::cout << "How many opponents do you want to play with?\n" << ">>> ";
        std::cin >> opponents;

        if (opponents > 22)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nONLY A MAXIMUM OF 22 OPPONENTS ARE POSSIBLE!\n";
        }
        if (opponents <= 0)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nMORE OPPONENTS ARE NEEDED!\n";
        }
    } while (opponents > 22 || opponents < 1);

    #pragma endregion

    ////////// DECLARATION
    std::map <Card, int> available_cards;
    ////////// END DECLARATION

    // Generates hash map of 52 card deck
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            const Card cardToInsert = * new Card{i+1, j+1};
            available_cards.insert(std::pair<Card, int>(cardToInsert, 1));
        }
    }
    
    ////////// DECLARATION
    int community_counter = 0, player_card_counter = 0;
    Card card, pre_evaluation[5], evaluation[7];
    Card* player_card = new Card[(opponents+1)*2];
    ////////// END DECLARATION

    // Creates distinct cards for all players and community cards
    do
    {
        card.number = rand() % 13 + 1;
        card.suit = rand() % 4 + 1;

        if (available_cards[card] == 1)
        {
            if (community_counter < 5)
            {
                pre_evaluation[community_counter] = card;
                community_counter++;
            }
            else
            {
                player_card[player_card_counter] = card;
                player_card_counter++;
            }
            available_cards[card] = 0;
        }
    } while (player_card_counter < (opponents+1)*2);

    ////////// DECLARATION
    int index_counter = -1;
    int *high_card1, *pair1, *two_pair1, *trips1, *straight1, *flush1, *full_house1, *quads1, *straightflush1;
    Player_Hand* possible_winner = new Player_Hand[opponents+1];
    ////////// END DECLARATION

    player_card_counter = 0;

    std::cout << "\nTHE FOLLOWING LIST SHOWS ALL PLAYER HANDS AND THEIR STRENGTH:\n";

    while (player_card_counter < (opponents+1)*2)
    {
        // Sets the index for winner array
        index_counter++;

        // Copies the communnity cards into evaluation array
        for (int i = 0; i < 5; i++)
            evaluation[i] = pre_evaluation[i];

        // Copies 2 cards of the player_card array into evaluation array
        for (int i = 5; i < 7; i++)
        {
            evaluation[i] = player_card[player_card_counter];
            player_card_counter++;
        }

        #pragma region Outputting Player Hand
        
        // Prints out player number
        if (index_counter == 0)
            std::cout << "\nPlayer #"<< index_counter+1 << "(YOU)" << std::endl;
        else
            std::cout << "\nPlayer #"<< index_counter+1 << std::endl;

        // Prints out community cards and player cards for each player
        std::cout << "Community Cards: {";
        for(int i = 0; i < 4; i++)
            std::cout << "{" << evaluation[i].number << ", " << evaluation[i].suit << "}, ";
        std::cout << "{" << evaluation[4].number << ", " << evaluation[4].suit << "}}";
        std::cout << "\nPlayer Hand: {{" << evaluation[5].number << ", " << evaluation[5].suit << "}, ";
        std::cout << "{" << evaluation[6].number << ", " << evaluation[6].suit << "}}\n";

        #pragma endregion

        // The pointers contain the values that evaluate each hand strength
        straightflush1 = straightflush(evaluation);
        quads1 = quads(evaluation);
        full_house1 = full_house(evaluation);
        flush1 = flush(evaluation);
        straight1 = straight(evaluation);
        trips1 = trips(evaluation);
        two_pair1 = two_pair(evaluation);
        pair1 = pair(evaluation);
        high_card1 = high_card(evaluation);

        ///// The following finds the hand strength for each player /////
        if (straightflush1[0] == 1)
        {
            std::cout << "Hand strength: STRAIGHT FLUSH" << std::endl;
            set_stats(possible_winner, index_counter, 200, straightflush1[1], 0, 0, 0, 0);
        }
        else if (quads1[0] == 1)
        {
            std::cout << "Hand strength: QUADS" << std::endl;
            set_stats(possible_winner, index_counter, 190, quads1[1], quads1[2], 0, 0, 0);
        }
        else if (full_house1[0] == 1)
        {
            std::cout << "Hand strength: FULL HOUSE" << std::endl;
            set_stats(possible_winner, index_counter, 180, full_house1[1], full_house1[2], 0, 0, 0);
        }
        else if (flush1[0] == 1)
        {
            std::cout << "Hand strength: FLUSH" << std::endl;
            set_stats(possible_winner, index_counter, 170, flush1[1], flush1[2], flush1[3],
                      flush1[4], flush1[5]);
        }
        else if (straight1[0] == 1)
        {
            std::cout << "Hand strength: STRAIGHT" << std::endl;
            set_stats(possible_winner, index_counter, 160, straight1[1], 0, 0, 0, 0);
        }
        else if (trips1[0] == 1)
        {
            std::cout << "Hand strength: TRIPS" << std::endl;
            set_stats(possible_winner, index_counter, 150, trips1[1], trips1[2], trips1[3], 0, 0);
        }
        else if (two_pair1[0] == 1)
        {
            std::cout << "Hand strength: TWO PAIR" << std::endl;
            set_stats(possible_winner, index_counter, 140, two_pair1[1], two_pair1[2], two_pair1[3], 0, 0);
        }
        else if (pair1[0] == 1)
        {
            std::cout << "Hand strength: PAIR" << std::endl;
            set_stats(possible_winner, index_counter, 130, pair1[1], pair1[2], pair1[3], pair1[4], 0);
        }
        else
        {
            std::cout << "Hand strength: HIGH CARD" << std::endl;
            set_stats(possible_winner, index_counter, high_card1[0], high_card1[0], high_card1[1],
                    high_card1[2], high_card1[3], high_card1[4]);
        }

        #pragma region Memory Deallocation

        // Deallocates memory from the functions
        delete [] straightflush1; 
        straightflush1 = 0;
        delete [] quads1; 
        quads1 = 0;
        delete [] full_house1; 
        full_house1 = 0;
        delete [] flush1; 
        flush1 = 0; 
        delete [] straight1; 
        straight1 = 0;
        delete [] trips1;
        trips1 = 0;
        delete [] two_pair1;
        two_pair1 = 0;
        delete [] pair1; 
        pair1 = 0;
        delete [] high_card1; 
        high_card1 = 0;

        #pragma endregion
    }

    delete [] player_card;
    player_card = 0;

    ////////// DECLARATION
    int* wins;
    int same_points_counter = 0, true_winner = 0;
    bool multiple_wins = false;
    ////////// END DECLARATION

    /*  The rest of this program determines the winner based on kickers and highcards
    First the program is sorted by player points, and the highest players points are 
    compared, then the high card (if need be), then the kickers (if need be). Some only 
    require the highcards to be compared or some of the kickers. */

    // sort_by, Sorts player hands by points (whatever is written in the first parameter)
    sort_by("points", possible_winner, opponents+1);
    // Counts the amount of players that have the same amount of points (whatever is written in the first parameter)
    same_points_counter = counter_for("points", possible_winner, opponents+1);
    
    // If no more than 1 player is counted to have a certain amount of points, then they are the true winner
    /**** The rest of the program follows a similar pattern to the one explained above ****/
    if (same_points_counter == 1)
        true_winner = possible_winner[0].player_number;
    else
    {
        int same_high_card_counter = 1;

        sort_by("highcard", possible_winner, same_points_counter);
        same_high_card_counter = counter_for("highcard", possible_winner, same_points_counter);

        // Finds winner for straightflush and straight
        if ((same_high_card_counter > 1 && possible_winner[0].points == 200) || 
            (same_high_card_counter > 1 && possible_winner[0].points == 160))
        {
            wins = multiple_winners(possible_winner, same_high_card_counter);
            multiple_wins = true;
        }
        else if (same_high_card_counter > 1)
        {
            int same_kicker_1_counter = 1;

            sort_by("kicker1", possible_winner, same_high_card_counter);
            same_kicker_1_counter = counter_for("kicker1", possible_winner, same_high_card_counter);

            int same_kicker_2_counter = 1, same_kicker_3_counter = 1, same_kicker_4_counter = 1;

            switch (possible_winner[0].points)
            {
                case 190: // Finds winner for quads and fullhouse
                case 180:
                {
                    if (same_kicker_1_counter > 1)
                    {
                        wins = multiple_winners(possible_winner, same_high_card_counter);
                        multiple_wins = true;
                    }
                    else
                        true_winner = compare_kicker("kicker1", possible_winner, same_high_card_counter);
                    break;
                }
                case 150: // Finds winner for trips and two pair
                case 140:
                {
                    if (same_kicker_1_counter > 1)
                    {
                        sort_by("kicker2", possible_winner, same_kicker_1_counter);
                        same_kicker_2_counter = counter_for("kicker2", possible_winner, same_kicker_1_counter);

                        if (same_kicker_2_counter > 1)
                        {
                            wins = multiple_winners(possible_winner, same_kicker_2_counter);
                            multiple_wins = true;
                        }
                        else
                            true_winner = compare_kicker("kicker2", possible_winner, same_kicker_1_counter);
                    }
                    else
                        true_winner = compare_kicker("kicker1", possible_winner, same_high_card_counter);
                    break;
                }
                case 130: // Finds winner for single pair
                {
                    if (same_kicker_1_counter > 1)
                    {
                        sort_by("kicker2", possible_winner, same_kicker_1_counter);
                        same_kicker_2_counter = counter_for("kicker2", possible_winner, same_kicker_1_counter);

                        if (same_kicker_2_counter > 1)
                        {
                            sort_by("kicker3", possible_winner, same_kicker_2_counter);
                            same_kicker_3_counter = counter_for("kicker3", possible_winner, same_kicker_2_counter);

                            if (same_kicker_3_counter > 1)
                            {
                                wins = multiple_winners(possible_winner, same_kicker_3_counter);
                                multiple_wins = true;
                            }
                            else
                                true_winner = compare_kicker("kicker3", possible_winner, same_kicker_2_counter);
                        }
                        else
                            true_winner = compare_kicker("kicker2", possible_winner, same_kicker_1_counter);
                    }
                    else
                        true_winner = compare_kicker("kicker1", possible_winner, same_high_card_counter);
                    break;
                }
                case 170: // Finds winner for flush and high cards
                default:
                {
                    if (same_kicker_1_counter > 1)
                    {
                        sort_by("kicker2", possible_winner, same_kicker_1_counter);
                        same_kicker_2_counter = counter_for("kicker2", possible_winner, same_kicker_1_counter);

                        if (same_kicker_2_counter > 1)
                        {
                            sort_by("kicker3", possible_winner, same_kicker_2_counter);
                            same_kicker_3_counter = counter_for("kicker3", possible_winner, same_kicker_2_counter);

                            if (same_kicker_3_counter > 1)
                            {
                                sort_by("kicker4", possible_winner, same_kicker_3_counter);
                                same_kicker_4_counter = counter_for("kicker4", possible_winner, same_kicker_3_counter);

                                if (same_kicker_4_counter > 1)
                                {
                                    wins = multiple_winners(possible_winner, same_kicker_4_counter);
                                    multiple_wins = true;
                                }
                                else
                                    true_winner = compare_kicker("kicker4", possible_winner, same_kicker_3_counter);
                            }
                            else
                                true_winner = compare_kicker("kicker3", possible_winner, same_kicker_2_counter);
                        }
                        else
                            true_winner = compare_kicker("kicker2", possible_winner, same_kicker_1_counter);
                    }
                    else
                        true_winner = compare_kicker("kicker1", possible_winner, same_high_card_counter);
                    break;
                }
            }
        }
        else
            true_winner = possible_winner[0].player_number;
    }

    delete [] possible_winner;
    possible_winner = 0;

    #pragma region Handles Output For Winning Hand(s)

    if (multiple_wins)
    {
        std::cout << "\nITS A TIE BETWEEN PLAYERS";
        for (int i = 2; i < (wins[1])+1; i++)
            std::cout << " #" << wins[i] << ",";
        std::cout << " and #" << wins[(wins[1])+1] << "!\n\n";

        delete [] wins;
        wins = 0;
    }
    else
    {
        std::cout << "\n    <******* THE WINNER IS *******>\n";
        std::cout << "      -----    PLAYER #" << true_winner << "    -----" << std::endl;
        std::cout << "   <**** !! CONGRATULATIONS !! ****>\n\n";
    }

    #pragma endregion

    return 0;
}


////////////////////////////////////////////////////////////////////////////
/************************** FUNCTION DEFINITIONS **************************/
////////////////////////////////////////////////////////////////////////////


// Sorts array from highest number to lowest card number regardless of suit
void sort_card_number(struct Card arr[], int size_of_array)
{
    for (int i = 0; i < size_of_array-1; i++)
    {
        for (int j = 0; j < size_of_array-i-1; j++)
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
void set_stats(struct Player_Hand arr[], int index, int points_earned, int high_card, int k1, int k2, int k3, int k4)
{
    arr[index].player_number = index+1;
    arr[index].points = points_earned;
    arr[index].high_card = high_card;
    arr[index].kicker1 = k1;
    arr[index].kicker2 = k2; 
    arr[index].kicker3 = k3;
    arr[index].kicker4 = k4;
}


// Sorts player hand array based on specified parameters from highest value to lowest value
void sort_by(std::string choice, struct Player_Hand arr[], int size_of_array)
{
    bool swap_elements = false;
    for (int i = 0; i < size_of_array-1; i++)
    {
        for (int j = 0; j < size_of_array-i-1; j++)
        {
            if (choice == "highcard" && arr[j].high_card < arr[j+1].high_card)
                swap_elements = true;
            else if (choice == "points" && arr[j].points < arr[j+1].points)
                swap_elements = true;
            else if (choice == "kicker1" && arr[j].kicker1 < arr[j+1].kicker1)
                swap_elements = true;
            else if (choice == "kicker2" && arr[j].kicker2 < arr[j+1].kicker2)
                swap_elements = true;
            else if (choice == "kicker3" && arr[j].kicker3 < arr[j+1].kicker3)
                swap_elements = true;
            else if (choice == "kicker4" && arr[j].kicker4 < arr[j+1].kicker4)
                swap_elements = true;

            if (swap_elements)
            {
                Player_Hand temp;
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
            swap_elements = false;
        }
    }
}


// Returns the counted amount of specified elements that have the same value
int counter_for(std::string choice, struct Player_Hand arr[], int size_of_array)
{
    int new_counter = 1, hold = 0;
    for (int i = 0; i < size_of_array-1; i++)
    {
        hold = new_counter;

        if (choice == "highcard" && arr[i].high_card == arr[i+1].high_card)
            new_counter++;
        else if (choice == "points" && arr[i].points == arr[i+1].points)
            new_counter++;
        else if (choice == "kicker1" && arr[i].kicker1 == arr[i+1].kicker1)
            new_counter++;
        else if (choice == "kicker2" && arr[i].kicker2 == arr[i+1].kicker2)
            new_counter++;
        else if (choice == "kicker3" && arr[i].kicker3 == arr[i+1].kicker3)
            new_counter++;
        else if (choice == "kicker4" && arr[i].kicker4 == arr[i+1].kicker4)
            new_counter++;

        if (hold == new_counter)
            break;
    }

    return new_counter;
}


// Compares kickers and retunrs the larger value
int compare_kicker(std::string choice, struct Player_Hand arr[], int size_of_array)
{
    int true_winner = 0;
    for (int i = 0; i < size_of_array-1; i++)
    {
        if (choice == "kicker1")
        {
            if (arr[i].kicker1 > arr[i+1].kicker1)
                true_winner = arr[i].player_number;
            else if (arr[i].kicker1 < arr[i+1].kicker1)
                true_winner = arr[i+1].player_number;
        }
        else if (choice == "kicker2")
        {
            if (arr[i].kicker2 > arr[i+1].kicker2)
                true_winner = arr[i].player_number;
            else if (arr[i].kicker2 < arr[i+1].kicker2)
                true_winner = arr[i+1].player_number;
        }
        else if (choice == "kicker3")
        {
            if (arr[i].kicker3 > arr[i+1].kicker3)
                true_winner = arr[i].player_number;
            else if (arr[i].kicker3 < arr[i+1].kicker3)
                true_winner = arr[i+1].player_number;
        }
        else if (choice == "kicker4")
        {
            if (arr[i].kicker4 > arr[i+1].kicker4)
                true_winner = arr[i].player_number;
            else if (arr[i].kicker4 < arr[i+1].kicker4)
                true_winner = arr[i+1].player_number;
        }

        if (true_winner > 0)
            break;
    }

    return true_winner;
}


// Finds the player number of the multiple winners
int* multiple_winners(struct Player_Hand arr[], int size_of_array)
{
    int j = 2, counter = 1;
    int *status = new int[size_of_array+2];

    for (int i = 0; i < size_of_array-1; i++)
    {
        counter++;
        status[j] = arr[i].player_number;
        j++;
        if (i == size_of_array)
            status[j] = arr[i+1].player_number;
    }

    // status[0] indicates that there were multiple winners (1 = true, 0 = false)
    status[0] = 1;
    // status[1] = how many ties there are
    status[1] = counter;

    return status;
}


//////////////////// HAND STRENGTH FUNCTIONS ////////////////////
/* SIDE NOTE: The following functions will return an array "status" 
that returns several values to determine serveral factors, which
are mentioned specifically at the end of each function */
/* Remember: Aces can act as the number 1 and 14 */
 

int* high_card(struct Card arr[])
{
    int* status = new int[5];

    // Changes Ace value from 1 to 14
    for (int i = 0; i < 7; i++)
        if (arr[i].number == 1)
            arr[i].number = 14;

    sort_card_number(arr, 7);

    // Copies 5 highest cards into status array
    for (int i = 0; i < 5; i++)
        status[i] = arr[i].number;

    // status[0-4] = kickers
    return status;
}


int* pair(struct Card arr[])
{
    int* status = new int[5];
    Card hold[7];
    
    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    // Changes Ace value from 1 to 14
    for (int i = 0; i < 7; i++)
        if (arr[i].number == 1)
            arr[i].number = 14;

    sort_card_number(arr, 7);

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
        for (int i = 0; i < 7; i++)
            if (arr[i].number == status[1])
                arr[i].number = -1;

        sort_card_number(arr, 7);
        status[2] = arr[0].number;
        status[3] = arr[1].number;
        status[4] = arr[2].number;
    }

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether single pair is present or not, status[1] = highcard of single pair, status[2] = first kicker
    status[3] = second kicker, status[4] = last kicker */
    return status;
}


int* two_pair(struct Card arr[])
{
    int* status = new int[4];
    Card hold[7];
    
    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    // Changes Ace value from 1 to 14
    for (int i = 0; i < 7; i++)
        if (arr[i].number == 1)
            arr[i].number = 14;

    sort_card_number(arr, 7);

    int counter = 0;
    // Finds two pairs and sets their values
    for (int i = 0; i < 7; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            counter++;
            if (counter > 1)
            {
                status[0] = 1;
                status[2] = arr[i].number;
                break;
            }
            else
                status[1] = arr[i].number;
        }
    }

    // Sets kicker if two pairs are present
    if (status[0] == 1)
    {
        for (int i = 0; i < 7; i++)
            if (arr[i].number == status[1] || arr[i].number == status[2])
                arr[i].number = -1;

        sort_card_number(arr, 7);
        status[3] = arr[0].number;
    }

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether a two pair is present or not, status[1] = value of highest pair, 
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

    // Changes Ace value from 1 to 14
    for (int i = 0; i < 7; i++)
        if (arr[i].number == 1)
            arr[i].number = 14;

    sort_card_number(arr, 7);

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
        for (int i = 0; i < 7; i++)
            if (arr[i].number == status[1])
                arr[i].number = -1;
        
        sort_card_number(arr, 7);
        status[2] = arr[0].number;
        status[3] = arr[1].number;
    }

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether or not trips are present, status[1] = trips high card, 
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

    sort_card_number(arr, 7);

    int counter = 0;
    // Sets all but one of any repeated cards as -1, to not interfer with following algorithm
    for (int i = 0; i < 7-counter; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            Card temp = {-1, -1};
            arr[i] = temp;
            sort_card_number(arr, 7);
            counter++;
            i = 0;
        }
    }

    sort_card_number(arr, 7);
    
    bool ace = false;
    // Determines if there is a straight taking into account the ace as number 1 and 14
    do
    {
        // If ace is present algorithm will check for ace high straight
        if (ace)
            sort_card_number(arr, 7);

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

    // status[0] = wether or not there is a straight, status[1] = highcard of straight
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

    sort_card_number(arr, 7);

    bool ace = false;
    // Similar to straight function, finds highest number for the straight
    do
    {
        // If ace is present algorithm will check for ace high straight
        if (ace)
            sort_card_number(arr, 7);

        // Sets the highcard and kickers
        for (int i = 1; i < 5; i++)
        {
            if ((counter1 >= 5 && i == 1) || (counter2 >= 5 && i == 2) ||
                (counter3 >= 5 && i == 3) || (counter4 >= 5 && i == 4))
            {
                status[0] = 1;
                status[6] = i;
                int status_counter = 0;
                for (int j = 0; j < 7; j++)
                {
                    if (arr[j].suit == i)
                    {
                        status_counter++;
                        switch(status_counter)
                        {
                            case 1:
                                status[1] = arr[j].number;
                                break;
                            case 2:
                                status[2] = arr[j].number;
                                break;
                            case 3:
                                status[3] = arr[j].number;
                                break;
                            case 4:
                                status[4] = arr[j].number;
                                break;
                            case 5:
                                status[5] = arr[j].number;
                                break;
                        }
                    }
                }
                break;
            }
        }

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

    // Hold array copies old values back after collecting information about the array for the function
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether or not there is a flush, status[1] = highcard of flush, 
    status[2-5] = kickers, status[6] = suit of flush */
    return status;
}


int* full_house(struct Card arr[])
{
    int *status = new int[3];
    Card hold[7];

    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    // Changes Ace value from 1 to 14
    for (int i = 0; i < 7; i++)
        if (arr[i].number == 1)
            arr[i].number = 14;

    sort_card_number(arr, 7);

    bool trips_present = false, pair_present = false;
    // Finds if trips are present
    for (int i = 0; i < 5; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number)
        {
            status[1] = arr[i].number;
            trips_present = true;
            break;
        }
    }

    if (trips_present)
    {
        // Removes trips so trips arent confused as pairs
        for (int i = 0; i < 7; i++)
            if (arr[i].number == status[1])
                arr[i].number = -1;

        sort_card_number(arr, 7);
        
        // Checks for a pair
        for (int i = 0; i < 3; i++)
        {
            if (arr[i].number == arr[i+1].number)
            {
                status[2] = arr[i].number;
                pair_present = true;
                break;
            }
        }
    }

    // If pair and trips are present, then full house is true
    if (pair_present && trips_present)
        status[0] = 1;

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether or not there is a full house, status[1] = highcard of trips,
    status[2] = highcard of pair */
    return status;
}


int* quads(struct Card arr[])
{
    int* status = new int[3];
    Card hold[7];

    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    sort_card_number(arr, 7);

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
        for (int i = 0; i < 7; i++)
            if (arr[i].number == status[1])
                arr[i].number = -1;
    
        sort_card_number(arr, 7);
        status[2] = arr[0].number;
    }

    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    // status[0] = wether or not there are quads, status[1] = highcard of quads, status[2] = kicker
    return status;
}


int* straightflush(struct Card arr[])
{
    int* status = new int[2];
    Card hold[7];

    // Creates a hold array before manipulation, so original array is not changed
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    int* flush1 = flush(arr);
    int counter = 0;
    bool skip_straight = false;
    // Chekcs for a flush
    if (flush1[0] == 1)
    {
        // If a card suit is not corresponding to the flush it is removed to prevent interference with algorithm
        for (int i = 0; i < 7; i++)
        {
            if (flush1[6] != arr[i].suit)
            {
                Card temp = {-1, -1};
                arr[i] = temp;
            }
        }
    }
    else
        skip_straight = true;
    
    delete [] flush1;
    flush1 = 0;

    // Checks for a straight
    if (!skip_straight)
    {
        sort_card_number(arr, 7);
    
        bool ace = false;
        // Determines if there is a straight taking into account the ace as number 1 and 14
        do
        {
            // If ace is present algorithm will check for ace high straight
            if (ace)
                sort_card_number(arr, 7);

            // Checks for straight
            for (int i = 0; i < 3-counter; i++)
            {
                if (arr[i].number == arr[i+1].number+1 && arr[i+1].number == arr[i+2].number+1 &&
                    arr[i+2].number == arr[i+3].number+1 && arr[i+3].number == arr[i+4].number+1)
                {
                    status[0] = 1;
                    status[1] = arr[i].number;
                    break;
                }
            }

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
    }
    
    // Copies back hold array into original array after manipulation
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    // status[0] = wether or not there is a straightflush, status[1] = highcard of straightflush
    return status;
}