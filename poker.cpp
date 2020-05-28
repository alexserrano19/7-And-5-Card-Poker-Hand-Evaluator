
var tet =0;



    int high_card;
    int points;
    int kicker1;
    int kicker2;
    int kicker3;
    int kicker4;
};

void menu();
void order_number(struct Card arr[], int size_of_array);
void order(struct Eval_Card arr[], int size_of_array, std::string choice);
void set_stats(struct Eval_Card arr[], int size_of_array, int p, int hc, int k1, int k2, int k3, int k4);
int counter(struct Eval_Card arr[], int size_of_array, std::string choice);
int compare_kicker(struct Eval_Card arr[], int size_of_array, std::string choice);
int* multiple_wins(struct Eval_Card arr[], int size_of_array);
int* high_card(struct Card arr[]);
int* pairs(struct Card arr[]);
int* trips(struct Card arr[]);
int* straight(struct Card arr[]);
int* flush(struct Card arr[]);
int* quads(struct Card arr[]);
int* straightflush(struct Card arr[]);

bool operator<(const Card& t1, const Card& t2)
{
    if (t1.number == t2.number)
        return (t1.suit < t2.suit);
    else
        return (t1.number < t2.number);
}

int main()
{
    int opponents = 0, flop_turn_river_counter = 0, inserted_card_counter = 0, w_counter = 0, same_points_counter = 0;
    int final_winner = 0, selection = 0;
    bool multiple_winners = false, option2 = false;
    std::map <Card, int> available_cards;
    Card card, pre_eval[5], eval[7];
    int *high_card1, *pairs1, *trips1, *straight1, *flush1, *quads1, *straightflush1, *wins;
    Card * player_card = new Card[(opponents+1)*2];
    Eval_Card * winner = new Eval_Card[opponents+1];

    srand(time(0));

    std::cout << "\n========---------------------------------========\n";
    std::cout << " ***** !! pOkEr PoKeR POKER PoKeR pOkEr !! ***** \n";
    std::cout << "========---------------------------------========\n\n";

    menu();
    do
    {
        std::cin >> selection;
        if (selection < 1 || selection > 2)
        {
            std::cout << "Invalid option... Please TRY AGAIN!!\n";
            std::cout << "-----> ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (selection < 1 || selection > 2);

    if (selection == 1)
    {
        // Sets the desired amount of opponents to play against
        do
        {
            std::cout << "\nHow many opponents do you want to play with?\n" << ">>> ";
            std::cin >> opponents;

            if (opponents > 22)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nONLY A MAXIMUM OF 22 OPPONENTS ARE POSSIBLE!";
            }
            if (opponents <= 0)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nMORE OPPONENTS ARE NEEDED!";
            }
        } while (opponents > 22 || opponents < 1);

        std::cout << "\nTHE FOLLOWING LIST SHOWS ALL PLAYER HANDS AND THEIR STRENGTH:\n";

        // Sets all possible cards in 52 card deck into a hashmap, from numbers 1-13 and suit 1-4
        for (int i = 0; i < 13; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                const Card cardToInsert = * new Card{i+1, j+1};
                available_cards.insert(std::pair<Card, int>(cardToInsert, 1));
            }
        }

        // Selects a random and unique pair if cards for a player
        do
        {
            card.number = rand() % 13 + 1;
            card.suit = rand() % 4 + 1;

            // If a card is not already picked the key value is 1 and saves the card into an array
            if (available_cards[card] == 1)
            {
                // Saves first 5 picked cards into a "pre_evaluation" array
                if (flop_turn_river_counter < 5)
                {
                    pre_eval[flop_turn_river_counter] = card;
                    flop_turn_river_counter++;
                }
                else
                {
                    player_card[inserted_card_counter] = card;
                    inserted_card_counter++;
                }
                // Once a card is picked the key value is changed to 0
                available_cards[card] = 0;
            }
        } while (inserted_card_counter < ((opponents+1)*2)+1);
    }
    else
    {
        option2 = true;
        std::cout << "\nEnter the community cards (flop, turn, and river):\n";
        std::cout << "** Enter the cards with number first and suit second with everything separated by a whitespace **\n";
        std::cout << "** Suits are numbers too, for example, Ace of spades is '1 4' or '1 3', etc. **\n";
        std::cout << "** Numbers range from 1 to 13 and the suit ranges from 1 to 4 **\n";
        std::cout << "** Input example '1 2 3 2 8 4 13 1 13 2' = {{1,2},{3,2},{8,4},{13,1},{13,2}} => {number, suit} **\n";
        std::cout << "-----> ";
        for (int i = 0; i < 5; i++)
        {
            std::cin >> pre_eval[i].number;
            std::cin >> pre_eval[i].suit;
        }
        std::cout << "\nEnter the player hand:" << std::endl;
        std::cout << "** Same restricitons from before apply **" << std::endl;
        std::cout << "-----> ";
        for (int i = 0; i < 2; i++)
        {
            std::cin >> player_card[i].number;
            std::cin >> player_card[i].suit;
        }
    }

    // Restart counter
    inserted_card_counter = 0;

    // Entire do while loop evaluates card strength for each player
    do
    {
        // Copies all pre_eval array (flop, turn, and river cards) into larger "evaluation array"
        for (int i = 0; i < 5; i++)
            eval[i] = pre_eval[i];

        /* Copies index 0 and 1 from player_cards into last two spots of the evaluation array... every 2 indexes (0/1,2/3,3/4, etc.)
        act as a player hand dealt during pre_flop*/ 
        for (int i = 5; i < 7; i++)
        {
            eval[i] = player_card[inserted_card_counter];
            inserted_card_counter++;
        }

        // Player hands and and community cards are displayed
        if (w_counter == 0 && !option2)
            std::cout << "\nPlayer #"<< w_counter+1 << "(YOU)";
        else if (!option2)
            std::cout << "Player #"<< w_counter+1;
        std::cout << "\nCommunity Cards: {";
        for (int i = 0; i < 4; i++)
            std::cout << "{" << eval[i].number << ", " << eval[i].suit << "}, ";
        std::cout << "{" << eval[4].number << ", " << eval[4].suit << "}}";
        std::cout << "\nPlayer Hand: {{" << eval[5].number << ", " << eval[5].suit << "}, ";
        std::cout << "{" << eval[6].number << ", " << eval[6].suit << "}}\n";

        // Following variables ending in "1" are pointers for results returned after running each function that evaluates hand strength
        // I SUGGEST READING FUNCTION DEFINITIONS TO UNDERSTAND THE PUROPSE OF EACH FUNCTION & ITS WORKINGS
        high_card1 = high_card(eval);
        pairs1 = pairs(eval);
        trips1 = trips(eval);
        straight1 = straight(eval);
        flush1 = flush(eval);
        quads1 = quads(eval);
        straightflush1 = straightflush(eval);

        // Finds best 5 card combination for each hand and sets all their values into an array for further evaluation of ties and wins
        if (straightflush1[0] == 1)
        {
            std::cout << "Hand strength: STRAIGHT FLUSH\n" << std::endl;
            set_stats(winner, w_counter, 200, straightflush1[1], 0, 0, 0, 0);
        }
        else if (quads1[0] == 1)
        {
            std::cout << "Hand strength: QUADS\n" << std::endl;
            set_stats(winner, w_counter, 190, quads1[1], quads1[2], 0, 0, 0);
        }
        else if (pairs1[0] == 1 && trips1[0] == 1)
        {
            std::cout << "Hand strength: FULL HOUSE\n" << std::endl;
            set_stats(winner, w_counter, 180, trips1[1], pairs1[1], pairs1[1], 0, 0);
        }
        else if (flush1[0] == 1)
        {
            std::cout << "Hand strength: FLUSH\n" << std::endl;
            set_stats(winner, w_counter, 170, flush1[1], flush1[2], flush1[3],
                      flush1[4], flush1[5]);
        }
        else if (straight1[0] == 1)
        {
            std::cout << "Hand strength: STRAIGHT\n" << std::endl;
            set_stats(winner, w_counter, 160, straight1[1], 0, 0, 0, 0);
        }
        else if (trips1[0] == 1)
        {
            std::cout << "Hand strength: TRIPS\n" << std::endl;
            set_stats(winner, w_counter, 150, trips1[1], trips1[2], trips1[3], 0, 0);
        }
        else if (pairs1[0] == 1 && pairs1[2] == 1)
        {
            std::cout << "Hand strength: TWO PAIR\n" << std::endl;
            set_stats(winner, w_counter, 140, pairs1[1], pairs1[3], pairs1[4], 0, 0);
        }
        else if (pairs1[0] == 1 && pairs1[2] == 0)
        {
            std::cout << "Hand strength: PAIR\n" << std::endl;
            set_stats(winner, w_counter, 130, pairs1[1], pairs1[4], pairs1[5], pairs1[6], 0);
        }
        else
        {
            std::cout << "Hand strength: HIGH CARD\n" << std::endl;
            set_stats(winner, w_counter, high_card1[0], high_card1[0], high_card1[1],
                      high_card1[2],high_card1[3],high_card1[4]);
        }

        // Breaks out of loops if menu option 2 is chosen, and make opponents = 0
        if (option2)
            break;

        w_counter++;

    } while (inserted_card_counter < ((opponents+1)*2)-1);

    if (!option2)
    {
        // After all card strength are evaluated, the best 5-card combination mucst be chosen
        // Firstly all cards will be arranged in descending order of points
        order(winner, opponents+1, "points");
        // Function counts the amount of players that have the same points, indicating the same hand
        same_points_counter = counter(winner, opponents+1, "points");
    }

    // If multiple people have the same hand strength, th program will need to evaluate the cards' kickers
    /* The following counters are initialized at 1, so if a counter is greater than 1, it suggests multiple players
    have the same value for the final parameter of the "counter" function */
    // Example -> if the "same_points_counter" is 1, it implies that no players have the same points
    if (same_points_counter == 1 || option2)
        final_winner = winner[0].player_number;
    else if (!option2)
    {
        int same_highcard_counter = 1, same_kicker1_counter = 1, same_kicker2_counter = 1, same_kicker3_counter = 1, same_kicker4_counter = 1;

        // Pattern continues, highcards are are ordered if multiple players have the same amount of points
        order(winner, same_points_counter, "highcard");
        // Those players with the same points are arranged based on the highcard, then kickers as necessary 
        same_highcard_counter = counter(winner, same_points_counter, "highcard");

        // For straight flushes and straights if multiple players have the same highcard, then they are all tied
        if ((same_highcard_counter > 1 && winner[0].points == 200) || (same_highcard_counter > 1 && winner[0].points == 160))
        {
            wins = multiple_wins(winner, same_highcard_counter);
            multiple_winners = true;
        }
        else if (same_highcard_counter > 1)
        {
            order(winner, same_highcard_counter, "kicker1");
            same_kicker1_counter = counter(winner, same_highcard_counter, "kicker1");

            // The same pattern of the order and count function are used to compare all cards important in a poker hand
            switch (winner[0].points)
            {
                case 190:
                case 180:
                    if (same_kicker1_counter > 1)
                    {
                        wins = multiple_wins(winner, same_highcard_counter);
                        multiple_winners = true;
                    }
                    else
                        final_winner = compare_kicker(winner, same_highcard_counter, "kicker1");
                    break;
                case 150:
                case 140:
                    if (same_kicker1_counter > 1)
                    {
                        order(winner, same_kicker1_counter, "kicker2");
                        same_kicker2_counter = counter(winner, same_kicker1_counter, "kicker2");

                        if (same_kicker2_counter > 1)
                        {
                            wins = multiple_wins(winner, same_kicker2_counter);
                            multiple_winners = true;
                        }
                        else
                            final_winner = compare_kicker(winner, same_kicker1_counter, "kicker2");
                    }
                    else
                        final_winner = compare_kicker(winner, same_highcard_counter, "kicker1");
                    break;
                case 130:
                    if (same_kicker1_counter > 1)
                    {
                        order(winner, same_kicker1_counter, "kicker2");
                        same_kicker2_counter = counter(winner, same_kicker1_counter, "kicker2");

                        if (same_kicker2_counter > 1)
                        {
                            order(winner, same_kicker2_counter, "kicker3");
                            same_kicker3_counter = counter(winner, same_kicker2_counter, "kicker3");

                            if (same_kicker3_counter > 1)
                            {
                                wins = multiple_wins(winner, same_kicker3_counter);
                                multiple_winners = true;
                            }
                            else
                                final_winner = compare_kicker(winner, same_kicker2_counter, "kicker3");
                        }
                        else
                            final_winner = compare_kicker(winner, same_kicker1_counter, "kicker2");
                    }
                    else
                        final_winner = compare_kicker(winner, same_highcard_counter, "kicker1");
                    break;
                case 170:
                default:
                    if (same_kicker1_counter > 1)
                    {
                        order(winner, same_kicker1_counter, "kicker2");
                        same_kicker2_counter = counter(winner, same_kicker1_counter, "kicker2");

                        if (same_kicker2_counter > 1)
                        {
                            order(winner, same_kicker2_counter, "kicker3");
                            same_kicker3_counter = counter(winner, same_kicker2_counter, "kicker3");

                            if (same_kicker3_counter > 1)
                            {
                                order(winner, same_kicker3_counter, "kicker4");
                                same_kicker4_counter = counter(winner, same_kicker3_counter, "kicker4");

                                if (same_kicker4_counter > 1)
                                {
                                    wins = multiple_wins(winner, same_kicker4_counter);
                                    multiple_winners = true;
                                }
                                else
                                    final_winner = compare_kicker(winner, same_kicker3_counter, "kicker4");
                            }
                            else
                                final_winner = compare_kicker(winner, same_kicker2_counter, "kicker3");
                        }
                        else
                            final_winner = compare_kicker(winner, same_kicker1_counter, "kicker2");
                    }
                    else
                        final_winner = compare_kicker(winner, same_highcard_counter, "kicker1");
                    break;
            }
        }
        else
            final_winner = winner[0].player_number;
    }

    // Evalutes wether or not there are multiple winner and displays the winners as an output
    if (multiple_winners && !option2)
    {
        std::cout << "\n  <****";
        for (int i = 0; i < (wins[1])*2; i++)
            std::cout << "*";
        std::cout << " ITS A TIE!! ";
        std::cout << "****";
        for (int i = 0; i < (wins[1])*2; i++)
        {
            std::cout << "*";
            if (i + 1 == wins[1] * 2)
                std::cout << ">\n";
        }
        std::cout << "The winners are PLAYERS";
        for (int i = 2; i < (wins[1])+1; i++)
            std::cout << " #" << wins[i] << ",";
        std::cout << " and #" << wins[(wins[1])+1] << "!\n";
        std::cout << "  <***********";
        for (int i = 0; i < (wins[1])*(7/2); i++)
            std::cout << "*";
        std::cout << "************>\n\n";
    }
    else if (!option2)
    {
        std::cout << "\n    <******* THE WINNER IS *******>\n";
        std::cout << "      -----    PLAYER #" << final_winner << "    -----" << std::endl;
        std::cout << "   <**** !! CONGRATULATIONS !! ****>\n\n";
    }

    delete [] player_card;
    delete [] winner;

    return 0;
}

