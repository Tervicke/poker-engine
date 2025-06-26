//
// Created by tervicke on 24/6/25.
#include<vector>
#include "handrank.h"
#include "card.h"
#include<bits/stdc++.h>


HandRank getBestHand(const std::vector<std::vector<Card>>& combos) ;
std::vector<std::vector<Card>> getAll5CardCombos(const std::vector<Card>& cards);

HandRank::HandRank(Hand hand , std::vector<Card> kickers)
: hand(hand) , kickers(kickers)
{}
Hand HandRank::getHand()
{
    return hand;
}
bool HandRank::operator<(const HandRank& other) const
{
    if (hand != other.hand)
        return hand < other.hand;

    auto getValuesDescending = [](const std::vector<Card>& cards) {
        std::vector<int> values;
        for (const auto& c : cards)
            values.push_back(c.getValue());
        std::sort(values.begin(), values.end(), std::greater<>());
        return values;
    };

    std::vector<int> myvals = getValuesDescending(kickers);
    std::vector<int> othervals = getValuesDescending(other.kickers);
    return myvals < othervals;
}
HandRank EvaluateHand(const std::vector<Card>& cards)
{
    std::vector<int> v(15, 0);
    for (auto& card : cards)
        v[card.getValue()]++;

    bool isflush = true;
    bool isstraight = false;
    Suit suit = cards[0].getSuit();
    for (int i = 1; i < 5; i++) {
        if (cards[i].getSuit() != suit) {
            isflush = false;
            break;
        }
    }

    // sort cards descending by value
    std::vector<Card> sortedCards = cards;
    std::sort(sortedCards.begin(), sortedCards.end(), [](const Card& a, const Card& b) {
        return a.getValue() > b.getValue();
    });

    // check straight
    int straightHigh = -1;
    for (int i = 14; i >= 5; --i) {
        if (v[i] && v[i-1] && v[i-2] && v[i-3] && v[i-4]) {
            isstraight = true;
            straightHigh = i;
            break;
        }
    }
    // special case A-2-3-4-5
    if (!isstraight && v[14] && v[2] && v[3] && v[4] && v[5]) {
        isstraight = true;
        straightHigh = 5;
    }

    if (isflush && isstraight) {
        if (straightHigh == 14) {
            return HandRank(ROYAL_FLUSH, {});
        }
        return HandRank(STRAIGHT_FLUSH, {Card(suit, straightHigh)});
    }

    for (int i = 14; i >= 2; --i) {
        if (v[i] == 4) {
            std::vector<Card> quad, kicker;
            for (auto& c : sortedCards) {
                if (c.getValue() == i) quad.push_back(c);
                else kicker.push_back(c);
            }
            kicker.resize(1);
            return HandRank(FOUR_OF_A_KIND, kicker);
        }
    }

    int three = -1, pair = -1;
    for (int i = 14; i >= 2; --i) {
        if (v[i] == 3 && three == -1) three = i;
        else if (v[i] >= 2 && i != three) pair = std::max(pair, i);
    }
    if (three != -1 && pair != -1) {
        return HandRank(FULL_HOUSE, {});
    }

    if (isflush) {
        return HandRank(FLUSH, sortedCards);
    }

    if (isstraight) {
        return HandRank(STRAIGHT, {Card(suit, straightHigh)});
    }

    if (three != -1) {
        std::vector<Card> trips, kickers;
        for (auto& c : sortedCards) {
            if (c.getValue() == three) trips.push_back(c);
            else kickers.push_back(c);
        }
        kickers.resize(2);
        return HandRank(THREE_OF_A_KIND, kickers);
    }

    std::vector<int> pairs;
    for (int i = 14; i >= 2; --i) {
        if (v[i] == 2) pairs.push_back(i);
    }

    if (pairs.size() >= 2) {
        std::vector<Card> kickers;
        for (auto& c : sortedCards) {
            if (c.getValue() != pairs[0] && c.getValue() != pairs[1])
                kickers.push_back(c);
        }
        kickers.resize(1);
        return HandRank(TWO_PAIR, kickers);
    }

    if (pairs.size() == 1) {
        std::vector<Card> kickers;
        for (auto& c : sortedCards) {
            if (c.getValue() != pairs[0])
                kickers.push_back(c);
        }
        kickers.resize(3);
        return HandRank(ONE_PAIR, kickers);
    }

    std::vector<Card> highCards = sortedCards;
    highCards.resize(5);
    return HandRank(HIGH_CARD, highCards);
}
std::string toHandString(const Hand hand ) {
    switch (hand) {
    case HIGH_CARD:        return "High Card";
    case ONE_PAIR:         return "One Pair";
    case TWO_PAIR:         return "Two Pair";
    case THREE_OF_A_KIND:  return "Three of a Kind";
    case STRAIGHT:         return "Straight";
    case FLUSH:            return "Flush";
    case FULL_HOUSE:       return "Full House";
    case FOUR_OF_A_KIND:   return "Four of a Kind";
    case STRAIGHT_FLUSH:   return "Straight Flush";
    case ROYAL_FLUSH:      return "Royal Flush";
    default:               return "Unknown Hand";
    }
}
void HandRank::testHandRankComparison() {
    std::vector<std::pair<std::string, std::vector<Card>>> hands = {
        {
            "Royal Flush",
            {Card(SPADE, 10), Card(SPADE, 11), Card(SPADE, 12), Card(SPADE, 13), Card(SPADE, 14), Card(CLUB, 3), Card(DIAMOND, 5)}
        },
        {
            "One Pair (Aces)",
            {Card(CLUB, 14), Card(DIAMOND, 14), Card(HEART, 5), Card(SPADE, 7), Card(CLUB, 9), Card(HEART, 2), Card(DIAMOND, 3)}
        },
        {
            "Two Pair (Kings and Tens)",
            {Card(CLUB, 13), Card(DIAMOND, 13), Card(HEART, 10), Card(SPADE, 10), Card(HEART, 2), Card(SPADE, 3), Card(CLUB, 6)}
        },
        {
            "Straight",
            {Card(CLUB, 5), Card(DIAMOND, 6), Card(HEART, 7), Card(SPADE, 8), Card(HEART, 9), Card(DIAMOND, 2), Card(CLUB, 11)}
        },
        {
            "High Card",
            {Card(SPADE, 2), Card(DIAMOND, 5), Card(CLUB, 9), Card(HEART, 11), Card(CLUB, 13), Card(DIAMOND, 4), Card(HEART, 3)}
        }
    };

    // Evaluate each hand from 7 cards
    std::vector<std::pair<HandRank, std::string>> evaluated;
    for (const auto& [name, cards] : hands) {
        auto allCombos = getAll5CardCombos(cards); // returns vector<vector<Card>> of 21 combinations
        HandRank rank = getBestHand(allCombos);
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

std::vector<std::vector<Card>> getAll5CardCombos(const std::vector<Card>& cards) {
    std::vector<std::vector<Card>> combos;

    // Should always be 7
    assert(cards.size() == 7);

    // Generate all 5-card combinations from 7 cards
    std::vector<int> indices(7);
    std::iota(indices.begin(), indices.end(), 0);

    // Generate combinations using 3 nested loops (easier than std::next_permutation here)
    for (int i = 0; i < 7; ++i) {
        for (int j = i + 1; j < 7; ++j) {
            // Skip two indices (i and j) to leave 5 remaining
            std::vector<Card> combo;
            for (int k = 0; k < 7; ++k) {
                if (k != i && k != j) {
                    combo.push_back(cards[k]);
                }
            }
            combos.push_back(combo);
        }
    }

    return combos;
}
HandRank getBestHand(const std::vector<std::vector<Card>>& combos) {
    assert(!combos.empty());

    HandRank best = EvaluateHand(combos[0]);
    for (size_t i = 1; i < combos.size(); ++i) {
        HandRank current = EvaluateHand(combos[i]);
        if (best < current) {
            best = current;
        }
    }
    return best;
}