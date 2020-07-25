#include <iostream>
#include <chrono>
#include "speedDealing.h"

SpeedDealing::SpeedDealing() : continuousLoopOn(false), checkForWinner(false),
playersPerContinuousLoop(0), outputGames(0), duration(0), tempDuration(0),
numberOfLoops(0), continuousLoopingRequirement(0)
{}

bool SpeedDealing::getContinuousLoopOnBool() const
{
    return continuousLoopOn;
}

bool SpeedDealing::getCheckForWinnerBool() const
{
    return checkForWinner;
}

int SpeedDealing::getPlayersPerLoop() const
{
    return playersPerContinuousLoop;
}

void SpeedDealing::setCheckForWinnerBool(bool w)
{
    checkForWinner = w;
}

// Handles loading times and output activation/deactivation
void SpeedDealing::processLoadingTime()
{
    if (numberOfLoops == 0)
    {
        // Holds timestamp of when speed dealing starts
        time1 = std::chrono::high_resolution_clock::now();
        std::clog << "\n\n\nLOADING...\n\n\n";
        // Deactivates cout output until looping is over
        std::cout.rdbuf(NULL);
    }
    // Activates 'cout' stream if games are outputted in continuous loop
    if (continuousLoopingRequirement-numberOfLoops == outputGames)
    {
        std::cout.rdbuf(orig_buf);
        checkForWinner = true;
    }
    time2 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::seconds>(time2 - time1).count();
    // Outputs loading time every 5 seconds, with stats of the hands evalauted
    if (duration % 5 == 0 && duration > tempDuration)
    {
        tempDuration = duration;
        std::clog << "LOADING...\n";
        std::clog << "Time loading: " << duration << " seconds\n";
        std::clog << "Games completed: " << numberOfLoops << "\n";
        std::clog << "Hands evaluated: " << numberOfLoops*playersPerContinuousLoop << "\n\n\n";
    }
    numberOfLoops++;
}

// Handles all the questions for speed dealing option
void SpeedDealing::speedDealingOption(UserInterface ui, bool sevenCardGame)
{
    continuousLoopOn = true;
    checkForWinner = false;
    playersPerContinuousLoop = 0;
    numberOfLoops = 0;

    do
    {
        std::cout << "\nFor how many games will you like to speed deal cards?\n" << ">>> ";
        std::cin >> continuousLoopingRequirement;

        if (continuousLoopingRequirement < 2)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "\nVALUE MUST BE GREATER THAN 1!";
        }

    } while (continuousLoopingRequirement < 2);

    playersPerContinuousLoop = ui.numberPlayersPerGame(sevenCardGame, false, 0, true);
    
    std::cout << "\nFor how many games will you like player hand output?\n" << ">>> ";

    while (!(std::cin >> outputGames) || outputGames > continuousLoopingRequirement || outputGames < 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "\nINVALID AMOUNT! MAX: " << continuousLoopingRequirement << " MIN: 0";
        std::cout << "\nFor how many games will you like player hand output?\n" << ">>> ";
    }
}

// Used in the final loop of speed dealing and resets all variables
void SpeedDealing::handleFinalContinuousLoop()
{
    if ((numberOfLoops == continuousLoopingRequirement) && continuousLoopOn)
    {
        if (outputGames >= 0)
        {
            // Calculates final loading itme
            std::clog << "FINISHED LOADING!\n";
            duration = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count();
            int wholeSecond = duration/1000000;
            std::clog << "Total loading time: " << wholeSecond << "." << duration-(wholeSecond*1000000) << " seconds\n\n\n";
        }
        // Activates output
        if (outputGames == 0)
            std::cout.rdbuf(orig_buf);
        continuousLoopOn = false;
        tempDuration = 0;
    }
}