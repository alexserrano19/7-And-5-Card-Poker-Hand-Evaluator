#include <iostream>
#include <iomanip>
#include <random>
#include "speedDealing.h"

int main()
{
    Dealer dealer;
    UserInterface UI;
    SpeedDealing speed;

    UI.printGameBanner();
    UI.selectFirstMenuOption();

    switch (UI.getFirstSelection())
    {
        case '5': UI.setSevenCardGameBool(false); break;
        case '7': UI.setSevenCardGameBool(true); break;
        default: UI.printByeMessage(); return 0;
    }

    do 
    {
        if (!speed.getContinuousLoopOnBool())
        {
            UI.selectSecondMenuOption(UI.getSevenCardGameBool());
            speed.setCheckForWinnerBool(true);
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
                dealer.setPlayers(UI.numberPlayersPerGame(UI.getSevenCardGameBool(), 
                                    speed.getContinuousLoopOnBool(), speed.getPlayersPerLoop()));
                dealer.generateShuffledDeck();
                dealer.populateCardArrays(UI.getSevenCardGameBool());
                UI.printHandBanner();

                // Determines hand strength of every player in a game
                while(dealer.getCardIndex() < dealer.getLoopRequirement())
                {
                    dealer.evaluatePlayerHand(UI.getSevenCardGameBool());
                    UI.processPlayerHand(dealer.getPrintableHand(), dealer.getPossibleWinnerArray(), 
                                            dealer.getPossibleWinnerIndex(), UI.getSevenCardGameBool());
                }
                
                // Determines winner if not in continuous loop or if output is desired
                if (speed.getCheckForWinnerBool())
                    UI.winningHandOutput(dealer.determineWinner());

                dealer.deallocateMemoryAndResetMembers();
                break;
            }
            case '2':
            {
                speed.speedDealingOption(UI, UI.getSevenCardGameBool());
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