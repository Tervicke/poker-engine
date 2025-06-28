//
// Created by tervicke on 24/6/25.
#include <iostream>
#include "deck.h"
#include "card.h"
#include "handrank.h"
#include "player.h"
#include "game.h"

#include <cassert>
#include <iomanip>
#include <sstream>

Game::Game(std::vector<Player> players)
    : deck(),
      players(players),
      flopRevealed(false),
      turn(Card()),
      turnRevealed(false),
      river(Card()),
      riverRevealed(false)
{}
bool Game::DistributeHoleCards()
{
    for (auto& player : players)
    {
        auto holecards = deck.drawCards(2);
        if ( holecards.size() < 2 ) return false;
        player.setHoleCards( {holecards[0] , holecards[1]});
    }
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

    //printPlayerCards("PLAYER 1", p1);
    //printPlayerCards("PLAYER 2", p2);

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

//returns if the player won and also the best hand
std::pair<bool,std::string> Game::decideWinner()
{
    auto cards = getCommunityCards();
    cards.push_back(players[0].getHoleCards().first);
    cards.push_back(players[0].getHoleCards().second);
    HandRank mainbest = getBestHand(getAll5CardCombos(cards));
    cards.pop_back();
    cards.pop_back();
    HandRank oppbest(HIGH_CARD , {},{});
    for (int p = 1 ; p < players.size(); ++p)
    {
        cards.push_back(players[p].getHoleCards().first);
        cards.push_back(players[p].getHoleCards().second);
        auto opp = getBestHand(getAll5CardCombos(cards));
        if (p == 1)
        {
            oppbest = opp;
        }else
        {
            if (oppbest < opp)
            {
                oppbest = opp;
            }
        }
        cards.pop_back();
        cards.pop_back();
    }
    if (oppbest < mainbest)
    {
        auto handstr = toHandString(mainbest.getHand());
        return {true,handstr};
    }
    auto handstr = toHandString(oppbest.getHand());
    return {false,handstr};
}

Deck Game::getDeck()
{
    return deck;
}
std::pair<double,double> Game::monteCarloSimulate()
{

    Deck copy = deck;
    std::vector<Card> cards;
    //default win and loss chances 0% , 0%
    int win = 0 , loss = 0;
    int trials = 10000;
    //if the hole cards are distributed.
    if (CardsDistributed)
    {
        std::cout << "comes here" << std::endl;
        for (int i = 0  ; i < trials; i++)
        {
            std::cout << cards.size() << std::endl;
            assert(cards.size() == 0);
            //get the flop
            //simulate the turn if not revealed
            if (!flopRevealed)
            {
                auto simulated_flop = copy.drawCards(3);
                cards.insert(cards.end(), simulated_flop.begin(), simulated_flop.end());
            }else
            {
                cards.insert(cards.end(),flop.begin(),flop.end());
            }

            if (!turnRevealed)
            {
                auto simulated_turn = copy.drawCards(1)[0];
                cards.push_back(simulated_turn);
            }else
            {
                cards.push_back(turn);
            }

            if (!riverRevealed)
            {

                auto simulated_river = copy.drawCards(1) [0];
                cards.push_back(simulated_river);
            }else
            {
                cards.push_back(river);
            }

            //push the player hole cards
            cards.push_back(players[0].getHoleCards().first);
            cards.push_back(players[0].getHoleCards().second);

            HandRank mainbest = getBestHand(getAll5CardCombos(cards));
            cards.pop_back();
            cards.pop_back();

            HandRank oppbest(HIGH_CARD,{},{});

            for (int p = 0 ; p < players.size() - 1 ; p++)
            {
                auto hole1= copy.drawCards(1)[0];
                auto hole2 = copy.drawCards(1)[0];

                cards.push_back(hole1);
                cards.push_back(hole2);

                HandRank opp = getBestHand(getAll5CardCombos(cards));

                if (p == 0) oppbest = opp;
                else
                {
                    if (oppbest < opp)
                    {
                        oppbest = opp;
                    }
                }
            }
            if ( oppbest < mainbest)
            {
                win++;
            }else
            {
                loss++;
            }
            //clear the cards and reset the deck
            cards.clear();
            copy.reset();
        }
        std::cout << "ends here" << std::endl;
    }
    double dwin = (win * 100.0) / trials;
    double dloss = (loss * 100.0) / trials;
    return {dwin , dloss};
}
std::pair<bool,std::string> Game::RevealNext()
{
    if (stop) return{true,""};
    if (!CardsDistributed)
    {
        DistributeHoleCards();
        CardsDistributed  = true;
        stop = true;
        return {true,"HOLE"};
    }
    if (!flopRevealed)
    {
        RevealFlop();
        stop = true;
        return {true , "FLOP"};
    }
    if (!turnRevealed)
    {
        RevealTurn();
        stop = true;
        return {true,"TURN"};
    }
    if (!riverRevealed)
    {
        RevealRiver();
        stop = true;
        return {true,"RIVER"};
    }
    return {true,"END"};
}
std::vector<std::string> Game::GetFlopName() const
{
    std::vector<std::string> names;
    for (int i = 0 ; i < 3 ; i++)
    {
        names.push_back(flop[i].getName());
    }
    return names;
}
std::string Game::GetRiverName() const
{
    return river.getName();
}
std::string Game::GetTurnName() const
{
    return turn.getName();
}

std::vector<std::string> Game::GetPlayer1Cards()
{
    auto cards = players[0].getHoleCards();
    std::cout << cards.first.getName() << std::endl;
    std::cout << cards.second.getName() << std::endl;
    std::vector<std::string> names = {players[0].getHoleCards().first .getName() , players[0].getHoleCards().second.getName()};
    return names;
}
std::pair<std::string,std::string> Game::getProbabilityPercentage()
{
    auto formatDouble = [](double x) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << x;
        return oss.str();
    };
    auto [win, loss] = monteCarloSimulate();
    return {formatDouble(win), formatDouble(loss)};

}
std::vector<std::vector<std::string>> Game::GetAllplayersCards() const
{
   std::vector<std::vector<std::string>> names;
    for (auto player : players)
    {
        auto [card1 , card2] =  player.getHoleCards();
        names.push_back( {card1.getName(),card2.getName()});
    }
    return names;
}

HandRank Game::getBestPlayerHand()
{
    auto cards = getCommunityCards();
    auto [hole1 , hole2] = players[0].getHoleCards();
    cards.push_back(hole1);
    cards.push_back(hole2);
    return getBestHand(getAll5CardCombos(cards));
}
void Game::Reset()
{
    turnRevealed = false;
    riverRevealed = false;
    flopRevealed = false;
    CardsDistributed = false;
    deck.reset();
}