#include <vector>
#include<bits/stdc++.h>
#include"card.h"
#include"deck.h"
#include"handrank.h"
#include"player.h"
#include"game.h"
#include<SFML/Graphics.hpp>
#include<tinyxml2.h>
using namespace tinyxml2;

std::map<std::string,sf::IntRect> LoadCardRects(const std::string& filename)
{
    std::map<std::string,sf::IntRect> rects;
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
    {
        std::cerr << "Failed to load the xml file " << filename  << std::endl;
        return rects;
    }
    XMLElement* root = doc.FirstChildElement("Cards");
    if (root == nullptr)
    {
        std::cerr << "Missed <Cards> element";
        return rects;
    }
    for (XMLElement* sub = root->FirstChildElement("SubTexture"); sub != nullptr; sub = sub->NextSiblingElement("SubTexture"))
    {
        const char* name = sub->Attribute("name");
        const int x = sub->IntAttribute("x");
        const int y = sub->IntAttribute("y");
        const int width = sub->IntAttribute("width");
        const int height = sub->IntAttribute("height");
        if (name)
           rects[name]  = sf::IntRect(x,y,width,height);
    }
    return rects;
}

int main()
{
    Card c(HEART,6);
    std::string xmlfile = "/home/tervicke/CLionProjects/poker/assets/cards.xml";
    std::string spritefile = "/home/tervicke/CLionProjects/poker/assets/cards.png";
    auto rects = LoadCardRects(xmlfile);
    if (rects.find(c.getName()) == rects.end())
    {
        std::cerr << "Did not find the key" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Texture CardTexture;
    if (!CardTexture.loadFromFile(spritefile))
    {
        std::cerr << "Failed to load card texture" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Looking for: [" << c.getName() << "]\n";
    for (auto& p : rects) {
      //  std::cout << "Key in map: [" << p.first << "]\n";
        if (p.first == c.getName()) {
            std::cout << "MATCH FOUND!\n";
        }
    }
    auto rect = rects[c.getName()];
    std::cout << "Rect: (" << rect.left << ", " << rect.top << ", " << rect.width << ", " << rect.height << ")\n";
    sf::Sprite card;
    card.setTexture(CardTexture);
    card.setTextureRect(rects[c.getName()]);
    //card.setTextureRect(sf::IntRect(0, 0, 140, 190)); // SPADE2
    card.setPosition(100,100);
    sf::RenderWindow window(sf::VideoMode(800,800),"poker card");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(card);
        window.display();
    }
    return 0;
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