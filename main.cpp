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

void setRotationsOnSprites(std::vector<std::vector<sf::Sprite>>& playerSprites , const int& cardWidth , const int& cardHeight , const int& spacing);

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
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Could not load font!\n";
    }
    return font;
}
int main() {
    HandRank::testHandRankComparison();
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Poker Table");
    window.setFramerateLimit(60);

    sf::Font font = loadFont();

    std::string xmlfile = "assets/cards.xml";
    std::string spritefile = "assets/cards.png";
    std::string backFile = "assets/backs.png";
    auto rects = LoadCardRects(xmlfile);

    sf::Texture cardTexture;
    if (!cardTexture.loadFromFile(spritefile)) {
        std::cerr << "Failed to load card texture\n";
        return 1;
    }
    std::vector<Player> players;
    for (int i = 0 ; i < 4 ; i++)
    {
        Player p;
        players.push_back(p);
    }
    Game g(players);

    sf::Texture backTexture;
    if (!backTexture.loadFromFile(backFile))
    {
        std::cerr << "Failed to load card texture\n";
        return 1;
    }

    std::vector<std::vector<sf::Sprite>> playerSprites(4);

    float cardWidth = 140 * 0.7f;
    float cardHeight = 190 * 0.7f;
    float spacing = 20.f;

    std::vector<std::string> communityCardNames = {};

    std::vector<sf::Sprite> communityCards;

    float totalWidth = 5 * cardWidth + 4 * spacing;
    float startX = (WINDOW_WIDTH - totalWidth) / 2 + 20;
    float y = WINDOW_HEIGHT / 2 - cardHeight / 2;

    // Win / Loss Text
    sf::Text winText("Win 0%", font, 24);
    winText.setFillColor(sf::Color::White);
    winText.setPosition(20, 20);

    sf::Text lossText("Loss 0%", font, 24);
    lossText.setFillColor(sf::Color::White);
    lossText.setPosition(20, 60);

    //set probality
    auto [winpercent , losspercent] = g.getProbabilityPercentage();
    winText.setString("WIN: "+winpercent+" %");
    lossText.setString("LOSS: "+losspercent + " %");

    while (window.isOpen()) {
        //exit logic
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                g.stop = !g.stop;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                std::cout << "resetting" << std::endl;
                g.Reset();
                communityCardNames.clear();
                communityCards.clear();
                playerSprites.clear();
                playerSprites.resize(4);
                auto [winpercent , losspercent] = g.getProbabilityPercentage();
                winText.setString("WIN: "+winpercent+" %");
                lossText.setString("LOSS: "+losspercent + " %");
            }
        }
        if (auto [revealed , type] = g.RevealNext(); revealed){
            if (type == "HOLE")
            {
                auto player1cardnames = g.GetPlayer1Cards();

                for (int i = 0; i < 4; ++i)
                    playerSprites[i].clear();

                for (int i = 0; i < 4; ++i) {
                    if (i != 0)
                    {
                        for (int j = 0 ; j < 2 ; j++)
                        {
                            sf::Sprite sprite(backTexture, rects["BACK"]); //BACK returns the back texture
                            sprite.setScale(0.7f, 0.7f);
                            playerSprites[i].push_back(sprite);
                        }
                    }
                    else
                    {
                        for (const auto& name : player1cardnames)
                        {
                            std::cout << name << std::endl;
                            sf::Sprite sprite(cardTexture, rects[name]);
                            sprite.setScale(0.7f, 0.7f);
                            playerSprites[i].push_back(sprite);
                        }
                    }
                }
                setRotationsOnSprites(playerSprites , cardWidth , cardHeight , spacing);

                auto [winpercent , losspercent] = g.getProbabilityPercentage();
                winText.setString("WIN: "+winpercent+" %");
                lossText.setString("LOSS: "+losspercent + " %");
            }
            else if (type == "FLOP")
            {
                auto names = g.GetFlopName();
                for (int i = 0 ; i < 3 ; i++)
                {
                    sf::Sprite sprite(cardTexture, rects[names[i]]);
                    communityCards.push_back(sprite);
                }
                auto [winpercent , losspercent] = g.getProbabilityPercentage();
                winText.setString("WIN: "+winpercent+" %");
                lossText.setString("LOSS: "+losspercent + " %");
            }
            else if (type == "TURN"){
                sf::Sprite sprite(cardTexture, rects[g.GetTurnName()]);
                communityCards.push_back(sprite);

                auto [winpercent , losspercent] = g.getProbabilityPercentage();
                winText.setString("WIN: "+winpercent+" %");
                lossText.setString("LOSS: "+losspercent + " %");
            }else if (type == "RIVER")
            {
                sf::Sprite sprite(cardTexture, rects[g.GetRiverName()]);
                communityCards.push_back(sprite);

                auto [winpercent , losspercent] = g.getProbabilityPercentage();
                winText.setString("WIN: "+winpercent+" %");
                lossText.setString("LOSS: "+losspercent + " %");
            }
            else if (type == "END")
            {
                auto names = g.GetAllplayersCards();
                playerSprites.clear();
                for (int i = 0; i < 4; ++i)
                {
                    playerSprites.push_back({});
                    for (auto name : names[i])
                    {
                        sf::Sprite sprite(cardTexture, rects[name]);
                        sprite.setScale(0.7f, 0.7f);
                        sprite.setPosition(startX + i * (cardWidth + spacing), y);
                        playerSprites[i].push_back(sprite);
                    }
                }
                setRotationsOnSprites(playerSprites , cardWidth , cardHeight , spacing);
                auto userhand = g.getBestPlayerHand();
                std::cout << toHandString(userhand.getHand()) << std::endl;
            }
            //update position for community cards
            for (int i = 0; i < communityCards.size(); ++i) {
                communityCards[i].setScale(0.7f,0.7f);
                communityCards[i].setPosition(startX + i * (cardWidth + spacing), y);
            }
        }
        window.clear(sf::Color(0, 100, 0)); // green background


        window.draw(winText);
        window.draw(lossText);

        for (const auto& hand : playerSprites)
        {
            for (const auto& card : hand)
            {
                window.draw(card);
            }
        }

        for (auto& sprite: communityCards)
        {
            window.draw(sprite);
        }

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
*/
void setRotationsOnSprites(std::vector<std::vector<sf::Sprite>>& playerSprites, const int& cardWidth, const int& cardHeight, const int& spacing)
{
    // Bottom (player 0)
    {
        float baseX = WINDOW_WIDTH / 2.0f;
        float baseY = WINDOW_HEIGHT - 160.0f;
        float angleStart = -10.0f;
        float angleStep = 20.0f;

        for (int i = 0; i < 2; ++i) {
            auto& sprite = playerSprites[0][i];
            sprite.setOrigin(cardWidth / 2.0f, cardHeight / 2.0f);
            sprite.setRotation(angleStart + i * angleStep);
            sprite.setPosition(baseX + (i - 0.5f) * (cardWidth / 2.0f), baseY);
        }
    }

    // Right (player 1) – upright, fanned vertically
    {
        float baseX = WINDOW_WIDTH - 120.0f;
        float baseY = WINDOW_HEIGHT / 2.0f;
        float angleStart = -10.0f;
        float angleStep = 20.0f;

        for (int i = 0; i < 2; ++i) {
            auto& sprite = playerSprites[1][i];
            sprite.setOrigin(cardWidth / 2.0f, cardHeight / 2.0f);
            sprite.setRotation(angleStart + i * angleStep);
            sprite.setPosition(baseX, baseY + (i - 0.5f) * (cardHeight / 2.0f));
        }
    }

    // Top (player 2)
    {
        float baseX = WINDOW_WIDTH / 2.0f;
        float baseY = 100.0f;
        float angleStart = 10.0f;
        float angleStep = -20.0f;

        for (int i = 0; i < 2; ++i) {
            auto& sprite = playerSprites[2][i];
            sprite.setOrigin(cardWidth / 2.0f, cardHeight / 2.0f);
            sprite.setRotation(angleStart + i * angleStep);
            sprite.setPosition(baseX + (i - 0.5f) * (cardWidth / 2.0f), baseY);
        }
    }

    // Left (player 3) – upright, fanned vertically
    {
        float baseX = 120.0f;
        float baseY = WINDOW_HEIGHT / 2.0f;
        float angleStart = 10.0f;
        float angleStep = -20.0f;

        for (int i = 0; i < 2; ++i) {
            auto& sprite = playerSprites[3][i];
            sprite.setOrigin(cardWidth / 2.0f, cardHeight / 2.0f);
            sprite.setRotation(angleStart + i * angleStep);
            sprite.setPosition(baseX, baseY + (i - 0.5f) * (cardHeight / 2.0f));
        }
    }
}