//
// Created by tervicke on 24/6/25.
//
#ifndef HANDRANK_H
#define HANDRANK_H
#include"card.h"
enum Hand{
    HIGH_CARD = 1,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH,
};
class HandRank
{
private:
    Hand hand;
    std::vector<Card> kickers;
public:
    HandRank(Hand hand , std::vector<Card> kickers);
    Hand getHand();
    bool operator<(const HandRank& other) const;
    static void testHandRankComparison();
};

//some util function
HandRank EvaluateHand(const std::vector<Card>& cards);
std::string toHandString(const Hand hand );
#endif //HANDRANK_H

