//
// Created by tervicke on 24/6/25.
//

#ifndef DECK_H
#define DECK_H
#include <random>
#include "card.h"
class Deck
{
private:
    std::mt19937 gen;
    std::vector<Card> cards;
    void fillCards();
public:
    Deck();
    void print() const;
    [[nodiscard]] size_t getCount() const ;
    std::vector<Card> drawCards(const int val );
    void insert(Card c);
    void reset();
};
#endif //DECK_H
