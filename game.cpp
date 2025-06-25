//
// Created by tervicke on 24/6/25.
#include <iostream>
#include "deck.h"
#include "card.h"
#include "handrank.h"
#include "player.h"
#include "game.h"
Game::Game(Player  p1 , Player  p2)
    : deck() , p1(std::move(p1)) , p2(std::move(p2)) ,  flopRevealed ( false ) , turn ( Card() ) , turnRevealed( false )  , river( Card() ) , riverRevealed( false )
{
}
bool Game::DistributeHoleCards()
{
    auto holecard1 = deck.drawCards(2);
    auto holecard2 = deck.drawCards(2);
    if ( holecard1.size() < 2 || holecard2.size() < 2 ) return false;
    p1.setHoleCards( {holecard1[0] , holecard1[1]});
    p2.setHoleCards( {holecard2[0] , holecard2[1]});
    return true;
}
bool Game::RevealFlop()
{
    if ( !flopRevealed )
    {
        auto v = deck.drawCards(3);
        std::copy(v.begin(), v.end(), flop.begin());
        flopRevealed = true;
        return true;
    }
    return false;
}
bool Game::RevealTurn()
{
    if ( !turnRevealed )
    {
        turn = deck.drawCards(1)[0];
        turnRevealed = true;
        return true;
    }
    return false;
}
bool Game::RevealRiver()
{
    if (!riverRevealed)
    {
        river = deck.drawCards(1)[0];
        riverRevealed = true;
        return true;
    }
    return false;
}
void Game::PrintGameState() {
    auto printCardSection = [](const std::string& label, const auto& cards) {
        std::cout << label << ":\n";
        std::cout << "--------\n";
        for (const auto& card : cards) {
            card.print();
        }
        std::cout << "--------\n";
    };

    auto printPlayerCards = [](const std::string& playerName, const auto& player) {
        std::cout << playerName << " Cards:\n";
        auto holecards = player.getHoleCards();
        holecards.first.print();
        holecards.second.print();
    };

    std::cout << "\n==================== GAME STATE ====================\n";

    printPlayerCards("PLAYER 1", p1);
    printPlayerCards("PLAYER 2", p2);

    if (flopRevealed)
        printCardSection("FLOP: ", flop);

    if (turnRevealed)
    {
        std::cout << "TURN: ";
        turn.print();
    }

    if (riverRevealed)
    {
        std::cout << "RIVER: ";
        river.print();
    }

    std::cout << "====================================================\n";
}
void Game::setFlop(const Card& c1, const Card& c2, const Card& c3) {
    flop = {c1, c2, c3};
    flopRevealed = true;
}

void Game::setTurn(const Card& c) {
    turn = c;
    turnRevealed = true;
}

void Game::setRiver(const Card& c) {
    river = c;
    riverRevealed = true;
}
std::vector<Card> Game::getCommunityCards()
{
    std::vector<Card> cards;
    if (flopRevealed)
    {
        for (const auto& card : flop)
        {
            cards.push_back(card);
        }
    }
    if (turnRevealed)
    {
        cards.push_back(turn);
    }
    if (riverRevealed)
    {
        cards.push_back(river);
    }
    return cards;
}
std::vector<std::vector<Card>> Game::getAll5CardCombos(const std::vector<Card>& allcards) {
    std::vector<std::vector<Card>> combinations;
    int n = allcards.size();

    for (int i = 0; i < n - 4; ++i)
        for (int j = i + 1; j < n - 3; ++j)
            for (int k = j + 1; k < n - 2; ++k)
                for (int l = k + 1; l < n - 1; ++l)
                    for (int m = l + 1; m < n; ++m) {
                        combinations.push_back({allcards[i], allcards[j], allcards[k], allcards[l], allcards[m]});
                    }

    return combinations;
}
HandRank Game::getBestHand(std::vector<std::vector<Card>> combos)
{
    HandRank best = EvaluateHand(combos[0]);
    for (const auto& combo : combos) {
        HandRank hr = EvaluateHand(combo);
        if (best < hr)
        {
            best = hr;
        }
    }
    return best;
}
void Game::decideWinner()
{
    auto comcards = getCommunityCards();
    std::vector<Card> allcards = comcards;
    allcards.insert(allcards.end(), p1.getHoleCards().first);
    allcards.insert(allcards.end(), p1.getHoleCards().second);
    HandRank bestp1  = getBestHand(getAll5CardCombos(allcards));
    //remove the last 2 p1 cards
    allcards.pop_back();
    allcards.pop_back();
    allcards.push_back(p2.getHoleCards().first);
    allcards.push_back(p2.getHoleCards().second);
    HandRank bestp2 = getBestHand(getAll5CardCombos(allcards));
    if (bestp1 < bestp2)
    {
        std::cout << "PLAYER 2 WINS\n";
    }
    if (bestp2 < bestp1)
    {
        std::cout << "PLAYER 1 WINS\n";
    }
}

Deck Game::getDeck()
{
    return deck;
}
void Game::monteCarloSimulate()
{
    Deck copy = deck;
    auto cards = getCommunityCards();
    //assume hands can already be made without needing to add more cards
    if (getCommunityCards().size() == 5)
    {
        cards.push_back(p1.getHoleCards().first);
        cards.push_back(p1.getHoleCards().second);
        HandRank best = getBestHand(getAll5CardCombos(cards));

        //remove the p1 hole cards
        cards.pop_back();
        cards.pop_back();

        //simulate
        int win = 0 , loss = 0;
        int trials = 100000;

        for (int i = 0; i < trials; ++i)
        {

            auto hole1= copy.drawCards(1)[0];
            auto hole2 = copy.drawCards(1)[0];

            //insert the randomized
            cards.push_back(hole1);
            cards.push_back(hole2);

            HandRank oppbest = getBestHand(getAll5CardCombos(cards));
            if ( oppbest < best)
            {
                win++;
            }
            if (best < oppbest)
            {
                loss++;
            }

            //remove the 2 randomized cards from the total cards
            cards.pop_back();
            cards.pop_back();

            //insert the random hole cards back into the deck
            copy.insert(hole1);
            copy.insert(hole2);
        }
        std::cout << "SIMULATION STATS\n";
        std::cout << "TRIALS: " << trials << '\n';
        std::cout << "WIN: " << (100.0 * win / trials) << "%" << "\n";
        std::cout << "LOSS: " << (100.0 * loss / trials) <<"%" << '\n';
    }
}