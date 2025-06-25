//
// Created by tervicke on 24/6/25.
#include "player.h"
    Player::Player(){}
    Player::Player(const std::pair<Card,Card>& cards): HoleCards(cards)
    {
        setHoleCards(cards);
    }

    void Player::setHoleCards(const std::pair<Card,Card>& cards)
    {
        this->HoleCards = cards;
    }
    std::pair<Card,Card> Player::getHoleCards() const
    {
        return HoleCards;
    }
//
