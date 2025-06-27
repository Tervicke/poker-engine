//
// Created by tervicke on 24/6/25.
#include <iostream>
#include "deck.h"
#include "card.h"
#include "handrank.h"
#include "player.h"
#include "game.h"

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
void Game::decideWinner()
{
    auto comcards = getCommunityCards();
    std::vector<Card> allcards = comcards;
    //allcards.insert(allcards.end(), p1.getHoleCards().first);
    //allcards.insert(allcards.end(), p1.getHoleCards().second);
    HandRank bestp1  = getBestHand(getAll5CardCombos(allcards));
    //remove the last 2 p1 cards
    allcards.pop_back();
    allcards.pop_back();
    //allcards.push_back(p2.getHoleCards().first);
    //allcards.push_back(p2.getHoleCards().second);
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
std::pair<double,double> Game::monteCarloSimulate()
{

    Deck copy = deck;
    auto cards = getCommunityCards();
    //default win and loss chances 0% , 0%
    int win = 0 , loss = 0;
    int trials = 10000;

    //assume hands can already be made without needing to add more cards
    if (getCommunityCards().size()+2 >= 5)
    {
        std::cout << "calculatng probablity...." << std::endl;
        cards.push_back(players[0].getHoleCards().first);
        cards.push_back(players[0].getHoleCards().second);
        HandRank mainbest = getBestHand(getAll5CardCombos(cards));

        //remove the p1 hole cards
        cards.pop_back();
        cards.pop_back();

        //simulate

        for (int i = 0; i < trials; ++i)
        {
            HandRank oppbest(HIGH_CARD,{});
            for (int p = 0 ; p < players.size() - 1 ; p++)
            {
                auto hole1= copy.drawCards(1)[0];
                auto hole2 = copy.drawCards(1)[0];

                cards.push_back(hole1);
                cards.push_back(hole2);

                HandRank opp = getBestHand(getAll5CardCombos(cards));
                if (p == 0) oppbest = opp;
                else if (oppbest < opp)
                {
                    oppbest = opp;
                }

                cards.pop_back();
                cards.pop_back();

                //insert the random hole cards back into the deck
                copy.insert(hole1);
                copy.insert(hole2);
            }

            //check with the opponent best hand
            if ( oppbest < mainbest)
            {
                win++;
            }
            if (mainbest < oppbest)
            {
                loss++;
            }
        }
        std::cout <<" Done" << std::endl;
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