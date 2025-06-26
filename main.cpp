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
//	<SubTexture name="cardBack_blue3.png" x="280" y="380" width="140" height="190"/>
    rects["BACK"] = sf::IntRect(280,0,140,190);
    return rects;
}
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

sf::Font loadFont() {
    sf::Font font;
    if (!font.loadFromFile("/home/tervicke/CLionProjects/poker/assets/yunan.ttf")) {
        std::cerr << "Could not load font!\n";
    }
    return font;
}
int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Poker Table");
    window.setFramerateLimit(60);

    sf::Font font = loadFont();

    std::string xmlfile = "/home/tervicke/CLionProjects/poker/assets/cards.xml";
    std::string spritefile = "/home/tervicke/CLionProjects/poker/assets/cards.png";
    std::string backFile = "/home/tervicke/CLionProjects/poker/assets/backs.png";
    auto rects = LoadCardRects(xmlfile);

    sf::Texture cardTexture;
    if (!cardTexture.loadFromFile(spritefile)) {
        std::cerr << "Failed to load card texture\n";
        return 1;
    }
    
    sf::Texture backTexture;
    if (!backTexture.loadFromFile(backFile))
    {
        std::cerr << "Failed to load card texture\n";
        return 1;
    }
    std::vector<std::vector<std::string>> playerHands = {
        {"HEART1", "CLUB2"},       // Bottom (you)
        {"BACK", "BACK"},    // Right (rotated)
        {"BACK", "BACK"},      // Top
        {"BACK", "BACK"}    // Left (rotated)
    };

    std::vector<std::vector<sf::Sprite>> playerSprites(4);
    for (int i = 0; i < 4; ++i) {
        for (const std::string& name : playerHands[i]) {
            if (name == "BACK")
            {
                sf::Sprite sprite(backTexture, rects[name]);
                sprite.setScale(0.7f, 0.7f);
                playerSprites[i].push_back(sprite);
            }else
            {
                sf::Sprite sprite(cardTexture, rects[name]);
                sprite.setScale(0.7f, 0.7f);
                playerSprites[i].push_back(sprite);
            }
        }
    }

    float cardWidth = 140 * 0.7f;
    float cardHeight = 190 * 0.7f;
    float spacing = 20.f;

    // Bottom (you)
    for (int i = 0; i < 2; ++i)
        playerSprites[0][i].setPosition((WINDOW_WIDTH / 2 - cardWidth) + i * (cardWidth + spacing), WINDOW_HEIGHT - 160);

    // Top
    for (int i = 0; i < 2; ++i)
        playerSprites[2][i].setPosition((WINDOW_WIDTH / 2 - cardWidth) + i * (cardWidth + spacing), 100);

    // Right (rotated, vertically stacked)
    for (int i = 0; i < 2; ++i) {
        auto& sprite = playerSprites[1][i];
        sprite.setRotation(90);
        sprite.setOrigin(0, cardHeight); // flip pivot to top-left of rotated card
        sprite.setPosition(WINDOW_WIDTH - 120, (WINDOW_HEIGHT / 2 - cardWidth) + i * (cardWidth + spacing));
    }

    // Left (rotated, vertically stacked)
    for (int i = 0; i < 2; ++i) {
        auto& sprite = playerSprites[3][i];
        sprite.setRotation(90);
        sprite.setOrigin(0, cardHeight);
        sprite.setPosition(120, (WINDOW_HEIGHT / 2 - cardWidth) + i * (cardWidth + spacing));
    }

    std::vector<std::string> communityCardNames = {
        "HEART8", "CLUB4", "SPADE7", "DIAMOND2", "CLUB13"
    };

    std::vector<sf::Sprite> communityCards;

    for (const std::string& name : communityCardNames) {
        sf::Sprite sprite(cardTexture, rects[name]);
        sprite.setScale(0.6f, 0.6f);
        communityCards.push_back(sprite);
    }

    float totalWidth = 5 * cardWidth + 4 * spacing;
    float startX = (WINDOW_WIDTH - totalWidth) / 2 + 20;
    float y = WINDOW_HEIGHT / 2 - cardHeight / 2;

    for (int i = 0; i < communityCards.size(); ++i) {
        communityCards[i].setPosition(startX + i * (cardWidth + spacing), y);
    }

    // Win / Loss Text
    sf::Text winText("Win 54.3%", font, 24);
    winText.setFillColor(sf::Color::White);
    winText.setPosition(20, 20);

    sf::Text lossText("Loss 45.7%", font, 24);
    lossText.setFillColor(sf::Color::White);
    lossText.setPosition(20, 60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(0, 100, 0)); // green background

        window.draw(winText);
        window.draw(lossText);

        for (const auto& hand : playerSprites)
            for (const auto& card : hand)
                window.draw(card);

        for (auto& card : communityCards)
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