/* *************** FUNCTION DEFINITIONS *************** */

// Prints the menu
void menu()
{
   std::cout << std::setw(29) << "%%%%=======MENU=======%%%%\n";
   std::cout << std::setw(31) << "Choose one of the following: \n";
   std::cout << std::setw(30) << "1 - Generate poker hands\n";
   std::cout << std::setw(26) << "2 - Enter poker hand\n";
   std::cout << std::setw(29) << "%%%%==================%%%%\n";
   std::cout << "-----> ";
}

// Orders array of cards based on their number in descending order
void order_number(struct Card arr[], int size_of_array)
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

// Orders array based on desired element of Eval_Card struct in descending order
void order(struct Eval_Card arr[], int size_of_array, std::string choice)
{
    bool element = false;
    Eval_Card temp;
    // Bubble swaps in any element of the Eval_Card struct array descending order
    for (int i = 0; i < size_of_array-1; i++)
    {
        for (int j = 0; j < size_of_array-i-1; j++)
        {
            if (choice == "highcard" && arr[j].high_card < arr[j+1].high_card)
                element = true;
            else if (choice == "points" && arr[j].points < arr[j+1].points)
                element = true;
            else if (choice == "kicker1" && arr[j].kicker1 < arr[j+1].kicker1)
                element = true;
            else if (choice == "kicker2" && arr[j].kicker2 < arr[j+1].kicker2)
                element = true;
            else if (choice == "kicker3" && arr[j].kicker3 < arr[j+1].kicker3)
                element = true;
            else if (choice == "kicker4" && arr[j].kicker4 < arr[j+1].kicker4)
                element = true;

            if (element)
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
            element = false;
        }
    }
}

