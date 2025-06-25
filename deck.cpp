#include <random>
#include"card.h"
#include"deck.h"
//
// Created by tervicke on 24/6/25.
void Deck::fillCards()
{
    const std::vector<Suit> suits = {DIAMOND , SPADE , HEART , CLUB};
    for (int value = 1 ; value <= 13 ; value++ )
    {
        for (const auto suit : suits)
        {
            cards.emplace_back(suit,value);
        }
    }
}
Deck::Deck(): gen(std::random_device{}())
{
    this->fillCards();
}
void Deck::print() const
{
    for (const auto& card : cards)
    {
        card.print();
    }
}
[[nodiscard]] size_t Deck::getCount() const
{
    return cards.size();
}
std::vector<Card> Deck::drawCards(const int val )
{
    if (val > this->cards.size())
    {
        return {};
    }
    std::vector<Card> drawn;
    for (int i = 0 ; i < val ; i++)
    {
        std::uniform_int_distribution<> dist(0,cards.size()-1);
        const int index = dist(gen);
        std::swap(cards[index],cards.back());
        Card d = cards.back();
        drawn.emplace_back(d);
        cards.pop_back();
    }
    return drawn;
}
void Deck::insert(Card c)
{
    cards.emplace_back(c);
}