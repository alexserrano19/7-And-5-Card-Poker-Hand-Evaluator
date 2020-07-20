#include <iostream>
#include <iomanip>
#include <random>

#include "userInterface.h"
#include "speedDealing.h"

int main()
{
    UserInterface UI;
    SpeedDealing speed;
    Dealer dealer;
    bool sevenCardGame = false;

    UI.printGameBanner();
    UI.selectFirstMenuOption();

    switch (UI.getFirstSelection())
    {
        case '5': sevenCardGame = false; break;
        case '7': sevenCardGame = true; break;
        default: UI.printByeMessage(); return 0;
    }

    do 
    {
        if (!speed.getContinuousLoopOn())
        {
            UI.selectSecondMenuOption(sevenCardGame);
            speed.setCheckForWinner(true);
        }
        else
        {
            UI.setSecondSelection('1');
            speed.processLoadingTime();
        }

        switch(UI.getSecondSelection())
        {
            case '1':
            {
                dealer.setPlayers(UI.numberPlayersPerGame(sevenCardGame, speed.getContinuousLoopOn(), speed.getPlayersPerLoop()));
                dealer.generateShuffledDeck();
                dealer.populateHand(sevenCardGame);
                UI.printHandBanner();

                // Determines hand strength of every player in a game
                while(dealer.getCardIndex() < dealer.getLoopRequirement())
                {
                    dealer.evaluatePlayerHands(sevenCardGame);
                    UI.processPlayerHand(dealer.getSevenCardHand(), dealer.getPossibleWinnerArray(), 
                                            dealer.getPossibleWinnerIndex(), sevenCardGame);
                }
                
                // Determines winner if not in continuous loop or if output is desired
                if (speed.getCheckForWinner())
                    UI.winningHandOutput(dealer.determineWinner());

                dealer.deallocate();
                break;
            }
            case '2':
            {
                speed.speedDealingOption(UI, sevenCardGame);
                break;
            }
            case '3':
            case '4':
            {
                UI.processStatistics(UI.getSecondSelection());
                break;
            }
            default: UI.printByeMessage();
        }

        speed.handleFinalContinuousLoop();
        
    } while (UI.getSecondSelection() != 'Q');
    
    return 0;
}