// Sets the point value and card values for each player's five card hand
void set_stats(struct Eval_Card arr[], int size_of_array, int p, int hc, int k1, int k2, int k3, int k4)
{
    arr[size_of_array].player_number = size_of_array+1;
    arr[size_of_array].points = p;
    arr[size_of_array].high_card = hc;
    arr[size_of_array].kicker1 = k1;
    arr[size_of_array].kicker2 = k2;
    arr[size_of_array].kicker3 = k3;
    arr[size_of_array].kicker4 = k4;
}

// Counts and returns the amount of card elements are repeated to determine ties and winners
int counter(struct Eval_Card arr[], int size_of_array, std::string choice)
{
    bool element = false;
    int new_counter = 1;
    for (int i = 0; i < size_of_array-1; i++)
    {
        if ((choice == "highcard" && arr[i].high_card == arr[i+1].high_card) || 
            (choice == "points" && arr[i].points == arr[i+1].points) || 
            (choice == "kicker1" && arr[i].kicker1 == arr[i+1].kicker1) ||
            (choice == "kicker2" && arr[i].kicker2 == arr[i+1].kicker2) ||
            (choice == "kicker3" && arr[i].kicker3 == arr[i+1].kicker3) ||
            (choice == "kicker4" && arr[i].kicker4 == arr[i+1].kicker4))
            {
                element = true;
            }

        if (element)
            new_counter++;
        else
            break;

        element = false;
    }

    return new_counter;
}

