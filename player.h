//
// Created by tervicke on 24/6/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include "card.h"
class Player
{
private:
    std::pair<Card,Card> HoleCards;
public:
    Player();
    explicit Player(const std::pair<Card,Card>& cards);
    void setHoleCards(const std::pair<Card,Card>& cards);
    [[nodiscard]] std::pair<Card,Card> getHoleCards() const;
};
#endif //PLAYER_H
