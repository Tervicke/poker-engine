//
// Created by tervicke on 24/6/25.
//

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include "deck.h"
#include "card.h"
#include "handrank.h"
#include "player.h"
class Game
{
private:
    Deck deck;
    std::vector<Player> players;
    std::vector<Card> flop = std::vector(3 , Card() );
    bool flopRevealed = false;
    Card turn;
    bool turnRevealed = false;
    Card river;
    bool CardsDistributed = false;
    bool riverRevealed = false;
public:
    bool stop = true;
    Game(std::vector<Player> players);
    bool DistributeHoleCards();
    bool RevealFlop();
    bool RevealTurn();
    bool RevealRiver();
    void PrintGameState();
    void setFlop(const Card& c1, const Card& c2, const Card& c3);
    void setTurn(const Card& c) ;
    void setRiver(const Card& c) ;
    std::vector<Card> getCommunityCards();
    std::vector<std::vector<Card>> getAll5CardCombos(const std::vector<Card>& allcards);
    HandRank getBestHand(std::vector<std::vector<Card>> combos);
    void decideWinner();
    Deck getDeck() ;
    std::pair<double,double> monteCarloSimulate();
    std::pair<bool,std::string> RevealNext();
    std::vector<std::string> GetFlopName() const;
    std::string GetTurnName() const;
    std::vector<std::string> GetPlayer1Cards();
    std::string GetRiverName() const;
    std::pair<std::string,std::string> getProbabilityPercentage();
    std::vector<std::vector<std::string>> GetAllplayersCards() const;
    HandRank getBestPlayerHand();
    void Reset();
};
#endif //GAME_H