// Compares the desired kicker to determine ties or winners
int compare_kicker(struct Eval_Card arr[], int size_of_array, std::string choice)
{
    int final_winner = 0;
    for (int i = 0; i < size_of_array-1; i++)
    {
        if (choice == "kicker1")
        {
            if (arr[i].kicker1 > arr[i+1].kicker1)
                final_winner = arr[i].player_number;
            else if (arr[i].kicker1 < arr[i+1].kicker1)
                final_winner = arr[i+1].player_number;
        }
        else if (choice == "kicker2")
        {
            if (arr[i].kicker2 > arr[i+1].kicker2)
                final_winner = arr[i].player_number;
            else if (arr[i].kicker2 < arr[i+1].kicker2)
                final_winner = arr[i+1].player_number;
        }
        else if (choice == "kicker3")
        {
            if (arr[i].kicker3 > arr[i+1].kicker3)
                final_winner = arr[i].player_number;
            else if (arr[i].kicker3 < arr[i+1].kicker3)
                final_winner = arr[i+1].player_number;
        }
        else if (choice == "kicker4")
        {
            if (arr[i].kicker4 > arr[i+1].kicker4)
                final_winner = arr[i].player_number;
            else if (arr[i].kicker4 < arr[i+1].kicker4)
                final_winner = arr[i+1].player_number;
        }
        if (final_winner > 0)
            break;
    }

    return final_winner;
}

