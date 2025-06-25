//
// Created by tervicke on 24/6/25.
#include <iostream>
#include<string>
#include<map>
#include"card.h"


std::string getSuitAbbreviation(Suit suit) {
    switch (suit) {
    case SPADE:   return "S";
    case HEART:   return "H";
    case DIAMOND: return "D";
    case CLUB:    return "C";
    default:      return "?";
    }
}
std::string getSuitName(Suit suit)
{
    switch (suit)
    {
        case SPADE:   return "SPADE";
        case HEART:   return "HEART";
        case DIAMOND: return "DIAMOND";
        case CLUB:    return "CLUB";
        default: return "?";
    };
}

//prime map
std::map<int, int> PrimeMap = {
    {2, 2},
    {3, 3},
    {4, 5},
    {5, 7},
    {6, 11},
    {7, 13},
    {8, 17},
    {9, 19},
    {10, 23},
    {11, 29},  // Jack
    {12, 31},  // Queen
    {13, 37},  // King
    {1, 41},  // Ace
};

std::string getRankStr(int value) {
    switch (value) {
    case 11: return "J";
    case 12: return "Q";
    case 13: return "K";
    case 14: return "A";
    default: return std::to_string(value);
    }
}

Card::Card()
    : suit(NONE) , value(-1) , prime(-1)
{}
Card::Card(const Suit suit, const int value)
{
    this->suit = suit;
    this->value = value;
    this->prime = PrimeMap[value];
}
[[nodiscard]] int Card::getValue() const
{
    return value;
}
[[nodiscard]] Suit Card::getSuit() const
{
    return suit;
}
void Card::print() const
{
    std::cout << getRankStr(value) << " " << getSuitAbbreviation(suit) << std::endl;
}
[[nodiscard]] int Card::getPrimeValue() const
{
    return prime;
}
[[nodiscard]] std::string Card::getName() const
{
    return getSuitName(suit) + std::to_string(value);
}
