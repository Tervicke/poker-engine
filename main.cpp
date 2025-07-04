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

    sf::Text handstrText("",font,25);
    handstrText.setFillColor(sf::Color::White);
    handstrText.setPosition(20, 100);

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
                handstrText.setString("");
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
                auto [playerwon,handstr] = g.decideWinner();

                if (playerwon)
                {
                    winText.setString("WIN: 100 %");
                    lossText.setString("LOSS: 0 %");
                }
                else
                {
                    winText.setString("WIN: 0 %");
                    lossText.setString("LOSS: 100 %");
                }
                handstrText.setString("Winning hand: "+handstr);
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
        window.draw(handstrText);

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