// Determines if there are multiple winners
int* multiple_wins(struct Eval_Card arr[], int size_of_array)
{
    int j = 2, m_counter = 1, multiple_winners = 0;
    int *status = new int[size_of_array+2];
    for (int i = 0; i < size_of_array-1; i++)
    {
        m_counter++;
        multiple_winners = 1;
        status[j] = arr[i].player_number;
        j++;
        if (i+1 == size_of_array-1)
            status[j] = arr[i+1].player_number;
    }

    status[0] = multiple_winners;  // status[0] acts as bool were 1 = true and 0 = false
    status[1] = m_counter;         // status[1] = how many ties there are

    return status;
}

// Evaluates high card strength
int* high_card(struct Card arr[])
{
    int *status = new int[5];

    // Aces are changed to value 14, originally were set as 1 since in straights/flushed they act as the highest and lowest card
    for (int i = 0; i < 7; i++)
        if (arr[i].number == 1)
            arr[i].number = 14;

    order_number(arr, 7);

    for (int i = 0; i < 5; i++)
        status[i] = arr[i].number;

    // Returns all cards in player hand in case of kicker comparisons needed
    return status;
}

// Evaluates 1 or 2 pairs in hand, and is used to determine full houses aswell
int* pairs(struct Card arr[])
{
    int *status = new int[7], *trips1 = trips(arr);
    int counter = 0, size = 0;
    bool check_trips = false;
    Card hold[7];

    // Since every function manipulates the arrays a temp array is created so original array evaluated can be put back to normal
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    // Aces are changed to value 14, originally were set as 1 since in straights/flushed they act as the highest and lowest card
    for (int i = 0; i < 7; i++)
        if (arr[i].number == 1)
            arr[i].number = 14;

    order_number(arr, 7);

    // Used to determine full house is trips and a pair are present
    if (trips1[0] == 1)
    {
        for (int i = 0; i < 7; i++)
        {
            if (arr[i].number == trips1[1])
            {
                arr[i].number = -1;
                check_trips = true;
            }
        }
        order_number(arr, 7);
    }

    // If trips are present only 4 of the 7 cards are evaluated to determine the highest kickers
    if (check_trips)
        size = 4;
    else
        size = 7;

    // for loop checks for single or double pairs
    for (int i = 0; i < size-1; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            counter++;
            if (counter == 1)
            {
                status[0] = 1;
                status[1] = arr[i].number;
            }
            else if (counter == 2)
            {
                status[2] = 1;
                status[3] = arr[i].number;
            }
            i++;
        }
    }

    // Sets kickers for pairs and two pairs
    if (status[0] == 1 && status[2] == 0 && !check_trips)
    {
        for (int i = 0; i < 7; i++)
        {
            if (arr[i].number == status[1])
                arr[i].number = -1;
        }
        order_number(arr, 7);
        status[4] = arr[0].number;
        status[5] = arr[1].number;
        status[6] = arr[2].number;
    }
    else if (status[0] == 1 && status[2] == 1 && !check_trips)
    {
        for (int i = 0; i < 7; i++)
        {
            if (arr[i].number == status[1] || arr[i].number == status[3])
                arr[i].number = -1;
        }
        order_number(arr, 7);
        status[4] = arr[0].number;
    }

    // Hold array copies old values back after collecting information about the array for the function
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    /* status[0] = wether single pair is present or not, status[1] = highcard of single pair, status[2] = wether two pair is present
     status[3] = highcard of two pair, status[4] = top kicker of two pair or single pair, status[5] = second highest kicker of 
     single pair, status[6] = lowest kicker of two pair*/
    return status;
}

