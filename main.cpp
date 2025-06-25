#include <iostream>
#include <random>
#include <utility>
#include <vector>
#include<map>
#include<cassert>
#include<bits/stdc++.h>
#include"card.h"
#include"deck.h"
#include"handrank.h"
#include"player.h"
#include"game.h"
#include<SFML/Graphics.hpp>
void MonteCarloSimulate(Deck d , std::vector<Card> hand);
int main()
{
    /*
    std::pair<Card,Card> p1holecards = {
        Card(CLUB,8),
        Card(SPADE,7),
    };
    std::pair<Card,Card> p2holecards = {
        Card(CLUB,12),
        Card(DIAMOND,6),
    };
    Player p1(p1holecards);
    Player p2(p2holecards);
    Game g(p1,p2);
    g.RevealFlop();
    g.RevealTurn();
    g.RevealRiver();
    g.PrintGameState();
    g.monteCarloSimulate();
    g.decideWinner();
    */
    sf::RenderWindow window(sf::VideoMode(800, 600), "Poker Engine");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);
        window.display();
    }
}

/*
void testEvaluateHand() {
    // High Card
    std::vector<Card> highCard = {
        Card(HEART, 2), Card(CLUB, 5), Card(SPADE, 9), Card(DIAMOND, 11), Card(CLUB, 13)
    };
    assert(EvaluateHand(highCard).getHand() == HIGH_CARD);

    // One Pair
    std::vector<Card> onePair = {
        Card(HEART, 2), Card(CLUB, 2), Card(SPADE, 5), Card(DIAMOND, 9), Card(CLUB, 12)
    };
    assert(EvaluateHand(onePair).getHand() == ONE_PAIR);

    // Two Pair
    std::vector<Card> twoPair = {
        Card(HEART, 4), Card(CLUB, 4), Card(SPADE, 9), Card(DIAMOND, 9), Card(CLUB, 13)
    };
    assert(EvaluateHand(twoPair).getHand() == TWO_PAIR);

    // Three of a Kind
    std::vector<Card> threeKind = {
        Card(HEART, 7), Card(CLUB, 7), Card(SPADE, 7), Card(DIAMOND, 11), Card(CLUB, 13)
    };
    assert(EvaluateHand(threeKind).getHand() == THREE_OF_A_KIND);

    // Straight
    std::vector<Card> straight = {
        Card(HEART, 5), Card(CLUB, 6), Card(SPADE, 7), Card(DIAMOND, 8), Card(CLUB, 9)
    };
    assert(EvaluateHand(straight).getHand() == STRAIGHT);

    // Ace-low straight (optional extra check)
    std::vector<Card> aceLowStraight = {
        Card(HEART, 14), Card(CLUB, 2), Card(SPADE, 3), Card(DIAMOND, 4), Card(CLUB, 5)
    };
    assert(EvaluateHand(aceLowStraight).getHand() == STRAIGHT);

    // Flush
    std::vector<Card> flush = {
        Card(SPADE, 2), Card(SPADE, 5), Card(SPADE, 7), Card(SPADE, 9), Card(SPADE, 11)
    };
    assert(EvaluateHand(flush).getHand() == FLUSH);

    // Full House
    std::vector<Card> fullHouse = {
        Card(HEART, 6), Card(CLUB, 6), Card(SPADE, 6), Card(DIAMOND, 9), Card(CLUB, 9)
    };
    assert(EvaluateHand(fullHouse).getHand() == FULL_HOUSE);

    // Four of a Kind
    std::vector<Card> fourKind = {
        Card(HEART, 10), Card(CLUB, 10), Card(SPADE, 10), Card(DIAMOND, 10), Card(CLUB, 2)
    };
    assert(EvaluateHand(fourKind).getHand() == FOUR_OF_A_KIND);

    // Straight Flush
    std::vector<Card> straightFlush = {
        Card(HEART, 5), Card(HEART, 6), Card(HEART, 7), Card(HEART, 8), Card(HEART, 9)
    };
    assert(EvaluateHand(straightFlush).getHand() == STRAIGHT_FLUSH);

    // Royal Flush
    std::vector<Card> royalFlush = {
        Card(SPADE, 10), Card(SPADE, 11), Card(SPADE, 12), Card(SPADE, 13), Card(SPADE, 14)
    };
    assert(EvaluateHand(royalFlush).getHand() == ROYAL_FLUSH);

    std::cout << "✅ ALL HAND RANK TESTS PASSED ✅" << std::endl;
}
void testHandRankComparison() {
    std::vector<std::pair<std::string, std::vector<Card>>> hands = {
        {
            "Royal Flush",
            {Card(SPADE, 10), Card(SPADE, 11), Card(SPADE, 12), Card(SPADE, 13), Card(SPADE, 14)}
        },
        {
            "One Pair (Aces)",
            {Card(CLUB, 14), Card(DIAMOND, 14), Card(HEART, 5), Card(SPADE, 7), Card(CLUB, 9)}
        },
        {
            "Two Pair (Kings and Tens)",
            {Card(CLUB, 13), Card(DIAMOND, 13), Card(HEART, 10), Card(SPADE, 10), Card(HEART, 2)}
        },
        {
            "Straight",
            {Card(CLUB, 5), Card(DIAMOND, 6), Card(HEART, 7), Card(SPADE, 8), Card(HEART, 9)}
        },
        {
            "High Card",
            {Card(SPADE, 2), Card(DIAMOND, 5), Card(CLUB, 9), Card(HEART, 11), Card(CLUB, 13)}
        }
    };

    // Evaluate each hand
    std::vector<std::pair<HandRank, std::string>> evaluated;
    for (const auto& [name, cards] : hands) {
        HandRank rank = EvaluateHand(cards);
        evaluated.emplace_back(rank, name);
    }

    // Sort by strength
    std::sort(evaluated.begin(), evaluated.end());

    std::cout << "Sorted hands from weakest to strongest:\n";
    for (auto& [rank, name] : evaluated) {
        std::cout << toHandString(rank.getHand()) << " => " << name << "\n";
    }

    // Assert Royal Flush is strongest
    assert(evaluated.back().second == "Royal Flush");

    // Assert High Card is weakest
    assert(evaluated.front().second == "High Card");

    std::cout << "All hand comparisons passed.\n";
}
*/