//
// Created by tervicke on 24/6/25.
//

#ifndef CARD_H
#define CARD_H
#include<string>
#include<map>
enum Suit {
    NONE = -1,
    SPADE = 1,
    HEART = 2,
    DIAMOND = 4,
    CLUB = 8,
 };
std::string getSuitAbbreviation(Suit suit);
std::string getRankStr(int value) ;
class Card
{
private:
    Suit suit;
    int value;
    int prime;
public:
    Card();
    Card(const Suit suit, const int value);
    [[nodiscard]] int getValue() const;
    [[nodiscard]] Suit getSuit() const;
    void print() const;
    [[nodiscard]] int getPrimeValue() const;

};
#endif //CARD_H