// Determines wether or not there are trips
int* trips(struct Card arr[])
{
    int *status = new int[4];
    Card hold[7];

    // Since every function manipulates the arrays a temp array is created so original array evaluated can be put back to normal
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    // Aces are changed to value 14, originally were set as 1 since in straights/flushed they act as the highest and lowest card
    for (int i = 0; i < 7; i++)
        if (arr[i].number == 1)
            arr[i].number = 14;

    order_number(arr, 7);

    // Checks for trips after numbers are ordered
    for (int i = 0; i < 5; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number)
        {
            status[0] = 1;
            status[1] = arr[i].number;
            break;
        }
    }

    // If trips are present this sets their last two kickers
    if (status[0] == 1)
    {
        for (int i = 0; i < 7; i++)
            if (arr[i].number == status[1])
                arr[i].number = -1;
        order_number(arr, 7);
        status[2] = arr[0].number;
        status[3] = arr[1].number;
    }

    // Hold array copies old values back after collecting information about the array for the function
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    // status[0] = wether or not trips are present, status[1] = trips high card, status[2-1] = trips kicker1/kicker2
    return status;
}

// Determines wether or not there is a straight
int* straight(struct Card arr[])
{
    int counter = 0;
    int *status = new int[5];
    bool ace = false;
    Card hold[7];

    // Since every function manipulates the arrays a temp array is created so original array evaluated can be put back to normal
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    order_number(arr, 7);

    // Sets one of any repeated cards as -1, to not interfer with follwing algorithm
    for (int i = 0; i < 7 - counter; i++)
    {
        if (arr[i].number == arr[i+1].number)
        {
            Card hold1 = {-1, -1};
            arr[i] = hold1;
            order_number(arr, 7);
            counter++;
            i = 0;
        }
    }

    // Similar loop to the one in flush function
    do
    {
        if (ace)
            order_number(arr, 7);

        // Checks for straight, repeated card were taken out due to this
        for (int i = 0; i < 3; i++)
        {
            if (arr[i].number == arr[i+1].number+1 && arr[i+1].number == arr[i+2].number+1 &&
                arr[i+2].number == arr[i+3].number+1 && arr[i+3].number == arr[i+4].number+1)
            {
                status[0] = 1;
                status[1] = arr[i].number;
                status[2] = arr[i+1].number;
                status[3] = arr[i+2].number;
                status[4] = arr[i+3].number;
                status[5] = arr[i+4].number;
                break;
            }
        }

        if (ace)
            break;

        // Checks for ace
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

    // status[0] = wether or not there is a straight, status[1] = highcard of straight
    // status[2-4] = possible straights, for straight flush function
    return status;
}

// Determines wether or not there is a flush
int* flush(struct Card arr[])
{
    int counter1 = 0, counter2 = 0, counter3 = 0, counter4 = 0;
    int *status = new int[7];
    bool ace = false;
    Card hold[7];

    // Since every function manipulates the arrays a temp array is created so original array evaluated can be put back to normal
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    // Counts the amount of times a suit is repeated to determine flush
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

    // Exits out early is no flush is present
    if (counter1 < 5 && counter2 < 5 && counter3 < 5 && counter4 < 5)
        return status;

    order_number(arr, 7);

    // Finds the high card and kickers of the entire flush hand
    do
    {
        if (ace)
            order_number(arr, 7);

        // Sets the highcard and kickers
        for (int i = 1; i < 5; i++)
        {
            if ((counter1 == 5 && i == 1) || (counter2 == 5 && i == 2) ||
                (counter3 == 5 && i == 3) || (counter4 == 5 && i == 4))
            {
                status[0] = 1;
                status[6] = i;
                int s_counter = 0;
                for (int j = 0; j < 7; j++)
                {
                    if (arr[j].suit == i)
                    {
                        s_counter++;
                        switch(s_counter)
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

        // Checks if ace is present, if ace is present then it changes it and adjusts the highcard and kicker if needed
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

    // status[0] = wether or not there is a flush, status[1] = highcard of flush, status[2-5] = kickers
    // status[6] = suit corresponding to flush
    return status;
}

// Determines wether or not there are quads
int* quads(struct Card arr[])
{
    int *status = new int[3];
    Card hold[7];

    // Since every function manipulates the arrays a temp array is created so original array evaluated can be put back to normal
    for (int i = 0; i < 7; i++)
        hold[i] = arr[i];

    order_number(arr, 7);

    // Checks for quads
    for (int i = 0; i < 4; i++)
    {
        if (arr[i].number == arr[i+1].number && arr[i].number == arr[i+2].number &&
            arr[i].number == arr[i+3].number)
        {
            status[0] = 1;
            status[1] = arr[i].number;
        }
    }

    // If quads are present, this loop find its kicker
    if (status[0] == 1)
    {
        for (int i = 0; i < 7; i++)
            if (arr[i].number == status[1])
                arr[i].number = -1;
        order_number(arr, 7);
        status[2] = arr[0].number;
    }

    // Hold array copies old values back after collecting information about the array for the function
    for (int i = 0; i < 7; i++)
        arr[i] = hold[i];

    // status[0] = wether or not there are quads, status[1] = highcard of quads, status[2] = kicker
    return status;
}

// Determines wether or not there is a straight flush
int* straightflush(struct Card arr[])
{
    int *status = new int[2], *straight1 = straight(arr), *flush1 = flush(arr);
    int counter = 0;
    Card hold[7];

    // Makes sure there is a straight and a flush present in seven card hand
    if (straight1[0] == 1 && flush1[0] == 1)
    {
        // Since every function manipulates the arrays a temp array is created so original array evaluated can be put back to normal
        for (int i = 0; i < 7; i++)
            hold[i] = arr[i];

        // If a card is not of the suit correponding to the flush it is removed to prevent interference with further evaluation
        for (int i = 0; i < 7; i++)
        {
            if (status[6] != arr[i].suit)
            {
                Card temp = {0, 0};
                arr[i] = temp;
            }
        }

        // Determines wether the high card corresponding to a straight aso have the same suit corresponding to the flsuh
        for (int i = 1; i < 4; i++)
        {
            if (straight1[i] == flush1[i])
            {
                status[0] = 1;
                status[1] = straight1[i];
                break;
            }
        }
        
        // Hold array copies old values back after collecting information about the array for the function
        for (int i = 0; i < 7; i++)
            arr[i] = hold[i];
    }

    // status[0] = wether or not there is a straight flush, status[1] = highcard of straight flsuh
    return status;

}